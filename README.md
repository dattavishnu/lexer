# lexer, parser and interpreter

# Mini Language – Lexer, Parser & Interpreter (C++)

An educational C++ project that demonstrates how a simple programming language is built using three classic stages:

1. **Lexer (Tokenizer)**
2. **Parser (AST construction)**
3. **Interpreter (AST evaluation)**

This project is intentionally small and readable, meant for learning, experimentation, and extension.

---

## Overview

The processing pipeline looks like this:

Source Code  
↓  
Lexer → Token stream  
↓  
Parser → Abstract Syntax Tree (AST)  
↓  
Interpreter → Execution / Evaluation  


Each stage is implemented in modern C++ with clarity and correctness as the primary goals.

---

## Lexer

The lexer reads raw source text and converts it into a sequence of tokens.

### Supported token types

- **Identifiers**
x, foo, my_var

- **Keywords**
def, return, if, else, while, for, in,
break, continue, true, false, null


- **Numeric constants**
123, 45.67

- **String constants**
"hello world"

- **Operators**
/ =

- **Punctuation**
( ) { } . ; , | :

- **Special tokens**
- End-of-line
- End-of-input
- Unknown tokens for unsupported characters

### Lexer design notes

- Uses `std::string_view` to avoid unnecessary allocations
- ASCII-based character classification using simple helpers
- One-token-at-a-time interface via `Lexer::next()`
- Single-character operators and punctuation (educational scope)

---

## Parser

The parser consumes tokens from the lexer and constructs an **Abstract Syntax Tree (AST)** using a recursive-descent strategy.

### Grammar (simplified)

```text
assignment → identifier "=" expression
expression → term ("+" | "-") term
term       → factor ("*" | "/") factor
factor     → number
           | identifier
           | "(" expression ")"
```



### Supported features

- Variable assignments
- Arithmetic expressions
- Operator precedence
- Parenthesized expressions
- Sequential execution of statements

---

## Interpreter

The interpreter walks the AST and evaluates it.

### Runtime model

- A `Context` stores variable names and values
- Expressions are evaluated recursively
- Assignments update the context
- Integer arithmetic is supported

### Example

#### Input program


---

## Example Usage

```cpp
Parser p(
    "x = 3\n"
    "y = 4\n"
    "x = (x + y) * 3\n"
);

p.run();
```



