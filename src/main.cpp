#include <iostream>

#include "Cli/Cli.hpp"

int main(int argc, char **argv) {
    Cli cli = Cli();
    try {
        cli.command("vars()", "Выведет файл с переменными", ">>> vars() \nКол-во переменных в файле -> 5\n"
                                                            "\tПеременные из файла: \n"
                                                            "\t\tddx = ((1.3+PI*i)+(exp((1.3+PI*i))))^2+1\n"
                                                            "\t\tx = 1.3+PI*i\n"
                                                            "\t\txyz = (1.3+PI*i)+(exp((1.3+PI*i)))+((1.3+PI*i)+(exp((1.3+PI*i))))+(((1.3+PI*i)+(exp((1.3+PI*i))))^2+1)\n"
                                                            "\t\ty = exp((1.3+PI*i))\n"
                                                            "\t\tz = (1.3+PI*i)+(exp((1.3+PI*i)))",
                    {}, nullptr)
                .command("history()", "Выведет историю вычислений выражений", ">>> history()\n"
                                                                              "\tИстория вычислений: \n"
                                                                              "\t\t2 + 2 * 2 = 6\n"
                                                                              "\t\t5 + 6 = 11",
                         {{Flag("flag", "f", "it's description for flag", false, false)}}, nullptr)
                .parse(argc, argv);
    } catch (const std::invalid_argument& error) {
        std::cout << error.what() << "\033[0m\n";
        return 199;
    }
    return 0;
}