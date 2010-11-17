
# include "testing.h"

# ifdef _POSIX_SOURCE
#	define DEFINE_POSIX_SOURCE
# else
#	define DEFINE_POSIX_SOURCE \
		"<div class=\"paragraph\"><strong>" \
			"If your platform provides the macro <code>WEXITSTATUS</code>," \
			" please define <code>_POSIX_SOURCE</code> and compile again." \
		"</strong></div>"
# endif

DEFINE_REQUIREMENT(
	z01,
	"Platform behavior when returning EXIT_SUCCESS",
	"This test simply yields <code>EXIT_SUCCESS</code> as exit code.",
	SEVERITY_CRITICAL,
	"The test framework is unable to detect when a test returns <code>EXIT_SUCCESS</code> exit code." DEFINE_POSIX_SOURCE,
	EXIT_SUCCESS,
	NULL,
	NULL
){

	return(EXIT_SUCCESS);
}
