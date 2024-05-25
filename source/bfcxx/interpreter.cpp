#include <iostream>

#include <bfcxx/interpreter.hpp>

namespace bfcxx
{

auto interpreter::eval() -> void
{
  const token& tok = m_tokens[m_offset];

  switch (tok.literal) {
    case literals::move_left: {
      m_ptr = (m_ptr - tok.repeats) % m_memory.size();
      break;
    }
    case literals::move_right: {
      m_ptr = (m_ptr + tok.repeats) % m_memory.size();
      break;
    }
    case literals::increment: {
      m_memory.at(m_ptr) = static_cast<std::uint8_t>(
          (m_memory.at(m_ptr) + tok.repeats) % m_memory.size());
      break;
    }
    case literals::decrement: {
      m_memory.at(m_ptr) = static_cast<std::uint8_t>(
          (m_memory.at(m_ptr) - tok.repeats) % m_memory.size());
      break;
    }
    case literals::write_stdout: {
      std::cout << m_memory.at(m_ptr);
      break;
    }
    case literals::read_stdin: {
      int c = std::cin.get();
      m_memory.at(m_ptr) = static_cast<std::uint8_t>(c);
      break;
    }
    case literals::loop_start: {
      if (m_memory.at(m_ptr) == 0) {
        std::size_t depth = 1;
        while (depth != 0 && m_offset < m_tokens.size()) {
          const token& next = m_tokens[++m_offset];
          if (next.literal == literals::loop_start) {
            depth++;
          } else if (next.literal == literals::loop_end) {
            depth--;
          }
        }
      }
      break;
    }
    case literals::loop_end: {
      if (m_memory.at(m_ptr) != 0) {
        std::size_t depth = 1;
        while (depth != 0 && m_offset > 0) {
          const token& prev = m_tokens[--m_offset];
          if (prev.literal == literals::loop_start) {
            depth--;
          } else if (prev.literal == literals::loop_end) {
            depth++;
          }
        }
      }
      break;
    }
    default:
      break;
  }
  m_offset++;
}

auto interpreter::run() -> void
{
  while (m_offset < m_tokens.size()) {
    eval();
  }
}
}  // namespace bfcxx
