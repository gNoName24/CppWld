#ifndef COMMAND_TYPES_HPP
#define COMMAND_TYPES_HPP

#include "../Storage.hpp"

using namespace std;

using json = nlohmann::json;

class Player;

namespace CommandTypes {
    struct CommandNode;
    using CommandTree = map<string, shared_ptr<CommandNode>>;
    struct CommandNode {
        using CommandHandler = function<string(const vector<string>&, const json&, Player*, const CommandTree& commandTree)>;

        struct CommandInfo {
            CommandHandler handler;
            string description;
            string usage;
        };

        struct CommandGroup {
            shared_ptr<map<string, shared_ptr<CommandNode>>> children;
            string description;
            string usage;
        };

        variant<monostate, CommandInfo, CommandGroup> value;

        static shared_ptr<CommandNode> WithHandler(CommandHandler handler, string description = "", string usage = "") {
            auto node = make_shared<CommandNode>();
            node->value = CommandInfo{handler, description, usage};
            return node;
        }

        static shared_ptr<CommandNode> WithChildren(
            map<string, shared_ptr<CommandNode>>&& children,
            string description = "",
            string usage = "") {
            auto node = make_shared<CommandNode>();
            node->value = CommandGroup{
                make_shared<map<string, shared_ptr<CommandNode>>>(move(children)),
                description,
                usage
            };
            return node;
        }
    };
}

#endif
