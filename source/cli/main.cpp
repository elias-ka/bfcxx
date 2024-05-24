#include <fstream>
#include <iostream>
#include <string>

#include <bfcxx/lexer.hpp>

auto repl() -> void
{
  std::string line;
  while (true) {
    std::cout << ">>> ";
    std::getline(std::cin, line);
    if (line == "exit") {
      break;
    }
    bfcxx::lexer lexer {line};
    for (const auto& token : lexer.tokens()) {
      std::cout << token << '\n';
    }
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
  for (const auto& token : lexer.tokens()) {
    std::cout << token << '\n';
  }
}

auto main(int argc, char** argv) -> int
{
  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    run_file(argv[1]);
  } else {
    std::cerr << "Usage: " << argv[0] << " [path]\n";
    return 1;
  }
}
