#include <stdexcept>
#include <iostream>

#include "machine.hpp"
#include "runtime_type.hpp"

using namespace stackmachine;

void machine::execute(Instruction opcode) {
  
    if (!isRunning()) {
	throw std::logic_error{"machine is not running"};
    }

    switch (opcode.opcode) {
	case Opcode::ADD:
	    execAdd();
	    return;
	 case Opcode::SUB:
	    execSub();
	    return;
 	 case Opcode::MUL:
	    execMul();
	    return;
	 case Opcode::DIV:
	    execDiv();
	    return;
	 case Opcode::DUMP:
	    execDump();
	    return;
    }

    throw std::logic_error{"Unknown opcode error"};
}

void machine::execute(Instruction opcode, std::vector<PackedOperand> &&operands)
{
    if (!isRunning()) {
	throw std::logic_error{"machine is not running"};
    }

}

void machine::execute(Instruction opcode, PackedOperand &operand)
{   
    if (!isRunning()) {
	throw std::logic_error{"machine is not running"};
    }

    switch (opcode.opcode) {
	case Opcode::LOAD:
	    if (operand.isDataOperand()) execLoad(operand.getDataOperand());
	    return;
	case Opcode::PUSH:
	    if (operand.isSymbolOperand()) execPush(operand.getSymbolOperand());
	    return;
    } 

    throw std::logic_error{"Unknown opcode error"};
}

void machine::checkBounds(size_t size, const std::string &instr)
{
    if (size < 0 || size >= m_memsize) {
	throw std::out_of_range{"bounds error " + instr};
    }
}

void machine::execLoad(const OperandObject &operand)
{ 

    checkBounds(operand.nbytes, "load");
    push(operand);
    m_ip += operand.nbytes;
    if (static_cast<size_t>(m_ip) >= m_memsize) {
        // halt
    }    
}

void machine::execPush(const SymbolObject &symbol)
{
  auto result = pop();
  m_symbolTable.insert(symbol.name, result.entry);
  OperandEntry e = m_symbolTable.at(symbol.name); 
  std::cout << "NAME: " << e.datai << std::endl;
}

void machine::execAdd()
{
    OperandObject x = pop();
    OperandObject y = pop();
    OperandObject result = OperandObject::createFromOperands(x,y);
    Number *numX = valueOf(x);
    Number *numY = valueOf(y);
    m_numerator.add({numX, numY});
    Number *num = m_numerator.value();
    auto entry = typeOf(num);
    result.entry = entry;
    std::cout << num->value() << '\n';
    push(result);
    if (static_cast<size_t>(m_ip) >= m_memsize) {
	// halt
    }
}

void machine::execSub()
{

    OperandObject x = pop();
    OperandObject y = pop();
    OperandObject result = OperandObject::createFromOperands(x,y);
    Number *numX = valueOf(x);
    Number *numY = valueOf(y);
    m_numerator.sub({numX, numY});
    Number *num = m_numerator.value();
    auto entry = typeOf(num);
    result.entry = entry;
    std::cout << num->value() << '\n';
    push(result);
    if (static_cast<size_t>(m_ip) >= m_memsize) {
	// halt
    }
}

void machine::execMul()
{
    OperandObject x = pop();
    OperandObject y = pop();
    OperandObject result = OperandObject::createFromOperands(x,y);
    Number *numX = valueOf(x);
    Number *numY = valueOf(y);
    m_numerator.mul({numX, numY});
    Number *num = m_numerator.value();
    auto entry = typeOf(num);
    result.entry = entry;
    std::cout << num->value() << '\n';
    push(result);
    if (static_cast<size_t>(m_ip) >= m_memsize) {
	// halt
    }
}

void machine::execDiv()
{
	
    OperandObject x = pop();
    OperandObject y = pop();
    OperandObject result = OperandObject::createFromOperands(x,y);
    Number *numX = valueOf(x);
    Number *numY = valueOf(y);
    m_numerator.div({numX, numY});
    Number *num = m_numerator.value();
    auto entry = typeOf(num);
    result.entry = entry;
    std::cout << num->value() << '\n';
    push(result);
    if (static_cast<size_t>(m_ip) >= m_memsize) {
	// halt
    }
}

void machine::execDump()
{
	//
}

void machine::push(const OperandObject &op)
{
    m_stack.emplace_back(op.entry);
    m_operandStack.emplace_back(op);
    m_ip += op.nbytes;
}

const OperandObject &machine::pop()
{
    if (m_stack.empty() || m_operandStack.empty()) {
	std::cerr << "Stack is empty" << '\n';
	exit(EXIT_FAILURE);
    }

    OperandObject &op = m_operandStack.back();

    m_operandStack.pop_back();
    m_stack.pop_back();
    m_ip -= op.nbytes;
    return op;
}
