
# include "testing.h"


DEFINE_TEST(
	f02,
	"A child exception can be caught by catching a parent exception",
	"This test starts a <code>try</code> block, throws <code>ChildException</code> and catches it with a <code>catch(GrandparentException)</code> block.",
	NULL,
	EXIT_SUCCESS,
	"exception_WAS_caught",
	NULL
){

	E4C_BOOL caught = E4C_FALSE;

	ECHO(("before_USING_CONTEXT\n"));

	e4c_using_context(E4C_TRUE){

		ECHO(("before_TRY_block\n"));

		E4C_TRY{

			ECHO(("before_THROW\n"));

			E4C_THROW(ChildException, "I'm going to be caught.");

		}E4C_CATCH(GrandparentException){

			caught = E4C_TRUE;

			ECHO(("inside_CATCH_block\n"));

			ECHO(("catching__%s\n", e4c_get_exception()->name));
		}

		ECHO(("after_TRY_block\n"));

	}

	ECHO(("after_USING_CONTEXT\n"));

	if(caught){

		ECHO(("exception_WAS_caught\n"));

	}else{

		ECHO(("exception_WAS_NOT_caught\n"));

	}

	return(EXIT_SUCCESS);
}
