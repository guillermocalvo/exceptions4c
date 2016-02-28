
# include "testing.h"
# include <signal.h>


static e4c_signal_mapping custom_mappings[] = {

	E4C_IGNORE_SIGNAL(SIGTERM),
	E4C_SIGNAL_MAPPING(SIGINT, WildException),
	E4C_NULL_SIGNAL_MAPPING
};


DEFINE_TEST(
	i12,
	"Setting custom signal mappings",
	"This test sets custom signal mappings. Then a couple of <em>signals</em> are raised. The first one is ignored; the second one is converted into a user-defined exception.",
	NULL,
	EXIT_SUCCESS,
	"before_CONTEXT_END",
	"WildException"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_FALSE);

	ECHO(("before_SET_SIGNAL_MAPPINGS\n"));

	e4c_context_set_signal_mappings(custom_mappings);

	E4C_TRY{

		ECHO(("before_raise_SIGTERM\n"));

		raise(SIGTERM);

		ECHO(("before_raise_SIGINT\n"));

		raise(SIGINT);

		ECHO(("after_raise_SIGINT\n"));

	}E4C_CATCH(RuntimeException){

		ECHO(("inside_CATCH_block\n"));

		fprintf(stderr, e4c_get_exception()->name);
	}

	ECHO(("before_GET_SIGNAL_MAPPINGS\n"));

	if( e4c_context_get_signal_mappings() != custom_mappings ){

		fprintf(stderr, "UNEXPECTED_SIGNAL_MAPPINGS");
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
