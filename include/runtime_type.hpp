#ifndef RUNTIME_TYPE_HPP
#define RUNTIME_TYPE_HPP

#include <sstream>
#include <string>

#include "data_type.hpp"

namespace stackmachine {

class Number
{
public:
    virtual int dataType() = 0;
    virtual DataType rawDataType() = 0;
    virtual void setValue(const std::string &value) = 0;

    std::string value() const {
	return m_value;
    }

    const std::string &operator+(Number *a, Number *b)
    {
	std::stringstream ss;
	ss << std::stod(a->value()) + std::stod(b->value());

	return ss.str();	
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
    
    DataType rawDataType() override {
	return DataType::SHORT;
    }

    int dataType() override {
	return static_cast<int>(rawDataType());
    }

    void setValue(const std::string &value) override {
	short currValue = static_cast<short>(std::stoi(m_value));
	short val = static_cast<short>(std::stoi(value));

	m_value << currValue + val;
    }
};

class Int : public Number 
{
public:
    Int() = default;
    Int(int value) {m_value << value;}
    
    DataType rawDataType() override
    {
	return DataType::INT;
    }

    int dataType() override {
	return static_cast<int>(rawDataType());
    }

    void setValue(const std::string &value) override {
	int currValue = std::stoi(m_value);
	int val = std::stoi(value);

	m_value << currValue + val;
    }
};


class Long : public Number 
{
public:
    Long() = default;
    Long(long value) { m_value << value; }
    
    DataType rawDataType() override 
    {
        return DataType::LONG;
    }

    int dataType() override {
	return static_cast<int>(rawDataType());
    }

    void setValue(const std::string &value) override {
	long currValue = std::stol(m_value);
	long val = std::stol(value);

	m_value << currValue + val;
    }
};

class Float : public Number
{
public:
    Float() = default;
    
    Float(float value) {m_value << value; }

    DataType rawDataType() override 
    {
	return DataType::FLOAT;
    }

    int dataType() override {
	return static_cast<int>(rawDataType());
    }

    void setValue(const std::string &value) override {
	float currValue = std::stof(m_value);
	float val = std::stof(value);

	m_value << currValue + val;
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
    Number *m_value;
};

}

#endif
