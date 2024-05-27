#pragma once

#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <optional>
#include <vector>

namespace bfcxx
{
enum class op_kind : std::uint8_t
{
  move_left = '<',
  move_right = '>',
  increment = '+',
  decrement = '-',
  write_stdout = '.',
  read_stdin = ',',
  loop_start = '[',
  loop_end = ']',
};

struct op
{
  op_kind kind;
  std::size_t arg;

  auto operator==(const op& other) const -> bool = default;

  friend auto operator<<(std::ostream& os, const op& op) -> std::ostream&
  {
    os << std::format(
        "op {{ kind = {}, arg = {} }}", static_cast<char>(op.kind), op.arg);
    return os;
  }
};

class parser
{
public:
  explicit parser(std::string source);

  auto ops() -> std::vector<op>;

private:
  auto next() -> std::optional<op>;

  std::string m_source;
  std::size_t m_pos {0};
};

}  // namespace bfcxx
