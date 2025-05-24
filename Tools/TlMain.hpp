#ifndef TLMAIN_HPP
#define TLMAIN_HPP

#include "../Storage.hpp"

namespace TlMain {
    std::string getStringTime(const std::string& form);
    std::string getLanguageCode();
    std::string getCountryCode();
    std::string getLocaleCode();
    bool containsCyrillic(const std::string& str);
    sf::Vector2i to_Vector2i(const sf::Vector2u& vec);
    sf::Vector2i to_Vector2i(const sf::Vector2f& vec);
    sf::Vector2f to_Vector2f(const sf::Vector2i& vec);
    template <typename T>
    std::string vec2to_string(const sf::Vector2<T>& vec) {
        return "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")";
    }
    std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);
    int getMemoryUsage();
    sf::Vector2f truncateToOneDecimal(const sf::Vector2f& v);
    // Случайное число со всеми числоподобными видами
    template<typename T>
    T getRandom(T min, T max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(gen);
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(gen);
        } else {
            static_assert(false, "Тип должен быть целым или вещественным");
        }
    }
}

#endif
