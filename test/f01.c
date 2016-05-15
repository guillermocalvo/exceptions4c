
# include "testing.h"


DEFINE_TEST(
	f01,
	"Catching a specific exception",
	"This test starts a <code>try</code> block, throws <code>ChildException</code> and catches it with a <code>catch(ChildException)</code> block.",
	NULL,
	EXIT_SUCCESS,
	"exception_WAS_caught",
	NULL
){

	E4C_BOOL caught = E4C_FALSE;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW(ChildException, "I'm going to be caught.");

	}E4C_CATCH(ChildException){

		caught = E4C_TRUE;

		ECHO(("inside_CATCH_block\n"));

		ECHO(("catching__%s\n", e4c_get_exception()->name));
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	if(caught){

		ECHO(("exception_WAS_caught\n"));

	}else{

		ECHO(("exception_WAS_NOT_caught\n"));

	}

	return(EXIT_SUCCESS);
}
