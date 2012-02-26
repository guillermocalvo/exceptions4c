
# include <string.h>
# include "testing.h"


static void set_null_g01(int * * pointer)
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
	g01,
	"Signal SIGSEGV",
	"This test attempts to dereference a null pointer; the library signal handling is disabled, so the behavior of the program will be implementation-defined (typically the program will terminate abruptly).",
	"This functionality relies on the <a href=\"#requirement_z04\"><strong>platform's behavior when dereferencing a null pointer</strong></a>.",
	EXIT_WHATEVER,
	"before_NULL_POINTER",
	ERROR_WHATEVER
){

	int		integer			= 123;
	int *	pointer			= &integer;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_FALSE);

	/* some smartypants compilers might need to be fooled */
	/* pointer = NULL; */

	ECHO(("before_SET_NULL\n"));

	set_null_g01(&pointer);

	ECHO(("before_NULL_POINTER\n"));

	/*@-boundsread@*/
	integer = *pointer;
	/*@=boundsread@*/

	ECHO(("after_NULL_POINTER_%d\n", integer));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
