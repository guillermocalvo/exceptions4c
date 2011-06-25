
# include "testing.h"


DEFINE_REQUIREMENT(
	z02,
	"Platform behavior when returning EXIT_FAILURE",
	"This test simply yields <code>EXIT_FAILURE</code> as exit code.",
	SEVERITY_CRITICAL,
	"The test framework is unable to detect when a test returns <code>EXIT_FAILURE</code> exit code.",
	EXIT_FAILURE,
	OUTPUT_WHATEVER,
	ERROR_WHATEVER
){

	ECHO(("before_return_EXIT_FAILURE\n"));

	return(EXIT_FAILURE);
}
