
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
	const e4c_exception * exception_thrown;

	ECHO(("before_REUSING_CONTEXT_block\n"));

	{
		e4c_reusing_context(exception_thrown){

			ECHO(("inside_REUSING_CONTEXT_block\n"));

			break;
		}
	}

	ECHO(("after_REUSING_CONTEXT_block\n"));

	return(exception_thrown == NULL ? 123 : -123);
}
