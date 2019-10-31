#include <iostream>

#include "util.hpp"
#include "machine.hpp"

using namespace stackmachine;

int main(int argc, char *argv[]) {
    machine m{0xFFF};	
    if (argc < 2) {
		std::string line;
		const char *prompt = "machine> ";
		std::cout << prompt;
		while (std::getline(std::cin, line)) {
			if (equalsTo(line, "exit") || equalsTo(line, "quite")) {
				std::cout << "Stopped\n";
				return 0;
			}	    

			execute(line, m);
			std::cout << prompt;
		}
	}
}
