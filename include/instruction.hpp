#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <array>
#include <string>
#include <cstdint>

namespace stackmachine {

    class machine;

    enum class Opcode {
        LOAD = 128,
		ADD,
		SUB,
		MUL,
		DIV,
		PUSH,
    };

    struct Instruction {
        Opcode opcode;
		std::string name;
    };

    constexpr int N_INSTRUCTIONS = 6;

    const std::array<Instruction, N_INSTRUCTIONS> instructions {{
		{Opcode::LOAD, "load"},
		{Opcode::ADD, "add"},
		{Opcode::MUL, "mul"},
		{Opcode::DIV, "div"},
		{Opcode::SUB, "sub"},
		{Opcode::PUSH, "push"},
    }};

    void execute(const std::string &line, machine &m);
};

#endif
