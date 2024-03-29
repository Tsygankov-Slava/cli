#include "gtest/gtest.h"
#include <fstream>

#include "../build/cli.hpp"

class CliFixture : public testing::Test {
public:
    cli::Cli cli = cli::Cli();

    static int func(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) {
        std::vector<std::string> arguments = cli::Cli::convertArgumentsToType<std::string>(parsedArguments);
        for (auto &arg : parsedArguments) {
            std::cout << arg << "\n";
        }
        return 0;
    }

    static int func2(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) {
        std::cout << "Hello!\n";
        return 0;
    }

    static int func3(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) {
        std::cout << "Hello " << parsedFlags.at("name").value << " " << parsedFlags.at("surname").value << "!\n";
        return 0;
    }

    static int func4(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) {
        std::cout << "Flags: \n";
        for (auto &flag : parsedFlags) {
            std::cout << flag.first << " -> " << flag.second.value << "\n";
        }
        std::cout << "Arguments: \n";
        for (auto &arg : parsedArguments) {
            std::cout << arg << "\n";
        }
        return 0;
    }

    static int func5(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) {
        std::vector<std::string> arguments;
        try {
            arguments = cli::Cli::convertArgumentsToType<std::string>(parsedArguments);
        } catch (const std::invalid_argument &error) {
            std::cout << error.what() << "\n";
            return 3;
        }
        for (auto &argument : arguments) {
            std::cout << argument << " ";
        }
        return 0;
    }

    static int func6(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) {
        std::vector<int> arguments;
        try {
            arguments = cli::Cli::convertArgumentsToType<int>(parsedArguments);
        } catch (const std::invalid_argument &error) {
            std::cout << error.what() << "\n";
            return 3;
        }
        for (auto &argument : arguments) {
            std::cout << argument << " ";
        }
        return 0;
    }

    static int func7(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) {
        try {
            auto arguments = cli::Cli::convertArgumentsToType<bool>(parsedArguments);
            for (auto argument : arguments) {
                std::cout << argument << " ";
            }
        } catch (const std::invalid_argument &error) {
            std::cout << error.what() << "\n";
            return 3;
        }
        return 0;
    }

    void SetUp() override {
        cli.setDescriptionMaxWidth(7);

        cli.command("printArguments", "Displays the passed arguments", "$ printArguments file1.txt file2.txt\n>>> Arguments:\n file1.txt\n file2.txt", func, -1)
                .command("printTwoArguments", "Displays the passed arguments", func, 2)
                .command("printHello", "Displays the word \"Hello!\".", func2)
                .command("printName", "Displays \"Hello [entered name]!\".",
                         {cli::Flag("name", "n", "A flag that accepts a name as input.", true, true),
                          cli::Flag("surname", "s", "A flag that accepts a surname for entry.", true, true)},
                         func3)
                .command("printFlagsAndArguments", "Displays the passed flags and arguments", "$ printFlagsAndArguments file1.txt --dir value file2.txt\n>>> Flags:\n    flag -> value\n Arguments:\n    file1.txt\n    file2.txt",
                         {
                                 cli::Flag("dir", "d", "A flag that accepts a directory as input.", false, true),
                         },
                         func4, -1)
                .command("printStringArguments", "Displays the passed string arguments", func5, -1)
                .command("printIntArguments", "Displays the passed int arguments", func6, -1)
                .command("printBoolArguments", "Displays the passed bool arguments", func7, -1);
    }
};

std::vector<const std::string> getStringVector(const std::string &cmdArguments, const char separator = ' ') {
    std::vector<const std::string> arguments;
    std::string str;
    int index = 0;
    char symbol = cmdArguments[index];
    while (symbol != '\0') {
        if (symbol == separator) {
            arguments.push_back(str);
            str = "";
        } else {
            str += symbol;
        }
        index++;
        symbol = cmdArguments[index];
    }
    arguments.push_back(str);
    return arguments;
}

char **initArgv(const int argc, const std::string &cmdArguments) {
    char **argv = new char *[argc];
    std::vector<const std::string> arguments = getStringVector(cmdArguments);
    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[arguments[i].size() + 1];
        strcpy(argv[i], arguments[i].c_str());
    }
    return argv;
}

void deleteArgv(const int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

TEST_F(CliFixture, ErrorUnknownCommandFlag) {
    //Average
    int argc = 4;
    const std::string cmdArguments = "./cli --nocolor printName -k";
    char **argv = initArgv(argc, cmdArguments);

    //Act && Assert
    EXPECT_THROW({
        try {
            cli.parse(argc, argv);
        } catch (const std::invalid_argument &e) {
            EXPECT_STREQ(R"(ERROR: An unknown flag has been entered for the command "printName" -> "-k")", e.what());
            throw;
        }
    },
                 std::invalid_argument);

    deleteArgv(argc, argv);
}

TEST_F(CliFixture, ErrorUnknownCommand) {
    //Average
    int argc = 3;
    const std::string cmdArguments = "./cli --nocolor cmd";
    char **argv = initArgv(argc, cmdArguments);

    //Act && Assert
    EXPECT_THROW({
        try {
            cli.parse(argc, argv);
        } catch (const std::invalid_argument &e) {
            EXPECT_STREQ(R"(ERROR: Unknown command -> "cmd")", e.what());
            throw;
        }
    },
                 std::invalid_argument);

    deleteArgv(argc, argv);
}

TEST_F(CliFixture, ErrorUnknownFlag) {
    //Average
    int argc = 3;
    const std::string cmdArguments = "./cli --nocolor -f";
    char **argv = initArgv(argc, cmdArguments);

    //Act && Assert
    EXPECT_THROW({
        try {
            cli.parse(argc, argv);
        } catch (const std::invalid_argument &e) {
            EXPECT_STREQ(R"(ERROR: Unknown flag -> "-f")", e.what());
            throw;
        }
    },
                 std::invalid_argument);

    deleteArgv(argc, argv);
}

TEST_F(CliFixture, RequiredFlagNotEntered) {
    //Average
    int argc = 3;
    const std::string cmdArguments = "./cli --nocolor printName";
    char **argv = initArgv(argc, cmdArguments);

    //Act && Assert
    EXPECT_THROW({
        try {
            cli.parse(argc, argv);
        } catch (const std::invalid_argument &e) {
            EXPECT_STREQ(R"(ERROR: Required flag not entered -> "--name" OR "-n")", e.what());
            throw;
        }
    },
                 std::invalid_argument);

    deleteArgv(argc, argv);
}

TEST_F(CliFixture, CommandDoesNotExist) {
    //Average
    int argc = 4;
    const std::string cmdArguments = "./cli --nocolor help cmd";
    char **argv = initArgv(argc, cmdArguments);

    //Act && Assert
    EXPECT_THROW({
        try {
            cli.parse(argc, argv);
        } catch (const std::invalid_argument &e) {
            EXPECT_STREQ(R"(ERROR: Command "cmd" doesn't exist)", e.what());
            throw;
        }
    },
                 std::invalid_argument);

    deleteArgv(argc, argv);
}

TEST_F(CliFixture, FlagMustAcceptAnArgument) {
    //Average
    int argc = 4;
    const std::string cmdArguments = "./cli --nocolor printName -n";
    char **argv = initArgv(argc, cmdArguments);

    //Act && Assert
    EXPECT_THROW({
        try {
            cli.parse(argc, argv);
        } catch (const std::invalid_argument &e) {
            EXPECT_STREQ(R"(ERROR: Flag "--name" must accept an argument)", e.what());
            throw;
        }
    },
                 std::invalid_argument);

    deleteArgv(argc, argv);
}

void createAndWriteFileCurrentResult(const std::string &cmdArguments, const std::string &fileNameCurrentResult) {
    const std::string cmd = R"(cd ../cmake-build-debug; rm -rf ../tests/current_results/)" + fileNameCurrentResult + R"(; )" + cmdArguments + R"( >> ../tests/current_results/)" + fileNameCurrentResult;
    system(cmd.c_str());
}

void getCurrentCodeAndExpectedCode(const std::string &fileNameCurrentResult, std::string &currentCode, std::string &expectedCode) {
    std::ifstream inputCurrentResult(R"(../tests/current_results/)" + fileNameCurrentResult),
            inputExpectedResult(R"(../tests/expected_results/)" + fileNameCurrentResult);
    if (inputCurrentResult.is_open() && inputExpectedResult.is_open()) {
        std::string line;

        while (getline(inputCurrentResult, line, '\n')) {
            if (!line.empty()) {
                currentCode += line;
            }
        }

        while (getline(inputExpectedResult, line, '\n')) {
            if (!line.empty()) {
                expectedCode += line;
            }
        }
    } else {
        throw std::invalid_argument(R"(File opening error)");
    }

    currentCode.erase(remove(currentCode.begin(), currentCode.end(), '\n'), currentCode.end());
    currentCode.erase(remove(currentCode.begin(), currentCode.end(), ' '), currentCode.end());
    expectedCode.erase(remove(expectedCode.begin(), expectedCode.end(), '\n'), expectedCode.end());
    expectedCode.erase(remove(expectedCode.begin(), expectedCode.end(), ' '), expectedCode.end());
}

TEST_F(CliFixture, TestingPrintAllHelpFunction) {
    //Average
    int argc = 3;
    const std::string cmdArguments = "./cli --nocolor help";
    char **argv = initArgv(argc, cmdArguments);
    const std::string fileName = "1.txt";

    //Act
    cli.parse(argc, argv);
    createAndWriteFileCurrentResult(cmdArguments, fileName);
    std::string currentCode, expectedCode;
    try {
        getCurrentCodeAndExpectedCode(fileName, currentCode, expectedCode);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        EXPECT_TRUE(1);
    }

    //Assert
    ASSERT_EQ(currentCode, expectedCode);
    deleteArgv(argc, argv);
}

TEST_F(CliFixture, TestingPrintCmdHelpFunction_ManyCommands) {
    //Average
    int argc = 5;
    const std::string cmdArguments = "./cli --nocolor help printName printHello";
    char **argv = initArgv(argc, cmdArguments);
    const std::string fileName = "2.txt";

    //Act
    cli.parse(argc, argv);
    createAndWriteFileCurrentResult(cmdArguments, fileName);
    std::string currentCode, expectedCode;
    try {
        getCurrentCodeAndExpectedCode(fileName, currentCode, expectedCode);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        EXPECT_TRUE(1);
    }

    //Assert
    ASSERT_EQ(currentCode, expectedCode);
    deleteArgv(argc, argv);
}

TEST_F(CliFixture, TestingPrintCmdHelpFunction_OneCommand) {
    //Average
    int argc = 4;
    const std::string cmdArguments = "./cli --nocolor help printFlagsAndArguments";
    char **argv = initArgv(argc, cmdArguments);
    const std::string fileName = "3.txt";

    //Act
    cli.parse(argc, argv);
    createAndWriteFileCurrentResult(cmdArguments, fileName);
    std::string currentCode, expectedCode;
    try {
        getCurrentCodeAndExpectedCode(fileName, currentCode, expectedCode);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        EXPECT_TRUE(1);
    }

    //Assert
    ASSERT_EQ(currentCode, expectedCode);
    deleteArgv(argc, argv);
}

TEST_F(CliFixture, TestingFlagsWithValue) {
    //Average
    int argc = 7;
    const std::string cmdArguments = "./cli --nocolor printName -n Name -s Surname";
    char **argv = initArgv(argc, cmdArguments);
    const std::string fileName = "4.txt";

    //Act
    cli.parse(argc, argv);
    createAndWriteFileCurrentResult(cmdArguments, fileName);
    std::string currentCode, expectedCode;
    try {
        getCurrentCodeAndExpectedCode(fileName, currentCode, expectedCode);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        EXPECT_TRUE(1);
    }

    //Assert
    ASSERT_EQ(currentCode, expectedCode);
    deleteArgv(argc, argv);
}

TEST_F(CliFixture, TestingErrorFlagDoesNotExist) {
    //Average
    int argc = 5;
    const std::string cmdArguments = "./cli --nocolor help printName --flag";
    char **argv = initArgv(argc, cmdArguments);

    //Act && Assert
    EXPECT_THROW({
        try {
            cli.parse(argc, argv);
        } catch (const std::invalid_argument &e) {
            EXPECT_STREQ(R"(ERROR: Flag "--flag" doesn't exist)", e.what());
            throw;
        }
    },std::invalid_argument);

    deleteArgv(argc, argv);
}


TEST_F(CliFixture, TestingErrorCommandMustContainAtLeastOneArgument) {
    //Average
    int argc = 3;
    const std::string cmdArguments = "./cli --nocolor printArguments";
    char **argv = initArgv(argc, cmdArguments);

    //Act && Assert
    EXPECT_THROW({
        try {
            cli.parse(argc, argv);
        } catch (const std::invalid_argument &e) {
            EXPECT_STREQ(R"(ERROR: Command "printArguments" must contain at least one argument)", e.what());
            throw;
        }
    },std::invalid_argument);

    deleteArgv(argc, argv);
}

TEST_F(CliFixture, TestingErrorCommandMustContainTwoArguments) {
    //Average
    int argc = 4;
    const std::string cmdArguments = "./cli --nocolor printTwoArguments a";
    char **argv = initArgv(argc, cmdArguments);

    //Act && Assert
    EXPECT_THROW({
        try {
            cli.parse(argc, argv);
        } catch (const std::invalid_argument &e) {
            EXPECT_STREQ(R"(ERROR: Command "printTwoArguments" must contain 2 arguments)", e.what());
            throw;
        }
    },std::invalid_argument);

    deleteArgv(argc, argv);
}

TEST_F(CliFixture, TestingReadingArgumentsAndFlags) {
    //Average
    int argc = 6;
    const std::string cmdArguments = "./cli printFlagsAndArguments file1.txt --dir directory/ file2.txt";
    char **argv = initArgv(argc, cmdArguments);
    const std::string fileName = "5.txt";

    //Act
    cli.parse(argc, argv);
    createAndWriteFileCurrentResult(cmdArguments, fileName);
    std::string currentCode, expectedCode;
    try {
        getCurrentCodeAndExpectedCode(fileName, currentCode, expectedCode);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        EXPECT_TRUE(1);
    }

    //Assert
    ASSERT_EQ(currentCode, expectedCode);
    deleteArgv(argc, argv);
}

TEST_F(CliFixture, TestingErrorCommandArgumentsMustBeIntType) {
    //Average
    int argc = 5;
    const std::string cmdArguments = "./cli printIntArguments file1.txt file2.txt --nocolor";
    char **argv = initArgv(argc, cmdArguments);
    const std::string fileName = "6.txt";

    //Act
    cli.parse(argc, argv);
    createAndWriteFileCurrentResult(cmdArguments, fileName);
    std::string currentCode, expectedCode;
    try {
        getCurrentCodeAndExpectedCode(fileName, currentCode, expectedCode);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        EXPECT_TRUE(1);
    }

    //Assert
    ASSERT_EQ(currentCode, expectedCode);
    deleteArgv(argc, argv);
}

TEST_F(CliFixture, TestingErrorCommandArgumentsMustBeBoolType) {
    //Average
    int argc = 5;
    const std::string cmdArguments = "./cli printBoolArguments a 4 --nocolor";
    char **argv = initArgv(argc, cmdArguments);
    const std::string fileName = "7.txt";

    //Act
    cli.parse(argc, argv);
    createAndWriteFileCurrentResult(cmdArguments, fileName);
    std::string currentCode, expectedCode;
    try {
        getCurrentCodeAndExpectedCode(fileName, currentCode, expectedCode);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        EXPECT_TRUE(1);
    }

    //Assert
    ASSERT_EQ(currentCode, expectedCode);
    deleteArgv(argc, argv);
}

TEST_F(CliFixture, TestingConvertArgumentsToInt) {
    //Average
    int argc = 6;
    const std::string cmdArguments = "./cli printIntArguments -2 +102 10 --nocolor";
    char **argv = initArgv(argc, cmdArguments);
    const std::string fileName = "8.txt";

    //Act
    cli.parse(argc, argv);
    createAndWriteFileCurrentResult(cmdArguments, fileName);
    std::string currentCode, expectedCode;
    try {
        getCurrentCodeAndExpectedCode(fileName, currentCode, expectedCode);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        EXPECT_TRUE(1);
    }

    //Assert
    ASSERT_EQ(currentCode, expectedCode);
    deleteArgv(argc, argv);
}

TEST_F(CliFixture, TestingConvertArgumentsToBool) {
    //Average
    int argc = 7;
    const std::string cmdArguments = "./cli printBoolArguments true false 1 0 --nocolor";
    char **argv = initArgv(argc, cmdArguments);
    const std::string fileName = "9.txt";

    //Act
    cli.parse(argc, argv);
    createAndWriteFileCurrentResult(cmdArguments, fileName);
    std::string currentCode, expectedCode;
    try {
        getCurrentCodeAndExpectedCode(fileName, currentCode, expectedCode);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        EXPECT_TRUE(1);
    }

    //Assert
    ASSERT_EQ(currentCode, expectedCode);
    deleteArgv(argc, argv);
}

TEST_F(CliFixture, TestingConvertArgumentsToString) {
    //Average
    int argc = 8;
    const std::string cmdArguments = "./cli printStringArguments true 1 file1.txt +-100 -4 --nocolor";
    char **argv = initArgv(argc, cmdArguments);
    const std::string fileName = "10.txt";

    //Act
    cli.parse(argc, argv);
    createAndWriteFileCurrentResult(cmdArguments, fileName);
    std::string currentCode, expectedCode;
    try {
        getCurrentCodeAndExpectedCode(fileName, currentCode, expectedCode);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        EXPECT_TRUE(1);
    }

    //Assert
    ASSERT_EQ(currentCode, expectedCode);
    deleteArgv(argc, argv);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}