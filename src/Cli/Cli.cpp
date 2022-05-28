#include "Cli.h"

/**
* The function goes through the passed flags, puts the passed flags in the is Passed variable and writes the flag value (if such is assumed)
* Handles an error if there is not enough argument for the flag
* */
State Cli::parseFlags() {
    for (int i = 0; i < argc; ++i) {
        auto name = argv[i];
        if (flags.contains(name)) {
            auto &flag = flags.at(name);
            if (flag.action != nullptr) {
                flag.action();
            }
            flag.isPassed = true;
            if (flag.withValue) {
                if (i == argc - 1) {
                    return State::WRONG;
                }
                ++i;
                flag.value = argv[i];
                if (flag.value[0] == '-') {
                    return State::WRONG;
                }
            }
        } else if (arrAllProblemName.contains(name)) {
            (void) arrProblemName.insert(name);
        }
    }
    return State::OK;
}

/**
* The function checks whether all the required arguments regarding the Problem Name task have been entered
* */
State Cli::checkRequiredFlags() {
    for (const auto &[name, flag] : flags) {
        if (arrProblemName.contains(flag.problemName) && flag.isRequired && !flag.isPassed) {
            return WRONG;
        }
    }
    return OK;
}