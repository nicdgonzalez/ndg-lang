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
int64_t t_count = 0;  // Number of tokens in `tokens`.

void next_token() {
  current = (struct TokenInfo *) malloc(sizeof(struct TokenInfo));
  current->type = 0;
  current->name = (char *) calloc(79, sizeof(char));
  current->value = NULL;

  while ((token = *source++)) {
    int64_t index = 0;

    if (IN_ALPHABET(token) || (token == '_')) {
      current->type = kIdentifier;
      current->name[index++] = token;
      while (IS_ALPHANUMERIC(*source) || (*source == '_')) {
        current->name[index++] = *source++;
      }
      tokens[t_count++] = current;
      return;
    }
    else if (IS_NUMERIC(token)) {
      current->type = kNumber;
      int64_t *token_value = (int64_t *) malloc(sizeof(int64_t));
      *token_value = (token - '0');
      // Decimal values start with a value between 1 and 9 inclusive.
      if ((*token_value) > 0) {
        while (IS_BETWEEN(*source, '1', '9')) {
          *token_value = (((*token_value) * 10) + ((*source)++ - '0'));
        }
      }
      // Binary, Hexadecimal and Octal start with '0'.
      else {
        // Binary
        if ((*source == 'b') || (*source == 'B')) {
          break;
        }
        // Hexadecimal
        else if ((*source == 'x') || (*source == 'X')) {
          token = *++source;
          while (
              IS_NUMERIC(token)
              || IS_BETWEEN(token, 'a', 'f')
              || IS_BETWEEN(token, 'A', 'F')
          ) {
            *token_value = (
              ((*token_value) * 16)
              + (token & 15)
              + (token >= 'A' ? 9 : 0)
            );
            token = *++source;
          }
        }
        // Octal
        else {
          while ((*source >= '0') && (*source <= '7')) {
            *token_value = (((*token_value) * 8) + (*source - '0'));
          }
        }
      }
      current->value = token_value;
      tokens[t_count++] = current;
      return;
    }
    char *token_value;
    switch (token) {
      case '\'':
      case '"':
        return;  // TODO: Implement string literals and chars.
      case '/':
        if (*source == '/') {
          token_value = (char *) calloc(4096, sizeof(char));
          current->type = kSingleComment;
          source += 1;
          // Skip initial whitespaces and forward slashes.
          while (*source == '/' || *source == ' ')
            source += 1;
          while ((*source != '\n') && (*source != '\0'))
            token_value[index++] = *source++;
        }
        else if (*source == '*') {
          token_value = (char *) calloc(4096, sizeof(char));
          current->type = kMultiComment;
          while (
              (*source != '\0')
              && (!((*source == '*') && (*(source + 1) == '/')))
          )
            token_value[index++] = *source++;

          // skip characters to account for the (1) char look-ahead.
          source += 1;
        }
        else {
          token_value = NULL;
          current->type = kDivide;
        }
        realloc(token_value, index);
        current->value = token_value;
        tokens[t_count++] = current;
        return;

      case '\n':
        line++;
        return;

      case '=':
        if (*source == '=') {
          source += 1;
          current->type = kEqual;
        }
        else {
          current->type = kAssign;
        }
        tokens[t_count++] = current;
        return;

      case '+':
        if (*source == '+') {
          source += 1;
          current->type = kIncrement;
        }
        else {
          current->type = kAdd;
        }
        tokens[t_count++] = current;
        return;

      case '-':
        if (*source == '-') {
          source += 1;
          current->type = kDecrement;
        }
        else {
          current->type = kSubtract;
        }
        tokens[t_count++] = current;
        return;

      case '!':
        if (*source == '=') {
          source += 1;
          current->type = kNotEqual;
        }
        else {
          current->type = kLogicalNOT;
        }
        tokens[t_count++] = current;
        return;

      case '<':
        if (*source == '=') {
          source += 1;
          current->type = kLessEqual;
        }
        else {
          current->type = kLessThan;
        }
        tokens[t_count++] = current;
        return;

      case '>':
        if (*source == '=') {
          source += 1;
          current->type = kGreaterEqual;
        }
        else {
          current->type = kGreaterThan;
        }
        tokens[t_count++] = current;
        return;

      case '|':
        if (*source == '|') {
          source += 1;
          current->type = kLogicalOR;
        }
        else {
          current->type = kBitwiseOR;
        }
        tokens[t_count++] = current;
        return;

      case '&':
        if (*source == '&') {
          source += 1;
          current->type = kLogicalAND;
        }
        else {
          current->type = kBitwiseAND;
        }
        tokens[t_count++] = current;
        return;

      case '^':
        current->type = kBitwiseXOR;
        tokens[t_count++] = current;
        return;

      case '%':
        current->type = kModulo;
        tokens[t_count++] = current;
        return;

      case '*':
        current->type = kMultiply;
        tokens[t_count++] = current;
        return;

      case '[':
        current->type = kBracket;
        tokens[t_count++] = current;
        return;

      case '?':
        current->type = kTernaryConditional;
        tokens[t_count++] = current;
        return;

      case '~':
        current->type = kBitwiseNOT;
        tokens[t_count++] = current;
        return;

      case ';':
      case '{':
      case '}':
      case '(':
      case ']': 
      case ',':
      case ':':
        // Let the character be the token.
        current->type = token;
        tokens[t_count++] = current;
        return;

      default:
        // Handle condition for disallowed characters.
        return;

    }  // !switch
  }  // !while
  return;
}
