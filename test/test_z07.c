
# include <signal.h>
# include "testing.h"


static void handler_z07(int signal_number){

	printf("handler_%d\n", signal_number);
	fflush(stdout);

	printf("\nhandler_executed\n");
	fflush(stdout);

	exit(123);
}

DEFINE_REQUIREMENT(
	z07,
	"Platform ability to handle signal SIGSEGV",
	"This test sets a handler for signal <code>SIGSEGV</code> and then attempts to dereference a null pointer in order to verify that the handler is executed.",
	SEVERITY_CRITICAL,
	"The platform is unable to handle signal <code>SIGSEGV</code>. Therefore, the library can't convert it to <code>BadPointerException</code>.",
	123,
	"handler_executed",
	ERROR_WHATEVER
){
	int * null_pointer = NULL;
	int integer;

	/*
		Note:
		We'll be using fflush because when the signal is received, all bets are
		off; the buffered output of the program goes into the bit bucket.
	*/

	printf("before_SIGNAL\n");
	fflush(stdout);

	if( signal(SIGSEGV, handler_z07) == SIG_ERR ){
		printf("could_not_set_handler_%d\n", SIGSEGV);
		fflush(stdout);
	}else{
		printf("handler_was_set_%d\n", SIGSEGV);
		fflush(stdout);
	}


	printf("before_NULL_POINTER\n");
	fflush(stdout);

	/* some smartypants compilers might need to be fooled */
	/* if(null_pointer != &integer) null_pointer = NULL; */

	integer = *null_pointer;

	printf("after_NULL_POINTER_%d\n", integer);
	fflush(stdout);

	return(EXIT_SUCCESS);
}
