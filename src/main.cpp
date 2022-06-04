#include <iostream>

#include "Cli/Cli.hpp"


int main(int argc, char **argv) {
    Cli cli = Cli(argc, argv);
    cli.command("name", "description", "example", nullptr)
            .flag("flag1", "shortName", "description", false, false)
            .flag("flag2", "shortName", "description", false, false);
    cli.command("name2", "description2", "example2", nullptr)
            .flag("flag3", "shortName", "description", false, false)
            .flag("flag4", "shortName", "description", false, false);
    return 0;
}