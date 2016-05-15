
# include "testing.h"


DEFINE_TEST(
	b16,
	"e4c_print_exception(NULL)",
	"This test uses the library in an inconsistent way, by attempting to pass <code>NULL</code> to <code>e4c_print_exception</code>. The library must signal the misuse by throwing the exception <code>NullPointerException</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_PRINT_EXCEPTION",
	"NullPointerException"
){

	ECHO(("before_CONTEXT_BEGIN_block\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_PRINT_EXCEPTION\n"));

	e4c_print_exception(NULL);

	ECHO(("before_CONTEXT_END_block\n"));

	e4c_context_end();

	return(EXIT_SUCCESS);
}
