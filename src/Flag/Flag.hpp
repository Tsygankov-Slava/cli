#pragma once

#include <string>

class Flag {
public:
    std::string name;
    std::string shortName;
    std::string description;
    bool isRequired;
    bool withValue;
    std::string value;
    bool isPassed = true;

public:
    Flag(std::string name, std::string shortName, std::string description, bool isRequired, bool withValue)
        : name(std::move(name)), shortName(std::move(shortName)), description(std::move(description)), isRequired(isRequired), withValue(withValue){};
};