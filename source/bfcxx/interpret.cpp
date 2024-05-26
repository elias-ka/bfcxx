#include <algorithm>
#include <format>
#include <iostream>
#include <span>

#include <bfcxx/interpret.hpp>

namespace bfcxx
{
namespace
{
auto die(const std::string& msg) -> void
{
  std::cerr << "err: " << msg << '\n';
  // NOLINTNEXTLINE
  std::exit(1);
}

auto compute_jumptable(const std::vector<token>& tokens)
    -> std::vector<std::size_t>
{
  std::size_t offset {0};
  std::vector<std::size_t> jump_table(tokens.size(), 0);

  for (; offset < tokens.size(); offset++) {
    const token& tok = tokens[offset];
    if (tok.literal == literals::loop_start) {
      std::size_t depth = 1;
      std::size_t seek = offset;

      while (depth > 0 && ++seek < tokens.size()) {
        if (tokens[seek].literal == literals::loop_start) {
          depth++;
        } else if (tokens[seek].literal == literals::loop_end) {
          depth--;
        }
      }

      if (depth == 0) {
        jump_table[offset] = seek;
        jump_table[seek] = offset;
      } else {
        die(std::format("unmatched '[' at offset={}", offset));
      }
    }
  }
  return jump_table;
}

}  // namespace

auto interpret(const std::vector<token>& tokens) -> void
{
  std::vector<std::uint8_t> mem(30000, 0);
  std::size_t offset {0};
  std::size_t ptr {0};
  const auto jump_table = compute_jumptable(tokens);

  for (; offset < tokens.size(); offset++) {
    const token& tok = tokens[offset];

    switch (tok.literal) {
      case literals::move_left: {
        if (ptr - tok.repeats < 0) {
          die(std::format("out of bounds access at offset={}", offset));
          return;
        }
        ptr -= tok.repeats;
        break;
      }
      case literals::move_right: {
        if (ptr + tok.repeats >= mem.size()) {
          die(std::format("out of bounds access at offset={}", offset));
          return;
        }
        ptr += tok.repeats;
        break;
      }
      case literals::increment: {
        mem.at(ptr) =
            static_cast<std::uint8_t>((mem.at(ptr) + tok.repeats) % 256);
        break;
      }
      case literals::decrement: {
        mem.at(ptr) =
            static_cast<std::uint8_t>((mem.at(ptr) - tok.repeats + 256) % 256);
        break;
      }
      case literals::write_stdout: {
        std::cout.put(static_cast<char>(mem.at(ptr)));
        break;
      }
      case literals::read_stdin: {
        mem.at(ptr) = static_cast<std::uint8_t>(std::cin.get());
        break;
      }
      case literals::loop_start: {
        if (mem.at(ptr) == 0) {
          offset = jump_table[offset];
        }
        break;
      }
      case literals::loop_end: {
        if (mem.at(ptr) != 0) {
          offset = jump_table[offset];
        }
        break;
      }
      default: {
        die(std::format(
            "bad character '{}' at offset={}", tok.literal, offset));
        break;
      }
    }
  }
}
}  // namespace bfcxx
