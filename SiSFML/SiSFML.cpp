#include "SiSFML.hpp"

using namespace std;
using namespace TlMain;
using namespace TlConsole;

int localFrameCount = 0;

void SiSFML::valuesUpdate() {
    win.setTitle(windowName);

    frameCount++;

    localFrameCount++;
    if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
        fps = localFrameCount / fpsClock.getElapsedTime().asSeconds();
        fpsClock.restart();
        localFrameCount = 0;
    }

    display = to_Vector2i(win.getSize());
    // println(1, "valuesUpdate "+vec2to_string(SiSFML::display));
}

void SiSFML::run() {
    println(1, "Run() (ID: "+winID+")");
    sf::Clock deltaClock;
    win.setKeyRepeatEnabled(false);
    while (win.isOpen()) {
        valuesUpdate();
        while(auto event = win.pollEvent()) {
            ImGui::SFML::ProcessEvent(win, *event);
            if(event->is<sf::Event::Closed>()) {
                win.close();
                println(1, "Окно закрыто (ID: "+winID+")");
            }
            else if(const sf::Event::MouseButtonPressed* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                mouseTouchEvent(mousePress->button, mousePress->position);
            }
            else if (const sf::Event::KeyPressed* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                keyboardEvent(keyPress->code, true);
            }
            else if (const sf::Event::KeyReleased* keyRelease = event->getIf<sf::Event::KeyReleased>()) {
                keyboardEvent(keyRelease->code, false);
            }
            else if(auto resize = event->getIf<sf::Event::Resized>()) {
                sf::Vector2u newSize(resize->size.x, resize->size.y);
                println(1, "Изменение размера окна -> X: " + std::to_string(newSize.x) + ", Y: " + std::to_string(newSize.y) + " (ID: "+winID+")");
                sf::View view = win.getView();
                view.setSize(sf::Vector2f(newSize));
                view.setCenter(sf::Vector2f(newSize.x / 2.f, newSize.y / 2.f));
                win.setView(view);
            }
        }
        ImGui::SFML::Update(win, deltaClock.restart());

        /*ImGui::Begin("Debug");
        ImGui::Text("FPS: %.1f", fps);
        ImGui::End();*/

        win.clear();
        if(customFont) ImGui::PushFont(font);
        render();
        if(customFont) ImGui::PopFont();
        ImGui::SFML::Render(win);
        win.display();
    }
}

// Вспомогательный функции окна
void SiSFML::setWindowName(const string& newName) {
    if (updateClock.getElapsedTime().asSeconds() >= (1.0f / 60.0f)) {
        windowName = newName;
        win.setTitle(windowName);
        updateClock.restart();
    }
}
void SiSFML::setFont(const string& path, const float size) {
    font = io.Fonts->AddFontFromFileTTF(path.c_str(), size, nullptr, io.Fonts->GetGlyphRangesCyrillic());
}

// Рендерные функции окна

// БЕЗ ОПТИМИЗАЦИИ
sf::RectangleShape SiSFML::rect(const sf::Vector2f& position, const sf::Vector2f& size) const {
    sf::RectangleShape rect(size);
    rect.setPosition(position);
    return rect;
}

// С ОПТИМИЗАЦИИ
sf::RectangleShape SiSFML::srect(const sf::Vector2f& startPosition, const sf::Vector2f& size) const {
    sf::Vector2f endPosition = {((display.x/2)-(size.x/2))+startPosition.x, ((display.y/2)-(size.y/2))-startPosition.y};
    sf::RectangleShape srect(size);
    srect.setPosition(endPosition);
    return srect;
}
