
# include "testing.h"


static int set_zero_g02(int dummy)
/*@*/
{

	if(dummy == 0){
		return(1);
	}

	return(0);
}


DEFINE_TEST(
	g02,
	"Signal SIGFPE",
	"This test attempts to divide by zero; the library signal handling is disabled, so the behavior of the program will be implementation-defined (typically the program will terminate abruptly).",
	"This functionality relies on the <a href=\"#requirement_z05\"><strong>platform's behavior when dividing by zero</strong></a>.",
	EXIT_WHATEVER,
	"before_DIVISION_BY_ZERO",
	ERROR_WHATEVER
){

	int		divisor			= 10;
	int		integer			= 100;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_FALSE);

	/* some smartypants compilers might need to be fooled */
	/* divisor = 0; */

	ECHO(("before_SET_ZERO\n"));

	divisor = set_zero_g02(divisor);

	ECHO(("before_DIVISION_BY_ZERO\n"));

	integer = integer / divisor;

	ECHO(("after_DIVISION_BY_ZERO_%d\n", integer));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
