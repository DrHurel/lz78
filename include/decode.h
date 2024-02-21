#ifndef DECODE_H
#define DECODE_H

#include <string>

int32_t parseToDecode(const std::string &path, const std::string &outpath);
int32_t parseToDecodeDynamic(const std::string &path,
                             const std::string &outpath);
#endif // DECODE_H
