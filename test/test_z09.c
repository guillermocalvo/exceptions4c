
# include <signal.h>
# include "testing.h"


static void handler_z09(int signal_number){

	printf("handler_%d\n", signal_number);
	fflush(stdout);

	printf("\nhandler_executed\n");
	fflush(stdout);

	exit(123);
}

DEFINE_REQUIREMENT(
	z09,
	"Platform ability to handle signal SIGABRT",
	"This test sets a handler for signal <code>SIGABRT</code> and then calls function <code>abort</code> in order to verify that the handler is executed.",
	SEVERITY_CRITICAL,
	"The platform is unable to handle signal <code>SIGABRT</code>. Therefore, the library can't convert it to <code>AbortException</code>.",
	123,
	"handler_executed",
	ERROR_WHATEVER
){

	/*
		Note:
		We'll be using fflush because when the signal is received, all bets are
		off; the buffered output of the program goes into the bit bucket.
	*/

	printf("before_SIGNAL\n");
	fflush(stdout);

	if( signal(SIGABRT, handler_z09) == SIG_ERR ){
		printf("could_not_set_handler_%d\n", SIGABRT);
		fflush(stdout);
	}else{
		printf("handler_was_set_%d\n", SIGABRT);
		fflush(stdout);
	}


	printf("before_ABORT\n");
	fflush(stdout);

	abort();

	printf("after_ABORT\n");
	fflush(stdout);

	return(EXIT_SUCCESS);
}
