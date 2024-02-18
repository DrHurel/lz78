
#include "array"
#include "decode.h"
#include <fstream>

#include <iostream>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int writeDecodedText(std::array<int, 2> tube, const std::string &path) {
  std::cout << "writeDecodedText" << std::endl;
  char buf[20];
  std::ofstream fd;
  fd.open(path);
  if (!fd.is_open()) {
    return -1;
  }

  ssize_t readCheck = ::read(tube[0], &buf, sizeof(char));
  while (true) {
    std::cout << "write : " << buf << std::endl;
    if (readCheck == -1 || readCheck == 0) { // break on fail
      break;
    }

    fd << buf;
    readCheck = ::read(tube[0], &buf, sizeof(char));
  }

  fd << std::endl;
  close(tube[0]);

  return 1;
}