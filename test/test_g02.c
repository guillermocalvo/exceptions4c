
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

	/*
		Note:
		We'll be using fflush because when the signal is received, all bets are
		off; the buffered output of the program goes into the bit bucket.
	*/

	printf("before_CONTEXT_BEGIN\n");
	fflush(stdout);

	e4c_context_begin(e4c_false, e4c_print_exception);

	printf("before_DIVISION_BY_ZERO\n");
	fflush(stdout);

	/* some smartypants compilers might need to be fooled */
	/* if(zero != integer) zero = 0; */

	integer = 31416 / zero;

	printf("after_DIVISION_BY_ZERO_%d\n", integer);
	fflush(stdout);

	printf("before_CONTEXT_END\n");
	fflush(stdout);

	e4c_context_end();

	printf("after_CONTEXT_END\n");
	fflush(stdout);

	return(EXIT_SUCCESS);
}
