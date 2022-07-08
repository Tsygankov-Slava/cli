#include "Cli.hpp"

Cli &Cli::command(std::string name, std::string description, std::string example, FlagsVec flagsVec, std::function<void(std::vector<std::pair<std::string, std::string>>)> action) {
    Command cmd = Command(name, description, example, flagsVec, action);
    for (auto &flag : flagsVec.flags) {
        cmd.flags.insert(std::make_pair(flag.name, flag));
    }
    commands.insert(std::make_pair(name, cmd));
    return *this;
}

void Cli::parse(int argc, char **argv) {
    std::string cmd;
    for (int i = 1; i < argc+1; ++i) {
        bool isCmdOrFlag = false;
        if (i == argc) {
            if (commands.contains(cmd)) {
                isCmdOrFlag = true;
                auto func = commands.at(cmd).action;
                func(parsedFlags);
            }
            break;
        }
        std::string str = argv[i];
        if (commands.contains(str)) {
            isCmdOrFlag = true;
            if (!cmd.empty()) {
                auto func = commands.at(cmd).action;
                func(parsedFlags);
            }
            cmd = str;
            continue;
        }
        if (commands.contains(cmd)) {
            if (commands.at(cmd).flags.contains(str)) {
                isCmdOrFlag = true;
                std::string value;
                auto flag = commands.at(cmd).flags.at(str);
                if (flag.withValue) {
                    value = argv[++i];
                }
                parsedFlags.emplace_back(std::make_pair(flag.name, value));
            }
        }
        if (!isCmdOrFlag) {
            if (str[0] == '-') {
                throw std::invalid_argument("\033[31mERROR: Неизвестный флаг -> " + str);
            } else {
                throw std::invalid_argument("\033[31mERROR: Неизвестная команда -> " + str);
            }
        }
    }
}

void Cli::printHelp(std::map<std::string, Command> &commands) {
    std::cout << "\033[1;37m\033[1mPossible commands:\n";
    for (auto &cmd : commands) {
        std::cout << "\033[33m" << cmd.first << " \033[0m: " << cmd.second.description << "\n";
        if (!cmd.second.flags.empty()) {
            std::cout << "\033[34mPossible flags:\n";
        }
        for (auto &flag : cmd.second.flags) {
            std::cout << "\t\033[32m--" << flag.first;
            if (!flag.second.shortName.empty()) {
                std::cout << " \033[0mOR \033[32m-" << flag.second.shortName;
            }
            std::cout << " \033[0m: " << flag.second.description << "\n";
        }
        if (!cmd.second.example.empty()) {
            std::cout << "\033[34mEXAMPLE:\033[0m\n" << cmd.second.example << "\n";
        }
        std::cout << "\n";
    }
}