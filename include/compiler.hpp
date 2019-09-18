#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <string>
#include <vector>

#include "scanner.hpp"

namespace stackmachine {

    class scanner;

    class compiler {
    public:
	compiler() = default;
        compiler(const std::string &file): m_file(file) {}

	void load_file();

	void run();
    private:
	std::string m_file;

	std::vector<char> m_buf;

	scanner m_scanner;
    };

}
#endif
