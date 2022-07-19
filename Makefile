.PHONY: all clean

all:
	mkdir build; cd build; touch cli.hpp; cd ../src; grep -v "#include \"" Flag/Flag.hpp | cat >> ../build/cli.hpp | grep -v "#include \"" Flag/Flag.cpp | cat  >> ../build/cli.hpp; grep -v "#include \"" Command/Command.hpp | grep "[^#pragma once]" >> ../build/cli.hpp; grep -v "#include \"" Cli/Cli.hpp | grep "[^#pragma once]" >> ../build/cli.hpp; grep -v "#include \"" Cli/Cli.cpp | cat >> ../build/cli.hpp; grep -v "#include \"" Command/Command.cpp | cat >> ../build/cli.hpp;

clean:
	rm -rf build