
# include "testing.h"


DEFINE_TEST(
	g03,
	"Signal SIGABR",
	"This test calls <code>abort</code>; the library signal handling is disabled, so the behavior of the program will be implementation-defined (typically the program will terminate abruptly).",
	"This functionality relies on the <a href=\"#requirement_z06\"><strong>platform's behavior when aborting program</strong></a>.",
	EXIT_WHATEVER,
	"before_ABORT",
	ERROR_WHATEVER
){

	/*
		Note:
		We'll be using fflush because when the signal is received, all bets are
		off; the buffered output of the program goes into the bit bucket.
	*/

	printf("before_CONTEXT_BEGIN\n");
	fflush(stdout);

	e4c_context_begin(e4c_false, e4c_print_exception);

	printf("before_ABORT\n");
	fflush(stdout);

	abort();

	printf("after_ABORT\n");
	fflush(stdout);

	printf("before_CONTEXT_END\n");
	fflush(stdout);

	e4c_context_end();

	printf("after_CONTEXT_END\n");
	fflush(stdout);

	return(EXIT_SUCCESS);
}
