#include "font_finder.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <filesystem>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

// Maximum depth for recursive directory search to prevent stack overflow
static const int MAX_SEARCH_DEPTH = 10;

// 检查字体文件是否包含指定字符
static bool FontHasGlyph(const std::string& fontPath, const std::string& testChar)
{
    // 这里只能做简单的文件名过滤，真正的字体内码点检测需用 freetype 或更专业库。
    // 这里假设常见中文字体文件名包含 simsun、msyh、msjh、fangzheng、noto、hei、song、kai、yahei、NotoSans、Deng、Fang、PingFang、SourceHanSans、SourceHanSerif 等
    std::string lower = fontPath;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower.find("sim") != std::string::npos || lower.find("msyh") != std::string::npos ||
        lower.find("msjh") != std::string::npos || lower.find("fang") != std::string::npos ||
        lower.find("noto") != std::string::npos || lower.find("hei") != std::string::npos ||
        lower.find("song") != std::string::npos || lower.find("kai") != std::string::npos ||
        lower.find("yahei") != std::string::npos || lower.find("deng") != std::string::npos ||
        lower.find("pingfang") != std::string::npos || lower.find("sourcehansans") != std::string::npos ||
        lower.find("sourcehanserif") != std::string::npos || lower.find("wqy") != std::string::npos ||
        lower.find("wenquanyi") != std::string::npos || lower.find("cjk") != std::string::npos ||
        lower.find("arphic") != std::string::npos || lower.find("uming") != std::string::npos ||
        lower.find("ukai") != std::string::npos)
        return true;
    return false;
}

static bool HasFontExtension(const std::string& path)
{
    std::vector<std::string> fontExts = { ".ttf", ".ttc", ".otf" };
    for (const auto& ext : fontExts)
    {
        if (path.size() > ext.size() && path.substr(path.size() - ext.size()) == ext)
            return true;
    }
    return false;
}

// Validate path doesn't contain path traversal sequences
static bool IsValidPath(const std::string& path)
{
    if (path.empty())
        return false;
    // Check for path traversal patterns
    if (path.find("..") != std::string::npos)
        return false;
    return true;
}

#ifndef _WIN32
// Linux/macOS: Recursively search directory for fonts with depth limiting
static std::string SearchDirForFont(const std::string& dirPath, const std::string& testChar, int depth = 0)
{
    if (depth > MAX_SEARCH_DEPTH || dirPath.empty())
        return "";

    DIR* dir = opendir(dirPath.c_str());
    if (!dir)
        return "";

    std::string result;
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue;

        std::string fullPath = dirPath + "/" + name;
        struct stat statbuf;
        if (lstat(fullPath.c_str(), &statbuf) != 0)
            continue;

        // Skip symbolic links to prevent cycles
        if (S_ISLNK(statbuf.st_mode))
            continue;

        if (S_ISDIR(statbuf.st_mode))
        {
            // Recursively search subdirectories
            result = SearchDirForFont(fullPath, testChar, depth + 1);
            if (!result.empty())
                break;
        }
        else if (S_ISREG(statbuf.st_mode))
        {
            if (HasFontExtension(fullPath) && FontHasGlyph(fullPath, testChar))
            {
                result = fullPath;
                break;
            }
        }
    }
    closedir(dir);
    return result;
}
#endif

std::string FindFontWithGlyph(const std::string& testChar)
{
    std::vector<std::string> fontDirs;

#ifdef _WIN32
    // Windows 常见字体目录
    const char* windir = getenv("WINDIR");
    if (windir && IsValidPath(windir))
        fontDirs.push_back(std::string(windir) + "\\Fonts");
    fontDirs.push_back("C:/Windows/Fonts");

    for (const auto& dir : fontDirs)
    {
        if (!std::filesystem::exists(dir)) continue;
        for (const auto& entry : std::filesystem::directory_iterator(dir))
        {
            if (!entry.is_regular_file()) continue;
            std::string path = entry.path().string();
            if (HasFontExtension(path) && FontHasGlyph(path, testChar))
                return path;
        }
    }
#elif defined(__APPLE__)
    // macOS 常见字体目录
    fontDirs.push_back("/System/Library/Fonts");
    fontDirs.push_back("/Library/Fonts");
    const char* home = getenv("HOME");
    if (home && IsValidPath(home))
        fontDirs.push_back(std::string(home) + "/Library/Fonts");

    for (const auto& dir : fontDirs)
    {
        std::string found = SearchDirForFont(dir, testChar);
        if (!found.empty())
            return found;
    }
#else
    // Linux 常见字体目录
    fontDirs.push_back("/usr/share/fonts");
    fontDirs.push_back("/usr/local/share/fonts");
    const char* home = getenv("HOME");
    if (home && IsValidPath(home))
    {
        fontDirs.push_back(std::string(home) + "/.fonts");
        fontDirs.push_back(std::string(home) + "/.local/share/fonts");
    }
    // XDG_DATA_HOME 支持
    const char* xdgDataHome = getenv("XDG_DATA_HOME");
    if (xdgDataHome && IsValidPath(xdgDataHome))
        fontDirs.push_back(std::string(xdgDataHome) + "/fonts");

    for (const auto& dir : fontDirs)
    {
        std::string found = SearchDirForFont(dir, testChar);
        if (!found.empty())
            return found;
    }
#endif
    return "";
}
