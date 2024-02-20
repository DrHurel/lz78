
#include "encode.h"
#include "utils.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <memory>
#include <unistd.h>

bool isMax(int32_t newTag) { return newTag > (1 << 24) - 1; }

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

  auto temp = node_ptr;

  auto buf = std::array<char, 4>({0, 0, 0, 0});

  // ------ FILE READING ------
  std::ifstream fd;
  fd.open(path);

  if (!fd.is_open()) {

    close(tube[0]);
    throw failed_to_open(path);
    return -1;
  }
  // ------ FILE READING ------

  int32_t c = fd.get(); // read first char
  while (c != EOF) {
    // 894
    if (isMax(newTag)) { // max code size
      auto end = '\0';
      write(tube.at(1), &end, sizeof(char));
      fd.close();
      close(tube.at(1));

      return -1; // error
    }

    if (!temp->hasChild(c)) {

      if (auto n = std::make_shared<Node>(newTag); temp->append(n, c) != -1) {
        buf = codeToBuf(temp->getTag(), (char)c); // value that will be encoded

        write(tube.at(1), buf.data(),
              buf.size()); // send to background task to write
        ++newTag;

        temp = node_ptr;

      } else {
        throw node_failed_to_append();
      }

    } else {
      temp = temp->getChild(c);
    }

    c = fd.get();
  }
  auto end = EOF;
  write(tube.at(1), &end, sizeof(char));
  fd.close();

  close(tube.at(1));
  return 1;
}