
# include "testing.h"
# include <signal.h>


static e4c_signal_mapping custom_mappings[] = {

	E4C_SIGNAL_MAPPING(1024, WildException),
	E4C_NULL_SIGNAL_MAPPING
};


DEFINE_TEST(
	b20,
	"Mapping an unknown signal",
	"This test tries to create an invalid mapping for an unknown signal. The library must signal the misuse by throwing the exception <code>ExceptionSystemFatalError</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_SET_SIGNAL_MAPPINGS",
	"ExceptionSystemFatalError"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_FALSE);

	ECHO(("before_SET_SIGNAL_MAPPINGS\n"));

	e4c_context_set_signal_mappings(custom_mappings);

	E4C_TRY{

		ECHO(("before_raise_123\n"));

		raise(123);

		ECHO(("after_raise_123\n"));

	}E4C_CATCH(RuntimeException){

		ECHO(("inside_CATCH_block\n"));

		fprintf(stderr, e4c_get_exception()->name);
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
