#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

int32_t bytesToInt(std::array<char, 4> buf) {
  return static_cast<int32_t>(buf[3]) | static_cast<int32_t>(buf[0]) << 8 |
         static_cast<int32_t>(buf[1]) << 16 |
         static_cast<int32_t>(buf[2]) << 24;
}

std::string recreateWord(std::vector<int32_t> correpondance, int32_t code) {

  if ((code >> 8) == 0) {
    return std::string(1, char(code));
  }
  auto word = recreateWord(correpondance, correpondance.at(code >> 8));
  return word.append(1, char(code));
}

int32_t parseToDecode(const std::string &path,
                      const std::array<int32_t, 2> tube) {

  auto fd = std::ifstream(path);

  if (!fd.is_open()) {
    std::cerr << "fail to open" << std::endl;
    close(tube[0]);
    return -1;
  }

  auto correpondance = std::vector<int32_t>(1, '\0'); // init epsilon node
  std::array<char, 4> buf;

  int diff = 0;
  auto out = std::ofstream("out_test.txt");
  if (!out.is_open()) {
    std::cerr << "fail to open" << std::endl;
    close(tube[0]);
    return -1;
  }
  long check = 0;
  check = fd.readsome(buf.data(), sizeof(char) * 4);
  while (check != EOF) {

    for (int i = 0; i < 4; i++) {
      std::cout << "buf[" << i << "] : " << buf[i] << std::endl;
    }
    int code = bytesToInt(buf);
    if (code < 0) {
      break;
    }
    std::cout << "code : " << code << std::endl;
    out << recreateWord(correpondance, code);
    correpondance.push_back(code);
    check = fd.readsome(buf.data(), sizeof(char) * 4);
  }
  out << std::endl;
  out.close();
  close(tube[0]);
  return 1;
}
