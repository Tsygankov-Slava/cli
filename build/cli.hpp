#pragma once

#include <string>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include <vector>

#define CLI_VERSION_MAJOR "0"
#define CLI_VERSION_MINOR "1"
#define CLI_VERSION_PATCH "0"


namespace cli {
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
}


namespace cli {

    using FlagsType = std::map<std::string, Flag>;
    using CommandCallback = std::function<void(FlagsType &)>;

    class Command {
    public:
        std::string name;
        std::string description;
        std::string example;
        std::map<std::string, Flag> commandFlags;
        CommandCallback action;

    public:
        Command(std::string name, std::string description, std::string example, const std::vector<Flag> &commandFlags, CommandCallback action)
            : name(std::move(name)), description(std::move(description)), example(std::move(example)), action(std::move(action)) {
            for (auto &flag : commandFlags) {
                this->commandFlags.insert(std::make_pair(flag.name, flag));
            }
        };
    };
}



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

        Cli &command(const std::string &name, const std::string &description, const std::string &example, const std::vector<Flag> &commandFlag, const CommandCallback &action);

        void parse(int argc, char **argv);
        static void printAllHelp(std::map<std::string, Command> &commands, cli::Cli &cli);
        static void printCmdHelp(std::vector<std::string> &commandsName, std::map<std::string, Command> &commands, cli::Cli &cli);
        static std::string checkIsRequiredFlag(std::map<std::string, Flag> &inputFlags, std::map<std::string, Flag> &commandFlags, cli::Cli &cli);
        static std::string flagInCommand(std::map<std::string, Flag> &commandFlags, std::string &inputFlagName);
        static std::map<std::string, int> getCmdSizes(std::map<std::string, Command> &commands);
        static std::pair<int, char**> checkNocolor(cli::Cli &cli, int& argc, char **argv);
        static std::string paint(const std::string &str, const std::string &color, cli::Cli cli);

    private:
        std::map<std::string, Command> commands = {std::make_pair("help", Command("help", "Displays background information and prompts all kinds of commands", "", {},
                                                                                  [this](FlagsType &parsedFlags) {
                                                                                      printAllHelp(this->commands, *this);
                                                                                  }))};
    };
}// namespace cli
cli::Cli &cli::Cli::command(const std::string &name, const std::string &description, const std::string &example, const std::vector<Flag> &commandFlag, const CommandCallback &action) {
    Command cmd = Command(name, description, example, commandFlag, action);
    commands.insert(std::make_pair(name, cmd));
    return *this;
}

std::string cli::Cli::checkIsRequiredFlag(std::map<std::string, Flag> &inputFlags, std::map<std::string, Flag> &commandFlags, cli::Cli &cli) {
    for (auto &commandFlag : commandFlags) {
        auto flag = commandFlag.second;
        if (flag.isRequired) {
            if (!(inputFlags.count(flag.name) || inputFlags.count(flag.shortName))) {
                return paint(R"(ERROR: Required flag not entered -> "--)" + flag.name + R"(" OR "-)" + flag.shortName + R"(")", "red", cli);
            }
        }
    }
    return "";
}

std::string cli::Cli::flagInCommand(std::map<std::string, Flag> &commandFlags, std::string &inputFlagName) {
    for (auto &commandFlag : commandFlags) {
        auto flag = commandFlag.second;
        if (flag.name == inputFlagName || flag.shortName == inputFlagName) {
            return flag.name;
        }
    }
    return "";
}

std::pair<int, char **> cli::Cli::checkNocolor(cli::Cli &cli, int &argc, char **argv) {
    std::string cmd;
    for (int i = 0; i < argc; ++i) {
        cmd = argv[i];
        if (cmd == "--nocolor") {
            cli.nocolor = true;
            --argc;
            for (int j = i; j < argc; ++j) {
                argv[j] = argv[j + 1];
            }
        }
    }
    return std::make_pair(argc, argv);
}

void cli::Cli::parse(int argc, char **argv) {
    std::string cmd;
    std::string message;
    checkNocolor(*this, argc, argv);
    for (int i = 1; i < argc; ++i) {
        cmd = argv[i];
        if (commands.count(cmd)) {
            if (cmd == "help" && argc > 2) {
                std::vector<std::string> enteredCommands;
                for (int j = i + 1; j < argc; ++j) {
                    cmd = argv[j];
                    if (!commands.count(cmd)) {
                        throw std::invalid_argument(paint(R"(ERROR: Command ")" + cmd + R"(" doesn't exist)", "red", *this));
                    }
                    enteredCommands.push_back(cmd);
                }
                printCmdHelp(enteredCommands, commands, *this);
                break;
            }
            auto commandFlags = commands.at(cmd).commandFlags;
            std::map<std::string, Flag> flags;
            if (!commandFlags.empty()) {
                std::string flag;
                while (i + 1 < argc && !(flag = argv[i + 1]).empty() && flag[0] == '-' && ++i) {
                    std::string inputFlagName = flag;
                    inputFlagName.erase(std::remove(inputFlagName.begin(), inputFlagName.begin() + 2, '-'), inputFlagName.begin() + 2);
                    if ((inputFlagName = flagInCommand(commandFlags, inputFlagName)).empty()) {
                        throw std::invalid_argument(paint(R"(ERROR: An unknown flag has been entered for the command ")" + cmd + R"(" -> ")" + flag + R"(")", "red", *this));
                    }
                    auto commandFlag = commandFlags.at(inputFlagName);
                    if (commandFlag.withValue) {
                        ++i;
                        if (i == argc) {
                            throw std::invalid_argument(paint(R"(ERROR: Flag --)" + inputFlagName + R"(" must accept an argument)", "red", *this));
                        }
                        commandFlag.value = argv[i];
                    }
                    flags.insert({inputFlagName, commandFlag});
                }
                message = checkIsRequiredFlag(flags, commandFlags, *this);
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
                throw std::invalid_argument(paint(R"(ERROR: Unknown flag -> ")" + cmd + R"(")", "red", *this));
            } else {
                throw std::invalid_argument(paint(R"(ERROR: Unknown command -> ")" + cmd + R"(")", "red", *this));
            }
        }
    }
}

void cli::Cli::printAllHelp(std::map<std::string, Command> &commands, cli::Cli &cli) {
    std::string major = CLI_VERSION_MAJOR;
    std::string minor = CLI_VERSION_MINOR;
    std::string patch = CLI_VERSION_PATCH;
    std::cout << paint("CLI", "green", cli) + paint(" version ", "white", cli) + paint(major + "." + minor + '.' + patch, "yellow", cli) << "\n\n";
    std::cout << paint("Usage:\n", "yellow", cli) << paint("   command [flags] [arguments]\n\n", "white", cli) << paint("Commands:\n", "yellow", cli);
    std::map<std::string, int> sizes = getCmdSizes(commands);
    const int maxSize = std::max_element(sizes.begin(), sizes.end(), [](const auto &x, const auto &y) {
                            return x.second < y.second;
                        })->second;
    for (auto &cmd : commands) {
        std::string str = "  " + cmd.first;
        std::cout << paint(str, "green", cli);
        std::cout << std::setw(maxSize - sizes[cmd.first] + 2) << "";
        std::cout << paint(cmd.second.description, "white", cli) << "\n";
        if (!cmd.second.commandFlags.empty()) {
            std::cout << paint("\tFlags:\n", "yellow", cli);
        }
        for (auto &flag : cmd.second.commandFlags) {
            str = "      -" + flag.second.shortName + ", --" + flag.first;
            if (flag.second.withValue) {
                str += "=VALUE";
            }
            if (flag.second.isRequired) {
                str += paint("[", "green", cli) + paint("REQUIRED", "red", cli) + paint("]", "green", cli);
            }
            std::cout << paint(str, "green", cli);
            std::cout << std::right << std::setw(maxSize - sizes[flag.first] + 2) << "";
            std::cout << paint(flag.second.description, "white", cli) << "\n";
        }
    }
    std::cout << "\n";
}
std::map<std::string, int> cli::Cli::getCmdSizes(std::map<std::string, Command> &commands) {
    std::map<std::string, int> actualSize;
    for (auto &cmd : commands) {
        actualSize.insert(std::make_pair(cmd.first, (int) cmd.first.size() + 2));
        for (auto &flag : cmd.second.commandFlags) {
            int flagSize = (int) (flag.first.size() + flag.second.shortName.size() + 11);// 11 - number of special characters (spaces, commas, hyphens)
            if (flag.second.withValue) {
                flagSize += 6;// "=VALUE" == 6
            }
            if (flag.second.isRequired) {
                flagSize += 10;// "[REQUIRED]" == 10
            }
            actualSize.insert(std::make_pair(flag.first, flagSize));
        }
    }
    return actualSize;
}
void cli::Cli::printCmdHelp(std::vector<std::string> &commandsName, std::map<std::string, Command> &commands, cli::Cli &cli) {
    std::cout << paint("Usage:\n", "yellow", cli) << paint("   command [flags] [arguments]\n\n", "white", cli);
    if (commandsName.size() > 1) {
        std::cout << paint("Commands:\n", "yellow", cli);
    } else {
        std::cout << paint("Command:\n", "yellow", cli);
    }
    std::map<std::string, Command> commandsMap;
    for (auto &cmdName : commandsName) {
        commandsMap.insert({cmdName, commands.at(cmdName)});
    }
    std::map<std::string, int> sizes = getCmdSizes(commandsMap);
    const int maxSize = std::max_element(sizes.begin(), sizes.end(), [](const auto &x, const auto &y) {
                            return x.second < y.second;
                        })->second;
    for (auto &cmdName : commandsName) {
        auto cmd = commands.at(cmdName);
        std::string str = "  " + cmd.name;
        std::cout << paint(str, "green", cli);
        std::cout << std::setw(maxSize - sizes[cmdName] + 2) << "";
        std::cout << paint(cmd.description, "white", cli) << "\n";
        if (!cmd.commandFlags.empty()) {
            std::cout << paint("\tFlags:\n", "yellow", cli);
        }
        for (auto &flag : cmd.commandFlags) {
            str = "      -" + flag.second.shortName + ", --" + flag.first;
            if (flag.second.withValue) {
                str += "=VALUE";
            }
            if (flag.second.isRequired) {
                str += paint("[", "green", cli) + paint("REQUIRED", "red", cli) + paint("]", "green", cli);
            }
            std::cout << paint(str, "green", cli);
            std::cout << std::setw(maxSize - sizes[flag.first] + 2) << " ";
            std::cout << paint(flag.second.description, "white", cli) << "\n";
        }
        if (!cmd.example.empty()) {
            std::cout << paint("\tExample:", "yellow", cli) << paint("\n      ", "white", cli);
            for (auto &symbol : cmd.example) {
                std::cout << symbol;
                if (symbol == '\n') {
                    std::cout << "      ";
                }
            }
            std::cout << "\n\n";
        }
    }
}
std::string cli::Cli::paint(const std::string &str, const std::string &color, cli::Cli cli) {
    if (!cli.nocolor) {
        return cli.colors[color] + str + cli.colors["white"];
    }
    return str;
}



