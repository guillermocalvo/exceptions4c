
# include "testing.h"


DEFINE_TEST(
	g01,
	"Signal SIGSEGV",
	"This test attempts to dereference a null pointer; the library signal handling is disabled, so the behavior of the program will be implementation-defined (typically the program will terminate abruptly).",
	"This functionality relies on the <a href=\"#requirement_z04\"><strong>platform's behavior when dereferencing a null pointer</strong></a>.",
	EXIT_WHATEVER,
	"before_NULL_POINTER",
	ERROR_WHATEVER
){
	int * null_pointer = NULL;
	int integer;

	/*
		Note:
		We'll be using fflush because when the signal is received, all bets are
		off; the buffered output of the program goes into the bit bucket.
	*/

	printf("before_CONTEXT_BEGIN\n");
	fflush(stdout);

	e4c_context_begin(e4c_false, e4c_print_exception);

	printf("before_NULL_POINTER\n");
	fflush(stdout);

	/* some smartypants compilers might need to be fooled */
	/* if(null_pointer != &integer) null_pointer = NULL; */

	integer = *null_pointer;

	printf("after_NULL_POINTER_%d\n", integer);
	fflush(stdout);

	printf("before_CONTEXT_END\n");
	fflush(stdout);

	e4c_context_end();

	printf("after_CONTEXT_END\n");
	fflush(stdout);

	return(EXIT_SUCCESS);
}
