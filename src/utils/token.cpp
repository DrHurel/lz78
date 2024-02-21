#include "utils.h"

/**
 * The Token constructor initializes the _terminal_char and _tag members based
 * on the input buffer, performing size validation and bitwise operations.
 *
 * @param buf The `buf` parameter is a vector of unsigned characters that is
 * used to initialize the `Token` object. The constructor takes the fourth
 * element of the vector and assigns it to the
 * `_terminal_char` member variable. It then combines the first three elements
 * of the vector to form a 32-bit tag
 */
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

Token::Token(unsigned char terminal_char) : _terminal_char(terminal_char){};

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
