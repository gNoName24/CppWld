#ifndef SISFML_HPP
#define SISFML_HPP

#include "../Storage.hpp"

using namespace std;

class SiSFML {
protected:
    string winID;

public:
    sf::RenderWindow win;
    ImGuiIO io;
    SiSFML(const string& ID, unsigned int width, unsigned int height) {
        winID = ID;
        win.create(sf::VideoMode({width, height}), winID);
        /* win.create(sf::VideoMode::getDesktopMode(), winID);
        win.setPosition(sf::Vector2i(0,0)); */
        //if(!ImGui::SFML::Init(win)) cout << "INIT WIN ERROR, ID: " << winID << endl;

        //io = ImGui::GetIO();
        //io.IniFilename = nullptr;
        /*// ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16.5f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
        font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\courer.fon", 14.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        ImGui::SFML::UpdateFontTexture();*/

        win.setFramerateLimit(60);
    }

    virtual ~SiSFML() {}

    void initImGui() {
        if (!ImGui::SFML::Init(win)) {
            cout << "INIT WIN ERROR, ID: " << winID << endl;
        }

        io = ImGui::GetIO();
        io.IniFilename = nullptr;

        // font = io.Fonts->AddFontFromFileTTF(...);
        // ImGui::SFML::UpdateFontTexture();
    }

    // Важные переменные окна
    string windowName;
    int windowEvent = 0;

    sf::Clock clock; // Нельзя перезагружать

    sf::Clock updateClock;

    int frameCount = 0;

    sf::Clock fpsClock;
    float fps = 0.0f;

    bool customFont = false;
    ImFont* font;

    // Вспомогательный функции окна
    void setWindowName(const string& newName);
    void setFont(const string& path, const float size);
    void setWindowParent(const HWND& parent);

    // Вспомогательные переменные
    sf::Vector2i display;

    void valuesUpdate();

    void run();

    // Рендерные функции окна

    // БЕЗ ОПТИМИЗАЦИИ
    sf::RectangleShape rect(const sf::Vector2f& position, const sf::Vector2f& size) const;

    // С ОПТИМИЗАЦИЕЙ
    sf::RectangleShape srect(const sf::Vector2f& position, const sf::Vector2f& size) const;

protected:
    virtual void render() {}
    virtual void mouseTouchEvent(const sf::Mouse::Button& buttonType, const sf::Vector2i& mousePressPosition) {}
    virtual void keyboardEvent(sf::Keyboard::Key keyCode, bool pressed) {}
};

#endif
