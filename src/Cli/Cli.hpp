#pragma once

#include "../Command/Command.hpp"

class Cli {
public:
    std::vector<std::pair<std::string, std::string>> parsedFlags;

    Cli &command(std::string name, std::string description, std::string example, FlagsVec flagsVec, std::function<void(std::vector<std::pair<std::string, std::string>>)> action);

    void parse(int argc, char **argv);
    void printHelp(std::map<std::string, Command> &commands);


private:
    std::map<std::string, Command> commands = {std::make_pair("help", Command("help", "Выведет справочную информацию и подскажет всевозможные команды", "", {},
                                                                              [this](std::vector<std::pair<std::string, std::string>> parsedFlags) {
                                                                                  printHelp(this->commands);}))};
};