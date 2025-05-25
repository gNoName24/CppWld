#ifndef CHATCOMMANDMANAGER_HPP
#define CHATCOMMANDMANAGER_HPP

#include "../Storage.hpp"
#include "CommandTypes.hpp"
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
        using CommandHandler = CommandTypes::CommandNode::CommandHandler;
        using CommandTree = CommandTypes::CommandTree;
        using CommandNode = CommandTypes::CommandNode;

        static CommandTree commandTree;

        // Выполнение команды
        // У команд могут быть субкоманды.
        // Субкоманды при просмотре команды с субкомандами будут называться не субкомандами, а командами, и у них тоже есть возможность иметь свои субкоманды
        string processCommand(const string& input, Player* player) {
            istringstream stream(input);
            vector<string> tokens;
            string token, jsonStr;
            while(stream >> token) {
                if(token.front() == '{') {
                    jsonStr = token;
                    string next;
                    while(stream >> next) jsonStr += " " + next;
                    break;
                }
                tokens.push_back(token);
            }

            if(tokens.empty() || tokens[0][0] != '/') return "Invalid command - must start with /";

            json jsonData;
            if(!jsonStr.empty()) {
                try { jsonData = json::parse(jsonStr);
                } catch (const exception& e) { return string("Parsing JSON Error: ") + e.what(); }
            }

            const CommandTree* currentTree = &commandTree;
            shared_ptr<CommandNode> currentNode;
            CommandHandler handler = nullptr;
            size_t i = 0;

            for(; i < tokens.size(); ++i) {
                auto it = currentTree->find(tokens[i]);
                if(it == currentTree->end()) return "Unknown command or subcommand: " + tokens[i];

                currentNode = it->second;

                if(holds_alternative<CommandNode::CommandInfo>(currentNode->value)) {
                    handler = get<CommandNode::CommandInfo>(currentNode->value).handler;
                    ++i; // всё остальное - аргументы
                    break;
                } else if(holds_alternative<CommandNode::CommandGroup>(currentNode->value)) {
                    currentTree = get<CommandNode::CommandGroup>(currentNode->value).children.get();
                } else { return "Incomplete command at: " + tokens[i]; }
            }

            if(!handler) return "Incomplete command or no handler defined.";

            vector<string> handlerArgs(tokens.begin() + i, tokens.end());
            return handler(handlerArgs, jsonData, player, commandTree);
        }
    };
    Command command;

    bool chatOpened = false;
    bool chatFocus = false;

    static char inputBuffer[256];

    vector<string> chatHistory;
    vector<string> chatEnterHistory;
    int currentEnterHistoryTab = 0;
    bool chatEnterHistoryOn = false;

    // TAB - 60
    // UP - 73
    // DOWN - 74

    void keyEvent(const sf::Keyboard::Key& keyCode, const bool& pressed) {
        if(pressed) {
            int code = static_cast<int>(keyCode);
            if(code == 60) { // TAB
                chatOpened = !chatOpened;
            }
            if(keyCode == sf::Keyboard::Key::Up || keyCode == sf::Keyboard::Key::Down) {
                if(!chatEnterHistory.empty()) {
                    chatEnterHistoryOn = true;
                    int direction = (keyCode == sf::Keyboard::Key::Up) ? 1 : -1;
                    currentEnterHistoryTab += direction;

                    if(currentEnterHistoryTab < 0) { currentEnterHistoryTab = chatEnterHistory.size() - 1; }
                    else if(currentEnterHistoryTab >= chatEnterHistory.size()) { currentEnterHistoryTab = 0; }

                    //println(1, "size: "+to_string(chatEnterHistory.size()-1)+" / currentEnterHistoryTab"+to_string(currentEnterHistoryTab));
                    strcpy(inputBuffer, chatEnterHistory[currentEnterHistoryTab].c_str());
                }
            }
        }
    }

    void sendMessage(const string& message);

    // "[tree]Заголовок[content]Текст[/content][/tree]"
    struct TreeNode {
        std::string title;
        std::string content;
        std::vector<TreeNode> children;
    };

    TreeNode parseTree(const std::string& msg, size_t& pos) {
        TreeNode node;
        size_t contentStart = msg.find("[content]", pos);
        if(contentStart == std::string::npos) {
            pos = msg.length();
            return node;
        }

        // Извлечение заголовка
        node.title = msg.substr(pos, contentStart - pos);
        pos = contentStart + 9; // [content]

        while(pos < msg.length()) {
            size_t nextTree = msg.find("[tree]", pos);
            size_t contentEnd = msg.find("[/content]", pos);

            size_t nextTag = std::min(nextTree, contentEnd);

            if(nextTag == std::string::npos) {
                node.content += msg.substr(pos);
                pos = msg.length();
                break;
            }
            if(nextTag > pos) { node.content += msg.substr(pos, nextTag - pos); }

            if(nextTag == nextTree) {
                pos = nextTree + 6; // [tree]
                node.children.push_back(parseTree(msg, pos));
                size_t treeEnd = msg.find("[/tree]", pos);
                if(treeEnd != std::string::npos) { pos = treeEnd + 7; } // [/tree]
            } else if(nextTag == contentEnd) { pos = contentEnd + 10; break; } // [/content]
        }
        return node;
    }

    void renderTreeNode(const TreeNode& node, int level = 0, int& nodeId = *(new int(0))) {
        if(node.title.empty() && node.content.empty()) return;

        ImGui::PushID(nodeId++); // Уникальный айдишник

        if(!node.title.empty()) {
            bool open = ImGui::TreeNodeEx(node.title.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
            if(open) {
                if(!node.content.empty())
                    ImGui::TextUnformatted(node.content.c_str());
                for(const auto& child : node.children)
                    renderTreeNode(child, level + 1, nodeId);
                ImGui::TreePop();
            }
        } else if(!node.content.empty()) { ImGui::TextUnformatted(node.content.c_str()); }

        ImGui::PopID();
    }

    void renderChat() {
        if(!chatOpened) return;

        ImGui::Begin("Chat");

        static bool enableTreeView = true;
        ImGui::Checkbox("Enable Tree View", &enableTreeView);

        bool newMessage = false;
        if(chatEnterHistoryOn) {
            ImGui::SetKeyboardFocusHere(-1);
            chatEnterHistoryOn = false;
        }
        if(ImGui::InputText("ChatField", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            chatEnterHistory.push_back(string(inputBuffer));
            sendMessage(string(inputBuffer));
            memset(inputBuffer, 0, sizeof(inputBuffer)); // Очистка
            newMessage = true;
        }
        chatFocus = ImGui::IsItemFocused();

        ImGui::BeginChild("ChatMessages");

        if(enableTreeView) {
            int uniqueTreeId = 0;
            for(const auto& msg : chatHistory) {
                size_t pos = 0;
                while(pos < msg.length()) {
                    size_t treeStart = msg.find("[tree]", pos);
                    if(treeStart == std::string::npos) {
                        ImGui::TextUnformatted(msg.substr(pos).c_str());
                        break;
                    }
                    if(treeStart > pos)
                        ImGui::TextUnformatted(msg.substr(pos, treeStart - pos).c_str());

                    pos = treeStart + 6;
                    TreeNode root = parseTree(msg, pos);
                    renderTreeNode(root, 0, uniqueTreeId);

                    size_t treeEnd = msg.find("[/tree]", pos);
                    if(treeEnd != std::string::npos) { pos = treeEnd + 7; }
                }
            }
        } else { for(const auto& msg : chatHistory) ImGui::TextUnformatted(msg.c_str()); }

        if(newMessage && !chatHistory.empty()) ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }

private:
    Player* player;
};

#endif
