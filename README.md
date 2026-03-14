# JLang — A Custom Interpreted Language
![C++](https://img.shields.io/badge/language-C%2B%2B-blue)
![Status](https://img.shields.io/badge/status-experimental-orange)
![License](https://img.shields.io/badge/license-MIT-green)

JLang is a hand-built interpreter for a custom programming language, written in C++. 

It implements the full compiler pipeline : 
Lexer → Parser → AST → Semantic Analyzer → Interpreter.

---

## Language Overview

JLang is a statically typed, imperative language with a clean, minimal syntax.

### Types
| JLang Keyword | Meaning |
|-------------|---------|
| `num`       | Integer |
| `fact`      | Boolean |

### Keywords
| Keyword | Purpose            |
|---------|--------------------|
| `num`   | Declare an integer |
| `fact`  | Declare a boolean  |
| `print` | Print to stdout    |
| `if`    | Conditional        |
| `else`  | Else branch        |
| `loop`  | While-style loop   |
| `brk`   | Break out of loop  |
| `true`  | Boolean literal    |
| `false` | Boolean literal    |

### Operators
| Operator | Meaning        |
|----------|----------------|
| `+` `-` `*` `/` | Arithmetic |
| `==` `!=`       | Equality   |
| `<` `>`         | Comparison |
| `=`             | Assignment |

---

## Example Program

```
{
    num x = 5;
    fact flag = true;

    loop(x > 0){
        if(x == 3){
            print(999);
            brk;
        }

        print(x);
        x = x - 1;
    }

    if(flag){
        print(42);
    }
}
```

**Output:** 
```output
5
4
999
42
```

---

## Architecture

JLang is built as a classic four-stage interpreter pipeline:

```
Source File
    │
    ▼
[ Lexer ]         → Tokenizes raw source into a flat token stream
    │
    ▼
[ Parser ]        → Builds an Abstract Syntax Tree (AST) via recursive descent
    │
    ▼
[ Semantic ]      → Type checks and validates scoping/control flow rules
    │
    ▼
[ Interpreter ]   → Tree-walk execution of the AST
```

### Lexer (`lexer/`)
Scans source code character by character and emits typed tokens. Recognizes keywords, identifiers, numbers, operators, and punctuation. Throws a descriptive error on unexpected characters.

### Parser (`parser/`)
Recursive descent parser that produces a typed AST. Handles:
- Variable declarations with optional initializers
- Block scoping with `{}`
- `if / else` statements
- `loop` (while-style) with `brk` support
- `print` statements
- Assignments and full expression grammar (with correct precedence)

**Expression precedence (lowest → highest):**
```
assignment → equality → comparison → term → factor → unary → primary
```

### Semantic Analyzer (`semantic/`)
Performs static analysis before execution:
- **Symbol table:** scoped `unordered_map` stack tracks variable names and types
- **Type checking:** enforces that operands match, conditions are `fact`, arithmetic uses `num`
- **Control flow:** ensures `brk` only appears inside a `loop`
- **Scope rules:** catches re-declaration of variables in the same scope, and use of undeclared variables

### Interpreter (`interpreter/`)
Tree-walk interpreter that evaluates the AST directly. Uses a scope stack of `unordered_map<string, int>` (booleans are stored as 0/1). Handles `brk` via a `BreakException` thrown through the call stack.

---

## Project Structure

```
.
├── main.cpp
├── lexer/
│   ├── lexer.h
│   └── lexer.cpp
├── parser/
│   ├── parser.h
│   └── parser.cpp
├── semantic/
│   ├── semantic.h
│   └── semantic.cpp
├── interpreter/
│   ├── interpreter.h
│   └── interpreter.cpp
└── makefile
```

---

## Build & Run

### Requirements
- C++17 or later
- A C++ compiler (g++ / clang++)

### Compile
```bash
g++ -std=c++17 -o jpp main.cpp lexer/lexer.cpp parser/parser.cpp semantic/semantic.cpp interpreter/interpreter.cpp
```
then **Run** using
```bash
./jpp
```
#### OR 

Compile using `makefile`
```bash
mingw32-make
```
then **Run** using 
```bash
main
```

Either way you will be prompted to enter the path to your source file:
```
Enter Source Code Path :
test/source_code.txt
```

---

## Error Handling

JLang reports errors from each stage with context:

| Stage    | Example Error |
|----------|---------------|
| Lexer    | `LEXER ERROR : unexpected character at line : 3` |
| Parser   | `Syntax Error : at line 5 :: Expected ;` |
| Semantic | `Semantic Error : Type Mismatch` |
| Semantic | `Semantic Error : Using Break Outside the loop` |
| Runtime  | `Division by zero` |

---

## Why JLang ?

JLang was built as a learning project to explore how programming languages work internally.  

The goal was to understand how real-world compilers structure their frontends and runtimes, while challenging myself to build a low-level systems project from scratch.

---


## Current Limitations (V1)

- Integer and boolean types only (no strings or floats)
- No functions or user-defined procedures
- No logical operators (`&&`, `||`, `!`)

---

## Future Improvements

Planned upgrades:
- IR (Intermediate Representation)
- bytecode VM
- functions
- strings & float
- logical operators (`&&`, `||`, `!`)
- better error messages

---

## License

MIT — do whatever you want with it.
