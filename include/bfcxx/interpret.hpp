#pragma once

#include <span>

#include <bfcxx/lexer.hpp>

namespace bfcxx
{
auto interpret(const std::vector<token>& tokens) -> void;
}  // namespace bfcxx
