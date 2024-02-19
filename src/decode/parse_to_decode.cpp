#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

struct Token {
  int32_t token;
  unsigned char terminal_char;
};

struct Token extractData(std::array<char, 4> buf) {
  unsigned char terminal_char = buf.at(3);
  int32_t token = 0;
  for (int i = 0; i < 3; i++) {
    token |= static_cast<int32_t>((unsigned char)buf.at(i)) << (i * 8);
  }

  return {token, terminal_char};
}

std::string
recreateWord(std::vector<std::shared_ptr<struct Token>> correpondance,
             std::shared_ptr<struct Token> token) {

  if (token->token == 0) {
    return std::string(1, token->terminal_char);
  }
  auto word = recreateWord(correpondance, correpondance.at(token->token));
  return word.append(1, token->terminal_char);
}

int32_t parseToDecode(const std::string &path,
                      const std::array<int32_t, 2> tube) {

  auto fd = std::ifstream(path);

  if (!fd.is_open()) {
    std::cerr << "fail to open" << std::endl;
    close(tube[0]);
    return -1;
  }

  auto correpondance = std::vector<std::shared_ptr<struct Token>>(
      1, nullptr); // init epsilon node
  std::array<char, 4> buf;

  auto out = std::ofstream("out_test.txt");
  if (!out.is_open()) {
    std::cerr << "fail to open" << std::endl;
    close(tube[0]);
    return -1;
  }
  long check = 0;
  check = fd.readsome(buf.data(), sizeof(char) * 4);
  while (check != EOF) {

    auto data = extractData(buf);
    std::cout << "data.token : " << data.token << std::endl;
    std::cout << "data.char : " << data.terminal_char << std::endl;

    if (data.token > correpondance.size() || data.token < 0) {
      std::cout << "break" << std::endl;
      std::cout << correpondance.size() << std::endl;

      break;
    }
    auto payload = std::make_shared<struct Token>(data);
    out << recreateWord(correpondance, std::shared_ptr<struct Token>(payload));

    correpondance.push_back(std::make_unique<struct Token>(data));
    check = fd.readsome(buf.data(), sizeof(char) * 4);
  }
  out << std::endl;
  out.close();
  close(tube[0]);
  return 1;
}
