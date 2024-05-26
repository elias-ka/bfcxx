#pragma once

#include <span>

#include <bfcxx/parser.hpp>

namespace bfcxx
{
auto interpret(const std::vector<op>& ops) -> void;
}  // namespace bfcxx
