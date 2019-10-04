#ifndef DATA_TYPE_HPP
#define DATA_TYPE_HPP

#include <unordered_map>
#include <string>
#include <utility>
#include <vector>

namespace stackmachine {

    enum class DataType { //NOTE: The order is important!
        SHORT = 64,
        INT,
	LONG,
        FLOAT,
    };

    enum class OperandType {
	DATA = 255,
	SYMBOL
    };

    union OperandEntry {
	int datai;
	float dataf;
	short datas;
	long datal;
    };
    
    static std::unordered_map<DataType, std::string> datatypes {
	{DataType::INT, "i"},
	{DataType::FLOAT, "f"},
	{DataType::SHORT, "s"},
        {DataType::LONG, "l"}
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
    };

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
	
	std::pair<iterator, bool> insert(const std::string &key, OperandEntry &&symbol);

	std::pair<iterator, bool> insert(const std::string &key, OperandEntry &symbol);

	std::pair<iterator, bool> insert(const value_type &value);
	
	std::pair<iterator, bool> insert(value_type &&value);

	const_iterator lookup(const std::string &symbol) const;

	const OperandEntry at(const std::string &symbol) const;

	void reserve(size_t size);

	void rehash(size_t size);

	void swap(SymbolTable &other);

	iterator begin() { return iterator(this); }
	iterator end() { return iterator(this, m_buckets.size()); }

	const_iterator cbegin() const { return const_iterator(this); }
	const_iterator cend() const { return const_iterator(this, m_buckets.size()); }
    private:

	size_t getIndexByKey(std::string key) const;

	size_t probeForward(size_t i) const;

	std::pair<iterator, bool> insert_impl(const std::string &key, const OperandEntry &symbol);
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
