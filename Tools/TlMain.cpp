#include "TlMain.hpp"

namespace TlMain {
    // Получение времени в виде string
    std::string getStringTime(const std::string& form) {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        struct tm *parts = localtime(&time_t_now);
        std::ostringstream oss;
        oss << std::put_time(parts, form.c_str());
        return oss.str();
    }
    std::string getLanguageCode() {
        wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
        if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH)) {
            char localeAnsi[LOCALE_NAME_MAX_LENGTH];
            WideCharToMultiByte(CP_ACP, 0, localeName, -1, localeAnsi, LOCALE_NAME_MAX_LENGTH, NULL, NULL);
            std::string full(localeAnsi);
            size_t pos = full.find('-');
            if (pos != std::string::npos) {
                return full.substr(0, pos);
            }
        }
        return "unknown";
    }
    std::string getCountryCode() {
        wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
        if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH)) {
            char localeAnsi[LOCALE_NAME_MAX_LENGTH];
            WideCharToMultiByte(CP_ACP, 0, localeName, -1, localeAnsi, LOCALE_NAME_MAX_LENGTH, NULL, NULL);
            std::string full(localeAnsi);
            size_t pos = full.find('-');
            if (pos != std::string::npos && pos + 1 < full.size()) {
                return full.substr(pos + 1);
            }
        }
        return "UNKNOWN";
    }
    std::string getLocaleCode() {
        return getLanguageCode() + "_" + getCountryCode();
    }
    // Проверка строки на кириллицу
    bool containsCyrillic(const std::string& str) {
        try {
            // UTF-8 -> UTF-32
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            std::wstring wideStr = converter.from_bytes(str);
            for (wchar_t ch : wideStr) {
                if (ch >= 0x0400 && ch <= 0x04FF) {
                    return true;
                }
            }
        } catch (const std::range_error& e) {
            std::cerr << "Ошибка преобразования строки: " << e.what() << std::endl;
            return false;
        }
        return false;
    }
    // Vector2u в Vector2i
    sf::Vector2i to_Vector2i(const sf::Vector2u& vec) {
        return sf::Vector2i(static_cast<int>(vec.x), static_cast<int>(vec.y));
    }
    // Vector2f в Vector2i
    sf::Vector2i to_Vector2i(const sf::Vector2f& vec) {
        return sf::Vector2i(static_cast<int>(vec.x), static_cast<int>(vec.y));
    }
    // Vector2i в Vector2f
    sf::Vector2f to_Vector2f(const sf::Vector2i& vec) {
        return sf::Vector2f(static_cast<float>(vec.x), static_cast<float>(vec.y));
    }
    // Замена текста
    std::string replaceAll(const std::string& str, const std::string& from, const std::string& to) {
        if (from.empty()) return str;
        std::string result = str;
        size_t start_pos = 0;
        while ((start_pos = result.find(from, start_pos)) != std::string::npos) {
            result.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return result;
    }
    // Получения кол-во использованный байтов приложением в оперативной памяти
    int getMemoryUsage() {
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
            return pmc.WorkingSetSize;
        } else { return -1; }
    }
    // Оставление лишь 0.0
    sf::Vector2f truncateToOneDecimal(const sf::Vector2f& v) {
        auto truncate = [](float value) -> float {
            float temp = std::floor((value + 0.0001f) * 10.0f);
            return temp / 10.0f;
        };
        return sf::Vector2f(truncate(v.x), truncate(v.y));
    }
}
