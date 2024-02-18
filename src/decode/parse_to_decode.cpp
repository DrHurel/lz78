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

  if ((code >> 16) == 0) {
    return std::string(1, char(code));
  }
  auto word = recreateWord(correpondance, correpondance.at(code >> 16));
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
  bool isAchar = false;
  auto correpondance = std::vector<int32_t>(1, '\0'); // init epsilon node
  int32_t buf = 0;

  int diff = 0;
  auto out = std::ofstream("out_test.txt");
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
      auto word = recreateWord(correpondance, buf << 16);
      out.write(word.data(), word.size() - 1);
      auto c = char(temp);
      out.write(&c, sizeof(char));
    }

    buf = fd.get();
  }

  out.close();
  close(tube[0]);
  return 1;
}
