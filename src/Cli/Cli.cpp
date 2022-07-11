#include "Cli.hpp"

Cli &Cli::command(std::string name, std::string description, std::string example, FlagsVec flagsVec, std::function<void(std::vector<std::pair<std::string, std::string>>)> action) {
    Command cmd = Command(name, description, example, flagsVec, action);
    for (auto &flag : flagsVec.flags) {
        cmd.flags.insert(std::make_pair(flag.name, flag));
    }
    commands.insert(std::make_pair(name, cmd));
    return *this;
}

std::string Cli::checkIsRequired(int argc, char **argv, FlagsVec &flags) {
    for (auto &flag : flags.flags) {
        if (flag.isRequired) {
            bool isPassed = false;
            for (int i = 1; i < argc; i++) {
                if (argv[i] == "--" + flag.name || argv[i] == '-' + flag.shortName) {
                    isPassed = true;
                }
            }
            if (!isPassed) {
                return "\033[31mERROR: Не введён обязательный флаг -> --" + flag.name + " OR -" + flag.shortName;
            }
        }
    }
    return "";
}

std::string Cli::checkWrongFlag(int argc, char **argv, FlagsVec &flags, int start, std::string &cmd) {
    for (int i = start; i < argc; ++i) {
        bool isWrong = true;
        std::string inputFlag = argv[i];
        if (inputFlag[0] != '-') {
            break;
        }
        for (auto &flag : flags.flags) {
            if (inputFlag == "--" + flag.name || inputFlag == '-' + flag.shortName) {
                isWrong = false;
                break;
            }
        }
        if (isWrong) {
            return "\033[31mERROR: Введён неизвестный флаг для команды \"" + cmd + "\" -> " + inputFlag;
        }
    }
    return "";
}

std::string Cli::flagInCommand(Command &cmd, std::string &flag) {
    for (auto &f : cmd.flagsVec.flags) {
        if ("--" + f.name == flag || '-' + f.shortName == flag) {
            return f.name;
        }
    }
    return "";
}

void Cli::parse(int argc, char **argv) {
    std::string cmd;
    for (int i = 1; i < argc+1; ++i) {
        bool isCmdOrFlag = false;
        if (i == argc) {
            if (commands.contains(cmd)) {
                isCmdOrFlag = true;
                auto func = commands.at(cmd).action;
                if (func) {
                    func(parsedFlags);
                }
            }
            break;
        }
        std::string str = argv[i];
        if (commands.contains(str)) {
            isCmdOrFlag = true;
            if (!cmd.empty()) {
                auto func = commands.at(cmd).action;
                if (func) {
                    func(parsedFlags);
                }
            }
            cmd = str;
        }
        if (commands.contains(cmd)) {
            auto message = checkIsRequired(argc, argv, commands.at(cmd).flagsVec);
            if (!message.empty()) {
                throw std::invalid_argument(message);
            }
            message = checkWrongFlag(argc, argv, commands.at(cmd).flagsVec, i, cmd);
            if (!message.empty()) {
                throw std::invalid_argument(message);
            }
            auto command = commands.at(cmd);
            auto s = flagInCommand(command, str);
            if (!s.empty()) {
                isCmdOrFlag = true;
                std::string value;
                int pos = 1;
                if (str[1] == '-') {
                    pos++;
                }
                auto flag = command.flags.at(s);
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
