#include <iostream>

#include "Cli/Cli.h"


void printHelpInfo();

int main(int argc, char **argv) {
    Cli cli = Cli(argc, argv).flag("--help",  "", "", false, false, printHelpInfo)
                      .flag("--in", "encode", "", true, true)
                      .flag("--out", "encode", "", true, true);
    auto state = cli.parseFlags();
    if (state == State::WRONG) {
        std::cout << "WRONG_1";
        return 1;
    }
    state = cli.checkRequiredFlags();
    if (state == State::WRONG) {
        std::cout << "WRONG_2";
        return 1;
    }
    return 0;
}

void printHelpInfo() {
    std::cout << "Описание флагов:\n"
                 "\t--help        : Вывод справки.\n"
                 "\t--description : Описание программы.\n"
                 "\t--in          : Флаг через который нужно передавать файл, который надо сжать. [!обязательный флаг, требующий аргумент после себя]\n"
                 "\t--out         : Флаг через который нужно передать предпочтительное название сжатого файла. [!обязательный флаг, требующий аргумент после себя]\n"
                 "\n"
                 "Пример:\n"
                 ">>> ./Huffman_Compression --in input.txt --out output.txt\n"
                 "В примере выше мы передаём в нашу программу для сжатия файл input.txt, а на выходе получаем сжатый файл output.txt\n"
                 ">>> ./Huffman_Compression --in C:/Test/input.txt --out D:/Test/output.txt\n"
                 "В примере выше мы передаём в нашу программу для сжатия файл input.txt, который находится на диске C:/ в директории Test/, а на выходе получаем сжатый файл output.txt, который будет находится на диске D:/ в директории Test/\n";
}