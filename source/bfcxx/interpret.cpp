#include <cstddef>
#include <format>
#include <iostream>

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

[[nodiscard]] auto compute_jumptable(const std::vector<op>& ops)
    -> std::vector<std::size_t>
{
  std::size_t offset {0};
  std::vector<std::size_t> jump_table(ops.size(), 0);

  for (; offset < ops.size(); offset++) {
    const auto op = ops[offset];
    if (op.kind == op_kind::loop_start) {
      std::size_t depth = 1;
      std::size_t seek = offset;

      while (depth > 0 && ++seek < ops.size()) {
        if (ops[seek].kind == op_kind::loop_start) {
          depth++;
        } else if (ops[seek].kind == op_kind::loop_end) {
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

auto interpret(const std::vector<op>& ops) -> void
{
  const std::size_t byte_size = 256;
  const std::size_t mem_size = 30000;
  std::vector<std::uint8_t> mem(mem_size, 0);
  std::size_t offset {0};
  std::size_t ptr {0};
  const auto jump_table = compute_jumptable(ops);

  for (; offset < ops.size(); offset++) {
    const auto op = ops[offset];

    switch (op.kind) {
      case op_kind::move_left: {
#ifdef BFCXX_WITH_WRAPAROUND
        ptr = (ptr + mem.size() - op.arg) % mem.size();
#else
        ptr -= op.arg;
#endif
        break;
      }
      case op_kind::move_right: {
#ifdef BFCXX_WITH_WRAPAROUND
        ptr = (ptr + op.arg) % mem.size();
#else
        ptr += op.arg;
#endif
        break;
      }
      case op_kind::increment: {
        mem.at(ptr) =
            static_cast<std::uint8_t>((mem.at(ptr) + op.arg) % byte_size);
        break;
      }
      case op_kind::decrement: {
        mem.at(ptr) =
            static_cast<std::uint8_t>((mem.at(ptr) - op.arg) % byte_size);
        break;
      }
      case op_kind::write_stdout: {
        std::cout.put(static_cast<char>(mem.at(ptr)));
        break;
      }
      case op_kind::read_stdin: {
        mem.at(ptr) = static_cast<std::uint8_t>(std::cin.get());
        break;
      }
      case op_kind::loop_start: {
        if (mem.at(ptr) == 0) {
          offset = jump_table[offset];
        }
        break;
      }
      case op_kind::loop_end: {
        if (mem.at(ptr) != 0) {
          offset = jump_table[offset];
        }
        break;
      }
    }
  }
}
}  // namespace bfcxx
