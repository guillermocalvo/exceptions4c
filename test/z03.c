
# include "testing.h"


DEFINE_REQUIREMENT(
	z03,
	"Platform behavior when returning 123",
	"This test simply yields <code>123</code> as exit code.",
	SEVERITY_CRITICAL,
	"The test framework is unable to detect when a test returns <code>123</code> exit code.",
	123,
	OUTPUT_WHATEVER,
	ERROR_WHATEVER
){

	ECHO(("before_return_123\n"));

	return(123);
}
