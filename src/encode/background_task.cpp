
#include "array"
#include "encode.h"
#include <fstream>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

std::array<char, 4> intToBytes(int32_t buf) {
  return {static_cast<char>(buf), static_cast<char>(buf >> 8),
          static_cast<char>(buf >> 16), static_cast<char>(buf >> 24)};
}

int32_t writeEncodedData(std::array<int32_t, 2> tube, const std::string &path) {
  auto buf = std::array<char, 4>();
  std::ofstream fd;
  fd.open(path);
  if (!fd.is_open()) {
    return -1;
  }

  ssize_t readCheck = ::read(tube[0], buf.data(), 4);
  while (readCheck != -1 && readCheck != 1) {

    fd.write(buf.data(), 4);
    // Write the string to the file
    readCheck = ::read(tube[0], buf.data(), 4);
  }

  fd << std::endl;
  fd.close();
  close(tube[0]);

  return 1;
}