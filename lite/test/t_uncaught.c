
#include "testing.h"


TEST_CASE(test_uncaught_exception){

    E4C_THROW(NullPointerException, "This is an uncaught exception");
}
