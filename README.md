# lexer

A smalllexer (tokenizer) written in modern C++ that demonstrates basic lexical analysis: splitting a source string into tokens such as identifiers, keywords, numbers, strings and punctuation.

This repository currently contains a single implementation in `lexer.cpp`. See the implementation here: [lexer.cpp](https://github.com/dattavishnu/lexer/blob/e70c2ba4ca5db5dc2a47bd2d6428f75186b9a7e3/lexer.cpp)

## Features

- Tokenizes:
  - identifiers and keywords (e.g. `def`, `return`, `if`, ...)
  - numeric constants (digits and `.`)
  - quoted string constants
  - punctuation: `(`, `)`, `{`, `}`, `.`, `;`, `,`, `|`, `:`
  - end-of-line token
  - end-of-input token
  - unknown tokens for any other single character
- Uses `std::string_view` to avoid extra allocations when producing token lexemes.
- Small, self-contained single-file example good for learning and experimentation.

## Token types

The lexer exposes these token types (as an enum `Token::Type`):

- identifier
- constant
- leftcurly
- rightcurly
- keyword
- string
- leftparam
- rightparam
- dot
- semicolon
- coma
- pipe
- end
- eol
- colan
- cons_str
- unknown

## Build

Requires a C++17-capable compiler (for `std::string_view` and other language features).

Example with g++ or clang++:

```bash
g++ -std=c++17 lexer.cpp -o lexer
```

On Windows with MSVC, enable C++17 (Visual Studio 2017 or later):

- Using Developer Command Prompt:
  cl /std:c++17 lexer.cpp

(Adjust flags as necessary for your environment.)

## Run

The `main()` in `lexer.cpp` contains a short demo string. After building, run the produced executable:

```bash
./lexer
```

It will print each token's type and lexeme, for example:

identifier  |def|
identifier  |func|
leftparam   |(|
identifier  |a|
coma        |,|
identifier  |b|
rightparam  |)|
leftcurly   |{|
keyword     |return|
identifier  |a|
unknown     |+|
identifier  |b|
semicolon   |;|
rightcurly  |}|
... etc.

(Your actual output formatting may vary slightly based on the demo code in `main()`.)

## Example usage / How it works (high level)

- The `Lexer` takes a C-string pointer to the source text.
- `next()` returns the next `Token` until the `end` token is reached.
- Helper methods implement recognition for:
  - identifiers and keywords,
  - numeric constants,
  - quoted strings,
  - single-character punctuators.
- Keywords are recognized by comparing a `std::string_view` against a static list.

## Limitations & notes

- This is an educational/demo lexer — it does not handle:
  - multi-character operators (e.g. `==`, `!=`, `<=`, `>=`, `+=`, etc.)
  - escape sequences inside string literals
  - comments
  - floating-point parsing beyond simple `.` acceptance in the numeric character set (no validation)
  - Unicode identifiers
- The `is_identifier_char` and `is_numeric` helpers are implemented with switch/case for ASCII characters only.
- The `unknown` token is returned for any unrecognized single character.
- `cons_str` token type is used to represent string constants from the `const_string()` method; there is also a `string` token type in the enum (left for extension or refactor).

## Contributing

This project is a simple demo. If you want to extend it:

- Add support for comments (line `//` and block `/* ... */`).
- Add support for multi-character operators.
- Improve numeric literal parsing and validation.
- Add escape sequence handling in strings.
- Add unit tests and a small test harness.

Feel free to open issues or PRs in the repository.

## License

MIT License

Copyright (c) 2026 dattavishnu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
