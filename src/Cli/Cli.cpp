#include "Cli.hpp"

cli::Cli &cli::Cli::command(const std::string &name, const std::string &description, const std::string &example, const cli::CommandCallback &action, const std::string &argumentsTypeDeclaration, bool canContainEmptyArgumentList) {
    Command cmd = Command(name, description, example, action, argumentsTypeDeclaration, canContainEmptyArgumentList);
    commands.insert(std::make_pair(name, cmd));
    return *this;
}

cli::Cli &cli::Cli::command(const std::string &name, const std::string &description, const std::vector<Flag> &commandFlag, const cli::CommandCallback &action, const std::string &argumentsTypeDeclaration, bool canContainEmptyArgumentList) {
    Command cmd = Command(name, description, commandFlag, action, argumentsTypeDeclaration, canContainEmptyArgumentList);
    commands.insert(std::make_pair(name, cmd));
    return *this;
}

cli::Cli &cli::Cli::command(const std::string &name, const std::string &description, const cli::CommandCallback &action, const std::string &argumentsTypeDeclaration, bool canContainEmptyArgumentList) {
    Command cmd = Command(name, description, action, argumentsTypeDeclaration, canContainEmptyArgumentList);
    commands.insert(std::make_pair(name, cmd));
    return *this;
}

cli::Cli &cli::Cli::command(const std::string &name, const std::string &description, const std::string &example, const std::vector<Flag> &commandFlag, const CommandCallback &action, const std::string &argumentsTypeDeclaration, bool canContainEmptyArgumentList) {
    Command cmd = Command(name, description, example, commandFlag, action, argumentsTypeDeclaration, canContainEmptyArgumentList);
    commands.insert(std::make_pair(name, cmd));
    return *this;
}

cli::Cli &cli::Cli::setDescriptionMaxWidth(const int value) {
    descriptionMaxWidth = value;
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

void cli::Cli::checkNocolor(cli::Cli &cli, int &argc, char **argv) {
    std::string cmd;
    for (int i = 1; i < argc; ++i) {
        cmd = argv[i];
        if (cmd == "--nocolor") {
            cli.nocolor = true;
            --argc;
            for (int j = i; j < argc; ++j) {
                argv[j] = argv[j + 1];
            }
        }
    }
}

std::pair<std::string, std::string> cli::Cli::getTypeAndCountArguments(std::string &cmd) {
    std::string argumentsTypeDeclaration = commands.at(cmd).argumentsTypeDeclaration;
    auto pos1 = argumentsTypeDeclaration.find('[');
    auto pos2 = argumentsTypeDeclaration.find(']');

    std::string argumentsType = argumentsTypeDeclaration;
    std::string argumentsCount = "-1";

    auto argumentsSize = argumentsTypeDeclaration.size();
    if (!(pos1 > argumentsSize && pos2 > argumentsSize)) {
        argumentsType = argumentsTypeDeclaration.substr(0, argumentsSize - (pos2 - pos1) - 1);
        argumentsCount = argumentsTypeDeclaration.substr(pos1 + 1, pos2 - pos1 - 1);
    }

    return std::make_pair(argumentsType, argumentsCount);
}

bool cli::Cli::isNumber(const std::string &number) {
    bool sign = false;
    for (const char &symbol : number) {
        if (!(std::isdigit(symbol))) {
            if ((symbol == '-' || symbol == '+') && !sign) {
                sign = true;
            } else {
                return false;
            }
        }
    }
    return true;
}

std::string cli::Cli::checkCorrectType(std::string &type, std::string &countString, int &count) {
    auto it = std::find(argumentsAndFlagsType.begin(), argumentsAndFlagsType.end(), type);
    std::string message;
    if (it == argumentsAndFlagsType.end()) {
        message = paint(R"(ERROR: Arguments type incorrect -> )" + type, "red", *this);
    } else {
        if (!isNumber(countString)) {
            message = paint(R"(ERROR: Arguments count incorrect -> )" + countString, "red", *this);
        } else {
            count = std::stoi(countString);
            if (count < -1) {
                message = paint(R"(ERROR: Arguments count incorrect -> )" + countString, "red", *this);
            }
        }
    }
    return message;
}

std::string cli::Cli::checkNumberOfArgumentsPassed(std::string &cmd, cli::ArgumentsType &parsedArguments) {
    std::string argumentsType, argumentsCountString;
    int argumentsCount;
    std::tie(argumentsType, argumentsCountString) = getTypeAndCountArguments(cmd);
    std::string message = checkCorrectType(argumentsType, argumentsCountString, argumentsCount);
    if (!message.empty()) {
        return message;
    }
    commands.at(cmd).argumentsType = argumentsType;
    auto countParsedArguments = parsedArguments.size();

    if (argumentsCount == -1) {
        if (!commands.at(cmd).canContainEmptyArgumentList && countParsedArguments == 0) {
            return paint(R"(ERROR: Command ")" + cmd + R"(" must contain at least one argument)", "red", *this);
        }
    } else if (argumentsCount != countParsedArguments) {
        std::string argWord = (argumentsCount > 1) ? "arguments" : "argument";
        return paint(R"(ERROR: Command ")" + cmd + R"(" must contain )" + std::to_string(argumentsCount) + ' ' + argWord, "red", *this);
    }
    return "";
}

void cli::Cli::parseFlagsAndArguments(std::string &cmd, int argc, char **argv, int &i, FlagsType &commandFlags, FlagsType &parsedFlags, ArgumentsType &parsedArguments) {
    while (++i < argc) {
        std::string lexeme = argv[i];
        if (lexeme[0] == '-') {
            std::string flag = lexeme;
            lexeme.erase(std::remove(lexeme.begin(), lexeme.begin() + 2, '-'), lexeme.begin() + 2);
            if ((lexeme = flagInCommand(commandFlags, lexeme)).empty()) {
                throw std::invalid_argument(paint(R"(ERROR: An unknown flag has been entered for the command ")" + cmd + R"(" -> ")" + flag + R"(")", "red", *this));
            }
            auto commandFlag = commandFlags.at(lexeme);
            if (commandFlag.withValue) {
                ++i;
                if (i == argc) {
                    throw std::invalid_argument(paint(R"(ERROR: Flag "--)" + lexeme + R"(" must accept an argument)", "red", *this));
                }
                commandFlag.value = argv[i];
            }
            parsedFlags.insert({lexeme, commandFlag});
        } else {
            parsedArguments.push_back(lexeme);
        }
    }

    std::string message = checkIsRequiredFlag(parsedFlags, commandFlags, *this);
    if (!message.empty()) {
        throw std::invalid_argument(message);
    }

    message = checkNumberOfArgumentsPassed(cmd, parsedArguments);
    if (!message.empty()) {
        throw std::invalid_argument(message);
    }
}

std::string cli::Cli::convertArgumentsToString(cli::ArgumentsType &parsedArguments, std::vector<std::string> &convertedParsedArguments, const std::string &cmd) {
    for (auto &argument : parsedArguments) {
        convertedParsedArguments.push_back(argument);
    }
    return "";
}

std::string cli::Cli::convertArgumentsToInt(cli::ArgumentsType &parsedArguments, std::vector<int> &convertedParsedArguments, const std::string &cmd) {
    std::string message;
    for (auto &argument : parsedArguments) {
        if (isNumber(argument)) {
            convertedParsedArguments.push_back(std::stoi(argument));
        } else {
            message = R"(ERROR: Command ")" + cmd + R"(" Arguments must be int type)";
        }
    }
    return message;
}

std::string cli::Cli::convertArgumentsToBool(cli::ArgumentsType &parsedArguments, std::vector<bool> &convertedParsedArguments, const std::string &cmd) {
    std::string message;
    for (auto &argument : parsedArguments) {
        if (argument == "1" || argument == "true") {
            convertedParsedArguments.push_back(true);
        } else if (argument == "0" || argument == "false") {
            convertedParsedArguments.push_back(false);
        } else {
            message = R"(ERROR: Command ")" + cmd + R"(" Arguments must be bool type)";
        }
    }
    return message;
}


void cli::Cli::parse(int &argc, char **argv) {
    std::string cmd;
    checkNocolor(*this, argc, argv);
    for (int i = 1; i < argc; ++i) {
        cmd = argv[i];
        if (commands.count(cmd)) {
            if (cmd == "help" && argc > 2) {
                std::vector<std::string> enteredCommands;
                for (int j = i + 1; j < argc; ++j) {
                    cmd = argv[j];
                    if (!commands.count(cmd)) {
                        if (cmd[0] == '-') {
                            throw std::invalid_argument(paint(R"(ERROR: Flag ")" + cmd + R"(" doesn't exist)", "red", *this));
                        }
                        throw std::invalid_argument(paint(R"(ERROR: Command ")" + cmd + R"(" doesn't exist)", "red", *this));
                    }
                    enteredCommands.push_back(cmd);
                }
                printCmdHelp(enteredCommands, commands, *this);
                break;
            }
            auto command = commands.at(cmd);
            auto commandFlags = command.flags;
            ArgumentsType parsedArguments;
            FlagsType parsedFlags;
            parseFlagsAndArguments(cmd, argc, argv, i, commandFlags, parsedFlags, parsedArguments);
            auto argumentsType = command.argumentsType;
            if (argumentsType == "String" || argumentsType == "string") {
                std::vector<std::string> convertedParsedArguments;
                convertArgumentsToString(parsedArguments, convertedParsedArguments, cmd);
            } else if (argumentsType == "Int" || argumentsType == "int") {
                std::vector<int> convertedParsedArguments;
                convertArgumentsToInt(parsedArguments, convertedParsedArguments, cmd);
            } else if (argumentsType == "Bool" || argumentsType == "bool") {
                std::vector<bool> convertedParsedArguments;
                convertArgumentsToBool(parsedArguments, convertedParsedArguments, cmd);
            }
            auto action = command.action;
            if (action) {
                action(parsedFlags, parsedArguments);
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

void cli::Cli::lineWrapping(std::string &description, const int maxSize, int size, cli::Cli &cli) {
    unsigned len, index = 0;
    description += ' ';
    std::string descriptionString;
    while (index < description.size()) {
        len = 0;
        descriptionString = "";
        while (index < description.size() && len < cli.descriptionMaxWidth) {
            char symbol = description[index];
            if (symbol == '\n') {
                ++index;
                break;
            }
            if (!(symbol == ' ' && descriptionString.empty())) {
                descriptionString += symbol;
                ++len;
            }
            ++index;

            if ((len >= cli.descriptionMaxWidth) && (symbol != ' ')) {
                auto lastSpaceIndex = descriptionString.find_last_of(' ', descriptionString.size());
                if (lastSpaceIndex == std::string::npos) {
                    lastSpaceIndex = 0;
                }
                unsigned distance = descriptionString.size() - lastSpaceIndex;
                if (distance > 0 && distance < 4) {
                    unsigned countSymbols = 4 - distance;
                    descriptionString += description.substr(index, countSymbols);
                    index += countSymbols;
                }
                distance = description.find_first_of(' ', index - 1) - index;
                if (distance > 0 && distance < 4) {
                    descriptionString += description.substr(index, description.find_first_of(' ', index - 1) - index);
                    index += distance + 1;
                }
            }
        }

        if (index < description.size() && description[index - 1] != ' ' && description[index] != ' ') {
            descriptionString += '-';
        }
        if (size != 0) {
            descriptionString += ' ';
            const unsigned spacePosition = descriptionString.find_first_of(' ', 0);
            const std::string firstWorld = descriptionString.substr(0, spacePosition);
            descriptionString = paint(firstWorld, "blue", cli) + descriptionString.substr(spacePosition, descriptionString.size());
        }
        if (!descriptionString.empty()) {
            std::cout << std::setw(maxSize - size + 2) << "";
            std::cout << descriptionString << "\n";
        }
        size = 0;
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
        int cmdSize = sizes[cmd.first];

        std::string cmdDescription = cmd.second.description;
        const unsigned spacePositionCmd = cmdDescription.find_first_of(' ', 0);
        const std::string firstWorldCmd = cmdDescription.substr(0, spacePositionCmd);

        std::cout << paint(str, "green", cli);
        if (cmdDescription.size() < cli.descriptionMaxWidth) {
            cmdDescription = paint(firstWorldCmd, "blue", cli) + cmdDescription.substr(spacePositionCmd, cmdDescription.size());
            std::cout << std::setw(maxSize - cmdSize + 2) << "";
            std::cout << cmdDescription << "\n";
        } else {
            lineWrapping(cmdDescription, maxSize, cmdSize, cli);
        }
        auto commandFlags = cmd.second.flags;
        if (!commandFlags.empty()) {
            std::cout << paint("    Flags:\n", "yellow", cli);
        }
        for (auto &flag : commandFlags) {
            str = "      -" + flag.second.shortName + ", --" + flag.first;
            if (flag.second.withValue) {
                str += "=VALUE";
            }
            if (flag.second.isRequired) {
                str += paint("[", "green", cli) + paint("REQUIRED", "red", cli) + paint("]", "green", cli);
            }

            int flagSize = sizes[flag.first];
            std::string flagDescription = flag.second.description;
            const unsigned spacePositionFlag = flagDescription.find_first_of(' ', 0);
            const std::string firstWorldFlag = flagDescription.substr(0, spacePositionFlag);

            std::cout << paint(str, "green", cli);
            if (flagDescription.size() < cli.descriptionMaxWidth) {
                flagDescription = paint(firstWorldFlag, "blue", cli) + flagDescription.substr(spacePositionFlag, flagDescription.size());
                std::cout << std::setw(maxSize - flagSize + 2) << "";
                std::cout << flagDescription << "\n";
            } else {
                lineWrapping(flagDescription, maxSize, flagSize, cli);
            }
        }
    }
    std::cout << "\n";
}
std::map<std::string, int> cli::Cli::getCmdSizes(std::map<std::string, Command> &commands) {
    std::map<std::string, int> actualSize;
    for (auto &cmd : commands) {
        actualSize.insert(std::make_pair(cmd.first, (int) cmd.first.size() + 2));
        for (auto &flag : cmd.second.flags) {
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
        int cmdSize = sizes[cmd.name];
        std::string cmdDescription = cmd.description;
        const unsigned spacePositionCmd = cmdDescription.find_first_of(' ', 0);
        const std::string firstWorldCmd = cmdDescription.substr(0, spacePositionCmd);

        std::string str = "  " + cmd.name;

        std::cout << paint(str, "green", cli);
        if (cmdDescription.size() < cli.descriptionMaxWidth) {
            cmdDescription = paint(firstWorldCmd, "blue", cli) + cmdDescription.substr(spacePositionCmd, cmdDescription.size());
            std::cout << std::setw(maxSize - sizes[cmdName] + 2) << "";
            std::cout << cmdDescription << "\n";
        } else {
            lineWrapping(cmdDescription, maxSize, cmdSize, cli);
        }

        auto commandFlags = cmd.flags;
        if (!commandFlags.empty()) {
            std::cout << paint("\tFlags:\n", "yellow", cli);
        }
        for (auto &flag : commandFlags) {
            str = "      -" + flag.second.shortName + ", --" + flag.first;
            if (flag.second.withValue) {
                str += "=VALUE";
            }
            if (flag.second.isRequired) {
                str += paint("[", "green", cli) + paint("REQUIRED", "red", cli) + paint("]", "green", cli);
            }

            int flagSize = sizes[flag.first];
            std::string flagDescription = flag.second.description;
            const unsigned spacePositionFlag = flagDescription.find_first_of(' ', 0);
            const std::string firstWorldFlag = flagDescription.substr(0, spacePositionFlag);


            std::cout << paint(str, "green", cli);
            if (flagDescription.size() < cli.descriptionMaxWidth) {
                flagDescription = paint(firstWorldFlag, "blue", cli) + flagDescription.substr(spacePositionFlag, flagDescription.size());
                std::cout << std::setw(maxSize - sizes[flag.first] + 2) << " ";
                std::cout << flagDescription << "\n";
            } else {
                lineWrapping(flagDescription, maxSize, flagSize, cli);
            }
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
std::string cli::Cli::paint(const std::string &str, const std::string &color, cli::Cli &cli) {
    if (!cli.nocolor) {
        return cli.colors[color] + str + cli.colors["white"];
    }
    return str;
}
