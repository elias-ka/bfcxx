# bfcxx: A brainfuck interpreter

Brainfuck is a  [Turing complete](https://en.wikipedia.org/wiki/Turing_completeness) esoteric programming language created in 1993 by Urban Müller. The language operates using a pointer that moves along an array of memory cells, each of which can store a single byte, although some implementations use 4-byte cells.

It consists of only 8 commands:
| Command | bfcxx interpretation |
| ------- | ---------------------|
| `<`     | Move the pointer to the left |
| `>`     | Move the pointer to the right |
| `+`     | Increment the memory cell at the pointer |
| `-`     | Decrement the memory cell at the pointer |
| `.`     | Write the character at the pointer to standard output |
| `,`     | Read a character from standard input and store it at the pointer |
| `[`     | Jump forward to the command after the matching `]` if the memory cell at the pointer is zero |
| `]`     | Jump back to the command after the matching `[` if the memory cell at the pointer is nonzero |

## Building
A C++20 compatible compiler and CMake 3.21 are required. To build the project, run:
```sh
cmake --build --preset <preset-name>
```
where `<preset-name>` is the name of the preset to use. To list all available presets, run:
```sh
cmake --list-presets
```
This will create an `out` directory with the compiled executable for the CLI at `/out/build/<preset-name>/source/bfcxx/cli/bfcxx`.

## Usage
The interpreter can be used as a read-eval-print loop (primitive, only a single line at a time) or to interpret a file:
```sh
bfcxx [path-to-file]
```
`/programs` contains a few example programs to test the interpreter.

## Optimizations
The interpreter uses couple optimizations to improve performance:
- **Jump table**: Jump table is used to quickly find the matching bracket for `[` and `]` commands.
- **Run-length encoding**: Consecutive `+`, `-`, `<`, and `>` commands are combined into a single command thus reducing the number of operations.
