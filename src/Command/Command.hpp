#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include "../Flag/Flag.hpp"

namespace cli {

    using FlagsType = std::map<std::string, Flag>;
    using ArgumentsType = std::vector<std::string>;
    using CommandCallback = std::function<void(FlagsType &, ArgumentsType &)>;

    class Command {
    public:
        std::string name;
        std::string description;
        std::string example;
        std::map<std::string, Flag> flags;
        CommandCallback action;
        int argumentsCount;
        bool canContainEmptyArgumentList;

    public:
        //cli.command("Name", "Description", "Example", action, argumentsCount, canContainEmptyArgumentList)
        Command(std::string name, std::string description, std::string example, CommandCallback action, int argumentsCount = 0, bool canContainEmptyArgumentList = false)
            : name(std::move(name)), description(std::move(description)), example(std::move(example)), action(std::move(action)), argumentsCount(argumentsCount), canContainEmptyArgumentList(canContainEmptyArgumentList) {};

        //cli.command("Name", "Description", {Flags}, action, argumentsCount, canContainEmptyArgumentList)
        Command(std::string name, std::string description, const std::vector<Flag> &flags, CommandCallback action, int argumentsCount = 0, bool canContainEmptyArgumentList = false)
            : name(std::move(name)), description(std::move(description)), action(std::move(action)), argumentsCount(argumentsCount), canContainEmptyArgumentList(canContainEmptyArgumentList) {
            for (auto &flag : flags) {
                this->flags.insert(std::make_pair(flag.name, flag));
            }
        };

        //cli.command("Name", "Description", action, argumentsCount, canContainEmptyArgumentList)
        Command(std::string name, std::string description, CommandCallback action, int argumentsCount = 0, bool canContainEmptyArgumentList = false)
            : name(std::move(name)), description(std::move(description)), action(std::move(action)), argumentsCount(argumentsCount), canContainEmptyArgumentList(canContainEmptyArgumentList) {};

        // cli.command("Name", "Description", "Example", {Flags}, action, argumentsCount, canContainEmptyArgumentList)
        Command(std::string name, std::string description, std::string example, const std::vector<Flag> &flags, CommandCallback action, int argumentsCount = 0, bool canContainEmptyArgumentList = false)
            : name(std::move(name)), description(std::move(description)), example(std::move(example)), action(std::move(action)), argumentsCount(argumentsCount), canContainEmptyArgumentList(canContainEmptyArgumentList) {
            for (auto &flag : flags) {
                this->flags.insert(std::make_pair(flag.name, flag));
            }
        };
    };
}// namespace cli
