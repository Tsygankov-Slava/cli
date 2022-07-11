#include <iostream>

#include "Cli/Cli.hpp"

int main(int argc, char **argv) {
    Cli cli = Cli();
    try {
        cli.command("vars", "Выведет файл с переменными", "description for vars",
                    {}, nullptr)
                .command("history", "Выведет историю вычислений выражений", ">>> history()\n"
                                                                              "\tИстория вычислений: \n"
                                                                              "\t\t2 + 2 * 2 = 6\n"
                                                                              "\t\t5 + 6 = 11",
                         {{Flag("flag", "f", "it's description for flag", true, false)}}, nullptr)
                .parse(argc, argv);
    } catch (const std::invalid_argument& error) {
        std::cout << error.what();
        return 2;
    }
    return 0;
}
