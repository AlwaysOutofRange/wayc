#pragma once

#define roundup_4(n) (((n) + 3) & -4)

#define wayc_assert(cond)                                                      \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, "Assertion failed: %s:%d: %s\n", __FILE__, __LINE__,     \
              #cond);                                                          \
      abort();                                                                 \
    }                                                                          \
  } while (0)
