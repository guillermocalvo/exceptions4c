
# include "testing.h"


DEFINE_TEST(
	a12,
	"retry() without beginning",
	"This test uses the library improperly, by attempting to <strong>call <code>retry()</code></strong>, without having called <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_RETRY",
	"ContextHasNotBegunYet"
){

	ECHO(("before_RETRY\n"));

	retry(10);

	ECHO(("after_RETRY\n"));

	return(EXIT_SUCCESS);
}
