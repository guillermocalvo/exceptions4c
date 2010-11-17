
# include "testing.h"


DEFINE_TEST(
	a06,
	"e4c_context_end() without beginning",
	"This test uses the library improperly, by attempting to <strong>call <code>e4c_context_end()</code></strong>, without having called <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_FAILURE,
	"before_CONTEXT_END",
	"ContextHasNotBegunYet"
){

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	return(EXIT_SUCCESS);
}
