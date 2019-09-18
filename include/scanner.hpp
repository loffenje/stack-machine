#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <cstdint>
#include <utility>
#include <vector>
#include <string>
#include <cctype>

namespace stackmachine {

    enum class TokenKind {
        COMMA,
	COLON,
	SEMICOLON,
	NUMBER,
	LITERAL
    };


    struct Token {
         TokenKind kind;
	 void *literal;
    };

    class scanner {
    public:
	scanner() = default;

	scanner(std::vector<char> buf): m_buffer(std::move(buf)), m_current(0), m_start(0) {}
	
	std::vector<Token> scan();

	void consume_number();

	void consume_literal();

	void add_token(TokenKind t);

	void add_token(TokenKind t, void *literal);

        char peek() const;

	char peek_forward() const;
        
	char next_token();

   private:

	int32_t m_current;

	int32_t m_start;

	std::vector<Token> m_tokens;

	std::vector<char> m_buffer;
    };

}

#endif
