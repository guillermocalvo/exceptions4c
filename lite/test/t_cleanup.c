
#include "testing.h"


/**
 * Cleanup
 */
TEST_CASE{

    volatile int created     = 0;
    volatile int destroyed   = 0;
    volatile int started     = 0;
    volatile int finished    = 0;

    E4C_TRY{

        created = 1;

        E4C_TRY{

            started = 1;

            E4C_THROW(NullPointerException, "Get me out of here");

            finished = 1; /* this should not happen */

        }E4C_FINALLY{

            destroyed = 1;
        }

    }E4C_CATCH(NullPointerException){

        printf("No problem :-)");
    }

    TEST_ASSERT(created);
    TEST_ASSERT(started);
    TEST_ASSERT(!finished);
    TEST_ASSERT(destroyed);
}
