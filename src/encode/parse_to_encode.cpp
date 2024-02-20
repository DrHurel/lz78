
#include "encode.h"
#include "utils.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <unistd.h>

bool isMax(int32_t newTag) { return newTag > (1 << 24) - 1; }

struct tag_out_of_size : public std::exception {};

std::array<char, 4> codeToBuf(int tag, char c) {
  if (tag > (1 << 24) - 1) {
    throw tag_out_of_size();
  }

  return {static_cast<char>(tag), static_cast<char>(tag >> 8),
          static_cast<char>(tag >> 16), c};
}

int32_t parseToEncode(const std::string &path,
                      const std::array<int32_t, 2> tube) {

  auto node_ptr = std::make_shared<Node>(0); // init epsilon node
  uint32_t newTag = 1;
  std::cout << "Start" << std::endl;
  auto temp = node_ptr;

  auto buf = std::array<char, 4>({0, 0, 0, 0});
  std::cout << "array created" << std::endl;
  // ------ FILE READING ------
  std::ifstream fd;
  fd.open(path);

  if (!fd.is_open()) {
    std::cout << "fail to open" << std::endl;
    close(tube[0]);
    return -1;
  }
  // ------ FILE READING ------

  int32_t c = fd.get(); // read first char
  while (c != EOF) {
    // 894
    if (isMax(newTag)) { // max code size
      auto end = '\0';
      write(tube[1], &end, sizeof(char));
      fd.close();
      close(tube[1]);

      return -1; // error
    }

    if (!temp->hasChild(c)) {
      std::cout << "c: " << c << std::endl;
      std::cout << "newTag: " << newTag << std::endl;
      if (auto n = std::make_shared<Node>(newTag); temp->append(n, c) != -1) {
        buf = codeToBuf(temp->getTag(), (char)c); // value that will be encoded

        write(tube[1], buf.data(),
              buf.size()); // send to background task to write
        ++newTag;

        temp = node_ptr;

      } else {
        std::cerr << "node->append fail " << std::endl;
        return -1;
      }

    } else {
      temp = temp->getChild(c);
    }

    c = fd.get();
  }
  auto end = '\0';
  write(tube[1], &end, sizeof(char));
  fd.close();

  close(tube[1]);
  return 1;
}