
# include "testing.h"


DEFINE_TEST(
	a13,
	"reacquire() without beginning",
	"This test uses the library improperly, by attempting to <strong>call <code>reacquire()</code></strong>, without having called <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_REACQUIRE",
	"ContextHasNotBegunYet"
){

	ECHO(("before_REACQUIRE\n"));

	E4C_REACQUIRE(10);

	ECHO(("after_REACQUIRE\n"));

	return(EXIT_SUCCESS);
}
