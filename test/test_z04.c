
# include "testing.h"


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
	int * null_pointer = NULL;
	int integer;

	ECHO(("before_NULL_POINTER\n"));

	/* some smartypants compilers might need to be fooled */
	/* if(null_pointer != &integer) null_pointer = NULL; */

	integer = *null_pointer;

	ECHO(("after_NULL_POINTER_%d\n", integer));

	return(EXIT_SUCCESS);
}
