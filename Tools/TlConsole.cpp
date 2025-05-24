#include "TlMain.hpp"
#include "TlConsole.hpp"

namespace TlConsole {
    void setColor(const int& type) {
        std::cout << "\033[" << type << "m";
    }

    void println(const int& type, const std::string& text) {
        switch(type) {
        case 1: std::cout << "[INFO] "; break;
        case 2: std::cout << "[WARNING] "; break;
        case 3: std::cout << "[ERROR] "; break;
        default: break; }
        std::cout << TlMain::getStringTime("%H:%M:%S - ") << text << std::endl;
    }
    void println(const int& type, const std::string& tag, const std::string& text) {
        switch(type) {
        case 1: std::cout << "["+tag+"/INFO] "; break;
        case 2: std::cout << "["+tag+"/WARNING] "; break;
        case 3: std::cout << "["+tag+"/ERROR] "; break;
        default: break; }
        std::cout << TlMain::getStringTime("%H:%M:%S - ") << text << std::endl;
    }
}
