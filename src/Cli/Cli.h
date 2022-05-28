#pragma once

#include <iostream>
#include <map>
#include <set>

#include "../Flag/Flag.h"

enum State { EXIT,
             WRONG,
             OK };

class Cli {
    int argc;
    char **argv;
    std::map<std::string, Flag> flags;
    std::set<std::string> arrAllProblemName;
    std::set<std::string> arrProblemName;

public:
    Cli(int argc, char **argv) : argc(argc), argv(argv){};

    Cli &flag(const std::string &name, const std::string &problemName = "", const std::string &description = "", const bool isRequired = false, const bool withValue = false, void (*action)() = nullptr) {
        (void) flags.insert(std::make_pair(name, Flag(name, problemName, description, isRequired, withValue, action)));
        (void) arrAllProblemName.insert(problemName);
        return *this;
    }

    State parseFlags();
    State checkRequiredFlags();
};
