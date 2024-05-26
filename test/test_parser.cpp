#include <string>

#include <bfcxx/parser.hpp>
#include <catch2/catch_test_macros.hpp>

#define REQUIRE_OPS(source, expected_ops) \
  do { \
    bfcxx::parser parser {source}; \
    const auto ops = parser.ops(); \
    REQUIRE(ops == expected_ops); \
  } while (false)

TEST_CASE("All valid characters are parsed", "[parser]")
{
  const std::string source = "+-<>[],.";
  const std::vector<bfcxx::op> expected_ops = {
      {bfcxx::op_kind::increment, 1},
      {bfcxx::op_kind::decrement, 1},
      {bfcxx::op_kind::move_left, 1},
      {bfcxx::op_kind::move_right, 1},
      {bfcxx::op_kind::loop_start, 1},
      {bfcxx::op_kind::loop_end, 1},
      {bfcxx::op_kind::read_stdin, 1},
      {bfcxx::op_kind::write_stdout, 1},
  };
  REQUIRE_OPS(source, expected_ops);
}

TEST_CASE("Invalid characters are ignored", "[parser]")
{
  const std::string source = "abc123!@#  \t\n\r";
  const std::vector<bfcxx::op> expected_ops = {};
  REQUIRE_OPS(source, expected_ops);
}

TEST_CASE("Repetitive characters are parsed", "[parser]")
{
  const std::string source = "+++++--->><<<<";
  const std::vector<bfcxx::op> expected_ops = {
      {bfcxx::op_kind::increment, 5},
      {bfcxx::op_kind::decrement, 3},
      {bfcxx::op_kind::move_right, 2},
      {bfcxx::op_kind::move_left, 4},
  };
  REQUIRE_OPS(source, expected_ops);
}
