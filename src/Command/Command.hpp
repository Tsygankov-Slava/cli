#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include "../Flag/Flag.hpp"

using FlagsType = std::map<std::string, Flag>;
using CommandCallback = std::function<void(FlagsType &)>;

class Command {
public:
    std::string name;
    std::string description;
    std::string example;
    std::map<std::string, Flag> commandFlags;
    CommandCallback action;

public:
    Command(std::string name, std::string description, std::string example, const std::vector<Flag> &commandFlags, CommandCallback action)
        : name(std::move(name)), description(std::move(description)), example(std::move(example)), action(std::move(action)) {
        for (auto &flag : commandFlags) {
            this->commandFlags.insert(std::make_pair(flag.name, flag));
        }
    };
};
