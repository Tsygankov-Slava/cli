#include <iostream>

#include "Cli/Cli.hpp"

void func(flagsType &parsedFlags) {
    std::cout << "Hi!\n";
    for (auto &[name, flag] : parsedFlags) {
        std::cout << name << " -> " << flag.value << "\n";
    }
}

int main(int argc, char **argv) {
    Cli cli = Cli();
    try {
        cli.command("vars", "Выведет файл с переменными", "example for vars",
                    {}, func)
                .command("history", "Выведет историю вычислений выражений", ">>> history()\n"
                                                                            "\tИстория вычислений: \n"
                                                                            "\t\t2 + 2 * 2 = 6\n"
                                                                            "\t\t5 + 6 = 11",
                         {{Flag("flag", "f", "it's description for flag", true, true)}}, func)
                .parse(argc, argv);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        return 2;
    }
    return 0;
}
