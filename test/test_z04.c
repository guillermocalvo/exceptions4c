
# include <string.h>
# include "testing.h"


static void set_null_z04(int * * pointer)
/*@modifies
	pointer
@*/
{

	int * null_pointer = NULL;

	/*@-boundsread@*/
	memcpy(pointer, &null_pointer, sizeof(pointer) );
	/*@=boundsread@*/
}


DEFINE_REQUIREMENT(
	z04,
	"Platform behavior when dereferencing a null pointer",
	"This test attempts to dereference a null pointer in order to verify that the program terminates.",
	SEVERITY_CRITICAL,
	"The platform does not stop your program when dereferencing a null pointer. Therefore, the library can't detect segmentation faults.",
	EXIT_WHATEVER,
	"before_NULL_POINTER",
	ERROR_WHATEVER
){

	int		integer			= 123;
	int *	pointer			= &integer;

	/* some smartypants compilers might need to be fooled */
	/* pointer = NULL; */

	ECHO(("before_SET_NULL\n"));

	set_null_z04(&pointer);

	ECHO(("before_NULL_POINTER\n"));

	/*@-boundsread@*/
	integer = *pointer;
	/*@=boundsread@*/

	ECHO(("after_NULL_POINTER_%d\n", integer));

	return(EXIT_SUCCESS);
}
