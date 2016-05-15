
# include "testing.h"


void aux1(void);
void aux2(void);
void aux3(void);
void aux4(void);
void aux5(void);


/**
 * Catching an exception thrown deep down the call stack
 *
 * This test starts a `try` block, and calls a function. That function calls
 * another one, and so on. Eventually, one of those functions will throw an
 * exception that will be caught by a `catch(RuntimeException)` right next to
 * the `try` block.",
 *
 */
TEST_CASE{

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        aux1();

    }E4C_CATCH(RuntimeException){

        caught = E4C_TRUE;

        TEST_ASSERT_EQUALS(e4c_get_exception()->type, &RuntimeException);
    }

    e4c_context_end();

    TEST_ASSERT(caught);
}


void aux1(void){

    aux2();
}

void aux2(void){

    E4C_TRY{

        aux3();

    }E4C_FINALLY{

        /* The exception has not been caught yet */
        TEST_ASSERT_EQUALS(e4c_get_status(), e4c_failed);
    }
}

void aux3(void){

    aux4();
}

void aux4(void){

    E4C_TRY{

        aux5();

    }E4C_CATCH(ProgramSignalException){

        TEST_FAIL("Block `catch(ProgramSignalException)` cannot handle a RuntimeException");
    }
}

void aux5(void){

    E4C_THROW(RuntimeException, "I'm going to be caught.");

    TEST_FAIL("RuntimeException should have been thrown");
}
