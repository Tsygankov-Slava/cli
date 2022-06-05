#pragma once

#include "../Command/Command.hpp"

class Cli {
public:
    std::vector<std::pair<std::string, std::string>> parsedFlags;
    Cli &command(std::string name, std::string description, std::string example, FlagsVec flagsVec, std::function<void(std::vector<std::pair<std::string, std::string>>)> action) {
        Command cmd = Command(name, description, example, flagsVec, action);
        for (auto &flag : flagsVec.flags) {
            cmd.flags.insert(std::make_pair(flag.name, flag));
        }
        commands.insert(std::make_pair(name, cmd));
        return *this;
    }

    void parse(int argc, char **argv) {
        std::string cmd = "";
        for (int i = 1; i < argc+1; ++i) {
            if (i == argc) {
                auto func = commands.at(cmd).action;
                func(parsedFlags);
                break;
            }
            std::string str = argv[i];
            if (commands.contains(str)) {
                if (cmd != "") {
                    auto func = commands.at(cmd).action;
                    func(parsedFlags);
                }
                cmd = str;
                continue;
            }
            if (commands.at(cmd).flags.contains(str)) {
                std::string value = "";
                auto flag = commands.at(cmd).flags.at(str);
                if (flag.withValue) {
                    value = argv[++i];
                }
                parsedFlags.emplace_back(std::make_pair(flag.name, value));
            }
        }
    }

    void printHelp(std::map<std::string, Command> &commands) {
        for (auto &cmd : commands) {
            std::cout << cmd.first << " : " << cmd.second.description << "\n";
            for (auto &flag : cmd.second.flags) {
                std::cout << "    " << flag.first << "  " << flag.second.shortName << " : " << flag.second.description << "\n";
            }
            std::cout <<"Example: " <<  cmd.second.example << "\n";
        }
    }


private:
    std::map<std::string, Command> commands = {std::make_pair("help", Command("help", "description", "example", {},
                                                                              [this](std::vector<std::pair<std::string, std::string>> parsedFlags) {
                                                                                  printHelp(this->commands);
                                                                              }))};
};