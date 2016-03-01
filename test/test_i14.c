
# include "testing.h"


static char foobar[] = "FOOBAR";

static void * custom_initialize_handler(const e4c_exception * exception){

	fprintf(stderr, "CUSTOM_INITIALIZE_HANDLER\n");

	return(foobar);
}


DEFINE_TEST(
	i14,
	"Mixing custom initialization handler and formatted message",
	"This test sets custom initialization handler and throws an exception with a formatted message.",
	NULL,
	EXIT_SUCCESS,
	"before_CONTEXT_END",
	"WildException_FOOBAR"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_FALSE);

	ECHO(("before_SET_HANDLERS\n"));

	e4c_context_set_handlers(NULL, NULL, custom_initialize_handler, NULL);

	E4C_TRY{

		ECHO(("before_THROWF\n"));

# ifdef E4C_THROWF

		E4C_THROWF(WildException, "%s_%s", "FORMATTED", "MESSAGE");

# else

		E4C_THROW(WildException, "No variadic macros support");

# endif

		ECHO(("after_THROWF\n"));

	}E4C_CATCH(RuntimeException){

		ECHO(("inside_CATCH_block\n"));

		fprintf(stderr, "%s_%s", e4c_get_exception()->name, (const char *)e4c_get_exception()->custom_data);
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
