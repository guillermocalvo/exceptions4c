
# include "testing.h"


static void aux(void)
/*@globals
	fileSystem,
	internalState,

	AssertionException,
	NotEnoughMemoryException,
	NullPointerException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	ECHO(("before_TRY_block\n"));

	E4C_TRY{
		ECHO(("inside_TRY_block\n"));
		E4C_THROW(TamedException, "Hey! you cannot return in the middle of a catch block.");
	}E4C_CATCH(TamedException){
		ECHO(("inside_CATCH_block\n"));
		return;
	}

	ECHO(("after_TRY_block\n"));
}

DEFINE_TEST(
	b09,
	"return... in the middle of a catch{...} block",
	"This test uses the library in an inconsistent way, by <strong>returning from a function in the middle of a <code>catch</code> block</strong>. The library must signal the misuse by throwing the exception <code>ExceptionSystemFatalError</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_CONTEXT_END",
	"ExceptionSystemFatalError"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CALL_FUNCTION_aux\n"));

	aux();

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
