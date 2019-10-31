#include <stdexcept>
#include <iostream>

#include "machine.hpp"
#include "runtime_type.hpp"

#ifdef DEBUG
#include <cstdio>
#endif

using namespace stackmachine;

void machine::execute(Instruction opcode)
{
	if (!isRunning()) {
		throw std::logic_error{ "machine is not running" };
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
	}

	throw std::logic_error{ "Unknown opcode error" };
}

void machine::execute(Instruction opcode, std::vector<PackedOperand>&& operands)
{
	if (!isRunning()) {
		throw std::logic_error{ "machine is not running" };
	}
}

void machine::execute(Instruction opcode, PackedOperand& operand)
{
	if (!isRunning()) {
		throw std::logic_error{ "machine is not running" };
	}

	switch (opcode.opcode) {
	case Opcode::LOAD:
		if (operand.isDataOperand()) execLoad(operand.getDataOperand());
		return;
	case Opcode::PUSH:
		if (operand.isSymbolOperand()) execPush(operand.getSymbolOperand());
		return;
	}

	throw std::logic_error{ "Unknown opcode error" };
}

void machine::checkBounds(size_t size, const std::string& instr)
{
	if (size < 0 || size >= m_memsize) {
		throw std::out_of_range{ "bounds error " + instr };
	}
}

void machine::execLoad(const OperandObject& operand)
{
	checkBounds(operand.nbytes, "load");
	push(operand);
	m_ip += operand.nbytes;

	checkMemory();
}

void machine::execPush(const SymbolObject& symbol)
{
	auto result = pop();
	m_symbolTable.insert(symbol.name, result.entry);
	OperandEntry e = m_symbolTable.at(symbol.name);
#ifdef DEBUG 
	std::cout << "NAME: " << e.datai << '\n';
#endif
}

void machine::flush()
{
	for (int i = 0; i < m_allocObjStack.size(); i++) {
		popObj();
	}
}

void machine::execAdd()
{
	// pop elements from the stack
	OperandObject x = pop();
	OperandObject y = pop();

	// define what data type should be casted to 
	OperandObject result = OperandObject::createFromOperands(x, y);

	// allocate and track objects in order to flush them later 
	Number* numX = valueOf(x);
	trackObject(numX);

	Number* numY = valueOf(y);
	trackObject(numY);

	m_numerator.add({ numX, numY });
	Number* num = m_numerator.value();
	trackObject(num);

	// pop unused objects and collect them via gc later
	flush();

	OperandEntry entry = typeOf(num);
	result.entry = entry;
	std::cout << num->value() << '\n';
	push(result);

	checkMemory();
}

void machine::execSub()
{
	// pop elements from the stack
	OperandObject x = pop();
	OperandObject y = pop();

	// define what data type should be casted to 
	OperandObject result = OperandObject::createFromOperands(x, y);

	// allocate and track objects in order to flush them later 
	Number* numX = valueOf(x);
	trackObject(numX);

	Number* numY = valueOf(y);
	trackObject(numY);

	m_numerator.sub({ numX, numY });
	Number* num = m_numerator.value();
	trackObject(num);

	// pop unused objects and collect them via gc later
	flush();

	OperandEntry entry = typeOf(num);
	result.entry = entry;
	std::cout << num->value() << '\n';
	push(result);

	checkMemory();
}

void machine::execMul()
{
	// pop elements from the stack
	OperandObject x = pop();
	OperandObject y = pop();

	// define what data type should be casted to 
	OperandObject result = OperandObject::createFromOperands(x, y);

	// allocate and track objects in order to flush them later 
	Number* numX = valueOf(x);
	trackObject(numX);

	Number* numY = valueOf(y);
	trackObject(numY);

	m_numerator.mul({ numX, numY });
	Number* num = m_numerator.value();
	trackObject(num);

	// pop unused objects and collect them via gc later
	flush();

	OperandEntry entry = typeOf(num);
	result.entry = entry;
	std::cout << num->value() << '\n';
	push(result);

	checkMemory();
}

void machine::execDiv()
{
	// pop elements from the stack
	OperandObject x = pop();
	OperandObject y = pop();

	// define what data type should be casted to 
	OperandObject result = OperandObject::createFromOperands(x, y);

	// allocate and track objects in order to flush them later 
	Number* numX = valueOf(x);
	trackObject(numX);

	Number* numY = valueOf(y);
	trackObject(numY);

	m_numerator.div({ numX, numY });
	Number* num = m_numerator.value();
	trackObject(num);

	// pop unused objects and collect them via gc later
	flush();

	OperandEntry entry = typeOf(num);
	result.entry = entry;
	std::cout << num->value() << '\n';
	push(result);

	checkMemory();
}

void machine::checkMemory()
{
	if (static_cast<size_t>(m_ip) >= m_memsize) {
		// halt
		throw std::runtime_error{ "Out of memory" };
	}
}

void machine::trackObject(Object* obj)
{

#ifdef DEBUG
	printf("NUM OBJECTS: %d\n", m_numObjects);
	printf("MAX OBJECTS: %d\n", m_maxObjects);
#endif
	if (m_numObjects == m_maxObjects) gc();
	obj->next = m_firstObject;
	m_firstObject = obj;
	pushObj(obj);

	m_numObjects++;
}

void machine::pushObj(Object * obj)
{
	m_allocObjStack.emplace_back(obj);
}

Object* machine::popObj()
{
	if (!m_allocObjStack.empty()) {
		Object* obj = m_allocObjStack.back();
		m_allocObjStack.pop_back();

		return obj;
	}

	return nullptr;
}

void machine::push(const OperandObject & op)
{
	m_stack.emplace_back(op);
	m_ip += op.nbytes;
}

const OperandObject& machine::pop()
{
	if (m_stack.empty()) {
		std::cerr << "Stack underflow" << '\n';
		exit(EXIT_FAILURE);
	}

	OperandObject& op = m_stack.back();
	m_stack.pop_back();
	m_ip -= op.nbytes;

	return op;
}

void machine::gc()
{
#ifdef DEBUG
	int numObjects = m_numObjects;
#endif

	m_GC.boot(this);

	m_maxObjects = m_numObjects * 2;
#ifdef DEBUG
	printf("Collected %d objects, %d remaining\n", numObjects - m_numObjects, m_numObjects);
#endif
}
