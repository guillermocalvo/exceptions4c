
# include <signal.h>
# include <string.h>
# include "testing.h"


static void set_null_z07(int * * pointer)
/*@modifies
	pointer
@*/
{

	int * null_pointer = NULL;

	/*@-boundsread@*/
	memcpy(pointer, &null_pointer, sizeof(pointer) );
	/*@=boundsread@*/
}

static void handler_z07(int signal_number)
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
	z07,
	"Platform ability to handle signal SIGSEGV",
	"This test sets a handler for signal <code>SIGSEGV</code> and then attempts to dereference a null pointer in order to verify that the handler is executed.",
	SEVERITY_CRITICAL,
	"The platform is unable to handle signal <code>SIGSEGV</code>. Therefore, the library can't convert it to <code>BadPointerException</code>.",
	123,
	"handler_executed",
	ERROR_WHATEVER
){

	int		integer			= 123;
	int *	pointer			= &integer;
	void	(*previous_handler)(int);

	ECHO(("before_SIGNAL\n"));

	previous_handler = signal(SIGSEGV, handler_z07);
	if(previous_handler == SIG_ERR){
		ECHO(("could_not_set_handler_%d\n", SIGSEGV));
	}else{
		ECHO(("handler_was_set_%d\n", SIGSEGV));
	}

	/* some smartypants compilers might need to be fooled */
	/* pointer = NULL; */

	ECHO(("before_SET_NULL\n"));

	set_null_z07(&pointer);

	ECHO(("before_NULL_POINTER\n"));

	/*@-boundsread@*/
	integer = *pointer;
	/*@=boundsread@*/

	ECHO(("after_NULL_POINTER_%d\n", integer));

	return(EXIT_SUCCESS);
}
