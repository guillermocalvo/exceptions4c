
# include "testing.h"


DEFINE_TEST(
	g05,
	"Division by zero exception",
	"This test attempts to divide by zero; the library signal handling is enabled; there is no <code>catch</code> block. The behavior of the program will no longer be undefined: it will terminate because of the uncaught exception <code>ArithmeticException</code>.",
	"This functionality relies on the <a href=\"#requirement_z08\"><strong>platform's ability to handle signal <code>SIGFPE</code></strong></a>.",
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"before_DIVISION_BY_ZERO",
	"ArithmeticException"
){

	int zero = 0;
	int integer = 100;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(e4c_true, e4c_print_exception);

	ECHO(("before_DIVISION_BY_ZERO\n"));

	/* some smartypants compilers might need to be fooled */
	/* if(zero != integer) zero = 0; */

	integer = integer / zero;

	ECHO(("after_DIVISION_BY_ZERO_%d\n", integer));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
