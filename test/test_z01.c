
# include "testing.h"


DEFINE_REQUIREMENT(
	z01,
	"Platform behavior when returning EXIT_SUCCESS",
	"This test simply yields <code>EXIT_SUCCESS</code> as exit code.",
	SEVERITY_CRITICAL,
	"The test framework is unable to detect when a test returns <code>EXIT_SUCCESS</code> exit code.",
	EXIT_SUCCESS,
	OUTPUT_WHATEVER,
	ERROR_WHATEVER
){

	ECHO(("before_return_EXIT_SUCCESS\n"));

	return(EXIT_SUCCESS);
}
