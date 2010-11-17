
# include <signal.h>
# include "testing.h"


static void handler_z12(int signal_number){

	printf("handler_%d\n", signal_number);
	fflush(stdout);

	printf("\nhandler_executed\n");
	fflush(stdout);

	exit(123);
}

DEFINE_REQUIREMENT(
	z12,
	"Platform ability to keep silent at signal SIGABRT",
	"This test sets a handler for signal <code>SIGABRT</code> and then checks whether the program prints something on <code>stderr</code> when it receives the signal.",
	SEVERITY_NOT_CRITICAL,
	"Your program prints something on <code>stderr</code> when it receives signal <code>SIGABRT</code>. Therefore, you will not be able to keep the error output stream clean.",
	EXIT_WHATEVER,
	NULL,
	NULL
){

	/*
		Note:
		We'll be using fflush because when the signal is received, all bets are
		off; the buffered output of the program goes into the bit bucket.
	*/

	printf("before_SIGNAL\n");
	fflush(stdout);

	if( signal(SIGABRT, handler_z12) == SIG_ERR ){
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
