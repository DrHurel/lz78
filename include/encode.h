#ifndef ENCODE_H
#define ENCODE_H

#include <array>
#include <string>

int32_t parseToEncode(const std::string &path,
                      const std::array<int32_t, 2> tube);

int32_t writeEncodedData(std::array<int32_t, 2> tube, const std::string &path);
int32_t writeEncodedDataDynamic(std::array<int32_t, 2> tube,
                                const std::string &path);
#endif // ENCODE_H
