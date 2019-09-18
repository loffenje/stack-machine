#include <fstream>
#include <vector>
#include <utility>
#include <iostream>
#include <stdexcept>

#include "compiler.hpp"


using namespace stackmachine;

void compiler::load_file() {
    std::ifstream file (m_file, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
         throw std::runtime_error("file not exist");
    }

    std::streamsize size = file.tellg();
    std::vector<char> buffer(size + 1);
    if (file.read(buffer.data(), size)) {
        scanner s{buffer};
	m_scanner = s;
    }
}


void compiler::run() {
    load_file();
    
    auto tokens = m_scanner.scan();
    for (auto i: tokens) {
        std::cout << i.literal << '\n';
    }
    //compile(tokens);
}



