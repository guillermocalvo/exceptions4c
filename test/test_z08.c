
# include <signal.h>
# include "testing.h"


static void handler_z08(int signal_number){

	ECHO(("handler_%d\n", signal_number));

	ECHO(("\nhandler_executed\n"));

	exit(123);
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
	int zero = 0;
	int integer = 1010;
	
	ECHO(("before_SIGNAL\n"));

	if( signal(SIGFPE, handler_z08) == SIG_ERR ){
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
