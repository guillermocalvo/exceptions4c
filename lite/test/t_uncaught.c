
#include "testing.h"


/**
 * Uncaught exception
 */
TEST_CASE{

    E4C_THROW(NullPointerException, "This is an uncaught exception");
}
