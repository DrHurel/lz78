#include "decode.h"
#include "encode.h"
#include "utils.h"
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

int32_t main(int32_t argc, char *argv[]) {

  int32_t status;
  if (argc < 3) {
    std::cout << "usage : " << argv[0] << " [mode] [target]" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << "Start" << std::endl;
  std::array<int32_t, 2> tube;
  pipe(tube.data());

  int32_t pid;

  if (std::string(argv[1]) == "compress") {
    if ((pid = fork()) == -1) {
      exit(EXIT_FAILURE);
    } else if (pid != 0) {
      std::cout << "Compressing..." << std::endl;
      parseToEncode(argv[2], tube);
      std::cout << "Compressing done" << std::endl;
    } else {
      std::cout << "Writing..." << std::endl;
      writeEncodedData(tube, std::string(argv[2]) + ".lz");
      std::cout << "Writing : done" << std::endl;
    }
  }

  if (std::string(argv[1]) == "decode") {
    if ((pid = fork()) == -1) {
      exit(EXIT_FAILURE);
    } else if (pid != 0) {
      std::cout << "Decompressing..." << std::endl;
      parseToDecode(argv[2], tube);
      std::cout << "Decompressing done" << std::endl;
    } else {
      std::cout << "Writing..." << std::endl;
      writeDecodedText(tube, std::string(argv[2]) + ".text");
      std::cout << "Writing : done" << std::endl;
    }
  }

  waitpid(-1, &status, 0);
  std::cout << "end" << std::endl;
}
