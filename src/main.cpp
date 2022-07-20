//#include "Cli/Cli.hpp"
#include "../build/cli.hpp" // Подключаем нашу библиотеку для использования CLI (путь до библиотеки может отличаться)

void func(FlagsType &parsedFlags); // Объявляем функцию, которая будет вызывать при вызове команды printHello
void func2(FlagsType &parsedFlags); // Объявляем функцию, которая будет вызывать при вызове команды printName

int main(int argc, char **argv) {
    auto cli = Cli();
    try {
        cli.command("printHello", "Выводит на экран слово \"Hello!\"", "$ printHello \n >>> Hello!", {}, func) // Добавляем команду printHello
                .command("printName", "Выведет на экран \"Hello введённое_имя!\"", "$ printName -n Name\n >>> Hello Name!",
                         {
                                 Flag("name", "n", "Флаг, который принимает имя на вход", true, true),
                                 Flag("surname", "s", "Флаг, который принимает фамилию на вход", true, true)
                         }, func2) // Добавляем команду printName и указываем флаги name и surname
                .parse(argc, argv); // Обязательно вызываем функцию parse c аргументами argc и argv
    } catch (const std::invalid_argument &error) { // Обрабатываем какие-либо ошибки
        std::cout << error.what();
        return 2; // код завершения программы при ошибке
    }
    return 0;
}

void func(FlagsType &parsedFlags) { // Определение функции команды printHello
    std::cout << "Hello!\n";
}

void func2(FlagsType &parsedFlags) { // Определение функции команды printName
    std::cout << "Hello " << parsedFlags.at("name").value << " " << parsedFlags.at("surname").value << "!\n";
}