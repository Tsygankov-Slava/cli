#include <iostream>

#include "Cli/Cli.hpp"
#include "Command/Command.hpp"

int main(int argc, char **argv) {
    Cli cli = Cli();
    cli.command("name1", "description", "example",
                {{Flag("flag1", "shortName", "description", false, false),
                  Flag("flag2", "shortName", "description", false, false)}},
                nullptr)
            .command("name2", "description2", "example2",
                     {{Flag("flag3", "shortName", "description", false, false)}}, nullptr)
            .parse(argc, argv);

    return 0;
}