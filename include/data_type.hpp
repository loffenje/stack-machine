#ifndef DATA_TYPE_HPP
#define DATA_TYPE_HPP

#include <unordered_map>
#include <string>
#include <list>
#include <type_traits>
#include <utility>
#include <functional>

namespace stackmachine {

    enum class DataType {
        INT = 64,
        FLOAT,
        SHORT,
        LONG
    };

    enum class OperandType {
	DATA,
	SYMBOL
    };

    static std::unordered_map<DataType, std::string> datatypes {
        {DataType::INT, "i"},
        {DataType::FLOAT, "f"},
        {DataType::SHORT, "s"},
        {DataType::LONG, "l"}
    };

    union OperandEntry {
	int datai;
	float dataf;
	short datas;
	long datal;
    };
    
    template <typename T>
    constexpr T &get(const OperandEntry &u)
    {
	return *((T *)&u);
    }

    struct OperandObject {
	size_t nbytes;
	DataType type;
	OperandEntry entry;

	void add(const OperandObject &x, const OperandObject &y);
	void sub(const OperandObject &x, const OperandObject &y); 
	void mul(const OperandObject &x, const OperandObject &y); 
	void div(const OperandObject &x, const OperandObject &y);
    };



    template <DataType v>
    struct Data2Type {
        enum {value = v};
    };

    template <typename T>
    struct TypeOf 
    {
        typedef T type;
    };

    template <typename T>
    TypeOf<T> type_of(const T& v )
    {
	return TypeOf<T>();
    }

    typedef Data2Type<DataType::INT> Int_;
    typedef Data2Type<DataType::FLOAT> Float_;
    typedef Data2Type<DataType::SHORT> Short_;
    typedef Data2Type<DataType::LONG> Long_;

    template <typename T>
    auto getValue(const OperandObject &op)
    {
	if constexpr (std::is_same<Int_, T>::value) {
	    return op.entry.datai;
	} else if constexpr (std::is_same<Float_, T>::value) {
	   return op.entry.dataf;
	} else if constexpr (std::is_same<Long_, T>::value) {
	   return op.entry.datal;
	} else if constexpr (std::is_same<Short_, T>::value) {
	   return op.entry.datas;
	} else {
	   return op.entry.datai;	
	}
    }

        
	void OperandObject::add(const OperandObject &x, const OperandObject &y) 
	{
	    if (x.type == DataType::FLOAT) {
		
		entry.dataf = get<float>(x.entry) + getValue<Int_>(y);
		return;
	    }

	    if (x.type == DataType::INT) {
		entry.datai = get<int>(x.entry) + getValue<Int_>(y);
		return;
	    }

	    if (x.type == DataType::SHORT) {
		entry.datas = get<short>(x.entry) + getValue<Short_>(y);
		return;
	    }

	    if (x.type == DataType::LONG) {
	        entry.datal = get<long>(x.entry) + getValue<Long_>(y);
		return;
	    }

	}


	/*void OperandObject::sub(const OperandObject &x, const OperandObject &y) 
	{
	    if (x.type == DataType::FLOAT) {
		entry.dataf = get<float>(x.entry) - getValue(y.type, y);
		return;
	    }

	    if (x.type == DataType::INT) {
		entry.datai = get<int>(x.entry) - getValue(y.type, y);
		return;
	    }

	    if (x.type == DataType::SHORT) {
		entry.datas = get<short>(x.entry) - getValue(y.type, y);
		return;
	    }

	    if (x.type == DataType::LONG) {
	        entry.datal = get<long>(x.entry) - getValue(y.type, y);
		return;
	    }

	}

	void OperandObject::mul(const OperandObject &x, const OperandObject &y)
	{
	    if (x.type == DataType::FLOAT) {
		entry.dataf = get<float>(x.entry) * getValue(y.type, y);
		return;
	    }

	    if (x.type == DataType::INT) {
		entry.datai = get<int>(x.entry) * getValue(y.type, y);
		return;
	    }

	    if (x.type == DataType::SHORT) {
		entry.datas = get<short>(x.entry) * getValue(y.type, y);
		return;
	    }

	    if (x.type == DataType::LONG) {
	        entry.datal = get<long>(x.entry) * getValue(y.type, y);
		return;
	    }

	}


	void OperandObject::div(const OperandObject &x, const OperandObject &y)
	{
	    if (x.type == DataType::FLOAT) {
		entry.dataf = get<float>(x.entry) / getValue(y.type, y);
		return;
	    }

	    if (x.type == DataType::INT) {
		entry.datai = get<int>(x.entry) / getValue(y.type, y);
		return;
	    }

	    if (x.type == DataType::SHORT) {
		entry.datas = get<short>(x.entry) / getValue(y.type, y);
		return;
	    }

	    if (x.type == DataType::LONG) {
	        entry.datal = get<long>(x.entry) / getValue(y.type, y);
		return;
	    }
	}*/

	static auto createFromOperands(const OperandObject &x, const OperandObject &y)
	{
	    OperandObject result;
            if (x.type == DataType::FLOAT || y.type == DataType::FLOAT) {
                result.type = DataType::FLOAT;
                result.nbytes = sizeof(float);
    	    }
    
    	    if (x.type == DataType::LONG || y.type == DataType::LONG) {
                result.type = DataType::LONG;
		result.nbytes = sizeof(long);
    	    }

    	   if (x.type == DataType::INT || y.type == DataType::INT) {
      	       result.type = DataType::INT;
       	       result.nbytes = sizeof(int);
    	   }	
	   
           return result;
       }
    
    struct SymbolObject {
	std::string name;
	uint32_t stack_pos;
    };


    template <typename Hash, typename T>
    class HashTableIt {
    public:
    	using value_type = T;
        using reference = T&;
	using pointer = T*;
 
    public:
	explicit HashTableIt(): m_st(nullptr), m_index(0) {}
	explicit HashTableIt(Hash *st, size_t index): m_st(st), m_index(index) {}
	explicit HashTableIt(Hash *st): m_st(st) {} 

	HashTableIt &operator++() {
	    ++m_index;
	    return *this;
	}

	reference operator*() const { return m_st->m_buckets[m_index]; }
	
	pointer operator->() const { return &m_st->m_buckets[m_index]; }
	
	bool operator==(const HashTableIt &other) const {
	    return other.m_st == m_st && other.m_index == m_index;
	}

	bool operator!=(const HashTableIt &other) const {
	    return !(other == *this);
	}

    private:
	Hash *m_st;	
	size_t m_index;
	friend Hash;
    };

    class SymbolTable {
    public:	
	using value_type = std::pair<std::string, OperandEntry>;
	using buckets = std::vector<value_type>;
	using iterator = HashTableIt<SymbolTable, value_type>;
        using const_iterator = HashTableIt<const SymbolTable, const value_type>;
	friend class HashTableIt<SymbolTable, value_type>;
	friend class HashTableIt<const SymbolTable, const value_type>;
	
    public:
	SymbolTable() = default;
	SymbolTable(const SymbolTable &) = delete;
	const SymbolTable &operator=(SymbolTable &) = delete;

	SymbolTable(SymbolTable &other, size_t size): m_size(size) {
	    int pow2 = 1;
	    while (pow2 < size) {
		pow2 <<= 1;
	    }

	    OperandEntry op{};
	    m_buckets.resize(pow2, std::make_pair(std::string(), op));
	    // TODO: fix this, infinite loop while push
	    /*for (auto it = other.begin(); it != other.end(); ++it) {
		insert(*it);
	    }*/
	}

    public:
	
	std::pair<iterator, bool> insert(const std::string &key, OperandEntry &&symbol) {
	    return insert_impl(key, std::move(symbol));
	}


	std::pair<iterator, bool> insert(const std::string &key, OperandEntry &symbol) {
	    return insert_impl(key, std::move(symbol));
	}

	std::pair<iterator, bool> insert(const value_type &value) {
	    return insert_impl(value.first, value.second);
	}

	
	std::pair<iterator, bool> insert(value_type &&value) {
	    return insert_impl(value.first, std::move(value.second));
	}
        
	const_iterator lookup(const std::string &symbol) const {
	    for (size_t i = getIndexByKey(symbol);; i = probeForward(i)) {
		if (m_buckets[i].first.compare(symbol) == 0) {
		    return const_iterator(this, i);
		}
	    }

	    return cend();
	}

	const OperandEntry at(const std::string &symbol) const 
	{
	    const_iterator it = lookup(symbol);

	    if (it != cend()) {
		return it->second;
	    }

	    std::out_of_range{"the symbol is out of range"};
	}

	void reserve(size_t size)
	{
	    if (size * 2 > m_buckets.size()) {
		rehash(size << 1);
	    }
	}

	void rehash(size_t size)
	{
	    size_t new_size = std::max(size, m_size * 2);
	    SymbolTable other(*this, new_size);
	    swap(other);
	}

	void swap(SymbolTable &other)
	{
	    std::swap(m_buckets, other.m_buckets);
	    std::swap(m_size, other.m_size);
	}

	iterator begin() { return iterator(this); }
	iterator end() { return iterator(this, m_buckets.size()); }

	const_iterator cbegin() const { return const_iterator(this); }
	const_iterator cend() const { return const_iterator(this, m_buckets.size()); }
    private:

	size_t getIndexByKey(std::string key) const
	{
	    const size_t mask = m_buckets.size() - 1;

	    return std::hash<std::string>{}(key) % mask;
	}

	size_t probeForward(size_t i) const 
	{
	    const size_t mask = m_buckets.size() - 1;

	    return (i + 1) % mask;
	}

	std::pair<iterator, bool> insert_impl(const std::string &key, const OperandEntry &symbol)
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

    private:
	buckets m_buckets;
	size_t m_size = 0;
    };


    class PackedOperand {
    public:
	
	explicit PackedOperand(OperandObject &t): m_data(DataObject::get(std::move(t))) {}

	explicit PackedOperand(SymbolObject &t): m_data(DataObject::get(std::move(t))) {}	

        bool isDataOperand() const noexcept
	{
	    return m_data.type == OperandType::DATA;
	}	

	bool isSymbolOperand() const noexcept
	{
	    return m_data.type == OperandType::SYMBOL;
	}

	const OperandObject &getDataOperand() const
	{
	    return m_data.dataOp;
	}

	const SymbolObject &getSymbolOperand() const
	{
	    return m_data.symbolOp;
	}

    private:
	struct Data {
	     OperandType type;
	     union {
                 OperandObject dataOp;
  	         SymbolObject symbolOp;
	     };
    
	     Data(const Data &rhs) {
		type = rhs.type;
		dataOp = rhs.dataOp;
		symbolOp = rhs.symbolOp;
	     }

	     Data(OperandType t, OperandObject &data): type{t},  dataOp{data} {} 

	     Data(OperandType t, SymbolObject &data): type{t}, symbolOp{data} {}
	     
	     ~Data() {}
	};


     struct DataObject {
	static Data get(OperandObject &&t) {
	    return Data{OperandType::DATA, t};
	}

	static Data get(SymbolObject &&t) {
	    return Data{OperandType::SYMBOL, t};
	}

     };
    private:
	Data m_data;	
    };
}

#endif
