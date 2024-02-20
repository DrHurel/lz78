#include "utils.h"
#include <memory>
#include <string>
#include <vector>

std::string recreateWord(std::vector<std::shared_ptr<Token>> correpondance,
                         std::shared_ptr<Token> token) {

  if (token->getTag() == 0) {
    return std::string(1, token->getTerminalChar());
  }
  auto word = recreateWord(correpondance, correpondance.at(token->getTag()));
  return word.append(1, token->getTerminalChar());
}