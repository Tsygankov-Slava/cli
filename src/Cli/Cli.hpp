#pragma once

#include <iomanip>
#include <vector>

#include "../Command/Command.hpp"
#include "../cli_config.h"

namespace cli {
    class Cli {
    public:
        Cli &command(const std::string &name, const std::string &description, const std::string &example, const std::vector<Flag> &commandFlag, const CommandCallback &action);

        void parse(int argc, char **argv);
        static void printAllHelp(std::map<std::string, Command> &commands);
        static void printCmdHelp(std::vector<std::string> &commandsName, std::map<std::string, Command> &commands);
        static std::string checkIsRequiredFlag(std::map<std::string, Flag> &inputFlags, std::map<std::string, Flag> &commandFlags);
        static std::string flagInCommand(std::map<std::string, Flag> &commandFlags, std::string &inputFlagName);
        static std::vector<int> getCmdSizes(std::map<std::string, Command> &commands);

    private:
        std::map<std::string, Command> commands = {std::make_pair("help", Command("help", "Выведет справочную информацию и подскажет всевозможные команды", "", {},
                                                                                  [this](FlagsType &parsedFlags) {
                                                                                      printAllHelp(this->commands);
                                                                                  }))};
    };
}