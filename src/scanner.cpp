#include "scanner.hpp"


using namespace stackmachine;

std::vector<Token> scanner::scan() {
    while(!m_buffer.empty()) {
        char c = next_token();
        switch(c) {
	    case ',': add_token(TokenKind::COMMA); break;
	    case ':': add_token(TokenKind::COLON); break;
	    case ';': add_token(TokenKind::SEMICOLON); break;
 	    default: {
               if (std::isdigit(c)) {
                   consume_number();
      	       } else if(std::isalpha(c)) {
  		   consume_literal();
	       }		 

	       break;
	    }
	}
    }

    return m_tokens;
}

char scanner::next_token() {
    m_current++;
    return m_buffer[m_current - 1];
}

void scanner::add_token(TokenKind t) {
     m_tokens.emplace_back(Token{t, nullptr});
}

void scanner::add_token(TokenKind t, void *literal) {
     m_tokens.emplace_back(Token{t, literal});
}

char scanner::peek() const {
    return m_buffer[m_current];
}

char scanner::peek_forward() const {
    return m_buffer[m_current + 1];
}

void scanner::consume_number() {
    while (std::isdigit(peek())) next_token();

    if (peek() == '.' && std::isdigit(peek_forward())) {
        next_token();

	while (std::isdigit(peek())) next_token();
    } 

    std::string source(m_buffer.begin(), m_buffer.end());

    std::string number = source.substr(m_start + 1, m_current - 1);
    double number_literal = std::stof(number);
    
    add_token(TokenKind::NUMBER, &number_literal);

}

void scanner::consume_literal() {
    while (std::isalpha(peek())) next_token();
    
    std::string source(m_buffer.begin(), m_buffer.end());
    add_token(TokenKind::LITERAL, &source);
}
