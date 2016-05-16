
#include "testing.h"


E4C_DEFINE_EXCEPTION(CustomException, "Custom exception.", RuntimeException);


/**
 * Caught exception
 */
 TEST_CASE{

    E4C_TRY{

        E4C_THROW(CustomException, "This is a custom exception");

    }E4C_CATCH(CustomException){

        printf("The custom exception was caught: %s\n", E4C_EXCEPTION.type->name);
    }
}
