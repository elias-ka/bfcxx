#include <fstream>
#include <iostream>
#include <span>
#include <string>

#include <bfcxx/interpret.hpp>
#include <bfcxx/parser.hpp>

auto repl() -> void
{
  std::string line;
  while (true) {
    std::cout << ">>> ";
    if (!std::getline(std::cin, line)) {
      break;
    }
    if (line.empty()) {
      continue;
    }
    if (line == "exit") {
      break;
    }
    bfcxx::parser parser {line};
    bfcxx::interpret(parser.ops());
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

  bfcxx::parser parser {source};
  bfcxx::interpret(parser.ops());
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
