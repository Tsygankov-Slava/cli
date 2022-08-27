//#include "Cli/Cli.hpp"
#include "../build/cli.hpp"// Подключаем нашу библиотеку для использования CLI (путь до библиотеки может отличаться)


void func(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printArguments
void func2(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments);// Объявляем функцию, которая будет вызывать при вызове команды printHello
void func3(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments);// Объявляем функцию, которая будет вызывать при вызове команды printName
void func4(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments);// Объявляем функцию, которая будет вызывать при вызове команды printFlagsAndArguments

int main(int argc, char **argv) {
    auto cli = cli::Cli();
    cli.setDescriptionMaxWidth(7);

    try {
        cli.command("printArguments", "Displays the passed arguments", "$ printArguments file1.txt file2.txt\n>>> Arguments:\n file1.txt\n    file2.txt", {}, func, -1) // Добавляем команду printArguments
           .command("printTwoArguments", "Displays the passed arguments", "", {}, func, 2)
           .command("printHello", "Displays the word \"Hello!\".", "$ printHello \n>>> Hello!", {}, func2)                                                              // Добавляем команду printHello
           .command("printName", "Displays \"Hello [entered name]!\".", "$ printName -n Name\n>>> Hello Name!",
                         {
                                 cli::Flag("name", "n", "A flag that accepts a name as input.", true, true),              // Объявляем флаг "--name" для команды printName
                                 cli::Flag("surname", "s", "A flag that accepts a surname for entry.", true, true)        // Объявляем флаг "--surname" для команды printName
                         },
                         func3)                                                                                                                                         // Добавляем команду printName и указываем флаги name и surname
           .command("printFlagsAndArguments", "Displays the passed flags and arguments", "$ printFlagsAndArguments file1.txt --dir value file2.txt\n>>> Flags:\n    flag -> value\n Arguments:\n    file1.txt\n    file2.txt",
                         {
                                 cli::Flag("dir", "d", "A flag that accepts a directory as input.", false, true),         // Объявляем флаг "--dir" для команды printName
                         },
                         func4, -1)                                                                                                                // Добавляем команду printFlagsAndArguments
                .parse(argc, argv);                                                                                                                                  // Обязательно вызываем функцию parse c аргументами argc и argv
    } catch (const std::invalid_argument &error) {                                                                                                                      // Обрабатываем какие-либо ошибки
        std::cout << error.what() << "\n";                                                                                                                              // Обязательно при выводе ошибок поставить символ переноса строки, иначе возможен вывод странных символов
        return 2;                                                                                                                                                       // код завершения программы при ошибке
    }
    return 0;
}


void func(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) {// Определение функции команды printArguments
    std::cout << "Arguments: \n";
    for (auto &arg : parsedArguments) {
        std::cout << arg << "\n";
    }
}

void func2(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) {// Определение функции команды printHello
    std::cout << "Hello!\n";
}

void func3(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) {// Определение функции команды printName
    std::cout << "Hello "
              << parsedFlags.at("name").value << " "
              << parsedFlags.at("surname").value << "!\n";
}

void func4(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) {// Определение функции команды printFlagsAndArguments
    std::cout << "Flags: \n";
    for (auto &flag : parsedFlags) {
        std::cout << flag.first << " -> " << flag.second.value << "\n";
    }
    std::cout << "Arguments: \n";
    for (auto &arg : parsedArguments) {
        std::cout << arg << "\n";
    }
}
