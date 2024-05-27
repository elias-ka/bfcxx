#include <algorithm>
#include <array>
#include <optional>

#include <bfcxx/parser.hpp>

namespace bfcxx
{

namespace
{
constexpr std::array<bool, 256> is_bf_char_byte_table = []
{
  std::array<bool, 256> table = {};
  table['+'] = true;
  table['-'] = true;
  table['>'] = true;
  table['<'] = true;
  table['['] = true;
  table[']'] = true;
  table[','] = true;
  table['.'] = true;
  return table;
}();

[[nodiscard]] auto is_bf_char(char c) -> bool
{
  // NOLINTNEXTLINE
  return is_bf_char_byte_table[static_cast<unsigned char>(c)];
}

[[nodiscard]] auto is_repeatable_char(char c) -> bool
{
  return c == '+' || c == '-' || c == '>' || c == '<';
}

}  // namespace

parser::parser(std::string source)
    : m_source {std::move(source)}
{
  m_source.erase(std::remove_if(m_source.begin(),
                                m_source.end(),
                                [](char c) { return !is_bf_char(c); }),
                 m_source.end());
}

auto parser::next() -> std::optional<op>
{
  while (m_pos < m_source.size()) {
    const char c = m_source[m_pos];
    if (!is_repeatable_char(c)) {
      m_pos++;
      return op {.kind = static_cast<op_kind>(c), .arg = 1};
    }
    auto begin = m_pos;
    while (m_pos < m_source.size() && m_source[m_pos] == c) {
      m_pos++;
    }
    return op {.kind = static_cast<op_kind>(c),
               .arg = static_cast<std::uint32_t>(m_pos - begin)};
  }
  return std::nullopt;
}

auto parser::ops() -> std::vector<op>
{
  std::vector<op> ops;
  while (auto t = next()) {
    ops.push_back(*t);
  }
  return ops;
}

}  // namespace bfcxx
