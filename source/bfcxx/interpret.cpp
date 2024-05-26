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
}  // namespace

auto interpret(const std::vector<token>& tokens) -> void
{
  std::vector<std::uint8_t> mem(30000, 0);
  std::size_t offset {0};
  std::size_t ptr {0};

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
        mem.at(ptr) += tok.repeats;
        if (mem.at(ptr) > 255) {
          mem.at(ptr) = 0;
        }
        break;
      }
      case literals::decrement: {
        mem.at(ptr) -= tok.repeats;
        if (mem.at(ptr) < 0) {
          mem.at(ptr) = 255;
        }
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
          std::size_t depth = 1;
          std::size_t saved_offset = offset;

          while (depth != 0) {
            if (++offset >= tokens.size()) {
              die(std::format("unmatched '[' at offset={}", saved_offset));
            }
            const token& t = tokens[offset];
            if (t.literal == literals::loop_start) {
              depth++;
            } else if (t.literal == literals::loop_end) {
              depth--;
            }
          }
        }
        break;
      }
      case literals::loop_end: {
        if (mem.at(ptr) != 0) {
          std::size_t depth = 1;
          std::size_t saved_offset = offset;

          while (depth != 0) {
            if (--offset == 0) {
              die(std::format("unmatched ']' at offset={}", saved_offset));
            }
            const token& t = tokens[offset];
            if (t.literal == literals::loop_start) {
              depth--;
            } else if (t.literal == literals::loop_end) {
              depth++;
            }
          }
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
