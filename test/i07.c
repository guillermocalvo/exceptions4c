
# include "testing.h"


static char foobar[] = "FOOBAR";

static void * custom_initialize_handler(const e4c_exception * exception){

	fprintf(stderr, "CUSTOM_INITIALIZE_HANDLER");

	(void)fflush(stderr);

	return(&foobar);
}


DEFINE_TEST(
	i07,
	"Setting a custom initialization handler",
	"This test sets a custom <em>initialization handler</em>. Then <em>throws</em> an exception; the <code>catch</code> block inspects the exception's <em>custom data</em>.",
	NULL,
	EXIT_SUCCESS,
	"INITIALIZATION_FOOBAR",
	"CUSTOM_INITIALIZE_HANDLER"
){
	volatile const char * initialization = NULL;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_SET_HANDLERS\n"));

	e4c_context_set_handlers(NULL, NULL, custom_initialize_handler, NULL);

	ECHO(("before_TRY\n"));

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW(WildException, "Initialize my custom data with the result of a function");

	}E4C_CATCH(WildException){

		ECHO(("inside_CATCH_block\n"));

		initialization = e4c_get_exception()->custom_data;

		ECHO(("CUSTOM_DATA_%s\n", initialization));
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("INITIALIZATION_%s\n", initialization));

	return(EXIT_SUCCESS);
}
