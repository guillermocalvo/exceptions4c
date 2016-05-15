
# include "testing.h"


static void aux(void * pointer)
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
	if(pointer == NULL){
		ECHO(("____aux_before_THROW\n"));
		E4C_THROW(WildException, "I'm not going to be thrown.");
	}else{
		ECHO(("____aux_no_exception_was_thrown\n"));
	}
}

static int ext(void)
/*@globals
	fileSystem,
	internalState,

	e4c_default_signal_mappings,

	AssertionException,
	NotEnoughMemoryException,
	NullPointerException,
	RuntimeException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	volatile const char *	error = NULL;
	volatile E4C_BOOL		is_ready1;
	volatile E4C_BOOL		is_ready2;

	is_ready1 = e4c_context_is_ready();

	if(is_ready1){
		ECHO(("__ext_the_context_WAS_ready\n"));
	}else{
		ECHO(("__ext_the_context_WAS_NOT_ready\n"));
	}

	ECHO(("__ext_before_REUSING_CONTEXT\n"));

	{
		e4c_reusing_context(error, e4c_get_exception()->name){

			ECHO(("__ext_before_TRY_block\n"));

			E4C_TRY{

				int number = 123;

				ECHO(("__ext_before_CALL_FUNCTION_aux\n"));

				aux(&number);

				ECHO(("__ext_after_CALL_FUNCTION_aux\n"));

			}

			ECHO(("__ext_after_TRY_block\n"));
		}
	}

	ECHO(("__ext_after_REUSING_CONTEXT\n"));

	is_ready2 = e4c_context_is_ready();

	if(is_ready2){
		ECHO(("__ext_the_context_IS_ready\n"));
	}else{
		ECHO(("__ext_the_context_IS_NOT_ready\n"));
	}

	if( BOOL_NOT_EQUAL(is_ready1, is_ready2) ){
		return(112233);
	}

	if(error == NULL){

		ECHO(("__ext_there_was_no_error\n"));

		return(0);

	}else{

		ECHO(("__ext_there_was_an_error_%s\n", error));

		return(123);
	}

}

DEFINE_TEST(
	h02,
	"A library called by a exception-aware client",
	"This tests simulates a call to an external function (as in a library function). The client code is <em>exception-aware</em>, but the external function cannot simply rely on that. So the external function opens a <code>e4c_reusing_context</code> block and then the existing exception context is <em>reused</em>.",
	NULL,
	EXIT_SUCCESS,
	"result_was_0",
	NULL
){

	int result;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CALL_FUNCTION_ext\n"));

	result = ext();

	ECHO(("after_CALL_FUNCTION_ext\n"));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("result_was_%d\n", result));

	return(EXIT_SUCCESS);
}
