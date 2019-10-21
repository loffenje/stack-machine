#ifndef RUNTIME_TYPE_HPP
#define RUNTIME_TYPE_HPP

#include <sstream>
#include <string>

#include "data_type.hpp"

namespace stackmachine {

class Number
{
public:
    Number() { m_value << ""; }
    virtual int dataType() const = 0;
    virtual DataType rawDataType() const = 0;

    std::string value() const {
	return m_value.str();
    }

    void add(const std::string &num)
    {
	auto currValue = m_value.str().empty() ? 0 : std::stod(m_value.str());
	m_value.str("");
	m_value << currValue + std::stod(num);
    }


    void sub(const std::string &num)
    {
	if (m_value.str().empty()) {
	    m_value << std::stod(num);
	    return;
	}

	auto currValue = std::stod(m_value.str());
	m_value.str("");
	m_value << currValue - std::stod(num);
    }

    void mul(const std::string &num)
    {

	if (m_value.str().empty()) {
	    m_value << std::stod(num);
	    return;
	}

    	auto currValue = std::stod(m_value.str());
	m_value.str("");
	m_value << currValue * std::stod(num);
    }

    void div(const std::string &num)
    {

	if (m_value.str().empty()) {
	    m_value << std::stod(num);
	    return;
	}

    	auto currValue = std::stod(m_value.str());
	m_value.str("");
	m_value << currValue / std::stod(num);
    }

    virtual ~Number() {}

protected:
    std::stringstream m_value;
};


class Short : public Number 
{
public:
    Short() = default;
    Short(short value) { m_value << value; }
    
    DataType rawDataType() const override {
	return DataType::SHORT;
    }

    int dataType() const override {
	return static_cast<int>(rawDataType());
    }
};

class Int : public Number 
{
public:
    Int() = default;
    Int(int value) {m_value << value;}
    
    DataType rawDataType() const override
    {
	return DataType::INT;
    }

    int dataType() const override {
	return static_cast<int>(rawDataType());
    }
};


class Long : public Number 
{
public:
    Long() = default;
    Long(long value) { m_value << value; }
    
    DataType rawDataType() const override 
    {
        return DataType::LONG;
    }

    int dataType() const override {
	return static_cast<int>(rawDataType());
    }
};

class Float : public Number
{
public:
    Float() = default;
    
    Float(float value) {m_value << value; }

    DataType rawDataType() const override 
    {
	return DataType::FLOAT;
    }

    int dataType() const override {
	return static_cast<int>(rawDataType());
    }
};


Number *valueOf(const OperandObject &op);

OperandEntry typeOf(Number *number);

class OpNumerator 
{
public:
    void add(std::initializer_list<Number *> li);

    void sub(std::initializer_list<Number *> li);

    void mul(std::initializer_list<Number *> li);

    void div(std::initializer_list<Number *> li);

    Number *value() const 
    {
	return m_value;
    }

private:
    Number *getPriorNumber(std::initializer_list<Number *>) const;

private:
    Number *m_value;
};

}

#endif
