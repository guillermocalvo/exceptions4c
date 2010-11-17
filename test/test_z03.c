
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
	z03,
	"Platform behavior when returning 123",
	"This test simply yields <code>123</code> as exit code.",
	SEVERITY_CRITICAL,
	"The test framework is unable to detect when a test returns <code>123</code> exit code." DEFINE_POSIX_SOURCE,
	123,
	NULL,
	NULL
){

	return(123);
}
