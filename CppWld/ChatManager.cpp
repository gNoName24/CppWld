#include "ChatManager.hpp"
#include "ChatCommands.hpp"

char ChatManager::inputBuffer[256] = "";

void ChatManager::sendMessage(const string& message) {
    if(!message.empty()) {
        string output = message;
        if(message[0] == '/') {
            output = command.processCommand(message, player);
            chatHistory.push_back("[COMMAND] " + player->getNickname() + ": " + message);
        } else {
            output = player->getNickname() + ": " + output;
        }
        chatHistory.push_back(output);
    }
}

// COMMAND
ChatManager::Command::CommandTree ChatManager::Command::commandTree = {
    {"/help", CommandNode::WithHandler(&ChatCommands::Help,
        "Information of Commands",
        "/help <command> [subcommand]"
    )},
    {"/player",
        CommandNode::WithChildren(
            CommandTree{
                {"teleport", CommandNode::WithHandler(&ChatCommands::PlayerTeleport, "Player teleport", "/player teleport <x> <y>")},
                {"speed",
                    CommandNode::WithChildren(
                        CommandTree{
                            {"get", CommandNode::WithHandler(&ChatCommands::PlayerSpeedGet, "Player speed get", "/player speed get")},
                            {"set", CommandNode::WithHandler(&ChatCommands::PlayerSpeedSet, "Player speed set", "/player speed set <float>")}
                        },
                        "Player speed get/set",
                        "/player speed <get/set> *[args]"
                    )
                }
            },
            "Player commands",
            "/player <type> [args]"
        )
    },
    {"/global",
        CommandNode::WithChildren(
            CommandTree{
                {"set",
                    CommandNode::WithChildren(
                        CommandTree{
                            {"SSG_RenderChunkViewDistance",
                                CommandNode::WithHandler(&ChatCommands::GlobalSet_SSG_RenderChunkViewDistance, "Render view distance", "/global set SSG_RenderChunkViewDistance <int>")}
                        },
                        "Global value set",
                        "/global set <value> [args]"
                    )
                }
            },
            "Global (Settings Storage) commands",
            "/global <type> [args]"
        )
    }
};
