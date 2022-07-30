//#include "Cli/Cli.hpp"
#include "../build/cli.hpp" // Подключаем нашу библиотеку для использования CLI (путь до библиотеки может отличаться)

void func(cli::FlagsType &parsedFlags); // Объявляем функцию, которая будет вызывать при вызове команды printHello
void func2(cli::FlagsType &parsedFlags); // Объявляем функцию, которая будет вызывать при вызове команды printName

int main(int argc, char **argv) {
    auto cli = cli::Cli();
    try {
        cli.command("printHello", "Displays the word \"Hello!\"", "$ printHello \n>>> Hello!", {}, func) // Добавляем команду printHello
                .command("printName", "Displays \"Hello [entered name]!\"", "$ printName -n Name\n>>> Hello Name!",
                         {
                                 cli::Flag("name", "n", "A flag that accepts a name as input", true, true),
                                 cli::Flag("surname", "s", "A flag that accepts a surname for entry", true, true)
                         }, func2) // Добавляем команду printName и указываем флаги name и surname
                .parse(argc, argv); // Обязательно вызываем функцию parse c аргументами argc и argv
    } catch (const std::invalid_argument &error) { // Обрабатываем какие-либо ошибки
        std::cout << error.what() << "\n";
        return 2; // код завершения программы при ошибке
    }
    return 0;
}

void func(cli::FlagsType &parsedFlags) { // Определение функции команды printHello
    std::cout << "Hello!\n";
}

void func2(cli::FlagsType &parsedFlags) { // Определение функции команды printName
    std::cout << "Hello " << parsedFlags.at("name").value << " " << parsedFlags.at("surname").value << "!\n";
}
