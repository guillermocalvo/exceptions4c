
# include "testing.h"


E4C_DEFINE_EXCEPTION(CustomException, "This is a custom exception", RuntimeException);


/**
 * Finding the cause of the cause
 *
 * This test *throws* an exception from two nested `catch` blocks. The
 * outtermost `catch` block inspects the *cause of the cause*.
 *
 */
TEST_CASE{

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        E4C_TRY{

            E4C_TRY{

                E4C_THROW(CustomException, "This is the original cause of the issue");

            }E4C_CATCH(CustomException){

                e4c_exception_throw_verbatim_(&IllegalArgumentException, __FILE__, __LINE__, NULL, "First wrapper");
            }

        }E4C_CATCH(IllegalArgumentException){

            e4c_exception_throw_verbatim_(&RuntimeException, __FILE__, __LINE__, "test_function", "Second wrapper");
        }

    }E4C_CATCH(RuntimeException){

        const e4c_exception * exception = e4c_get_exception();

        TEST_ASSERT_EQUALS(exception->cause->type, &IllegalArgumentException);
        TEST_ASSERT_EQUALS(exception->cause->cause->type, &CustomException);

        e4c_print_exception(exception);
    }

    e4c_context_end();
}
