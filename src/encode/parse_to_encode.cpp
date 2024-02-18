
#include "encode.h"
#include "utils.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <unistd.h>

int32_t encodeToken(int32_t previous, char end) {
  return (previous << 8) + end;
}

bool isMax(int32_t newTag) { return newTag > (1 << 16) - 1; }

int32_t parseToEncode(const std::string &path,
                      const std::array<int32_t, 2> tube) {

  auto node = std::make_shared<Node>(0); // init epsilon node
  int32_t newTag = 1;

  auto temp = std::shared_ptr<Node>(node); // temp node to navigate the tree

  int32_t buf;
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
        buf = temp->getTag() + (c << 16); // value that will be encoded

        write(tube[1], &buf,
              sizeof(int32_t)); // send to background task to write
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
  buf = -1;
  write(tube[1], &buf, sizeof(int32_t));
  fd.close();

  close(tube[1]);
  return 1;
}