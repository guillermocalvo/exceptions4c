
# include "testing.h"


DEFINE_TEST(
	g04,
	"Division by zero exception",
	"This test attempts to divide by zero; the library signal handling is enabled; there is no <code>catch</code> block. The behavior of the program will no longer be undefined: it will terminate because of the an uncaught exception.",
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"before_DIVISION_BY_ZERO",
	"ArithmeticException"
){

	int zero = 0;
	int integer = 100;

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_DIVISION_BY_ZERO\n");
	fflush(stdout);

	/* some smartypants compilers might need to be fooled */
	/* if(zero != integer) zero = 0; */

	integer = integer / zero;

	printf("after_DIVISION_BY_ZERO\n");
	fflush(stdout);

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	return(EXIT_SUCCESS);
}
