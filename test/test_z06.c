
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

	/*
		Note:
		We'll be using fflush because when the signal is received, all bets are
		off; the buffered output of the program goes into the bit bucket.
	*/

	printf("before_ABORT\n");
	fflush(stdout);

	abort();

	printf("after_ABORT\n");
	fflush(stdout);

	return(EXIT_SUCCESS);
}
