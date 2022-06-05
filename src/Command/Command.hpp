#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include "../Flag/Flag.hpp"

struct FlagsVec {
    std::vector<Flag> flags;
};

class Command {
public:
    std::string name;
    std::string description;
    std::string example;
    FlagsVec flagsVec;
    std::map<std::string, Flag> flags;
    std::function<void(std::vector<std::pair<std::string, std::string>>)> action;

public:
    Command(std::string name, std::string description, std::string example, FlagsVec flagsVec, std::function<void(std::vector<std::pair<std::string, std::string>>)> action)
        : name(std::move(name)), description(std::move(description)), example(std::move(example)), flagsVec(std::move(flagsVec)), action(std::move(action)){};

};
