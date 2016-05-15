
# include "testing.h"


DEFINE_TEST(
	a08,
	"e4c_context_set_signal_mappings() without beginning",
	"This test uses the library improperly, by attempting to <strong>call <code>e4c_context_set_signal_mappings()</code></strong>, without having called <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_CONTEXT_SET_SIGNAL_MAPPINGS",
	"ContextHasNotBegunYet"
){

	ECHO(("before_CONTEXT_SET_SIGNAL_MAPPINGS\n"));

	e4c_context_set_signal_mappings(NULL);

	ECHO(("after_CONTEXT_SET_SIGNAL_MAPPINGS\n"));

	return(EXIT_SUCCESS);
}
