#include "ChatManager.hpp"

char ChatManager::inputBuffer[256] = "";

void ChatManager::sendMessage(const string& message) {
    if(!message.empty()) {
        string output = message;
        if(message[0] == '/') {
            output = command.processCommand(message, player);
        } else {
            output = player->getNickname() + ": " + output;
        }
        chatHistory.push_back(output);
    }
}

// COMMAND
map<string, ChatManager::Command::CommandHandler> ChatManager::Command::commandMap = {
    {"/help", ChatManager::Command::HandleHelp},
    {"/teleport", ChatManager::Command::HandleTeleport},
};

string ChatManager::Command::HandleHelp(const vector<string>&, const json&, Player* player) {
    ostringstream oss;
    oss << "Available commands:\n";
    for (const auto& pair : commandMap) {
        oss << "  " << pair.first << "\n";
    }
    return oss.str();
}

string ChatManager::Command::HandleTeleport(const vector<string>& args, const json&, Player* player) {
    if (args.size() < 2) return "Usage: /teleport <x> <y>";
    try {
        float x = std::stof(args[0]);
        float y = std::stof(args[1]);
        player->position = {x, y};

        return "Player " + player->getNickname() + " teleported to {" + to_string(x) + ", " + to_string(y) + "}";
    } catch (const exception& e) {
        return string("Invalid coordinates: ") + e.what();
    }
}
