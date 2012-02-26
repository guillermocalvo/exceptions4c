
# include "testing.h"


static int set_zero_g08(int dummy)
/*@*/
{

	if(dummy == 0){
		return(1);
	}

	return(0);
}


DEFINE_TEST(
	g08,
	"Catching a division by zero exception",
	"This test attempts to divide by zero; the library signal handling is enabled; the exception is caught (<code>catch(SignalException)</code>)and then the program exits.",
	"This functionality relies on the <a href=\"#requirement_z08\"><strong>platform's ability to handle signal <code>SIGFPE</code></strong></a>.",
	EXIT_SUCCESS,
	"after_CONTEXT_END",
	"the_signal_WAS_CAUGHT"
){

	E4C_BOOL	caught		= E4C_FALSE;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

		int		divisor			= 10;
		int		integer			= 100;

		/* some smartypants compilers might need to be fooled */
		/* divisor = 0; */

		ECHO(("before_SET_ZERO\n"));

		divisor = set_zero_g08(divisor);

		ECHO(("before_DIVISION_BY_ZERO\n"));

		integer = integer / divisor;

		ECHO(("after_DIVISION_BY_ZERO_%d\n", integer));

	}E4C_CATCH(SignalException){

		ECHO(("inside_CATCH_block\n"));

		caught = E4C_TRUE;

		ECHO(("catching__%s\n", e4c_get_exception()->name));

	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	if(caught){
		fprintf(stderr, "\nthe_signal_WAS_CAUGHT\n");
	}else{
		fprintf(stderr, "\nthe_signal_WAS_NOT_CAUGHT\n");
	}

	(void)fflush(stderr);

	return(EXIT_SUCCESS);

}
