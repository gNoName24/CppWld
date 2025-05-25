#include "ChatCommands.hpp"
#include "Player.hpp"

// /help
string ChatCommands::Help(const vector<string>& args, const json& jsonData, Player* player, const CommandTree& commandTree) {
    if(args.empty()) return HelpGeneral(commandTree);
    ostringstream oss;
    for(size_t i = 0; i < args.size(); ++i) {
        if(i != 0) oss << " ";
        oss << args[i];
    }
    return HelpCommand(oss.str(), commandTree);
}
string ChatCommands::HelpGeneral(const CommandTree& commandTree) {
    ostringstream oss;
    oss << "Available commands:\n";
    function<void(const CommandTree&, bool&)> printCommands;
    printCommands = [&](const CommandTree& tree, bool& lastWasText) {
        bool firstInGroup = true;
        for(const auto& [name, node] : tree) {
            bool isTree = holds_alternative<CommandNode::CommandGroup>(node->value);
            bool currentIsText = holds_alternative<CommandNode::CommandInfo>(node->value);
            if(currentIsText && !firstInGroup && lastWasText) oss << "\n";
            if(currentIsText) {
                const auto& info = get<CommandNode::CommandInfo>(node->value);
                string prefix = "   ";
                oss << prefix << name << " - " << info.description;
                lastWasText = true;
            } else if(isTree) {
                const auto& group = get<CommandNode::CommandGroup>(node->value);
                oss << "[tree]" << name << " - " << group.description << "[content]";
                bool childLastWasText = false;
                if(group.children && !group.children->empty()) printCommands(*group.children, childLastWasText);
                oss << "[/content][/tree]";
                lastWasText = false;
            }
            firstInGroup = false;
        }
    };
    bool lastWasText = false;
    printCommands(commandTree, lastWasText);

    oss << "Type '/help <command>' for detailed info";
    return oss.str();
}
string ChatCommands::HelpCommand(const string& commandPath, const CommandTree& commandTree) {
    // Разделение пути команды на части
    istringstream iss(commandPath);
    vector<string> parts{istream_iterator<string>{iss}, istream_iterator<string>{}};

    const CommandTree* currentTree = &commandTree;
    shared_ptr<CommandNode> currentNode = nullptr;
    string fullPath;

    for(size_t i = 0; i < parts.size(); ++i) {
        const string& name = parts[i];
        auto it = currentTree->find(name);
        if(it == currentTree->end()) return "Command not found: " + name;
        currentNode = it->second;

        if(!fullPath.empty()) fullPath += " ";
        fullPath += name;

        if(i == parts.size() - 1) break;

        if(holds_alternative<CommandNode::CommandGroup>(currentNode->value)) {
            const auto& group = get<CommandNode::CommandGroup>(currentNode->value);
            if(group.children && !group.children->empty()) {
                currentTree = group.children.get();
            } else { return "Command not found: " + commandPath; }
        } else { return "Command not found: " + commandPath; }
    }

    if(!currentNode) return "Command not found: " + commandPath;

    ostringstream oss;
    oss << "Help for: /" << fullPath << "\n";

    // Если обычная команда
    if(holds_alternative<CommandNode::CommandInfo>(currentNode->value)) {
        const auto& info = get<CommandNode::CommandInfo>(currentNode->value);
        oss << "Description: " << info.description << "\n";
        if(!info.usage.empty()) oss << "Usage: " << info.usage << "\n";
    }

    // Если команда с субкомандами
    if(holds_alternative<CommandNode::CommandGroup>(currentNode->value)) {
        const auto& group = get<CommandNode::CommandGroup>(currentNode->value);
        oss << "Description: " << group.description << "\n";
        if(!group.usage.empty()) oss << "Usage: " << group.usage << "\n";

        if(group.children && !group.children->empty()) {
            oss << "Subcommands:\n";

            function<void(const CommandTree&, int, const string&)> printSubcommands;
            printSubcommands = [&](const CommandTree& tree, int level, const string& prefix) {
                bool first = true;
                for(const auto& [name, node] : tree) { string indent(level * 3, ' ');
                    if(holds_alternative<CommandNode::CommandInfo>(node->value)) {
                        const auto& info = get<CommandNode::CommandInfo>(node->value);
                        oss << indent << name << " - " << info.description << "\n";
                    } else if(holds_alternative<CommandNode::CommandGroup>(node->value)) {
                        const auto& subgroup = get<CommandNode::CommandGroup>(node->value);
                        oss << indent << name << " - " << subgroup.description << "\n";
                        if(subgroup.children && !subgroup.children->empty()) {
                            printSubcommands(*subgroup.children, level + 1, prefix + name + " ");
                        }
                    }
                }
            };
            printSubcommands(*group.children, 1, fullPath + " ");
        }
    }
    return oss.str();
}

// /player
string ChatCommands::PlayerTeleport(const vector<string>& args, const json&, Player* player, const CommandTree& commandTree) {
    if(args.size() < 2) return "Usage: /teleport <x> <y>";
    try {
        player->position = {stof(args[0]), stof(args[1])};
        return "Player " + player->getNickname() + " teleported to {" + to_string(player->position.x) + ", " + to_string(player->position.y) + "}";
    } catch (const exception& e) { return string("Error: ") + e.what(); }
}

string ChatCommands::PlayerSpeedGet(const vector<string>& args, const json&, Player* player, const CommandTree& commandTree) {
    return "Player " + player->getNickname() + " speed: " + to_string(player->getSpeed());
}
string ChatCommands::PlayerSpeedSet(const vector<string>& args, const json&, Player* player, const CommandTree& commandTree) {
    if(args.size() < 1) return "Usage: /player speed set <float>";
    float old = player->getSpeed();
    try {
        player->setSpeed(stof(args[0]));
        return "Player " + player->getNickname() + " speed set from " + to_string(old) + " to " + args[0];
    } catch (const exception& e) { return string("Error: ") + e.what(); }
}

// /global
string ChatCommands::GlobalSet_SSG_RenderChunkViewDistance(const vector<string>& args, const json&, Player* player, const CommandTree& commandTree) {
    if(args.size() < 1) return "Usage: /global set SSG_RenderChunkViewDistance <int>";
    float old = SSG_RenderChunkViewDistance;
    try {
        SSG_RenderChunkViewDistance = stof(args[0]);
        return "Global Settings Storage SSG_RenderChunkViewDistance set from " + to_string(old) + " to " + args[0];
    } catch (const exception& e) { return string("Error: ") + e.what(); }
}
