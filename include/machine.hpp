#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <cstdint>
#include <vector>

#include "instruction.hpp"
#include "runtime_type.hpp"
#include "gc.hpp"

#define DEBUG

namespace stackmachine {

class machine {
public:
	
    friend class GC;

    machine(const size_t memsize): m_memsize{memsize} {}

    int64_t getIP() const;

    void execute(Instruction opcode, std::vector<PackedOperand> &&operands);

    void execute(Instruction opcode, PackedOperand &operand);

    void execute(Instruction opcode);

    machine(const machine &) = delete;
    machine &operator=(const machine &) = delete;
    
private:

    void push(const OperandObject &obj);
    
    const OperandObject &pop();

    void pushObj(Object *obj);

    Object *popObj();

    bool isRunning() const { return m_running; }

    void execLoad(const OperandObject &operand);

    void execPush(const SymbolObject &operand);

    void execAdd();

    void execSub();

    void execMul();

    void execDiv();

    void flush();

    void checkMemory();

    void trackObject(Object *object);

    void gc();

    void checkBounds(size_t size, const std::string &instr);
private:

    int64_t m_ip{0};

    std::vector<OperandObject> m_stack;

    std::vector<Object *> m_allocObjStack;

    SymbolTable m_symbolTable;

    Object *m_firstObject{nullptr};

    GC m_GC;

    OpNumerator m_numerator;

    size_t m_memsize;

    int m_numObjects{0};

    int m_maxObjects{GC_THRESHOLD};

    bool m_running{true};
};

}

#endif
