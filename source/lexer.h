#ifndef NDGLANG_LEXER_H_
# define NDGLANG_LEXER_H_

# include <stdint.h>

/*
  Note:
    Because certain characters may be returned as tokens directly
    and the ASCII character table has (127) entries, to ensure each
    token is given a unique value, ``TokenType`` enumeration starts
    from ``128`` instead of ``0``.

    Operator tokens are ordered based on C's Operator Precedence.
*/
enum TokenType {
  kIdentifier = 128,
  kChar,
  kNumber,
  kSingleComment,
  kMultiComment,
  kAssign,
  kTernaryConditional,
  kLogicalOR,
  kLogicalAND,
  kBitwiseOR,
  kBitwiseXOR,
  kBitwiseAND,
  kEqual,
  kNotEqual,
  kLessThan,
  kLessEqual,
  kGreaterThan,
  kGreaterEqual,
  kLeftShift,
  kRightShift,
  kAdd,
  kSubtract,
  kMultiply,
  kDivide,
  kModulo,
  kLogicalNOT,
  kBitwiseNOT,
  kIncrement,
  kDecrement,
  kBracket,
};

struct TokenInfo {
  uint8_t type;
  char *name;
  void *value;
};

void next_token();

#endif  // !NDGLANG_LEXER_H_
