#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "lexer.h"

/* =========================
   Context
   ========================= */
class Context {
public:
    std::unordered_map<std::string, int> vars;

    int get(const std::string& name) const {
        auto it = vars.find(name);
        if (it == vars.end())
            throw std::runtime_error("Undefined variable: " + name);
        return it->second;
    }

    void set(const std::string& name, int value) {
        vars[name] = value;
    }
};

/* =========================
   Expression hierarchy
   ========================= */
class Expression {
public:
    virtual int interpret(Context& ctx) const = 0;
    virtual ~Expression() = default;
};

using Expr = std::shared_ptr<Expression>;

class Number : public Expression {
    int value;
public:
    explicit Number(int v) : value(v) {}
    int interpret(Context&) const override { return value; }
};

class Variable : public Expression {
    std::string name;
public:
    explicit Variable(std::string n) : name(std::move(n)) {}
    int interpret(Context& ctx) const override {
        return ctx.get(name);
    }
    const std::string& id() const { return name; }
};

class Binary : public Expression {
protected:
    Expr left, right;
public:
    Binary(Expr l, Expr r) : left(std::move(l)), right(std::move(r)) {}
};

class Add : public Binary {
public:
    using Binary::Binary;
    int interpret(Context& ctx) const override {
        return left->interpret(ctx) + right->interpret(ctx);
    }
};

class Sub : public Binary {
public:
    using Binary::Binary;
    int interpret(Context& ctx) const override {
        return left->interpret(ctx) - right->interpret(ctx);
    }
};

class Mul : public Binary {
public:
    using Binary::Binary;
    int interpret(Context& ctx) const override {
        return left->interpret(ctx) * right->interpret(ctx);
    }
};

class Div : public Binary {
public:
    using Binary::Binary;
    int interpret(Context& ctx) const override {
        return left->interpret(ctx) / right->interpret(ctx);
    }
};

class Assign : public Expression {
    std::string name;
    Expr value;
public:
    Assign(std::string n, Expr v)
        : name(std::move(n)), value(std::move(v)) {
    }

    int interpret(Context& ctx) const override {
        int v = value->interpret(ctx);
        ctx.set(name, v);
        return v;
    }
};

/* =========================
   Recursive Descent Parser
   Grammar:
     stmt   → identifier '=' expr | expr
     expr   → term ((+ | -) term)*
     term   → factor ((* | /) factor)*
     factor → constant | identifier | '(' expr ')'
   ========================= */
//CFG

class Parser {
    Lexer lexer;
    Token current;

    void advance() { current = lexer.next(); }

    Expr factor() {
        if (current.is(Token::Type::constant)) {
            int v = std::stoi(std::string(current.lexeme()));
            advance();
            return std::make_shared<Number>(v);
        }

        if (current.is(Token::Type::identifier)) {
            std::string name(current.lexeme());
            advance();
            return std::make_shared<Variable>(name);
        }

        if (current.is(Token::Type::leftparam)) {
            advance();
            Expr e = expr();
            if (!current.is(Token::Type::rightparam))
                throw std::runtime_error("Expected ')'");
            advance();
            return e;
        }

        throw std::runtime_error("Invalid factor");
    }

    Expr term() {
        Expr left = factor();
        while (current.is(Token::Type::mul) ||
            current.is(Token::Type::div)) {
            Token op = current;
            advance();
            Expr right = factor();

            if (op.is(Token::Type::mul))
                left = std::make_shared<Mul>(left, right);
            else
                left = std::make_shared<Div>(left, right);
        }
        return left;
    }

    Expr expr() {
        Expr left = term();
        while (current.is(Token::Type::plus) ||
            current.is(Token::Type::minus)) {
            Token op = current;
            advance();
            Expr right = term();

            if (op.is(Token::Type::plus))
                left = std::make_shared<Add>(left, right);
            else
                left = std::make_shared<Sub>(left, right);
        }
        return left;
    }

public:
    explicit Parser(const char* src) : lexer(src) {
        advance();
    }

    Expr parse() {
        // assignment: identifier '=' expr
        if (current.is(Token::Type::identifier)) {
            Token id = current;
            advance();
            if (current.is(Token::Type::assign)) {
                advance();
                Expr rhs = expr();
                return std::make_shared<Assign>(
                    std::string(id.lexeme()), rhs
                );
            }
            // rollback-like behavior
            lexer = Lexer(id.lexeme().data());
            advance();
        }
        return expr();
    }
};
