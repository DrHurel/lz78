#ifndef UTILS_H
#define UTILS_H

#include <array>
#include <memory>
#include <string>
#include <sys/types.h>
#include <vector>

class Node {

  uint32_t _tag;
  std::array<std::shared_ptr<Node>, 256> _chidren;

public:
  explicit Node(uint32_t tag);

  uint32_t getTag() const;

  int32_t append(std::shared_ptr<Node> node, int32_t key);

  bool hasChild(int32_t key) const;
  std::shared_ptr<Node> getChild(int32_t k) const;
};

class Token {
  int32_t _tag = 0;
  unsigned char _terminal_char;

public:
  explicit Token(std::vector<unsigned char> buf);
  explicit Token(unsigned char terminal_char);
  uint32_t getTag() const;
  unsigned char getTerminalChar() const;
};
std::string recreateWord(std::vector<std::shared_ptr<Token>> correpondance,
                         std::shared_ptr<Token> token);

struct invalid_buf_size : public std::exception {};
struct tag_out_of_size : public std::exception {};
struct node_failed_to_append : public std::exception {};
class failed_to_open : public std::exception {

  std::string path;

public:
  explicit failed_to_open(const std::string &path) : path(path){};

  const char *what() const throw() final { return path.c_str(); };
};

#endif