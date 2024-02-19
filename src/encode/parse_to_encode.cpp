
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

std::array<char, 4> codeToBuf(int tag, char c) {
  if (tag > (1 << 24) - 1) {
    throw std::runtime_error("tag is too big");
  }

  return {static_cast<char>(tag), static_cast<char>(tag >> 8),
          static_cast<char>(tag >> 16), c};
}

int32_t parseToEncode(const std::string &path,
                      const std::array<int32_t, 2> tube) {

  auto node = std::make_shared<Node>(0); // init epsilon node
  uint32_t newTag = 1;

  auto temp = std::shared_ptr<Node>(node); // temp node to navigate the tree

  auto buf = std::array<char, 4>();
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

    if (isMax(newTag)) { // max code size
      return -1;         // error
    }

    if (!temp->hasChild(c)) {

      if (auto n = std::make_unique<Node>(newTag);
          temp->append(std::move(n), c) != -1) {
        buf = codeToBuf(temp->getTag(), (char)c); // value that will be encoded

        write(tube[1], buf.data(),
              buf.size()); // send to background task to write
        ++newTag;
        temp = std::shared_ptr<Node>(node);

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