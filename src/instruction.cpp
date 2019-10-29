#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>

#include "instruction.hpp"
#include "util.hpp"
#include "data_type.hpp"
#include "machine.hpp"

using namespace stackmachine;

static auto findInstr(const std::string &name) 
{
    auto instr = std::find_if(std::begin(instructions), std::end(instructions), 
		    [&name](const auto &i) { return i.name.compare(name) == 0;} ); 

    if (instr != std::end(instructions)) {
	return *instr;
    }   

    throw std::out_of_range{"Instruction hasn't found"};
}


static PackedOperand parseOperand(std::string &operand)
{
    if (findAndExcludeSymbol(operand, "%")) {
	SymbolObject symbol{operand, 1};
	
	return PackedOperand{symbol};
    }

    if (findAndExcludeSymbol(operand, datatypes[DataType::FLOAT])) {
	OperandObject op{sizeof(float), DataType::FLOAT};
	op.entry.dataf = std::stof(operand);

	return PackedOperand{op};
    }
    
    if (findAndExcludeSymbol(operand, datatypes[DataType::SHORT])) {
	OperandObject op{sizeof(short), DataType::SHORT};
	op.entry.datas = static_cast<short>(std::stoi(operand));

	return PackedOperand{op};
    } 
    
    if (findAndExcludeSymbol(operand, datatypes[DataType::LONG])) {
	OperandObject op{sizeof(long), DataType::LONG};
	op.entry.datal = std::stol(operand);

	return PackedOperand{op};
    } 


    OperandObject op{sizeof(int), DataType::INT};
    op.entry.datai = std::stoi(operand);		
    
    return PackedOperand{op};
}

static std::vector<PackedOperand> parseOperands(std::vector<std::string> &operands)
{
    std::vector<PackedOperand> opObjects;
    
    for (auto &operand : operands) {
        
	opObjects.emplace_back(parseOperand(operand));
    }    

    return opObjects;
}

void stackmachine::execute(const std::string &line, machine &m)
{
    std::vector<std::string> input = split(line, ' ');
    auto opcode = findInstr(input[0]);
    std::vector<std::string> operands{};
    if (input.size() > 2) {

       std::copy(input.begin() + 1, input.end(), std::back_inserter(operands));	
       m.execute(opcode, parseOperands(operands));
    } else if (input.size() > 1) {    
    	input = split(input[1], ',');
	if (input.size() > 1) { 	
           std::copy(input.begin() + 1, input.end(), std::back_inserter(operands));	
           m.execute(opcode, parseOperands(operands)); 
	} else {
	    auto opObject = parseOperand(input[0]);
	    m.execute(opcode, opObject);
	}
    } else {
        m.execute(opcode);
    } 
}


