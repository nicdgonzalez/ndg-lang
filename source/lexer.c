#include "lexer.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <stdio.h>

#include "utils.h"

char *source;  // The entire input file source code.
char token;
struct TokenInfo *current;

uint64_t line = 0;
struct TokenInfo **tokens;
uint64_t token_count = 0;

void insert_token(int32_t _token_type, char *_name, void *_value) {
  if (!(IS_BETWEEN(_token_type, 0, kBracket))) {
    printf("Missing required argument `token_type`.\n");
    exit(EXIT_FAILURE);
  }
  current = (struct TokenInfo *) malloc(sizeof(struct TokenInfo));
  current->type = (uint8_t) _token_type;
  current->name = _name;
  current->value = _value;
  tokens[token_count++] = current;
  return;
}

void next_token() {

  while ((token = *source++)) {
    // Helper variables for `insert_token`.
    int32_t token_type = -1;
    char *identifier_name = NULL;
    void *token_value = NULL;
    int64_t index = 0;

    /*
      A switch statement is significantly faster than an if-else ladder
      if there are many nested if-else's involved. This is due to the
      creation of a jump table for switch during compilation. As a
      result, instead of checking which case is satisfied throughout
      execution, it just decides which case must be completed.

      https://www.scaler.com/topics/c/difference-between-if-else-and-switch/
    */

    switch (token) {
      case 'A': case 'a':
      case 'B': case 'b':
      case 'C': case 'c':
      case 'D': case 'd':
      case 'E': case 'e':
      case 'F': case 'f':
      case 'G': case 'g':
      case 'H': case 'h':
      case 'I': case 'i':
      case 'J': case 'j':
      case 'K': case 'k':
      case 'L': case 'l':
      case 'M': case 'm':
      case 'N': case 'n':
      case 'O': case 'o':
      case 'P': case 'p':
      case 'Q': case 'q':
      case 'R': case 'r':
      case 'S': case 's':
      case 'T': case 't':
      case 'U': case 'u':
      case 'V': case 'v':
      case 'W': case 'w':
      case 'X': case 'x':
      case 'Y': case 'y':
      case 'Z': case 'z':
      case '_':
        token_type = kIdentifier;
        identifier_name = (char *) calloc(79, sizeof(char));
        identifier_name[index++] = token;
        while (IS_ALPHANUMERIC(*source) || (*source == '_')) {
          identifier_name[index++] = *source++;
        }
        return insert_token(token_type, identifier_name, token_value);

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        token_type = kNumber;
        token_value = (int64_t *) malloc(sizeof(int64_t));
        *(int64_t *) token_value = (token - '0');
        // Decimal values start with a value between 1 and 9 inclusive.
        if (*(int64_t *) token_value > 0) {
          while (IS_BETWEEN(*source, '1', '9')) {
            *(int64_t *) token_value = (
              ((*(int64_t *) token_value) * 10)
              + ((*source)++ - '0')
            );
          }
        }
        // Binary, Hexadecimal and Octal start with '0' (e.g., 0xFF).
        else {
          // Binary
          if ((*source == 'b') || (*source == 'B')) {
          }
          // Hexadecimal
          else if ((*source == 'x') || (*source == 'X')) {
            token = *++source;
            while (
                IS_NUMERIC(token)
                || IS_BETWEEN(token, 'a', 'f')
                || IS_BETWEEN(token, 'A', 'F')
            ) {
              *(int64_t *) token_value = (
                ((*(int64_t *) token_value) * 16)
                + (token & 15)
                + (token >= 'A' ? 9 : 0)
              );
              token = *++source;
            }
          }
          // Octal
          else {
            while ((*source >= '0') && (*source <= '7')) {
              *(int64_t *) token_value = (
                ((*(int64_t *) token_value) * 8)
                + (*source - '0')
              );
            }
          }
        }
        return insert_token(token_type, identifier_name, token_value);
/*
      case '\'':
      case '"':
        return;
        return insert_token(token_type, identifier_name, token_value);  // TODO
*/
      case '/':
        token_type = kDivide;
        token_value = (char *) calloc(4096, sizeof(char));

        if (*source == '/') {
          token_type = kSingleComment;
          source += 1;
          // Skip initial whitespaces and forward slashes.
          while (*source == '/' || *source == ' ')
            source += 1;
          while ((*source != '\n') && (*source != '\0'))
            *(char *) token_value++ = *source++;
        }
        else if (*source == '*') {
          token_type = kMultiComment;
          while (
              (*source != '\0')
              && (!((*source == '*') && (*(source + 1) == '/')))
          )
            *(char *) token_value++ = *source++;

          // skip a character to account for the (1) char look-ahead.
          source += 1;
        }
        (index > 0) ? realloc(token_value, index) : (token_value = NULL);
        return insert_token(token_type, identifier_name, token_value);

      case '\n':
        line++;
        return;

      case '=':
        token_type = kAssign;
        if (*source == '=') {
          token_type = kEqual;
          source += 1;
        }
        return insert_token(token_type, identifier_name, token_value);

      case '+':
        token_type = kAdd;
        if (*source == '+') {
          token_type = kIncrement;
          source += 1;
        }
        return insert_token(token_type, identifier_name, token_value);

      case '-':
        token_type = kSubtract;
        if (*source == '-') {
          token_type = kDecrement;
          source += 1;
        }
        return insert_token(token_type, identifier_name, token_value);

      case '!':
        token_type = kLogicalNOT;
        if (*source == '=') {
          token_type = kNotEqual;
          source += 1;
        }
        return insert_token(token_type, identifier_name, token_value);

      case '<':
        token_type = kLessThan;
        if (*source == '=') {
          token_type = kLessEqual;
          source += 1;
        }
        return insert_token(token_type, identifier_name, token_value);

      case '>':
        token_type = kGreaterThan;
        if (*source == '=') {
          token_type = kGreaterEqual;
          source += 1;
        }
        return insert_token(token_type, identifier_name, token_value);

      case '|':
        token_type = kBitwiseOR;
        if (*source == '|') {
          token_type = kLogicalOR;
          source += 1;
        }
        return insert_token(token_type, identifier_name, token_value);

      case '&':
        token_type = kBitwiseAND;
        if (*source == '&') {
          token_type = kLogicalAND;
          source += 1;
        }
        return insert_token(token_type, identifier_name, token_value);

      case '^':
        token_type = kBitwiseXOR;
        return insert_token(token_type, identifier_name, token_value);

      case '%':
        token_type = kModulo;
        return insert_token(token_type, identifier_name, token_value);

      case '*':
        token_type = kMultiply;
        return insert_token(token_type, identifier_name, token_value);

      case '[':
        token_type = kBracket;
        return insert_token(token_type, identifier_name, token_value);

      case '?':
        token_type = kTernaryConditional;
        return insert_token(token_type, identifier_name, token_value);

      case '~':
        token_type = kBitwiseNOT;
        return insert_token(token_type, identifier_name, token_value);

      case ';':
      case '{':
      case '}':
      case '(':
      case ']': 
      case ',':
      case ':':
        // Let the character be the token.
        token_type = token;
        return insert_token(token_type, identifier_name, token_value);

      default:
        // Handle condition for disallowed characters.
        return;
    }  // !switch
  }  // !while
  return;
}
