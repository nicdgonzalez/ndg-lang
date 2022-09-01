#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "lexer.h"

// (MAJOR * 10000) + (MINOR * 100) + PATCH
# define NDGLANG_VERSION 100

extern char *source;
extern char token;
extern struct TokenInfo **tokens;
extern uint64_t t_count;

int32_t main(int32_t argc, char **argv) {
  if (argc < 2) {
    printf("Missing the input file command line argument.\n");
    exit(EXIT_FAILURE);
  }

  char *input_file = *(argv + 1);
  int64_t fd;
  if ((fd = open(input_file, 0)) < 0) {
    printf("Failed to open file: '%s'.\n", input_file);
    exit(EXIT_FAILURE);
  }

  int64_t pool_size = (256 * 1024); // arbitrary size
  if (!(source = (char *) malloc(pool_size))) {
    printf("Failed to allocate enough memory for the source code.\n");
    exit(EXIT_FAILURE);
  }
  if (!(tokens = (
      (struct TokenInfo **)
      calloc(pool_size, sizeof(struct TokenInfo))
  ))) {
    printf("Failed to allocate enough memory for the source code.\n");
    exit(EXIT_FAILURE);
  }

  int64_t index;
  if ((index = read(fd, source, (pool_size - 1))) < 0) {
    // read returns the file length on success and (x < 0) on error.
    printf("read() returned %lld.\n", index);
    exit(EXIT_FAILURE);
  }

  realloc(source, index);
  source[index] = '\0';  // append EOF character.
  close(fd);

  next_token();
  while (*source != '\0') {
    next_token();
  }

  for (int i = 0; i < t_count; i++) {
    printf("Token Type: '%d'\n", tokens[i]->type);
    printf("Token Name: '%s'\n", tokens[i]->name);
    if (tokens[i]->type == kNumber)
      printf("Token Value: '%d'\n", *(int32_t *) tokens[i]->value);
    else if (tokens[i]->type == kSingleComment)
      printf("Token Value: '%s'\n", (char *) tokens[i]->value);
    else if (tokens[i]->type == kMultiComment)
      printf("Token Value: '%s'\n", (char *) tokens[i]->value);
    printf("-------------------------------------------------\n");
  }

  return EXIT_SUCCESS;
}
