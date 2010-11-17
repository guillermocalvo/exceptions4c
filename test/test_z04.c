
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

	/*
		Note:
		We'll be using fflush because when the signal is received, all bets are
		off; the buffered output of the program goes into the bit bucket.
	*/

	printf("before_NULL_POINTER\n");
	fflush(stdout);

	/* some smartypants compilers might need to be fooled */
	/* if(null_pointer != &integer) null_pointer = NULL; */

	integer = *null_pointer;

	printf("after_NULL_POINTER_%d\n", integer);
	fflush(stdout);

	return(EXIT_SUCCESS);
}
