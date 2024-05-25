#pragma once

#include <array>
#include <cstdint>

#include <bfcxx/lexer.hpp>

namespace bfcxx
{
class interpreter
{
public:
  explicit interpreter(std::vector<token> tokens)
      : m_tokens {std::move(tokens)}
  {
  }

  auto run() -> void;

private:
  auto eval() -> void;

  static constexpr std::size_t memory_size = 30000;

  std::vector<token> m_tokens;
  std::array<std::uint8_t, memory_size> m_memory {};
  std::size_t m_offset {0};
  std::size_t m_ptr {0};
};
}  // namespace bfcxx
