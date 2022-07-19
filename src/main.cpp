#include "../build/cli.hpp" // Подключаем нашу библиотеку для использования CLI

void func(flagsType &parsedFlags); // Объявляем функцию, которая будет вызывать при вызове команды printHello
void func2(flagsType &parsedFlags); // Объявляем функцию, которая будет вызывать при вызове команды printName

int main(int argc, char **argv) {
    auto cli = Cli(); // Объявляем объект cli
    try {
        cli.command("printHello", "Выводит на экран слово \"Hello!\"", "$ printHello \n >>> Hello!",
                    {}, func) // Добавляем команду printHello
                .command("printName", "Выведет на экран \"Hello введённое_имя!\"", "$ printName -n Name\n"
                                                                                   ">>> Hello Name!",
                         {{Flag("name", "n", "Флаг, который принимает имя на вход", true, true)}, {Flag("surname", "s", "Флаг, который принимает фамилию на вход", true, true)}}, func2) // Добавляем команду printName и указываем флаги
                .parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        return 2;
    }
    return 0;
}

void func(flagsType &parsedFlags) {
    std::cout << "Hello!\n";
}

void func2(flagsType &parsedFlags) {
    std::cout << "Hello " << parsedFlags.at("name").value << " " << parsedFlags.at("surname").value << "!\n";
}