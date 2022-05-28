#pragma once

#include <string>
#include <utility>

class Flag {
public:
    const std::string name;
    const std::string problemName;
    const std::string description;
    const bool isRequired;
    const bool withValue;
    std::string value;
    bool isPassed = false;
    void (*action)();

public:
    Flag(std::string name, std::string problemName, std::string description, const bool isRequired, const bool withValue, void (*action)())
        : name(std::move(name)), problemName(std::move(problemName)), description(std::move(description)), isRequired(isRequired), withValue(withValue), action(action){};
};
