#include "Player.hpp"

#include "ChatManager.hpp"

Player::Player() {
    chat = std::make_unique<ChatManager>(this);
    println(1, "Новый игрок зарегистрирован");
    movementKeys[22] = false;
    movementKeys[18] = false;
    movementKeys[0] = false;
    movementKeys[3] = false;
}

ChatManager* Player::getChat() const {
    return chat.get();
}
