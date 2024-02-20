#include "utils.h"
#include <sys/types.h>

Node::Node(uint32_t tag) : _tag(tag) {}

uint32_t Node::getTag() const { return _tag; }
int32_t Node::append(std::shared_ptr<Node> node, int32_t key) {

  if (_chidren[key] != nullptr) {
    return -1;
  }

  _chidren[key] = std::move(node);

  return 1;
}

bool Node::hasChild(int32_t key) const { return _chidren.at(key) != nullptr; }

std::shared_ptr<Node> Node::getChild(int32_t k) const { return _chidren[k]; }