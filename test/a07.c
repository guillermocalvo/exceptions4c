
# include "testing.h"


DEFINE_TEST(
	a07,
	"e4c_context_get_signal_mappings() without beginning",
	"This test uses the library improperly, by attempting to <strong>call <code>e4c_context_get_signal_mappings()</code></strong>, without having called <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_CONTEXT_GET_SIGNAL_MAPPINGS",
	"ContextHasNotBegunYet"
){

	ECHO(("before_CONTEXT_GET_SIGNAL_MAPPINGS\n"));

	(void)e4c_context_get_signal_mappings();

	ECHO(("after_CONTEXT_GET_SIGNAL_MAPPINGS\n"));

	return(EXIT_SUCCESS);
}
