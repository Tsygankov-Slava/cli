#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include "../Flag/Flag.hpp"

typedef std::vector<std::string> flagsType;
typedef std::function<void(flagsType &)> function;

class Command {
public:
    std::string name;
    std::string description;
    std::string example;
    std::map<std::string, Flag> commandFlags;
    function action;

public:
    Command(std::string name, std::string description, std::string example, std::vector<Flag> commandFlags, function action)
        : name(std::move(name)), description(std::move(description)), example(std::move(example)), action(std::move(action)) {
        for (auto &flag : commandFlags) {
            this->commandFlags.insert(std::make_pair(flag.name, flag));
        }
    };
};
