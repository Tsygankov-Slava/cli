#pragma once

#include <vector>

#include "../Command/Command.hpp"

class Cli {
private:
    int argc;
    char **argv;
    std::map<std::string, Command> commands;

public:
    Cli(int argc, char **argv)
        : argc(argc), argv(argv){};

    Command &command(std::string name, std::string description, std::string example, std::function<void()> action) {
        Command cmd = Command(name, description, example, action);
        commands.insert(std::make_pair(name, cmd));
        return commands.at(name);
    }
};