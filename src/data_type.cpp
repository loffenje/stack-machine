#include <exception>
#include <functional>
#include "data_type.hpp"

using namespace stackmachine;
	
std::pair<SymbolTable::iterator, bool> SymbolTable::insert(const std::string &key, OperandEntry &&symbol)
{
	return insert_impl(key, std::move(symbol));
}

std::pair<SymbolTable::iterator, bool> SymbolTable::insert(const std::string &key, OperandEntry &symbol)
{
	return insert_impl(key, std::move(symbol));
}

std::pair<SymbolTable::iterator, bool> SymbolTable::insert(const value_type &value)
{
	return insert_impl(value.first, value.second);
}
	
std::pair<SymbolTable::iterator, bool> SymbolTable::insert(value_type &&value)
{
	return insert_impl(value.first, std::move(value.second));
}
        
SymbolTable::const_iterator SymbolTable::lookup(const std::string &symbol) const 
{
	for (size_t i = getIndexByKey(symbol);; i = probeForward(i)) {
		if (m_buckets[i].first.compare(symbol) == 0) {
		    return const_iterator(this, i);
		}
	}

	return cend();
}

const OperandEntry SymbolTable::at(const std::string &symbol) const 
{
	const_iterator it = lookup(symbol);
	if (it != cend()) {
		return it->second;
	}

	std::out_of_range{"the symbol is out of range"};
}

void SymbolTable::reserve(size_t size)
{
	if (size * 2 > m_buckets.size()) {
		rehash(size << 1);
	}
}

void SymbolTable::rehash(size_t size)
{
	size_t new_size = std::max(size, m_size * 2);
	SymbolTable other(*this, new_size);
	swap(other);
}

void SymbolTable::swap(SymbolTable &other)
{
	std::swap(m_buckets, other.m_buckets);
	std::swap(m_size, other.m_size);
}

size_t SymbolTable::getIndexByKey(std::string key) const
{
	const size_t mask = m_buckets.size() - 1;

	return std::hash<std::string>{}(key) % mask;
}

size_t SymbolTable::probeForward(size_t i) const 
{
	const size_t mask = m_buckets.size() - 1;

	return (i + 1) % mask;
}

std::pair<SymbolTable::iterator, bool> SymbolTable::insert_impl(const std::string &key, const OperandEntry &symbol)
{
	reserve(m_size + 1);
	for (size_t i = getIndexByKey(key);; i = probeForward(i)) {
		if (m_buckets[i].first.empty()) {
			m_buckets[i].second = symbol;
			m_buckets[i].first = key;
			m_size++;

			return std::make_pair(iterator(this, i), true);
		} else if (m_buckets[i].first.compare(key)) {
			return std::make_pair(iterator(this, i), false);
		}
	} 
}
