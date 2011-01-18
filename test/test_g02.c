
# include "testing.h"


DEFINE_TEST(
	g02,
	"Signal SIGFPE",
	"This test attempts to divide by zero; the library signal handling is disabled, so the behavior of the program will be implementation-defined (typically the program will terminate abruptly).",
	"This functionality relies on the <a href=\"#requirement_z05\"><strong>platform's behavior when dividing by zero</strong></a>.",
	EXIT_WHATEVER,
	"before_DIVISION_BY_ZERO",
	ERROR_WHATEVER
){
	int zero = 0;
	int integer = 1010;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(e4c_false, e4c_print_exception);

	ECHO(("before_DIVISION_BY_ZERO\n"));

	/* some smartypants compilers might need to be fooled */
	/* if(zero != integer) zero = 0; */

	integer = 31416 / zero;

	ECHO(("after_DIVISION_BY_ZERO_%d\n", integer));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
