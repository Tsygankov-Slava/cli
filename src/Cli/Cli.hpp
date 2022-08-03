#pragma once

#include <iomanip>
#include <vector>

#include "../Command/Command.hpp"
#include "../cli_config.h"

namespace cli {
    class Cli {
    public:
        bool nocolor = false;
        std::map<std::string, std::string> colors{
                {"red", "\x1B[31m"},
                {"green", "\x1B[32m"},
                {"yellow", "\x1B[33m"},
                {"blue", "\x1B[34m"},
                {"white", "\x1B[37m"}};
        int lineSizeOfDescription = 50;

        Cli &command(const std::string &name, const std::string &description, const std::string &example, const std::vector<Flag> &commandFlag, const CommandCallback &action);

        void parse(int &argc, char **argv);
        static void printAllHelp(std::map<std::string, Command> &commands, cli::Cli &cli);
        static void printCmdHelp(std::vector<std::string> &commandsName, std::map<std::string, Command> &commands, cli::Cli &cli);
        static std::string checkIsRequiredFlag(std::map<std::string, Flag> &inputFlags, std::map<std::string, Flag> &commandFlags, cli::Cli &cli);
        static std::string flagInCommand(std::map<std::string, Flag> &commandFlags, std::string &inputFlagName);
        static std::map<std::string, int> getCmdSizes(std::map<std::string, Command> &commands);
        static std::pair<int, char**> checkNocolor(cli::Cli &cli, int& argc, char **argv);
        static std::string paint(const std::string &str, const std::string &color, cli::Cli &cli);
        static void lineWrapping(std::string& description, int maxSize, int flagSize, cli::Cli &cli);

    private:
        std::map<std::string, Command> commands = {std::make_pair("help", Command("help", "Show help information.", "", {},
                                                                                  [this](FlagsType &parsedFlags) {
                                                                                      printAllHelp(this->commands, *this);
                                                                                  }))};
    };
}// namespace cli