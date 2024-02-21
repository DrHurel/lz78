#include "decode.h"
#include "utils.h"
#include <cstdint>
#include <cstdio>
#include <fstream>

#include <memory>
#include <ostream>
#include <string>
#include <unistd.h>

int32_t parseToDecodeDynamic(const std::string &path,
                             const std::string &outpath) {

  auto fd = std::ifstream(path);

  if (!fd.is_open()) {
    throw failed_to_open(path);
  }

  auto matching_tab =
      std::vector<std::shared_ptr<Token>>(1, nullptr); // init epsilon node
  char buf;
  auto payload = std::vector<unsigned char>();
  auto out = std::ofstream(outpath);
  if (!out.is_open()) {
    throw failed_to_open(outpath);
    return -1;
  }
  long check = 0;
  int count = 0;
  int timeout = 0;
  int encoding_size = 2;
  check = fd.readsome(&buf, sizeof(char));
  while (check != EOF && timeout < 1000) { // timeout to avoid infinite loop on
    // corrupted file
    long n = matching_tab.size();
    if (encoding_size == 2 && n > 256) {
      encoding_size = 3;
    }
    if (encoding_size == 3 && n > 65536) {
      encoding_size = 4;
    }
    if (check != 1) {
      check = fd.readsome(&buf, sizeof(char));
      timeout++;
      continue;
    } else { // reset timeout if a char is read
      timeout = 0;
      count++;
    }
    if (count == encoding_size) { // 4 in buffer means a token
      count = 0;
      for (int i = encoding_size; i < 4; i++) {
        payload.push_back(0);
      }
      payload.push_back((unsigned char)buf);
      auto token = Token(payload);

      auto token_ptr = std::make_shared<Token>(token);
      out << recreateWord(matching_tab, std::shared_ptr<Token>(token_ptr));

      matching_tab.push_back(std::make_unique<Token>(token));
      payload.clear();
    } else {
      payload.push_back((unsigned char)buf);
    }

    check = fd.readsome(&buf, sizeof(char));
  }

  out << std::endl;

  // Close file descriptors
  out.close();
  fd.close();
  return 1;
}
