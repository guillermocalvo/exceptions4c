
# include "testing.h"


DEFINE_TEST(
	b13,
	"break... in the middle of a e4c_reusing_context{..} block",
	"This test uses the library in an inconsistent way, by <strong>breaking out of a <code>e4c_reusing_context</code> block</strong>. The library must signal the misuse by throwing the exception <code>ContextNotEnded</code>.",
	NULL,
	EXIT_FAILURE,
	"after_REUSING_CONTEXT_block",
	"ContextNotEnded"
){
	volatile const e4c_exception * thrown_exception;

	ECHO(("before_REUSING_CONTEXT_block\n"));

	{
		e4c_reusing_context(thrown_exception){

			ECHO(("inside_REUSING_CONTEXT_block\n"));

			break;
		}
	}

	ECHO(("after_REUSING_CONTEXT_block\n"));

	return(thrown_exception == NULL ? 123 : -123);
}
