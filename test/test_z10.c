
# include <signal.h>
# include <string.h>
# include "testing.h"


static void set_null_z10(int * * pointer){

	int * null_pointer = NULL;

	memcpy(pointer, &null_pointer, sizeof(pointer) );
}

static void handler_z10(int signal_number){

	ECHO(("handler_%d\n", signal_number));

	ECHO(("\nhandler_executed\n"));

	exit(123);
}


DEFINE_REQUIREMENT(
	z10,
	"Platform ability to keep silent at signal SIGSEGV",
	"This test sets a handler for signal <code>SIGSEGV</code> and then checks whether the program prints something on <code>stderr</code> when it receives the signal.",
	SEVERITY_NOT_CRITICAL,
	"Your program prints something on <code>stderr</code> when it receives signal <code>SIGSEGV</code>. Therefore, you will not be able to keep the error output stream clean.",
	EXIT_WHATEVER,
	OUTPUT_WHATEVER,
	NULL
){

	int		integer			= 123;
	int *	pointer			= &integer;

	ECHO(("before_SIGNAL\n"));

	if( signal(SIGSEGV, handler_z10) == SIG_ERR ){
		ECHO(("could_not_set_handler_%d\n", SIGSEGV));
	}else{
		ECHO(("handler_was_set_%d\n", SIGSEGV));
	}

	/* some smartypants compilers might need to be fooled */
	/* pointer = NULL; */

	ECHO(("before_SET_NULL\n"));

	set_null_z10(&pointer);

	ECHO(("before_NULL_POINTER\n"));

	integer = *pointer;

	ECHO(("after_NULL_POINTER_%d\n", integer));

	return(EXIT_SUCCESS);
}
