#ifndef DECODE_H
#define DECODE_H

#include <memory>
#include <string>
#include <vector>

#include "utils.h"

int32_t parseToDecode(const std::string &path, const std::string &outpath);
int32_t parseToDecodeDynamic(const std::string &path,
                             const std::string &outpath);
#endif // DECODE_H
