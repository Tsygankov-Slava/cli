#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include "../Flag/Flag.hpp"

class Command {
public:
    std::string name;
    std::string description;
    std::string example;
    std::map<std::string, Flag> commandFlags;
    std::function<void(std::vector<std::pair<std::string, std::string>>)> action;

public:
    Command(std::string name, std::string description, std::string example, std::vector<Flag> commandFlags, std::function<void(std::vector<std::pair<std::string, std::string>>)> action)
        : name(std::move(name)), description(std::move(description)), example(std::move(example)), action(std::move(action)) {
        for (auto &flag : commandFlags) {
            this->commandFlags.insert(std::make_pair(flag.name, flag));
        }
    };
};
