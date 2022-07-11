#pragma once

#include "../Command/Command.hpp"

class Cli {
public:
    std::vector<std::pair<std::string, std::string>> parsedFlags;

    Cli &command(std::string name, std::string description, std::string example, FlagsVec flagsVec, std::function<void(std::vector<std::pair<std::string, std::string>>)> action);

    void parse(int argc, char **argv);
    static void printHelp(std::map<std::string, Command> &commands);
    static std::string checkIsRequired(int argc, char **argv, FlagsVec &flags);
    static std::string checkWrongFlag(int argc, char **argv, FlagsVec &flags, int start, std::string &cmd);
    static std::string flagInCommand(Command &cmd, std::string &flag);

private:
    std::map<std::string, Command> commands = {std::make_pair("help", Command("help", "Выведет справочную информацию и подскажет всевозможные команды", "", {},
                                                                              [this](std::vector<std::pair<std::string, std::string>> parsedFlags) {
                                                                                  printHelp(this->commands);}))};
};
