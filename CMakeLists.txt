cmake_minimum_required(VERSION 3.2)

project(ndglang
  VERSION "0.1.0"
  DESCRIPTION "Building a simple programming language from scratch."
  HOMEPAGE_URL "https://github.com/nicdgonzalez/ndg-lang"
)

add_executable(ndglang
  "source/main.c"
  "source/lexer.c"
  "source/hashtable.c"
)

add_compile_options(ndglang
  -Wall
  -Werror
  -Wpedantic
  -std=c99
)
