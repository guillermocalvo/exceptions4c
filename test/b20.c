
# include "testing.h"
# include <signal.h>


static e4c_signal_mapping custom_mappings[2] = {

    E4C_SIGNAL_MAPPING(1024, RuntimeException),
    E4C_NULL_SIGNAL_MAPPING
};


/**
 * Mapping an unknown signal
 *
 * This test tries to create an invalid mapping for an unknown signal.
 *
 * The library must signal the misuse by throwing the exception
 * `ExceptionSystemFatalError`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ExceptionSystemFatalError);

    e4c_context_begin(E4C_FALSE);

    e4c_context_set_signal_mappings(custom_mappings);

    E4C_TRY{

        raise(1024);

    }E4C_CATCH(RuntimeException){

        THIS_SHOULD_NOT_HAPPEN;
    }

    e4c_context_end();
}
