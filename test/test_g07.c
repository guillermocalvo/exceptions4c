
# include <string.h>
# include "testing.h"


static void set_null_g07(int * * pointer)
/*@modifies
	pointer
@*/
{

	int * null_pointer = NULL;

	/*@-boundsread@*/
	memcpy(pointer, &null_pointer, sizeof(pointer) );
	/*@=boundsread@*/
}


DEFINE_TEST(
	g07,
	"Catching a null pointer exception",
	"This test attempts to dereference a null pointer; the library signal handling is enabled; the exception is caught (<code>catch(BadPointerException)</code>) and then the program exits.",
	"This functionality relies on the <a href=\"#requirement_z07\"><strong>platform's ability to handle signal <code>SIGSEGV</code></strong></a>.",
	EXIT_SUCCESS,
	"after_CONTEXT_END",
	"the_signal_WAS_CAUGHT"
){

	E4C_BOOL	caught		= E4C_FALSE;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

		int		integer			= 123;
		int *	pointer			= &integer;

		/* some smartypants compilers might need to be fooled */
		/* pointer = NULL; */

		ECHO(("before_SET_NULL\n"));

		set_null_g07(&pointer);

		ECHO(("before_NULL_POINTER\n"));

		/*@-boundsread@*/
		integer = *pointer;
		/*@=boundsread@*/

		ECHO(("after_NULL_POINTER_%d\n", integer));

	}E4C_CATCH(BadPointerException){

		ECHO(("inside_CATCH_block\n"));

		caught = E4C_TRUE;

		ECHO(("catching__%s\n", e4c_get_exception()->name));

	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	if(caught){
		fprintf(stderr, "\nthe_signal_WAS_CAUGHT\n");
	}else{
		fprintf(stderr, "\nthe_signal_WAS_NOT_CAUGHT\n");
	}

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
