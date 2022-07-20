#pragma once

#include "../Command/Command.hpp"

class Cli {
public:
    Cli &command(const std::string &name, const std::string &description, const std::string &example, const std::vector<Flag> &commandFlag, const CommandCallback &action);

    void parse(int argc, char **argv);
    static void printHelp(std::map<std::string, Command> &commands);
    static std::string checkIsRequiredFlag(std::map<std::string, Flag> &inputFlags, std::map<std::string, Flag> &commandFlags);
    static std::string flagInCommand(std::map<std::string, Flag> &commandFlags, std::string &flag);

private:
    std::map<std::string, Command> commands = {std::make_pair("help", Command("help", "Выведет справочную информацию и подскажет всевозможные команды", "", {},
                                                                              [this](FlagsType &parsedFlags) { printHelp(this->commands); }))};
};
