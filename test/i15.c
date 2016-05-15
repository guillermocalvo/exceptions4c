
# include "testing.h"


static E4C_DEFINE_EXCEPTION(CustomException, "This is a custom exception", RuntimeException);


/**
 * Finding the cause of an exception
 *
 * This test *throws* an exception from an inner `catch` block. An outter
 * `catch` block inspects the *cause*.
 *
 */
TEST_CASE{

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        E4C_TRY{

            E4C_THROW(CustomException, "This is the original cause of the issue");

        }E4C_CATCH(RuntimeException){

            E4C_THROW(RuntimeException, "This is the wrapper exception");
        }

    }E4C_CATCH(RuntimeException){

        TEST_ASSERT_EQUALS(e4c_get_exception()->cause->type, &CustomException);
    }

    e4c_context_end();
}
