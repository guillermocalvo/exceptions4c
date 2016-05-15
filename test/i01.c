
# include "testing.h"


static E4C_DEFINE_EXCEPTION(CustomException, "This is a custom exception", RuntimeException);


/**
 * Print exception type
 *
 * This test calls `e4c_print_exception_type` passing a user-defined exception
 * type.
 *
 */
TEST_CASE{

    e4c_print_exception_type(&CustomException);
}
