#pragma once

#include <functional>
#include <map>

#include "../Flag/Flag.hpp"

class Command {
public:
    std::string name;
    std::string description;
    std::string example;
    std::map<std::string, Flag> flags;
    std::function<void()> action;

public:
    Command(std::string name, std::string description, std::string example, std::function<void()> action)
        : name(std::move(name)), description(std::move(description)), example(std::move(example)), action(std::move(action)){};

    Command &flag(std::string nameFlag, std::string shortNameFlag, std::string descriptionFlag, bool isRequiredFlag, bool withValueFlag) {
        Flag flag = Flag(nameFlag, shortNameFlag, descriptionFlag, isRequiredFlag, withValueFlag);
        flags.insert(std::make_pair(nameFlag, flag));
        return *this;
    }
};
