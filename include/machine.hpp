#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <cstdint>
#include <vector>

#include "instruction.hpp"
#include "data_type.hpp"

namespace stackmachine {

class machine {
public:
    
    typedef unsigned char byte;

    machine(const size_t memsize): m_memsize(memsize), m_memory(new byte[memsize]), m_running(true), m_ip(0), m_sp(0) {}
    ~machine() { 
        delete[] m_memory;
    }   

    int64_t getIP() const;

    void execute(Instruction opcode, std::vector<PackedOperand> &&operands);

    void execute(Instruction opcode, PackedOperand &operand);

    void execute(Instruction opcode);

    machine(const machine &) = delete;
    machine &operator=(const machine &) = delete;
    
private:

    void push(const OperandObject &obj);
    
    const OperandObject &pop();

    bool isRunning() const { return m_running; }

    void execLoad(const OperandObject &operand);

    void execPush(const SymbolObject &operand);

    void execAdd();

    void execSub();

    void execDump();

    void execMul();

    void execDiv();

    void checkBounds(size_t size, const std::string &instr);
private:

    int64_t m_ip;

    int64_t m_sp;

    std::vector<OperandEntry> m_stack;

    std::vector<OperandObject> m_operandStack;

    SymbolTable m_symbolTable;

    byte *m_memory;

    size_t m_memsize;

    bool m_running;
};

}

#endif
