// lexer.cpp : Defines the entry point for the application.
//

#include "lexer.h"
class Token {
public:
	enum class Type {
		identifier,
		constant,
		leftcurly,
		rightcurly,
		keyword,
		string,
		leftparam,
		rightparam,
		dot,
		semicolon,
		coma,
		pipe,
		end,
		eol,
		colan,
		cons_str,
		unknown
	};
	Token(Type type)noexcept : _Type(type) {}
	Token(Type type, const char* beg, const char* end) : _Type(type), _StrView(beg, std::distance(beg, end))  {}
	bool is(Type type) const noexcept { return type == _Type; }
	bool is_one_of(Type k1, Type k2) const noexcept { return is(k1) || is(k2); }
	std::string_view lexeme() const noexcept { return _StrView; }
	Type type() const noexcept { return _Type; }

	
private:
	std::string_view _StrView;
	Type _Type;
};

bool is_numeric(char ch) {
	switch (ch) {
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '0':
	case '.':
		return true;
	}
	return false;
}
bool is_identifier_char(char ch) noexcept {
	switch (ch) {
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return true;
	}
	return false;
}
bool is_keyword(const std::string_view str) noexcept {
	static const std::string_view keywords[] = {
		"def",
		"return",
		"if",
		"else",
		"while",
		"for",
		"in",
		"break",
		"continue",
		"true",
		"false",
		"null"
	};
	for (const auto& kw : keywords) {
		if (kw == str) return true;
	}
	return false;
}
class Lexer {
public:
	Lexer(const char* beg)noexcept: _beg(beg) {}
	Token next() noexcept;

private:
	Token identifier_or_keyword() noexcept;
	Token atomic(Token::Type type) noexcept;
	Token number() noexcept;
	const char* _beg;
	Token const_string() noexcept;
	char peek()const noexcept { return *_beg; }
	char get()noexcept { return *_beg++;}
};

Token Lexer::const_string() noexcept {
	const char* start = _beg;
	get(); // skip opening "
	while (peek() != '"' && peek() != '\0') get();
	get(); // skip closing "
	return Token(Token::Type::cons_str, start, _beg);
}
Token Lexer::number() noexcept {
	const char* start = _beg;
	while (is_numeric(peek())) get();
	return Token(Token::Type::constant, start, _beg);
}
Token Lexer::atomic(Token::Type type) noexcept {
	const char* start = _beg;
	get();
	return Token(type, start, _beg);
}

Token Lexer::identifier_or_keyword() noexcept {
	const char* start = _beg;
	while (is_identifier_char(peek())) get();
	std::string_view curr_word(start, std::distance(start, _beg));
	if (is_keyword(curr_word)) {
		return Token(Token::Type::keyword, start, _beg);
	}
	return Token(Token::Type::identifier, start, _beg);
}
bool is_space(char ch) {
	switch (ch) {
	case ' ':
	case '\t':
	case '\r':
	case '\n':
		return true;;
	}
	return false;
}
Token Lexer::next() noexcept {
	while (is_space(peek())) get();

	switch (peek()) {
	default:
		return atomic(Token::Type::unknown); // unknown token
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
		return identifier_or_keyword();
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return number();
	case '\0':
		return Token::Type::end;  // lets not go further
	case '\n':
		return atomic(Token::Type::eol);
	case '(':
		return atomic(Token::Type::leftparam);
	case ')':
		return atomic(Token::Type::rightparam);
	case '{':
		return atomic(Token::Type::leftcurly);
	case '}':
		return atomic(Token::Type::rightcurly);
	case ';':
		return atomic(Token::Type::semicolon);
	case ',':
		return atomic(Token::Type::coma);
	case '.':
		return atomic(Token::Type::dot);
	case '|':
		return atomic(Token::Type::pipe);
	case ':':
		return atomic(Token::Type::colan);
	case '"':
		return const_string();

	}
}
#include <iostream>
std::ostream& operator<<(std::ostream& os, const Token token) {
	
	static const char * type_names[] = {
		"identifier", 
		"constant",
		"leftcurly",
		"rightcurly",
		"keyword",
		"string",
		"leftparam",
		"rightparam",
		"dot",
		"semicolon",
		"coma",
		"pipe",
		"end",
		"eol",
		"colan",
		"cons_str",
		"unknown"
	};
	os << type_names[static_cast<int>(token.type())];
	return os;

}
int main()
{
	const char* str = "def func(a, b) {\n"
					  "return a + b;}\n "
					  "for  i : vec\n"
					  "print i";
							
	Lexer lexer(str);
	for (auto it = lexer.next(); !it.is(Token::Type::end); it = lexer.next()) {
		std::cout << it << "  "<< "|" << it.lexeme() << "|" << std::endl;
	}
	return 0;
}
