#ifndef TYPE_HOLDER_HPP
#define TYPE_HOLDER_HPP


#include <typeinfo>

constexpr const char *ID_INT = "i";
constexpr const char *ID_FLOAT = "f";
constexpr const char *ID_LONG = "l";
constexpr const char *ID_SHORT = "s";

struct OpType
{
    const char *type;
    OperandEntry value;

    static OpType typeOf(const OperandObject &op)
    {
	OpType ret{};
	if (op.type == DataType::INT) {
	    ret.type = typeid(op.entry.datai).name();
	    ret.value.datai = std::move(op.entry); 
	} else if (op.type == DataType::FLOAT) {
	    ret.type = typeid(op.entry.dataf).name();
	    ret.value.dataf = std::move(op.entry);
	} else if (op.type == DataType::LONG) {
	    ret.type = typeid(op.entry.datal).name();
	    ret.value.datal = std::move(op.entry);
	} else if (op.type == DataType::SHORT) {
	    ret.type = typeid(op.entry.datas).name();
	    ret.value.datas = std::move(op.entry);
	} 

	return ret;
    }
};

class OpNumerator 
{
public:
    template <typename ...Ts>
    void add(OpType entry, Ts... entries)
    {
	
        m_opBox = OpBox::createContext(entries);

	
    }
private:
    struct OpBox {
	std::vector<OpType> opTypes;
	bool isCreated;
	static OpBox createContext(std::initializer_list<OpType> types) 
	{
	    opTypes.insert(opTypes.end(), types.begin(), types.end());
	    isCreated = true;
	}


    };

private:
    OpBox m_opBox;

};

#endif
