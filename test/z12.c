
# include <signal.h>
# include "testing.h"


static void handler_z12(int signal_number)
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
	z12,
	"Platform ability to keep silent at signal SIGABRT",
	"This test sets a handler for signal <code>SIGABRT</code> and then checks whether the program prints something on <code>stderr</code> when it receives the signal.",
	SEVERITY_NOT_CRITICAL,
	"Your program prints something on <code>stderr</code> when it receives signal <code>SIGABRT</code>. Therefore, you will not be able to keep the error output stream clean.",
	EXIT_WHATEVER,
	OUTPUT_WHATEVER,
	NULL
){

	void (*previous_handler)(int);

	ECHO(("before_SIGNAL\n"));

	previous_handler = signal(SIGABRT, handler_z12);
	if(previous_handler == SIG_ERR){
		ECHO(("could_not_set_handler_%d\n", SIGABRT));
	}else{
		ECHO(("handler_was_set_%d\n", SIGABRT));
	}

	ECHO(("before_ABORT\n"));

	abort();

	/*@-unreachable@*/

	ECHO(("after_ABORT\n"));

	return(EXIT_SUCCESS);

	/*@=unreachable@*/
}
