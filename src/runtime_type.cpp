
#include <exception>
#include <algorithm>

#include "runtime_type.hpp"

using namespace stackmachine;


Number* stackmachine::valueOf(const OperandObject& op)
{
	OperandEntry entry = op.entry;
	switch (op.type) {
	case DataType::INT:
		return new Int(get<int>(entry));
	case DataType::FLOAT:
		return new Float(get<float>(entry));
	case DataType::LONG:
		return new Long(get<long>(entry));
	case DataType::SHORT:
		return new Short(get<short>(entry));
	}

	throw std::runtime_error{ "Invalid type conversion" };
};

OperandEntry stackmachine::typeOf(Number* number)
{
	OperandEntry opEntry;
	switch (number->rawDataType()) {
	case DataType::SHORT: {
		Short* snumber = static_cast<Short*>(number);
		opEntry.datas = static_cast<short>(std::stoi(snumber->value()));
		return opEntry;
	}
	case DataType::INT: {
		Int* inumber = static_cast<Int*>(number);
		opEntry.datai = std::stoi(inumber->value());
		return opEntry;
	}

	case DataType::LONG: {
		Long* lnumber = static_cast<Long*>(number);
		opEntry.datal = std::stol(lnumber->value());
		return opEntry;
	}

	case DataType::FLOAT: {
		Float* fnumber = static_cast<Float*>(number);
		opEntry.dataf = std::stof(fnumber->value());
		return opEntry;
	}
	}

	std::runtime_error{ "Invalid type of number" };
}

Number* OpNumerator::getPriorNumber(std::initializer_list<Number*> li) const
{
	Number* number = *std::max_element(li.begin(), li.end(), [](const Number * a, const Number * b) {
		return a->dataType() < b->dataType();
	});

	Number* priorNumber;
	switch (number->rawDataType()) {
	case DataType::FLOAT:
		priorNumber = new Float();
		break;
	case DataType::LONG:
		priorNumber = new Long();
		break;
	case DataType::INT:
		priorNumber = new Int();
		break;
	case DataType::SHORT:
		priorNumber = new Short();
		break;
	}

	return priorNumber;
}


void OpNumerator::add(std::initializer_list<Number*> li)
{
	Number* priorNumber = getPriorNumber(li);
	for (const auto& item : li) {
		switch (item->rawDataType()) {
		case DataType::SHORT: {
			Short* n = static_cast<Short*>(item);
			priorNumber->add(n->value());
			break;
		}

		case DataType::INT: {
			Int* n = static_cast<Int*>(item);
			priorNumber->add(n->value());
			break;
		}

		case DataType::LONG: {
			Long* n = static_cast<Long*>(item);
			priorNumber->add(n->value());
			break;
		}

		case DataType::FLOAT: {
			Float* n = static_cast<Float*>(item);
			priorNumber->add(n->value());
			break;
		}

		}
	}

	m_value = priorNumber;
}


void OpNumerator::sub(std::initializer_list<Number*> li)
{
	Number* priorNumber = getPriorNumber(li);
	for (const auto& item : li) {
		switch (item->rawDataType()) {
		case DataType::SHORT: {
			Short* n = static_cast<Short*>(item);
			priorNumber->sub(n->value());
			break;
		}

		case DataType::INT: {
			Int* n = static_cast<Int*>(item);
			priorNumber->sub(n->value());
			break;
		}

		case DataType::LONG: {
			Long* n = static_cast<Long*>(item);
			priorNumber->sub(n->value());
			break;
		}

		case DataType::FLOAT: {
			Float* n = static_cast<Float*>(item);
			priorNumber->sub(n->value());
			break;
		}

		}
	}

	m_value = priorNumber;
}


void OpNumerator::mul(std::initializer_list<Number*> li)
{
	Number* priorNumber = getPriorNumber(li);
	for (const auto& item : li) {
		switch (item->rawDataType()) {
		case DataType::SHORT: {
			Short* n = static_cast<Short*>(item);
			priorNumber->mul(n->value());
			break;
		}

		case DataType::INT: {
			Int* n = static_cast<Int*>(item);
			priorNumber->mul(n->value());
			break;
		}

		case DataType::LONG: {
			Long* n = static_cast<Long*>(item);
			priorNumber->mul(n->value());
			break;
		}

		case DataType::FLOAT: {
			Float* n = static_cast<Float*>(item);
			priorNumber->mul(n->value());
			break;
		}

		}
	}

	m_value = priorNumber;
}

void OpNumerator::div(std::initializer_list<Number*> li)
{
	Number* priorNumber = getPriorNumber(li);
	for (const auto& item : li) {
		switch (item->rawDataType()) {
		case DataType::SHORT: {
			Short* n = static_cast<Short*>(item);
			priorNumber->div(n->value());
			break;
		}

		case DataType::INT: {
			Int* n = static_cast<Int*>(item);
			priorNumber->div(n->value());
			break;
		}

		case DataType::LONG: {
			Long* n = static_cast<Long*>(item);
			priorNumber->div(n->value());
			break;
		}

		case DataType::FLOAT: {
			Float* n = static_cast<Float*>(item);
			priorNumber->div(n->value());
			break;
		}

		}
	}

	m_value = priorNumber;
}
