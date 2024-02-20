
#include "array"
#include "encode.h"
#include "utils.h"
#include <fstream>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int32_t writeEncodedData(std::array<int32_t, 2> tube, const std::string &path) {
  auto buf = std::array<char, 4>();
  std::ofstream fd;
  fd.open(path);
  if (!fd.is_open()) {
    close(tube[0]);
    throw failed_to_open(path);
  }

  ssize_t readCheck = ::read(tube[0], buf.data(), 4);
  while (readCheck != 1) {

    fd.write(buf.data(), buf.size());
    // Write the string to the file
    readCheck = ::read(tube[0], buf.data(), 4);
  }

  fd << std::endl;
  fd.close();
  close(tube[0]);

  return 1;
}