
#include "testing.h"


E4C_DEFINE_EXCEPTION(ColorException, "Color exception.", RuntimeException);
E4C_DEFINE_EXCEPTION(RedException, "Red exception.", ColorException);


/**
 * Exception hierarchy
 */
TEST_CASE{

    E4C_TRY{

        E4C_THROW(RedException, "This is a red exception");

        TEST_FAIL; /* this should not happen */

    }E4C_CATCH(ColorException){

        printf("The color exception was caught: %s\n", E4C_EXCEPTION.type->name);

        TEST_ASSERT( E4C_IS_INSTANCE_OF(RedException) );
        TEST_ASSERT( E4C_IS_INSTANCE_OF(RuntimeException) );
    }
}
