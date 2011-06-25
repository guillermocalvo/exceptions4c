
# include <signal.h>
# include "testing.h"


static int set_zero_z08(int dummy)
/*@*/
{

	if(dummy == 0){
		return(1);
	}

	return(0);
}

static void handler_z08(int signal_number)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	ECHO(("handler_%d\n", signal_number));

	ECHO(("\nhandler_executed\n"));

	/*@-exitarg@*/
	exit(123);
	/*@=exitarg@*/
}


DEFINE_REQUIREMENT(
	z08,
	"Platform ability to handle signal SIGFPE",
	"This test sets a handler for signal <code>SIGFPE</code> and then attempts to divide an integer by zero in order to verify that the handler is executed.",
	SEVERITY_CRITICAL,
	"The platform is unable to handle signal <code>SIGFPE</code>. Therefore, the library can't convert it to <code>ArithmeticException</code>.",
	123,
	"handler_executed",
	ERROR_WHATEVER
){

	int		divisor			= 10;
	int		integer			= 100;
	void	(*previous_handler)(int);

	ECHO(("before_SIGNAL\n"));

	previous_handler = signal(SIGFPE, handler_z08);
	if(previous_handler == SIG_ERR){
		ECHO(("could_not_set_handler_%d\n", SIGFPE));
	}else{
		ECHO(("handler_was_set_%d\n", SIGFPE));
	}

	/* some smartypants compilers might need to be fooled */
	/* divisor = 0; */

	ECHO(("before_SET_ZERO\n"));

	divisor = set_zero_z08(divisor);

	ECHO(("before_DIVISION_BY_ZERO\n"));

	integer = integer / divisor;

	ECHO(("after_DIVISION_BY_ZERO_%d\n", integer));

	return(EXIT_SUCCESS);
}
