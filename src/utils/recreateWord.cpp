#include "utils.h"
#include <memory>
#include <string>
#include <vector>

/**
 * The function `recreateWord` recursively reconstructs a word from a vector of
 * tokens by following tag references and appending terminal characters.
 *
 * @param matching_tab The `matching_tab` parameter is a vector of shared
 * pointers to `Token` objects.
 * @param token The `token` parameter is a `std::shared_ptr<Token>` representing
 * a token object. It is used to access the tag and terminal character of the
 * token within the function `recreateWord`.
 *
 * @return The function `recreateWord` is returning a `std::string` that
 * represents a word created by recursively concatenating terminal characters
 * from the `matching_tab` vector of shared pointers to tokens.
 */
std::string recreateWord(std::vector<std::shared_ptr<Token>> matching_tab,
                         std::shared_ptr<Token> token) {

  if (token->getTag() == 0) {
    return std::string(1, token->getTerminalChar());
  }
  auto word = recreateWord(matching_tab, matching_tab.at(token->getTag()));
  return word.append(1, token->getTerminalChar());
}