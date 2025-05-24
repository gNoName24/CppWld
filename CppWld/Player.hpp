#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../Storage.hpp"

class ChatManager;

using namespace std;
using namespace TlMain;
using namespace TlConsole;

class Player {
public:
    Player();

    float fieldOfView = 30;

    sf::Vector2f position = {0.f, 0.f};
    sf::Vector2i positionChunk = {-1, -1};

    void setNickname(const string& newNickname) {
        nickname = newNickname;
    }
    const string& getNickname() const {
        return nickname;
    }

    // CHAT
    std::unique_ptr<ChatManager> chat;
    ChatManager* getChat() const;

    // CONTROLLER
    std::unordered_map<int, bool> movementKeys;
    // W - 22
    // S - 18
    // A - 0
    // D - 3
    void controller(const sf::Keyboard::Key& keyCode, const bool& pressed) {
        movementKeys[static_cast<int>(keyCode)] = pressed;
    }
    void controllerTick() {
        position = truncateToOneDecimal(position);
        if(movementKeys[22]) { position.y += movementSpeed; }
        if(movementKeys[18]) { position.y -= movementSpeed; }
        if(movementKeys[0]) { position.x -= movementSpeed; }
        if(movementKeys[3]) { position.x += movementSpeed; }
    }
private:
    string nickname = "Untitled";
protected:
    float movementSpeed = 1.4;
};

#endif
