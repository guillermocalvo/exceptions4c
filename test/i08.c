
# include "testing.h"


static char foobar[] = "FOOBAR";

static void custom_finalize_handler(void * custom_data){

	fprintf(stderr, "CUSTOM_FINALIZATION_HANDLER_%s", (const char *)custom_data);

	(void)fflush(stderr);
}


DEFINE_TEST(
	i08,
	"Setting a custom finalization handler",
	"This test sets a custom <em>finalization handler</em>. Then <em>throws</em> an exception and <em>catches</em> it.",
	NULL,
	EXIT_SUCCESS,
	"before_CONTEXT_END",
	"CUSTOM_FINALIZATION_HANDLER_FOOBAR"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_SET_HANDLERS\n"));

	e4c_context_set_handlers(NULL, foobar, NULL, custom_finalize_handler);

	ECHO(("before_TRY\n"));

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW(WildException, "Finalize my custom data");

	}E4C_CATCH(WildException){

		ECHO(("inside_CATCH_block\n"));
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	return(EXIT_SUCCESS);
}
