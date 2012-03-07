
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
		ECHO(("______aux_before_THROW\n"));
		E4C_THROW(WildException, "I'm not going to be thrown.");
	}else{
		ECHO(("______aux_no_exception_was_thrown\n"));
	}
}

static int ext2(void * pointer)
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
		ECHO(("____ext2_the_context_WAS_ready\n"));
	}else{
		ECHO(("____ext2_the_context_WAS_NOT_ready\n"));
	}

	ECHO(("____ext2_before_REUSING_CONTEXT\n"));

	{
		e4c_reusing_context(error, e4c_get_exception()->name){

			E4C_TRY{

				ECHO(("before_CALL_FUNCTION_aux\n"));

				aux(pointer);

				ECHO(("after_CALL_FUNCTION_aux\n"));

			}

		}
	}

	ECHO(("after_REUSING_CONTEXT_ext2\n"));

	is_ready2 = e4c_context_is_ready();

	if(is_ready2){
		ECHO(("____ext2_the_context_IS_ready\n"));
	}else{
		ECHO(("____ext2_the_context_IS_NOT_ready\n"));
	}

	if( BOOL_NOT_EQUAL(is_ready1, is_ready2) ){
		return(112233);
	}

	if(error == NULL){

		ECHO(("____ext2_there_was_no_error\n"));

		return(0);

	}else{

		ECHO(("____ext2_there_was_an_error_%s\n", error));

		return(2000);
	}

}

static int ext1(void)
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
	volatile int			status_code = -1;

	is_ready1 = e4c_context_is_ready();

	if(is_ready1){
		ECHO(("__ext1_the_context_WAS_ready\n"));
	}else{
		ECHO(("__ext1_the_context_WAS_NOT_ready\n"));
	}

	ECHO(("__ext1_before_REUSING_CONTEXT\n"));

	{
		e4c_reusing_context(error, e4c_get_exception()->name){

			ECHO(("__ext1_before_TRY_block\n"));

			E4C_TRY{

				int number = 123;

				ECHO(("__ext1_before_CALL_FUNCTION_ext2\n"));

				status_code = ext2(&number);

				ECHO(("__ext1_after_CALL_FUNCTION_ext2\n"));

			}E4C_FINALLY{

				ECHO(("__ext1_inside_FINALLY_block\n"));

			}

			ECHO(("__ext1_after_TRY_block\n"));

		}
	}

	ECHO(("__ext1_after_REUSING_CONTEXT\n"));

	is_ready2 = e4c_context_is_ready();

	if(is_ready2){
		ECHO(("__ext1_the_context_IS_ready\n"));
	}else{
		ECHO(("__ext1_the_context_IS_NOT_ready\n"));
	}

	if( BOOL_NOT_EQUAL(is_ready1, is_ready2) ){
		return(112233);
	}

	if(error == NULL){

		ECHO(("__ext1_there_was_no_error\n"));

		return(status_code);

	}else{

		ECHO(("__ext1_there_was_an_error_%s\n", error));

		return(status_code + 123);
	}

}

DEFINE_TEST_LONG_DESCRIPTION(
	h08,
	"A library (exception-unaware client) calls another library",
	"This tests simulates a call to an external function (as in a library function). The client code is <em>exception-unaware</em>, but the external function uses the exception framework. So the external function opens a <code>e4c_reusing_context</code> and then a new exception context is created.",
		"Then the external function calls some function from another library, which opens another <code>e4c_reusing_context</code>. The exception context is now <em>reused</em>. By the time the original caller continues, the exception context has been ended.",
	NULL,
	EXIT_SUCCESS,
	"result_was_0",
	NULL
){
	int result = -1;
	E4C_BOOL is_ready;

	ECHO(("before_CALL_FUNCTION_ext1\n"));

	result = ext1();

	ECHO(("after_CALL_FUNCTION_ext1\n"));

	is_ready = e4c_context_is_ready();
	if(is_ready){
		ECHO(("oops_the_exception_context_is_ready_now\n"));
	}else{
		ECHO(("result_was_%d\n", result));
	}

	return(EXIT_SUCCESS);
}
