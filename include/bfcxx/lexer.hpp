#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <string_view>
#include <vector>

namespace bfcxx
{
namespace literals
{
static constexpr char move_left = '<';
static constexpr char move_right = '>';
static constexpr char increment = '+';
static constexpr char decrement = '-';
static constexpr char write_stdout = '.';
static constexpr char read_stdin = ',';
static constexpr char loop_start = '[';
static constexpr char loop_end = ']';
}  // namespace literals

struct token
{
  char literal;
  std::uint32_t repeats;

  friend auto operator==(const token& lhs, const token& rhs) -> bool
  {
    return lhs.literal == rhs.literal && lhs.repeats == rhs.repeats;
  }

  friend auto operator<<(std::ostream& os, const token& t) -> std::ostream&
  {
    os << "Token { literal: '" << t.literal << "', repeats: " << t.repeats
       << " }";
    return os;
  }
};

class lexer
{
public:
  explicit lexer(std::string source);

  auto tokens() -> std::vector<token>;

private:
  auto next() -> std::optional<token>;

  std::string m_source;
  std::size_t m_pos {0};
};

}  // namespace bfcxx
