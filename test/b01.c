
# include "testing.h"


DEFINE_TEST(
	b01,
	"try{...} after having ended",
	"This test uses the library in an inconsistent way, by attempting to <strong>start a <code>try</code> block</strong>, after having called <code>e4c_context_end()</code>. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_TRY_block",
	"ContextHasNotBegunYet"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

		ECHO(("inside_TRY_block\n"));

	}

	ECHO(("after_TRY_block\n"));

	return(EXIT_SUCCESS);
}
