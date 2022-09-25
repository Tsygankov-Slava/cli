#include "../build/cli.hpp" // Подключаем нашу библиотеку для использования CLI (путь до библиотеки может отличаться)
//#include "Cli/Cli.hpp"

int func(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments);  // Объявляем функцию, которая будет вызывать при вызове команды printArguments
int func2(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printHello
int func3(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printName
int func4(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printFlagsAndArguments
int func5(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printStringArguments
int func6(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printIntArguments
int func7(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printBoolArguments

int main(int argc, char **argv) {
    auto cli = cli::Cli();               // Создаём объект класса Cli
    cli.setDescriptionMaxWidth(7); // Задаём ширину описания

    try {
        cli.command("printArguments", "Displays the passed arguments", "$ printArguments file1.txt file2.txt\n>>> Arguments:\n file1.txt\n    file2.txt", func, -1)     // Добавляем команду printArguments
                .command("printTwoArguments", "Displays the passed arguments", func, 2)                                                                                      // Добавляем команду printTwoArguments
                .command("printHello", "Displays the word \"Hello!\".", "$ printHello \n>>> Hello!", func2)                                                                  // Добавляем команду printHello
                .command("printName", "Displays \"Hello [entered name]!\".", "$ printName -n Name\n>>> Hello Name!",
                         {
                                 cli::Flag("name", "n", "A flag that accepts a name as input.", true, true),              // Объявляем флаг "--name" для команды printName
                                 cli::Flag("surname", "s", "A flag that accepts a surname for entry.", true, true)        // Объявляем флаг "--surname" для команды printName
                         }, func3)                                                                                                                                      // Добавляем команду printName и указываем флаги name и surname
                .command("printFlagsAndArguments", "Displays the passed flags and arguments", "$ printFlagsAndArguments file1.txt --dir value file2.txt\n>>> Flags:\n"
                                                                                              "    flag -> value\n Arguments:\n    file1.txt\n    file2.txt",
                         {
                                 cli::Flag("dir", "d", "A flag that accepts a directory as input.", false, true),         // Объявляем флаг "--dir" для команды printName
                         }, func4, -1)                                                                                                                                  // Добавляем команду printFlagsAndArguments
                .command("printStringArguments", "Displays the passed string arguments", func5, -1)                                                                          // Добавляем команду printStringArguments
                .command("printIntArguments", "Displays the passed int arguments", func6, -1)                                                                                // Добавляем команду printIntArguments
                .command("printBoolArguments", "Displays the passed bool arguments", func7, -1)                                        // Добавляем команду printBoolArguments
                .parse(argc, argv);                                                                                                                                       // Обязательно вызываем функцию parse c аргументами argc и argv
    } catch (const std::invalid_argument &error) {                                                                                                                      // Обрабатываем какие-либо ошибки
        std::cout << error.what() << "\n";                                                                                                                              // Обязательно при выводе ошибок поставить символ переноса строки, иначе возможен вывод странных символов
        return 2;                                                                                                                                                       // код завершения программы при ошибке
    }
    return 0;
}


int func(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printArguments
    try {
        auto arguments = cli::Cli::convertArgumentsToType<std::string>(parsedArguments);
        std::cout << "Arguments: \n";
        for (auto &arg : arguments) {
            std::cout << arg << "\n";
        }
    } catch (const std::invalid_argument &error) {
        std::cout << error.what() << "\n";
        return 3;
    }
    return 0;
}

int func2(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printHello
    std::cout << "Hello!\n";
    return 0;
}

int func3(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printName
    std::cout << "Hello "
              << parsedFlags.at("name").value << " "
              << parsedFlags.at("surname").value << "!\n";
    return 0;
}

int func4(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printFlagsAndArguments
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

int func5(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printStringArguments
    try {
        auto arguments = cli::Cli::convertArgumentsToType<std::string>(parsedArguments);
        for (auto &argument : arguments) {
            std::cout << argument << " ";
        }
    } catch (const std::invalid_argument &error) {
        std::cout << error.what() << "\n";
        return 3;
    }
    return 0;
}

int func6(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printIntArguments
    try {
        auto arguments = cli::Cli::convertArgumentsToType<int>(parsedArguments);
        for (auto &argument : arguments) {
            std::cout << argument << " ";
        }
    } catch (const std::invalid_argument &error) {
        std::cout << error.what() << "\n";
        return 3;
    }
    return 0;
}

int func7(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printBoolArguments
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