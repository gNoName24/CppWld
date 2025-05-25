#ifndef CHATCOMMANDS_HPP
#define CHATCOMMANDS_HPP

#include "../Storage.hpp"
#include "CommandTypes.hpp"

class Player;

using namespace std;

class ChatCommands {
public:
    using CommandTree = CommandTypes::CommandTree;
    using CommandNode = CommandTypes::CommandNode;
    using CommandHandler = CommandNode::CommandHandler;

    // /help
    static string Help(const vector<string>& args, const json&, Player* player, const CommandTree& commandTree);
    static string HelpGeneral(const CommandTree& commandTree); // /help
    static string HelpCommand(const string& commandPath, const CommandTree& commandTree); // /help <command> [subcommand]

    // /player
    static string PlayerTeleport(const vector<string>& args, const json&, Player* player, const CommandTree& commandTree); // /player teleport <x> <y>

    static string PlayerSpeedGet(const vector<string>& args, const json&, Player* player, const CommandTree& commandTree); // /player speed set <float>
    static string PlayerSpeedSet(const vector<string>& args, const json&, Player* player, const CommandTree& commandTree); // /player speed get

    // /global
    static string GlobalSet_SSG_RenderChunkViewDistance(const vector<string>& args, const json&, Player* player, const CommandTree& commandTree); // /global set SSG_RenderChunkViewDistance <int>
};

#endif
