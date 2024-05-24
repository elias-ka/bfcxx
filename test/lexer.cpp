#include <string>

#include "lexer.hpp"

#include <catch2/catch_test_macros.hpp>

#define REQUIRE_TOKENS(source, expected_tokens) \
  do { \
    bfcxx::lexer lexer {source}; \
    const auto tokens = lexer.tokens(); \
    REQUIRE(tokens == expected_tokens); \
  } while (false)

TEST_CASE("All valid characters are lexed", "[lexer]")
{
  const std::string source = "+-<>[],.";
  const std::vector<bfcxx::token> expected_tokens = {
      {'+', 1},
      {'-', 1},
      {'<', 1},
      {'>', 1},
      {'[', 1},
      {']', 1},
      {',', 1},
      {'.', 1},
  };
  REQUIRE_TOKENS(source, expected_tokens);
}

TEST_CASE("Invalid characters are ignored", "[lexer]")
{
  const std::string source = "abc123!@#  \t\n\r";
  const std::vector<bfcxx::token> expected_tokens = {};
  REQUIRE_TOKENS(source, expected_tokens);
}

TEST_CASE("Repetitive characters are lexed", "[lexer]")
{
  const std::string source = "+++++--->><<<<";
  const std::vector<bfcxx::token> expected_tokens = {
      {'+', 5},
      {'-', 3},
      {'>', 2},
      {'<', 4},
  };
  REQUIRE_TOKENS(source, expected_tokens);
}
