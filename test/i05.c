
# include "testing.h"


static void custom_uncaught_handler(const e4c_exception * exception){

	fprintf(stderr, "CUSTOM_UNCAUGHT_HANDLER");

	(void)fflush(stderr);
}


DEFINE_TEST(
	i05,
	"Setting a custom uncaught handler",
	"This test sets a custom <em>uncaught handler</em>. Then <em>throws</em> an exception; there is no <code>catch</code> block to handle it.",
	NULL,
	IF_NOT_THREADSAFE(EXIT_FAILURE),
	"before_THROW",
	"CUSTOM_UNCAUGHT_HANDLER"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_SET_HANDLERS\n"));

	e4c_context_set_handlers(custom_uncaught_handler, NULL, NULL, NULL);

	ECHO(("before_THROW\n"));

	E4C_THROW(WildException, "You can't stop me now!");

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	return(EXIT_SUCCESS);
}
