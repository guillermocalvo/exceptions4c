
# include "testing.h"


DEFINE_TEST(
	b08,
	"break... in the middle of a try{...} block",
	"This test uses the library in an inconsistent way, by <strong>breaking out of a <code>try</code> block</strong>. The library must signal the misuse by throwing the exception <code>ExceptionSystemFatalError</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_CONTEXT_END",
	"ExceptionSystemFatalError"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{
		ECHO(("inside_TRY_block\n"));
		break;
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
