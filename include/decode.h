#ifndef DECODE_H
#define DECODE_H
#include <array>
#include <string>

int32_t parseToDecode(const std::string &path,
                      const std::array<int32_t, 2> tube);
int32_t writeDecodedText(std::array<int32_t, 2> tube, const std::string &path);
#endif // DECODE_H
