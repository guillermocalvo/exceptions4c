
# include <signal.h>
# include "testing.h"


static void handler_z11(int signal_number){

	ECHO(("handler_%d\n", signal_number));

	ECHO(("\nhandler_executed\n"));

	exit(123);
}

DEFINE_REQUIREMENT(
	z11,
	"Platform ability to keep silent at signal SIGFPE",
	"This test sets a handler for signal <code>SIGFPE</code> and then checks whether the program prints something on <code>stderr</code> when it receives the signal.",
	SEVERITY_NOT_CRITICAL,
	"Your program prints something on <code>stderr</code> when it receives signal <code>SIGFPE</code>. Therefore, you will not be able to keep the error output stream clean.",
	EXIT_WHATEVER,
	NULL,
	NULL
){
	int zero = 0;
	int integer = 1010;

	ECHO(("before_SIGNAL\n"));

	if( signal(SIGFPE, handler_z11) == SIG_ERR ){
		ECHO(("could_not_set_handler_%d\n", SIGFPE));
	}else{
		ECHO(("handler_was_set_%d\n", SIGFPE));
	}

	ECHO(("before_DIVISION_BY_ZERO\n"));

	/* some smartypants compilers might need to be fooled */
	/* if(zero != integer) zero = 0; */

	integer = 31416 / zero;

	ECHO(("after_DIVISION_BY_ZERO_%d\n", integer));

	return(EXIT_SUCCESS);
}
