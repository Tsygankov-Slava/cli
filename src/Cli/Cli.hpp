#pragma once

#include <iomanip>
#include <type_traits>
#include <vector>

#include "../Command/Command.hpp"
#include "../cli_config.h"

namespace cli {
    extern bool nocolor;
    extern std::map<std::string, std::string> colors;

    class Cli {
    public:

        Cli &command(const std::string &name, const std::string &description, const std::string &example, const CommandCallback &action, int argumentsCount = 0, bool canContainEmptyArgumentList = false);
        Cli &command(const std::string &name, const std::string &description, const std::vector<Flag> &commandFlag, const CommandCallback &action, int argumentsCount = 0, bool canContainEmptyArgumentList = false);
        Cli &command(const std::string &name, const std::string &description, const CommandCallback &action, int argumentsCount = 0, bool canContainEmptyArgumentList = false);
        Cli &command(const std::string &name, const std::string &description, const std::string &example, const std::vector<Flag> &commandFlag, const CommandCallback &action, int argumentsCount = 0, bool canContainEmptyArgumentList = false);
        Cli &setDescriptionMaxWidth(int value = 50);

        void parse(int &argc, char **argv);
        static void printAllHelp(std::map<std::string, Command> &commands, cli::Cli &cli);
        static void printCmdHelp(std::vector<std::string> &commandsName, std::map<std::string, Command> &commands, cli::Cli &cli);
        static std::string checkIsRequiredFlag(std::map<std::string, Flag> &inputFlags, std::map<std::string, Flag> &commandFlags, cli::Cli &cli);
        static std::string flagInCommand(std::map<std::string, Flag> &commandFlags, std::string &inputFlagName);
        static std::map<std::string, int> getCmdSizes(std::map<std::string, Command> &commands);
        static void checkNocolor(cli::Cli &cli, int &argc, char **argv);
        static std::string paint(const std::string &str, const std::string &color);
        static void lineWrapping(std::string &description, int maxSize, int flagSize, cli::Cli &cli);
        void parseFlagsAndArguments(std::string &cmd, int argc, char **argv, int &i, FlagsType &commandFlags, FlagsType &parsedFlags, ArgumentsType &parsedArguments);
        std::string checkNumberOfArgumentsPassed(std::string &cmd, ArgumentsType &parsedArguments);

        template<typename T>
        static std::vector<T> convertArgumentsToType(ArgumentsType &parsedArguments) {
            std::vector<T> convertArguments;
            std::string message = cli::Cli::convert<T>(parsedArguments, convertArguments);
            if (!message.empty()) {
                throw std::invalid_argument(message);
            }
            return convertArguments;
        }

    private:
        int descriptionMaxWidth = 50;
        std::map<std::string, Command> commands = {std::make_pair("help", Command("help", "Show help information.",
                                                                                  [this](FlagsType &parsedFlags, ArgumentsType &parsedArguments) {
                                                                                      printAllHelp(this->commands, *this);
                                                                                  }))};

        static bool isNumber(const std::string &number);

        template<typename T>
        static typename std::enable_if<std::is_same<T, std::string>::value, std::string>::type convert(ArgumentsType &parsedArguments, std::vector<T> &convertedParsedArguments) {
            for (auto &argument : parsedArguments) {
                convertedParsedArguments.push_back(argument);
            }
            return "";
        }

        template<typename T>
        static typename std::enable_if<std::is_same<T, int>::value, std::string>::type convert(ArgumentsType &parsedArguments, std::vector<T> &convertedParsedArguments) {
            std::string wrongArguments, message;
            for (auto &argument : parsedArguments) {
                if (isNumber(argument)) {
                    convertedParsedArguments.push_back(std::stoi(argument));
                } else {
                    wrongArguments += "\"" + argument + "\" | ";
                }
            }
            if (!wrongArguments.empty()) {
                wrongArguments = wrongArguments.substr(0, wrongArguments.size() - 3);
                message = paint("ERROR: Command arguments must be int type \n::Wrong argument -> " + wrongArguments, "red");
            }
            return message;
        }

        template<typename T>
        static typename std::enable_if<std::is_same<T, bool>::value, std::string>::type convert(ArgumentsType &parsedArguments, std::vector<T> &convertedParsedArguments) {
            std::string wrongArguments, message;
            for (auto &argument : parsedArguments) {
                if (argument == "1" || argument == "true") {
                    convertedParsedArguments.push_back(true);
                } else if (argument == "0" || argument == "false") {
                    convertedParsedArguments.push_back(false);
                } else {
                    wrongArguments += "\"" + argument + "\" | ";
                }
            }
            if (!wrongArguments.empty()) {
                wrongArguments = wrongArguments.substr(0, wrongArguments.size() - 3);
                message = paint("ERROR: Command arguments must be bool type \n::Wrong argument -> " + wrongArguments, "red");
            }
            return message;
        }
    };
}// namespace cli