
# include "testing.h"


DEFINE_TEST(
	d01,
	"Uncaught exception",
	"This test <strong>throws an exception</strong>; there is no <code>catch</code> block to handle it.",
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"before_THROW",
	"WildException"
){

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_THROW\n");

	E4C_THROW(WildException, "Nobody will catch me.");

	printf("after_THROW\n");

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	printf("before_EXIT_main\n");

	return(123);
}
