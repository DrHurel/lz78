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

struct Token extractData(std::array<unsigned char, 4> buf) {
  unsigned char terminal_char = buf.at(3);
  int32_t token = 0;
  for (int i = 0; i < 3; i++) {
    token |= static_cast<int32_t>(buf.at(i)) << (i * 8);
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

int32_t parseToDecode(const std::string &path) {

  auto fd = std::ifstream(path);

  if (!fd.is_open()) {
    std::cerr << "fail to open" << std::endl;

    return -1;
  }

  auto correpondance = std::vector<std::shared_ptr<struct Token>>(
      1, nullptr); // init epsilon node
  char buf;
  auto payload = std::vector<unsigned char>();
  auto out = std::ofstream("out_test.txt");
  if (!out.is_open()) {
    std::cerr << "fail to open" << std::endl;

    return -1;
  }
  long check = 0;
  int count = 0;
  int timeout = 0;
  check = fd.readsome(&buf, sizeof(char));
  while (check != EOF && timeout < 1000) {
    if (check != 1) {
      check = fd.readsome(&buf, sizeof(char));
      timeout++;
      continue;
    } else {
      timeout = 0;
      count++;
    }
    if (count == 4) {
      count = 0;
      auto data = extractData(
          {payload.at(0), payload.at(1), payload.at(2), (unsigned char)buf});

      if (data.token > static_cast<int32_t>(correpondance.size()) ||
          data.token < 0) {
        std::cout << "break" << std::endl;
        std::cout << correpondance.size() << std::endl;

        break;
      }
      auto token = std::make_shared<struct Token>(data);
      out << recreateWord(correpondance, std::shared_ptr<struct Token>(token));

      correpondance.push_back(std::make_unique<struct Token>(data));
      payload.clear();
    } else {
      payload.push_back((unsigned char)buf);
    }

    check = fd.readsome(&buf, sizeof(char));
  }
  out << std::endl;
  out.close();
  std::cout << "close" << std::endl;
  fd.close();
  return 1;
}
