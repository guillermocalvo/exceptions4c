
# include "testing.h"


static void aux5(void)
/*@globals
	fileSystem,
	internalState,

	NotEnoughMemoryException,
	NullPointerException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	ECHO(("inside_aux5\n"));

	ECHO(("before_THROW\n"));

	E4C_THROW(TamedException, "I'm going to be caught.");

	/*@-unreachable@*/

	ECHO(("before_EXIT_aux5\n"));

	/*@=unreachable@*/
}

static void aux4(void)
/*@globals
	fileSystem,
	internalState,

	AssertionException,
	NotEnoughMemoryException,
	NullPointerException,
	ProgramSignalException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	ECHO(("inside_aux4\n"));

	E4C_TRY{

		aux5();

	}E4C_CATCH(ProgramSignalException){

		ECHO(("inside_AUX4_CATCH_block\n"));

	}

	ECHO(("before_EXIT_aux4\n"));
}

static void aux3(void)
/*@globals
	fileSystem,
	internalState,

	AssertionException,
	NotEnoughMemoryException,
	NullPointerException,
	ProgramSignalException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	ECHO(("inside_aux3\n"));

	aux4();

	ECHO(("before_EXIT_aux3\n"));
}

static void aux2(void)
/*@globals
	fileSystem,
	internalState,

	AssertionException,
	NotEnoughMemoryException,
	NullPointerException,
	ProgramSignalException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	ECHO(("inside_aux2\n"));

	E4C_TRY{

		aux3();

	}

	ECHO(("before_EXIT_aux2\n"));
}


static void aux1(void)
/*@globals
	fileSystem,
	internalState,

	AssertionException,
	NotEnoughMemoryException,
	NullPointerException,
	ProgramSignalException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	ECHO(("inside_aux1\n"));

	aux2();

	ECHO(("before_EXIT_aux1\n"));
}

/*@-globs@*/ /* ProgramSignalException */
DEFINE_TEST(
	f07,
	"Catching an exception thrown deep down the call stack",
	"This test starts a <code>try</code> block, and calls a function. That function calls another one, and so on. Eventually, one of those functions will throw an exception that will be caught by a <code>catch(TamedException)</code> right next to the <code>try</code> block.",
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

		ECHO(("before_CALL_FUNCTION_aux1\n"));

		aux1();

		ECHO(("after_CALL_FUNCTION_aux1\n"));

	}E4C_CATCH(TamedException){

		caught = E4C_TRUE;

		ECHO(("inside_MAIN_CATCH_block\n"));

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
/*@=globs@*/ /* ProgramSignalException */
