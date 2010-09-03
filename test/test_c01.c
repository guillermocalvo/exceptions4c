
# include "testing.h"


DEFINE_TEST(
	c01,
	"Exiting program without ending",
	"This test uses the library improperly, by attempting to <strong>exit the program</strong>, without calling <code>e4c_context_end()</code> first. The exception context is begun, but it never gets ended because the program exits. The library must signal the misuse by throwing the exception <code>ContextNotEnded</code>.",
	EXIT_FAILURE,
	"before_EXIT_main",
	"ContextNotEnded"
){

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_EXIT_main\n");

	return(EXIT_SUCCESS);
}
