#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

std::string recreateWord(std::vector<int32_t> correpondance, int32_t code) {

  if (code == 0) {
    return "";
  }

  return recreateWord(correpondance, correpondance.at(code >> 16)) + (char)code;
}

int32_t parseToDecode(const std::string &path,
                      const std::array<int32_t, 2> tube) {

  auto fd = std::ifstream(path);

  if (!fd.is_open()) {
    std::cerr << "fail to open" << std::endl;
    close(tube[0]);
    return -1;
  }
  // int count = 0;
  bool isAchar = false;
  // int code = 0;

  // init epsilon node
  auto correpondance = std::vector<int32_t>(1, '\0'); // init epsilon node
  int32_t buf = 0;
  int count = 0;
  int diff = 0;
  auto out = std::ofstream("out_test.text");
  if (!out.is_open()) {
    std::cerr << "fail to open" << std::endl;
    close(tube[0]);
    return -1;
  }

  while (buf != EOF) {
    isAchar = !isAchar;

    buf = fd.get();
    if (buf == 0) {
      diff++;
      isAchar = false;
      continue;
    }
    count++;
    if (diff == 2) {

      correpondance.push_back(buf);

      out << char(buf);
      diff = 0;
    } else {
      int temp;
      diff = 0;
      do {
        temp = fd.get();

      } while (temp == 0);

      correpondance.push_back((buf << 16) + temp);

      out << recreateWord(correpondance, buf << 16) << char(temp);
    }

    buf = fd.get();
  }

  out.close();
  close(tube[0]);
  return 1;
}
