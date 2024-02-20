#ifndef UTILS_H
#define UTILS_H

#include <array>
#include <memory>
#include <sys/types.h>
class Node {

  int32_t _tag;
  std::array<std::shared_ptr<Node>, 256> _chidren;

public:
  explicit Node(int32_t tag);

  int32_t getTag() const;

  int32_t append(std::shared_ptr<Node> node, int32_t key);

  bool hasChild(int32_t key) const;
  std::shared_ptr<Node> getChild(int32_t k) const;
};

#endif