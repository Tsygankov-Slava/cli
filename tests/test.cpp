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

TEST_F(CliFixture, ErrorUnknownCommandFlag) {
    //Average
    int argc = 3;
    char **argv = new char *[argc];
    argv[0] = new char [6];
    argv[1] = new char [10];
    argv[2] = new char [3];

    strcpy(argv[0], "./cli");
    strcpy(argv[1], "printName");
    strcpy(argv[2], "-k");

    //Act
    std::invalid_argument e = std::invalid_argument("");
    try {
        cli.parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        e = error;
    }

    //Assert
    EXPECT_STREQ(std::invalid_argument("\x1B[31mERROR: An unknown flag has been entered for the command \"printName\" -> \"-k\"\n").what(), e.what());

    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

TEST_F(CliFixture, ErrorUnknownCommand) {
    //Average
    int argc = 2;
    char **argv = new char *[argc];
    argv[0] = new char [6];
    argv[1] = new char [4];

    strcpy(argv[0], "./cli");
    strcpy(argv[1], "cmd");

    //Act
    std::invalid_argument e = std::invalid_argument("");
    try {
        cli.parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        e = error;
    }

    //Assert
    EXPECT_STREQ(std::invalid_argument("\x1B[31mERROR: Unknown command -> \"cmd\"\n").what(), e.what());

    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

TEST_F(CliFixture, ErrorUnknownFlag) {
    //Average
    int argc = 2;
    char **argv = new char *[argc];
    argv[0] = new char [6];
    argv[1] = new char [3];

    strcpy(argv[0], "./cli");
    strcpy(argv[1], "-f");

    //Act
    std::invalid_argument e = std::invalid_argument("");
    try {
        cli.parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        e = error;
    }

    //Assert
    EXPECT_STREQ(std::invalid_argument("\x1B[31mERROR: Unknown flag -> \"-f\"\n").what(), e.what());

    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

TEST_F(CliFixture, RequiredFlagNotEntered) {
    //Average
    int argc = 2;
    char **argv = new char *[argc];
    argv[0] = new char [6];
    argv[1] = new char [10];

    strcpy(argv[0], "./cli");
    strcpy(argv[1], "printName");

    //Act
    std::invalid_argument e = std::invalid_argument("");
    try {
        cli.parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        e = error;
    }

    //Assert
    EXPECT_STREQ(std::invalid_argument("\x1B[31mERROR: Required flag not entered -> \"--name\" OR \"-n\"\n").what(), e.what());

    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

TEST_F(CliFixture, FlagMustAcceptAnArgument) {
    //Average
    int argc = 3;
    char **argv = new char *[argc];
    argv[0] = new char [6];
    argv[1] = new char [10];
    argv[2] = new char [3];

    strcpy(argv[0], "./cli");
    strcpy(argv[1], "printName");
    strcpy(argv[2], "-n");

    //Act
    std::invalid_argument e = std::invalid_argument("");
    try {
        cli.parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        e = error;
    }

    //Assert
    EXPECT_STREQ(std::invalid_argument("\x1B[31mERROR: Flag \"--name\" must accept an argument\n").what(), e.what());

    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}