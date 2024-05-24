#include <algorithm>
#include <array>
#include <iostream>
#include <optional>

#include <bfcxx/lexer.hpp>

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

lexer::lexer(std::string source)
    : m_source {std::move(source)}
{
  m_source.erase(std::remove_if(m_source.begin(),
                                m_source.end(),
                                [](char c) { return !is_bf_char(c); }),
                 m_source.end());
}

auto lexer::next() -> std::optional<token>
{
  while (m_pos < m_source.size()) {
    const char c = m_source[m_pos];
    if (!is_repeatable_char(c)) {
      m_pos++;
      return token {.literal = c, .repeats = 1};
    }

    auto begin = m_pos;
    while (m_pos < m_source.size() && m_source[m_pos] == c) {
      m_pos++;
    }
    return token {.literal = c,
                  .repeats = static_cast<std::uint32_t>(m_pos - begin)};
  }
  return std::nullopt;
}

auto lexer::tokens() -> std::vector<token>
{
  std::vector<token> tokens;
  for (auto t = next(); t; t = next()) {
    tokens.push_back(*t);
  }
  return tokens;
}

}  // namespace bfcxx
