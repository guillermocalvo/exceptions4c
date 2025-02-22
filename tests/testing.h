/*
 * Copyright 2025 Guillermo Calvo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define TEST_RESULT_PASS 0
#define TEST_RESULT_FAIL 1
#define TEST_RESULT_SKIP 77
#define TEST_RESULT_HARD 99

#define TEST_PRINT(stream, ...)                                                \
  do {                                                                         \
    (void) fprintf(stream, __VA_ARGS__);                                       \
    (void) fflush(stream);                                                     \
  } while(0)
#define TEST_PRINT_OUT(...)                                                    \
  TEST_PRINT(stdout, __VA_ARGS__)

#define TEST_PRINT_ERR(...)                                                    \
  TEST_PRINT(stderr, __VA_ARGS__)

#define TEST_PASS                                                              \
  do {                                                                         \
    TEST_PRINT_OUT(                                                            \
      "%s:%d [INFO] Test passed.\n",                                           \
      __FILE__,                                                                \
      __LINE__                                                                 \
    );                                                                         \
    exit(TEST_RESULT_PASS);                                                    \
  } while(0)

#define TEST_FAIL(...)                                                         \
  do {                                                                         \
    TEST_PRINT_ERR(__VA_ARGS__);                                               \
    exit(TEST_RESULT_FAIL);                                                    \
  } while(0)

#define TEST_SKIP(reason)                                                      \
  do {                                                                         \
    TEST_PRINT_ERR(                                                            \
      "%s:%d [WARNING] Test skipped because"                                   \
          " " reason ".\n",                                                    \
      __FILE__,                                                                \
      __LINE__                                                                 \
    );                                                                         \
    exit(TEST_RESULT_SKIP);                                                    \
  } while(0)

# define TEST_ASSERT_THAT(check, action)                                       \
  do {                                                                         \
    if (!(check)){                                                             \
      action;                                                                  \
    }                                                                          \
  } while(0)

#define TEST_ASSERT(check)                                                     \
  TEST_ASSERT_THAT(                                                            \
    check,                                                                     \
    TEST_FAIL(                                                                 \
      "%s:%d [ERROR] Test failed because"                                      \
          " assertion failed: " #check "\n",                                   \
      __FILE__,                                                                \
      __LINE__                                                                 \
    )                                                                          \
  )

#define TEST_ASSERT_EQUALS(check, format, found, expected)                     \
  TEST_ASSERT_THAT(                                                            \
    (check),                                                                   \
    TEST_FAIL(                                                                 \
      "%s:%d [ERROR] Test failed because"                                      \
          " `%s` is: " format " (expecting: " format ").\n",                   \
      __FILE__,                                                                \
      __LINE__,                                                                \
      #found,                                                                  \
      (found),                                                                 \
      (expected)                                                               \
    )                                                                          \
  )

#define TEST_ASSERT_INT_EQUALS(found, expected)                                \
  TEST_ASSERT_EQUALS(found == expected, "%d", found, expected)

#define TEST_ASSERT_PTR_EQUALS(found, expected)                                \
  TEST_ASSERT_EQUALS(found == expected, "%p", (void *) found, (void *) expected)

#define TEST_ASSERT_CHAR_EQUALS(found, expected)                               \
  TEST_ASSERT_EQUALS(found == expected, "'%c'", found, expected)

#define TEST_ASSERT_STR_EQUALS(found, expected)                                \
  TEST_ASSERT_EQUALS(strcmp(found, expected) == 0, "\"%s\"", found, expected)

#define TEST_ASSERT_STR_CONTAINS(haystack, needle)                             \
  TEST_ASSERT_THAT(                                                            \
    strstr((haystack), (needle)) != NULL,                                      \
    TEST_FAIL(                                                                 \
      "%s:%d [ERROR] Test failed because"                                      \
          " `%s` does not contain \"%s\" (found: \"%s\").\n",                  \
      __FILE__,                                                                \
      __LINE__,                                                                \
      #haystack,                                                               \
      (needle),                                                                \
      (haystack)                                                               \
    )                                                                          \
  )

#define TEST_ASSERT_BOOL_EQUALS(found, expected)                               \
  TEST_ASSERT_THAT(                                                            \
    (found) == (expected),                                                     \
    TEST_FAIL(                                                                 \
      "%s:%d [ERROR] Test failed because"                                      \
          " `%s` is: %s (expecting: %s).\n",                                   \
      __FILE__,                                                                \
      __LINE__,                                                                \
      #found,                                                                  \
      (found) ? "true" : "false",                                              \
      (expected) ? "true" : "false"                                            \
    )                                                                          \
  )

#define TEST_ASSERT_TRUE(found)                                                \
  TEST_ASSERT_BOOL_EQUALS(found, true)

#define TEST_ASSERT_FALSE(found)                                               \
  TEST_ASSERT_BOOL_EQUALS(found, false)

#define TEST_ASSERT_NULL(pointer)                                              \
  TEST_ASSERT_THAT(                                                            \
    (pointer) == NULL,                                                         \
    TEST_FAIL(                                                                 \
      "%s:%d [ERROR] Test failed because"                                      \
          " `%s` is not null.\n",                                              \
      __FILE__,                                                                \
      __LINE__,                                                                \
      #pointer                                                                 \
    )                                                                          \
  )

#define TEST_ASSERT_NOT_NULL(pointer)                                          \
  TEST_ASSERT_THAT(                                                            \
    (pointer) != NULL,                                                         \
    TEST_FAIL(                                                                 \
      "%s:%d [ERROR] Test failed because"                                      \
          " `%s` is null.\n",                                                  \
      __FILE__,                                                                \
      __LINE__,                                                                \
      #pointer                                                                 \
    )                                                                          \
  )
