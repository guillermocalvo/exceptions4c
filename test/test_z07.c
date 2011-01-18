
# include <signal.h>
# include "testing.h"


static void handler_z07(int signal_number){

	ECHO(("handler_%d\n", signal_number));

	ECHO(("\nhandler_executed\n"));

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

	ECHO(("before_SIGNAL\n"));

	if( signal(SIGSEGV, handler_z07) == SIG_ERR ){
		ECHO(("could_not_set_handler_%d\n", SIGSEGV));
	}else{
		ECHO(("handler_was_set_%d\n", SIGSEGV));
	}

	ECHO(("before_NULL_POINTER\n"));

	/* some smartypants compilers might need to be fooled */
	/* if(null_pointer != &integer) null_pointer = NULL; */

	integer = *null_pointer;

	ECHO(("after_NULL_POINTER_%d\n", integer));

	return(EXIT_SUCCESS);
}
