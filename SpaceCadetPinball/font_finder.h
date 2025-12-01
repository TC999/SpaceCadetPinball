#pragma once
#include <string>

// 尝试在系统常见字体目录中查找支持指定字符的字体
// 返回找到的字体文件绝对路径，否则返回空字符串
std::string FindFontWithGlyph(const std::string& testChar);
