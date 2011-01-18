
# include "testing.h"


DEFINE_TEST(
	f05,
	"An exception can only be caught once for each try{...} block",
	"This test starts a <code>try</code> block, throws <code>TamedException</code> and attempts to catch it twice with two <code>catch(TamedException)</code> blocks. The exception will only be caught by the first one.",
	NULL,
	EXIT_SUCCESS,
	"caught_by_FIRST_block_only",
	NULL
){

	e4c_bool caught1 = e4c_false;
	e4c_bool caught2 = e4c_false;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(e4c_true, e4c_print_exception);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW(TamedException, "I can only be caught once for each try block.");

	}E4C_CATCH(TamedException){

		caught1 = e4c_true;

		ECHO(("inside_FIRST_CATCH_block\n"));

	}E4C_CATCH(TamedException){

		caught2 = e4c_true;

		ECHO(("inside_SECOND_CATCH_block\n"));

	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	if(caught1 && caught2){

		ECHO(("caught_by_BOTH\n"));

	}else if(caught1){

		ECHO(("caught_by_FIRST_block_only\n"));

	}else if(caught2){

		ECHO(("caught_by_SECOND_block_only\n"));

	}else{

		ECHO(("caught_by_NONE\n"));

	}

	return(EXIT_SUCCESS);
}
