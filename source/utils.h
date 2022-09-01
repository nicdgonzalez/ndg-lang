#ifndef NDGLANG_GLOBAL_H_
# define NDGLANG_GLOBAL_H_

# define IS_BETWEEN(var, a, b) ((var >= a) && (var <= b))
# define IS_LOWER(var) IS_BETWEEN(var, 'a', 'z')
# define IS_UPPER(var) IS_BETWEEN(var, 'A', 'Z')
# define IS_NUMERIC(var) IS_BETWEEN(var, '0', '9')
# define IN_ALPHABET(var) (IS_LOWER(var) || IS_UPPER(var))
# define IS_ALPHANUMERIC(var) (IN_ALPHABET(var) || IS_NUMERIC(var))

#endif  // !NDGLANG_GLOBAL_H_
