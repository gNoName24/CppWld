#ifndef CHATCOMMANDMANAGER_HPP
#define CHATCOMMANDMANAGER_HPP

#include "../Storage.hpp"
#include "Player.hpp"

using namespace std;
using namespace TlMain;
using namespace TlConsole;

using json = nlohmann::json;

class ChatManager {
public:
    explicit ChatManager(Player* owner) : player(owner) { }

    class Command {
    public:
        using CommandHandler = string(*)(const vector<string>&, const json&, Player*);
        static map<string, CommandHandler> commandMap;

        static string HandleHelp(const vector<string>&, const json&, Player* player);
        static string HandleTeleport(const vector<string>& args, const json&, Player* player);

        string processCommand(const string& input, Player* player) {
            istringstream stream(input);
            string token;
            string command;
            vector<string> args;
            string jsonStr;

            while (stream >> token) {
                if (token[0] == '/' && command.empty()) { command = token;
                } else if (token.front() == '{') {
                    jsonStr = token;
                    string next;
                    while (stream >> next) jsonStr += " " + next;
                    break;
                } else { args.push_back(token); }
            }

            json jsonData;
            if (!jsonStr.empty()) {
                try { jsonData = json::parse(jsonStr);
                } catch (const exception& e) { return string("Parsing JSON Error: ") + e.what(); }
            }

            auto it = commandMap.find(command);
            if (it != commandMap.end()) { return it->second(args, jsonData, player);
            } else { return "Unknown Command: " + command; }
        }
    };
    Command command;

    bool chatOpened = false;
    bool chatFocus = false;

    static char inputBuffer[256];

    vector<string> chatHistory;

    // TAB - 60

    void keyEvent(const sf::Keyboard::Key& keyCode, const bool& pressed) {
        if(pressed) {
            if(static_cast<int>(keyCode) == 60) { // TAB
                chatOpened = !chatOpened;
            }
        }
    }

    void sendMessage(const string& message);

    void renderChat() {
        if(chatOpened) {
            ImGui::Begin("Chat");

            bool newMessage = false;

            if(ImGui::InputText("ChatField", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                sendMessage(string(inputBuffer));
                std::memset(inputBuffer, 0, sizeof(inputBuffer));
                newMessage = true;
            }
            chatFocus = ImGui::IsItemActive();

            // Чат
            ImGui::BeginChild("ChatMessages", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            for(const auto& message : chatHistory) {
                ImGui::TextUnformatted(message.c_str());
            }

            if(!chatHistory.empty() && newMessage) {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::EndChild();

            ImGui::End();
        }
    }

private:
    Player* player;
    /*static void HandleHelp(const std::vector<std::string>& args, const json& jsonData) {
        std::cout << "Available commands: /help, /say, /echo\n";
    }

    static void HandleSay(const std::vector<std::string>& args, const json& jsonData) {
        std::cout << "You said: ";
        for (const auto& arg : args) std::cout << arg << " ";
        std::cout << "\n";
    }

    static void HandleEcho(const std::vector<std::string>& args, const json& jsonData) {
        std::cout << "Echo JSON: " << jsonData.dump(2) << "\n";
    }

    using CommandHandler = void(*)(const std::vector<std::string>&, const json&);

    std::map<std::string, CommandHandler> commandMap = {
        {"/help", HandleHelp},
        {"/say", HandleSay},
        {"/echo", HandleEcho},
    };



    // GUI
    std::string commandBuffer;
    std::vector<std::string> commandHistory;

    void DrawCommandInterface(sf::RenderWindow& window) {
        const float height = 80.0f;
        const float width = static_cast<float>(window.getSize().x);

        ImGui::SetNextWindowSize(ImVec2(width, height));
        ImGui::SetNextWindowPos(ImVec2(0, window.getSize().y - height));

        ImGui::Begin("Command Console", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse
        );

        static char inputBuffer[2048] = "";

        // Сделать поле на всю ширину
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

        if (ImGui::InputText("##input", inputBuffer, sizeof(inputBuffer),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            std::string commandBuffer = inputBuffer;

            if (!commandBuffer.empty()) {
                ProcessCommand(commandBuffer);
                commandHistory.push_back(commandBuffer);
                commandBuffer.clear();
                std::memset(inputBuffer, 0, sizeof(inputBuffer));
            }
        }

        ImGui::End();
    }*/
};

#endif
