
# include <signal.h>
# include "testing.h"


static int set_zero_z11(int dummy)
/*@*/
{

	if(dummy == 0){
		return(1);
	}

	return(0);
}

static void handler_z11(int signal_number)
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
	z11,
	"Platform ability to keep silent at signal SIGFPE",
	"This test sets a handler for signal <code>SIGFPE</code> and then checks whether the program prints something on <code>stderr</code> when it receives the signal.",
	SEVERITY_NOT_CRITICAL,
	"Your program prints something on <code>stderr</code> when it receives signal <code>SIGFPE</code>. Therefore, you will not be able to keep the error output stream clean.",
	EXIT_WHATEVER,
	OUTPUT_WHATEVER,
	NULL
){

	int		divisor			= 10;
	int		integer			= 100;
	void	(*previous_handler)(int);

	ECHO(("before_SIGNAL\n"));

	previous_handler = signal(SIGFPE, handler_z11);
	if(previous_handler == SIG_ERR){
		ECHO(("could_not_set_handler_%d\n", SIGFPE));
	}else{
		ECHO(("handler_was_set_%d\n", SIGFPE));
	}

	/* some smartypants compilers might need to be fooled */
	/* divisor = 0; */

	ECHO(("before_SET_ZERO\n"));

	divisor = set_zero_z11(divisor);

	ECHO(("before_DIVISION_BY_ZERO\n"));

	integer = integer / divisor;

	ECHO(("after_DIVISION_BY_ZERO_%d\n", integer));

	return(EXIT_SUCCESS);
}
