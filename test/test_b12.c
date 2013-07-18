
# include "testing.h"


DEFINE_TEST(
	b12,
	"break... in the middle of a e4c_using_context{..} block",
	"This test uses the library in an inconsistent way, by <strong>breaking out of a <code>e4c_using_context</code> block</strong>. The library must signal the misuse by throwing the exception <code>ContextNotEnded</code>.",
	NULL,
	EXIT_WHATEVER,
	"after_USING_CONTEXT_block",
	"ContextNotEnded"
){

	ECHO(("before_USING_CONTEXT_block\n"));

	e4c_using_context(E4C_TRUE){

		ECHO(("inside_USING_CONTEXT_block\n"));

		break;
	}

	ECHO(("after_USING_CONTEXT_block\n"));

	return(EXIT_SUCCESS);
}
