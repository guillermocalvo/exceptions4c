
# include <signal.h>
# include "testing.h"


static void handler_z10(int signal_number){

	printf("handler_%d\n", signal_number);
	fflush(stdout);

	printf("\nhandler_executed\n");
	fflush(stdout);

	exit(123);
}

DEFINE_REQUIREMENT(
	z10,
	"Platform ability to keep silent at signal SIGSEGV",
	"This test sets a handler for signal <code>SIGSEGV</code> and then checks whether the program prints something on <code>stderr</code> when it receives the signal.",
	SEVERITY_NOT_CRITICAL,
	"Your program prints something on <code>stderr</code> when it receives signal <code>SIGSEGV</code>. Therefore, you will not be able to keep the error output stream clean.",
	EXIT_WHATEVER,
	NULL,
	NULL
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

	if( signal(SIGSEGV, handler_z10) == SIG_ERR ){
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
