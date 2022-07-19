#pragma once

#include <string>
#include <functional>
#include <iostream>
#include <map>
#include <vector>


class Flag {
public:
    std::string name;
    std::string shortName;
    std::string description;
    bool isRequired;
    bool withValue;
    std::string value;

public:
    Flag(std::string name, std::string shortName, std::string description, bool isRequired, bool withValue)
        : name(std::move(name)), shortName(std::move(shortName)), description(std::move(description)), isRequired(isRequired), withValue(withValue){};
};



typedef std::map<std::string, Flag> flagsType;
typedef std::function<void(flagsType &)> function;

class Command {
public:
    std::string name;
    std::string description;
    std::string example;
    std::map<std::string, Flag> commandFlags;
    function action;

public:
    Command(std::string name, std::string description, std::string example, std::vector<Flag> commandFlags, function action)
        : name(std::move(name)), description(std::move(description)), example(std::move(example)), action(std::move(action)) {
        for (auto &flag : commandFlags) {
            this->commandFlags.insert(std::make_pair(flag.name, flag));
        }
    };
};


class Cli {
public:
    Cli &command(std::string name, std::string description, std::string example, std::vector<Flag> commandFlag, function action);

    void parse(int argc, char **argv);
    static void printHelp(std::map<std::string, Command> &commands);
    static std::string checkIsRequiredFlag(std::map<std::string, Flag> &inputFlags, std::map<std::string, Flag> &commandFlags);
    static std::string flagInCommand(std::map<std::string, Flag> &commandFlags, std::string &flag);

private:
    std::map<std::string, Command> commands = {std::make_pair("help", Command("help", "Выведет справочную информацию и подскажет всевозможные команды", "", {},
                                                                              [this](flagsType &parsedFlags) { printHelp(this->commands); }))};
};

Cli &Cli::command(std::string name, std::string description, std::string example, std::vector<Flag> flags, function action) {
    Command cmd = Command(name, description, example, flags, action);
    commands.insert(std::make_pair(name, cmd));
    return *this;
}

std::string Cli::checkIsRequiredFlag(std::map<std::string, Flag> &inputFlags, std::map<std::string, Flag> &commandFlags) {
    for (auto &[name, flag] : commandFlags) {
        if (flag.isRequired) {
            if (!(inputFlags.contains(flag.name) || inputFlags.contains(flag.shortName))) {
                return "\033[31mERROR: Не введён обязательный флаг -> --" + flag.name + " OR -" + flag.shortName;
            }
        }
    }
    return "";
}

std::string Cli::flagInCommand(std::map<std::string, Flag> &commandFlags, std::string &flag) {
    for (auto &[name, f] : commandFlags) {
        if (f.name == flag || f.shortName == flag) {
            return f.name;
        }
    }
    return "";
}

void Cli::parse(int argc, char **argv) {
    std::string cmd;
    std::string message;
    for (int i = 1; i < argc; ++i) {
        cmd = argv[i];
        if (commands.contains(cmd)) {
            auto commandFlags = commands.at(cmd).commandFlags;
            std::map<std::string, Flag> flags;
            if (!commandFlags.empty()) {
                std::string flag;
                while (i + 1 < argc && !(flag = argv[i + 1]).empty() && flag[0] == '-' && ++i) {
                    std::string flagName = flag;
                    flagName.erase(std::remove(flagName.begin(), flagName.begin() + 2, '-'), flagName.begin() + 2);
                    if ((flagName = flagInCommand(commandFlags, flagName)).empty()) {
                        throw std::invalid_argument("\033[31mERROR: Введён неизвестный флаг для команды \"" + cmd + "\" -> " + flag);
                    }
                    auto commandFlag = commandFlags.at(flagName);
                    if (commandFlag.withValue) {
                        ++i;
                        commandFlag.value = argv[i];
                    }
                    flags.insert({flagName, commandFlag});
                }
                message = checkIsRequiredFlag(flags, commandFlags);
                if (!message.empty()) {
                    throw std::invalid_argument(message);
                }
            }
            auto action = commands.at(cmd).action;
            if (action) {
                action(flags);
            }
        } else {
            if (cmd[0] == '-') {
                throw std::invalid_argument("\033[31mERROR: Неизвестный флаг -> " + cmd);
            } else {
                throw std::invalid_argument("\033[31mERROR: Неизвестная команда -> " + cmd);
            }
        }
    }
}

void Cli::printHelp(std::map<std::string, Command> &commands) {
    std::cout << "\033[1;37m\033[1mPossible commands:\n";
    for (auto &cmd : commands) {
        std::cout << "\033[33m" << cmd.first << " \033[0m: " << cmd.second.description << "\n";
        if (!cmd.second.commandFlags.empty()) {
            std::cout << "\033[34mPossible flags:\n";
        }
        for (auto &flag : cmd.second.commandFlags) {
            std::cout << "\t\033[32m--" << flag.first;
            if (!flag.second.shortName.empty()) {
                std::cout << " \033[0mOR \033[32m-" << flag.second.shortName;
            }
            if (flag.second.isRequired) {
                std::cout << "\033[35m {!Обязательный флаг!} ";
            }
            if (flag.second.withValue) {
                std::cout << "\033[35m {!Должен принимать значение!} ";
            }
            std::cout << " \033[0m: " << flag.second.description << "\n";
        }
        if (!cmd.second.example.empty()) {
            std::cout << "\033[34mEXAMPLE:\033[0m\n"
                      << cmd.second.example << "\n";
        }
        std::cout << "\n";
    }
}
