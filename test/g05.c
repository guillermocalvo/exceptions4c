
# include "testing.h"


static int set_zero_g05(int dummy)
/*@*/
{

	if(dummy == 0){
		return(1);
	}

	return(0);
}


DEFINE_TEST(
	g05,
	"Division by zero exception",
	"This test attempts to divide by zero; the library signal handling is enabled; there is no <code>catch</code> block. The behavior of the program will no longer be undefined: it will terminate because of the uncaught exception <code>ArithmeticException</code>.",
	"This functionality relies on the <a href=\"#requirement_z08\"><strong>platform's ability to handle signal <code>SIGFPE</code></strong></a>.",
	IF_NOT_THREADSAFE(EXIT_FAILURE),
	"before_DIVISION_BY_ZERO",
	"ArithmeticException"
){

	int		divisor			= 10;
	int		integer			= 100;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	/* some smartypants compilers might need to be fooled */
	/* divisor = 0; */

	ECHO(("before_SET_ZERO\n"));

	divisor = set_zero_g05(divisor);

	ECHO(("before_DIVISION_BY_ZERO\n"));

	integer = integer / divisor;

	ECHO(("after_DIVISION_BY_ZERO_%d\n", integer));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
