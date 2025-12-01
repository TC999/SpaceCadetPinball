#include "font_finder.h"
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <wingdi.h>

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
        lower.find("sourcehanserif") != std::string::npos)
        return true;
    return false;
}

std::string FindFontWithGlyph(const std::string& testChar)
{
    // Windows 常见字体目录
    std::vector<std::string> fontDirs = {
        std::string(getenv("WINDIR")) + "\\Fonts",
        "C:/Windows/Fonts"
    };
    std::vector<std::string> fontExts = { ".ttf", ".ttc", ".otf" };
    for (const auto& dir : fontDirs)
    {
        if (!std::filesystem::exists(dir)) continue;
        for (const auto& entry : std::filesystem::directory_iterator(dir))
        {
            if (!entry.is_regular_file()) continue;
            std::string path = entry.path().string();
            for (const auto& ext : fontExts)
            {
                if (path.size() > ext.size() && path.substr(path.size() - ext.size()) == ext)
                {
                    if (FontHasGlyph(path, testChar))
                        return path;
                }
            }
        }
    }
    return "";
}
