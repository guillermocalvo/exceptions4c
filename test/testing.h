
# ifndef TESTING_H
# define TESTING_H


# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <e4c.h>


/* Output */
# define TEST_PRINT_PREFIX              "    - "
# define TEST_PRINT_SUFFIX              "\n"
# define TEST_PRINT(FORMAT, MESSAGE) \
    do{ \
        (void)fprintf(stdout, TEST_PRINT_PREFIX FORMAT TEST_PRINT_SUFFIX, (MESSAGE)); \
        (void)fflush(stdout); \
    }while(0)
# define TEST_ECHO(MESSAGE)             TEST_PRINT("%s", (MESSAGE))
# define TEST_DUMP(FORMAT, VARIABLE)    TEST_PRINT(#VARIABLE ": " FORMAT, (VARIABLE))
# define TEST_EXPECTING(EXCEPTION)      TEST_PRINT("Expecting exception: %s", #EXCEPTION)
# define THIS_SHOULD_NOT_HAPPEN         TEST_ECHO("ERROR! Something didn't quite work...")


/* Test Results */
# define TEST_RESULT_PASS               EXIT_SUCCESS
# define TEST_RESULT_FAIL               EXIT_FAILURE
# define TEST_RESULT_SKIP               77


/* Test Actions */
# define TEST_EXIT(MESSAGE, RESULT) \
    do{ \
        TEST_ECHO(MESSAGE); \
        exit(RESULT); \
    }while(0)
# define TEST_SKIP(MESSAGE)             TEST_EXIT(MESSAGE, TEST_RESULT_SKIP)
# define TEST_PASS(MESSAGE)             TEST_EXIT(MESSAGE, TEST_RESULT_PASS)
# define TEST_FAIL(MESSAGE)             TEST_EXIT(MESSAGE, TEST_RESULT_FAIL)
# define TEST_X_PASS(MESSAGE)           TEST_EXIT(MESSAGE, TEST_RESULT_FAIL)
# define TEST_X_FAIL(MESSAGE)           TEST_EXIT(MESSAGE, TEST_RESULT_PASS)
# define TEST_ASSERT_THAT(CHECK, ACTION) \
    do{ \
        if( !(CHECK) ){ \
            ACTION; \
        } \
    }while(0)

/* Assertions */
# define TEST_ASSERT(CHECK) \
    TEST_ASSERT_THAT( CHECK, TEST_FAIL("Assertion failed: " #CHECK "\n") )
# define TEST_X_ASSERT(CHECK) \
    TEST_ASSERT_THAT( CHECK, TEST_PASS("Assertion failed: " #CHECK "\n") )
# define TEST_ASSERT_EQUALS(FOUND, EXPECTED) \
    TEST_ASSERT_THAT( (FOUND) == (EXPECTED), TEST_FAIL(#FOUND " does not equals " #EXPECTED "\n") )
# define TEST_ASSERT_STRING_EQUALS(FOUND, EXPECTED) \
    TEST_ASSERT_THAT( strcmp( (FOUND), (EXPECTED) ) == 0, TEST_FAIL(#FOUND " does not contain string " #EXPECTED "\n") )


/* Test Cases */
# define TEST_CASE \
    \
    void test_case(void); \
    \
    int main(void){ \
        TEST_PRINT("Running test %s...", __FILE__); \
        test_case(); \
        return(EXIT_SUCCESS); \
    } \
    \
    void test_case(void)

# define TEST_X_CASE TEST_CASE


/* Third party library simulation */
# define LIBRARY_SUCCESS 0
# define LIBRARY_FAILURE -1
# define LIBRARY_FAILURE_IO -2
# define LIBRARY_FAILURE_ILLEGAL_ARGUMENT -3


# endif
