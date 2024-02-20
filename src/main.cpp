#include "decode.h"
#include "encode.h"

#include <array>
#include <cstdlib>
#include <iostream>
#include <ostream>
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
      writeEncodedDataDynamic(tube, std::string(argv[2]) + ".lz");
      std::cout << "Writing : done" << std::endl;
    }
  }

  if (std::string(argv[1]) == "decode") {
    if (argc < 4) {
      std::cout << "usage : " << argv[0] << " decode [target] [outpath]"
                << std::endl;
      exit(EXIT_FAILURE);
    }
    std::cout << "Decompressing..." << std::endl;
    parseToDecodeDynamic(argv[2], argv[3]);
    std::cout << "Decompressing done" << std::endl;
  }

  waitpid(-1, &status, 0);
  std::cout << "end" << std::endl;
}
