
# ifndef TESTING_H
# define TESTING_H


# include <stdlib.h>
# include <stdio.h>
# include <e4c_lite.h>


/* Test Results */
# define TEST_RESULT_PASS   EXIT_SUCCESS
# define TEST_RESULT_SKIP   77
# define TEST_RESULT_FAIL   EXIT_FAILURE


/* Test Actions */
# define TEST_EXIT(RESULT)  exit(RESULT)
# define TEST_PASS          TEST_EXIT(TEST_RESULT_PASS)
# define TEST_SKIP          TEST_EXIT(TEST_RESULT_SKIP)
# define TEST_FAIL          TEST_EXIT(TEST_RESULT_FAIL)
# define TEST_ASSERT(CHECK) \
    do{ \
        if( !(CHECK) ){ \
            (void)fprintf(stderr, "Assertion failed: %s\n", #CHECK); \
            TEST_FAIL; \
        } \
    }while(0)


/* Test Cases */
/* Test Cases */
# define TEST_CASE \
    \
    void test_case(void); \
    \
    int main(void){ \
        (void)printf("    - Running test %s...\n", __FILE__); \
        test_case(); \
        return(EXIT_SUCCESS); \
    } \
    \
    void test_case(void)


# endif
