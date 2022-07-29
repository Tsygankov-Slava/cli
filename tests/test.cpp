#include "gtest/gtest.h"

#include "../src/Cli/Cli.hpp"

class CliFixture : public testing::Test {
public:
    cli::Cli cli = cli::Cli();

    static void func(cli::FlagsType &parsedFlags) {// Определение функции команды printHello
        std::cout << "Hello!\n";
    }

    static void func2(cli::FlagsType &parsedFlags) {// Определение функции команды printName
        std::cout << "Hello " << parsedFlags.at("name").value << " " << parsedFlags.at("surname").value << "!\n";
    }

    void SetUp() override {
        cli.command("printHello", "Displays the word \"Hello!\"", "$ printHello \n >>> Hello!", {}, func)// Добавляем команду printHello
                .command("printName", "Displays \"Hello [entered name]!\"", "$ printName -n Name\n>>> Hello Name!",
                         {cli::Flag("name", "n", "A flag that accepts a name as input", true, true),
                          cli::Flag("surname", "s", "A flag that accepts a surname for entry", true, true)},
                         func2);
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
    const int argc = 3;
    const std::string cmdArguments = "./cli printName -k";
    char **argv = initArgv(argc, cmdArguments);

    //Act
    std::invalid_argument e = std::invalid_argument("");
    try {
        cli.parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        e = error;
    }

    //Assert
    EXPECT_STREQ("\x1B[31mERROR: An unknown flag has been entered for the command \"printName\" -> \"-k\"\n", e.what());

    deleteArgv(argc, argv);
}

TEST_F(CliFixture, ErrorUnknownCommand) {
    //Average
    int argc = 2;
    const std::string cmdArguments = "./cli cmd";
    char **argv = initArgv(argc, cmdArguments);

    //Act
    std::invalid_argument e = std::invalid_argument("");
    try {
        cli.parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        e = error;
    }

    //Assert
    EXPECT_STREQ(std::invalid_argument("\x1B[31mERROR: Unknown command -> \"cmd\"\n").what(), e.what());

    deleteArgv(argc, argv);
}

TEST_F(CliFixture, ErrorUnknownFlag) {
    //Average
    int argc = 2;
    const std::string cmdArguments = "./cli -f";
    char **argv = initArgv(argc, cmdArguments);

    //Act
    std::invalid_argument e = std::invalid_argument("");
    try {
        cli.parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        e = error;
    }

    //Assert
    EXPECT_STREQ(std::invalid_argument("\x1B[31mERROR: Unknown flag -> \"-f\"\n").what(), e.what());

    deleteArgv(argc, argv);
}

TEST_F(CliFixture, RequiredFlagNotEntered) {
    //Average
    int argc = 2;
    const std::string cmdArguments = "./cli printName";
    char **argv = initArgv(argc, cmdArguments);

    //Act
    std::invalid_argument e = std::invalid_argument("");
    try {
        cli.parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        e = error;
    }

    //Assert
    EXPECT_STREQ(std::invalid_argument("\x1B[31mERROR: Required flag not entered -> \"--name\" OR \"-n\"\n").what(), e.what());

    deleteArgv(argc, argv);
}

TEST_F(CliFixture, FlagMustAcceptAnArgument) {
    //Average
    int argc = 3;
    const std::string cmdArguments = "./cli printName -n";
    char **argv = initArgv(argc, cmdArguments);

    //Act
    std::invalid_argument e = std::invalid_argument("");
    try {
        cli.parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        e = error;
    }

    //Assert
    EXPECT_STREQ(std::invalid_argument("\x1B[31mERROR: Flag \"--name\" must accept an argument\n").what(), e.what());

    deleteArgv(argc, argv);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}