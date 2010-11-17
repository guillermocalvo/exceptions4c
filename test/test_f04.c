
# include "testing.h"


DEFINE_TEST(
	f04,
	"A child exception cannot be caught by catching a sibling exception",
	"This test starts a <code>try</code> block, throws <code>ChildException</code> and attempts to catch it with a <code>catch(SiblingException)</code> block. This, obviously, won't work, so the exception will be left uncaught.",
	NULL,
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"before_THROW",
	"ChildException"
){

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_TRY_block\n");

	E4C_TRY{

		printf("before_THROW\n");

		E4C_THROW(ChildException, "You cannot catch me by catching one of my supertype's subtypes.");

	}E4C_CATCH(SiblingException){

		printf("inside_CATCH_block\n");

		printf("catching__%s\n", e4c_get_exception()->name);
	}

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	return(EXIT_SUCCESS);
}
