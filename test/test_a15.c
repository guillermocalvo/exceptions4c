
# include "testing.h"


DEFINE_TEST(
	a15,
	"e4c_get_status() without beginning",
	"This test uses the library improperly, by attempting to <strong>call <code>e4c_get_status()</code></strong>, without having called <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_GET_STATUS",
	"ContextHasNotBegunYet"
){

	ECHO(("before_GET_STATUS"));

	(void)e4c_get_status();

	ECHO(("after_GET_STATUS\n"));

	return(EXIT_SUCCESS);
}
