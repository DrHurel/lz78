#include "utils.h"

Token::Token(std::vector<unsigned char> buf) : _terminal_char(buf.at(3)) {
  if (buf.size() != 4) {
    throw invalid_buf_size();
  }
  for (int i = 0; i < 3; i++) {
    _tag |= static_cast<uint32_t>(buf.at(i)) << (i * 8);
  }

  if (_tag > (1 << 24) - 1) {
    throw tag_out_of_size();
  }
};

Token::Token(unsigned char terminal_char)
    : _terminal_char(terminal_char){

      };

/**
 * @brief return the tag of the token which is the index of the previous token.
 *
 * @return uint32_t
 */
uint32_t Token::getTag() const { return _tag; }

/**
 * @brief return the terminal char of the token.
 *
 * @return unsigned char
 */
unsigned char Token::getTerminalChar() const { return _terminal_char; }
