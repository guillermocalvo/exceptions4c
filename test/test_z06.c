
# include "testing.h"


DEFINE_REQUIREMENT(
	z06,
	"Platform behavior when aborting program",
	"This test calls function <code>abort</code> in order to verify that the program terminates.",
	SEVERITY_CRITICAL,
	"The platform does not stop your program when <code>abort</code> is called. Therefore, the library can't detect program abortion.",
	EXIT_WHATEVER,
	"before_ABORT",
	ERROR_WHATEVER
){

	ECHO(("before_ABORT\n"));

	abort();

	/*@-unreachable@*/

	ECHO(("after_ABORT\n"));

	return(EXIT_SUCCESS);

	/*@=unreachable@*/
}
