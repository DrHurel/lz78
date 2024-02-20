
#include "array"
#include "encode.h"
#include "utils.h"
#include <fstream>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int32_t writeEncodedDataDynamic(std::array<int32_t, 2> tube,
                                const std::string &path) {
  auto buf = std::array<char, 4>();
  std::ofstream fd;
  fd.open(path);
  if (!fd.is_open()) {
    close(tube[0]);
    throw failed_to_open(path);
  }
  int bits_field = 2;
  int bits_readed = 0;
  ssize_t readCheck = read(tube[0], buf.data(), 4);
  while (readCheck != 1) { // not standard ? may create bug
    bits_readed++;
    if (bits_field == 2 && bits_readed > 256) {
      bits_field = 3;
    }
    if (bits_field == 3 && bits_readed > 65536) {
      bits_field = 4;
    }
    auto payload = std::vector<char>();
    switch (bits_field) {
    case 2:
      payload.push_back(buf[0]);
      payload.push_back(buf[3]);
      break;
    case 3:
      payload.push_back(buf[0]);
      payload.push_back(buf[1]);
      payload.push_back(buf[3]);
      break;
    case 4:
      payload.push_back(buf[0]);
      payload.push_back(buf[1]);
      payload.push_back(buf[2]);
      payload.push_back(buf[3]);
      break;
    default:
      break;
    }

    fd.write(payload.data(), payload.size());
    // Write the string to the file
    readCheck = ::read(tube[0], buf.data(), 4);
    payload.clear();
  }

  fd << std::endl;
  fd.close();
  close(tube[0]);

  return 1;
}