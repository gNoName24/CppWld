#pragma once

#include "../pch.hpp"

namespace TlConsole {
    void setColor(const int& type);
    void println(const int& type, const std::string& text);
    void println(const int& type, const std::string& tag, const std::string& text);
}
