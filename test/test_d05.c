
# include "testing.h"


DEFINE_TEST(
	d05,
	"Uncaught exception rethrown from a catch{...} block",
	"This test <strong>starts a <code>try</code> block and then throws an exception</strong>; there is a <code>catch</code> block that handles it and then <em>rethrows</em> it.",
	NULL,
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"before_RETHROW",
	"WildException"
){

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_TRY_block\n");

	E4C_TRY{
		printf("before_THROW\n");

		E4C_THROW(WildException, "I'm going to be caught.");

		printf("after_THROW\n");

	}E4C_CATCH(WildException){

		printf("before_RETHROW\n");

		E4C_THROW(*e4c_get_exception(), "Nobody will catch me this time.");

		printf("after_RETHROW\n");
	}

	printf("after_TRY_block\n");

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	return(EXIT_SUCCESS);
}
