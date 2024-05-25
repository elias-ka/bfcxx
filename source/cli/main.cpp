#include <fstream>
#include <iostream>
#include <span>
#include <string>

#include <bfcxx/interpreter.hpp>
#include <bfcxx/lexer.hpp>

auto repl() -> void
{
  std::string line;
  while (true) {
    std::cout << ">>> ";
    std::getline(std::cin, line);
    if (line.empty()) {
      continue;
    }
    if (line == "exit") {
      break;
    }
    bfcxx::lexer lexer {line};
    bfcxx::interpreter interpreter {lexer.tokens()};
    interpreter.run();
    std::cout << '\n';
  }
}

auto run_file(const std::string& path) -> void
{
  std::ifstream file {path};
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file '" << path << "'\n";
    return;
  }
  std::string source {std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>()};

  bfcxx::lexer lexer {source};
  bfcxx::interpreter interpreter {lexer.tokens()};
  interpreter.run();
}

auto main(int argc, char** argv) -> int
{
  auto args = std::span(argv, static_cast<std::size_t>(argc));

  if (args.size() == 1) {
    repl();
  } else if (args.size() == 2) {
    run_file(args[1]);
  } else {
    std::cerr << "Usage: " << args[0] << " [path]\n";
    return 1;
  }
}
