
#include <exception>
#include <algorithm>

#include "runtime_type.hpp"

using namespace stackmachine;


Number *stackmachine::valueOf(const OperandObject &op)
{
    OperandEntry entry = op.entry;
    switch(op.type) {
    case DataType::INT:
		return new Int(get<int>(entry));
    case DataType::FLOAT:
		return new Float(get<float>(entry));
    case DataType::LONG:
		return new Long(get<long>(entry));
    case DataType::SHORT:
		return new Short(get<short>(entry));
    }

    throw std::runtime_error{"Invalid type conversion"};
};

OperandEntry stackmachine::typeOf(Number *number)
{
    OperandEntry entry;
    switch (number->rawDataType()) {
    case DataType::SHORT: {
	    Short *snumber = static_cast<Short *>(number);
	    entry.datas = snumber->value();
	    return entry;
	}
    case DataType::INT: {
	    Int *inumber = static_cast<Int *>(number);
	    entry.datai = inumber->value();
	     
	    return entry;
	}

    case DataType::LONG: {
	    Long *lnumber = static_cast<Long *>(number);
	    entry.datal = lnumber->value();	
	    
	    return entry;
	}
    case DataType::FLOAT: {
	    Float *fnumber = static_cast<Float *>(number);
	    entry.dataf = fnumber->value();
	    
	    return entry;
	}
    }

    std::runtime_error{"Invalid type of number"};
}


void OpNumerator::add(std::initializer_list<Number *> li)
{

    std::sort(li.begin(), li.end(), [](Number *a, Number *b) {
	return a->dataType() > b->dataType();
    });

    Number *number = *li.begin();
    for (const auto &item : li) {
	switch (item->rawDataType()) {
	case DataType::SHORT: {
	    Short *n = static_cast<Short *>(item);
	    number->setValue(n + number);
	    break;
	}

	case DataType::INT: {
	    Int *n = static_cast<Int *>(item);
	    number->setValue(n + number); 
	    break;
	}

	case DataType::LONG: {
	    Long *n = static_cast<Long *>(item);
	    number->setValue(n + number);
	    break;	
	}

	case DataType::FLOAT: {
	    Float *n = static_cast<Float *>(item);
	    number->setValue(n + number);
	    break;
	}

	}
    }

    m_value = number;
}



