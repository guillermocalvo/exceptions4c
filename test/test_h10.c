
# include "testing.h"


static void aux(/*@null@*/ void * pointer)
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
		E4C_THROW(WildException, "The client of the external function will catch me.");
	}else{
		ECHO(("______aux_no_exception_was_thrown\n"));
	}
}

static int ext2(/*@null@*/ void * pointer)
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

	volatile int		status = EXIT_SUCCESS;
	volatile E4C_BOOL	is_ready1;
	volatile E4C_BOOL	is_ready2;

	is_ready1 = e4c_context_is_ready();

	if(is_ready1){
		ECHO(("____ext2_the_context_WAS_ready\n"));
	}else{
		ECHO(("____ext2_the_context_WAS_NOT_ready\n"));
	}

	ECHO(("before_REUSING_CONTEXT2\n"));

	{
		e4c_reusing_context(status, 332211){

			E4C_TRY{

				ECHO(("before_CALL_FUNCTION_aux\n"));

				aux(pointer);

				ECHO(("after_CALL_FUNCTION_aux\n"));

			}E4C_FINALLY{

				ECHO(("____ext2_inside_FINALLY_block\n"));

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

	return(status);
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
	volatile int			ext2_status;

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

				ECHO(("__ext1_before_CALL_FUNCTION_ext2\n"));

				ext2_status = ext2(NULL);

				ECHO(("__ext1_after_CALL_FUNCTION_ext2_%d\n", ext2_status));

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

		return(0);

	}else{

		ECHO(("__ext1_there_was_an_error_%s\n", error));

		return(123);
	}

}

DEFINE_TEST_LONG_DESCRIPTION(
	h10,
	"A library calls another library and lets its client catch a received exception",
	"This tests simulates a call to an external function (as in a library function). The client code is <em>exception-aware</em>, but the external function cannot simply rely on that. So the external function opens a <code>e4c_reusing_context</code> and then the existing exception context is <em>reused</em>. Then the external function calls some function from another library, which opens another <code>e4c_reusing_context</code>. The exception context is <em>reused</em> again.",
		"Then, an exception is thrown. The second external function does not catch it. The exception is propagated to the first external function, which does not catch it either. Finally, the caller of the first external function catches the exception and then continues.",
	NULL,
	EXIT_SUCCESS,
	"the_result_is_456",
	NULL
){

	int result = -1;

	ECHO(("before_USING_CONTEXT\n"));

	e4c_using_context(E4C_TRUE){

		ECHO(("before_TRY_block\n"));

		E4C_TRY{

			ECHO(("before_CALL_FUNCTION_ext1\n"));

			result = ext1();

			ECHO(("after_CALL_FUNCTION_ext1\n"));

		}E4C_CATCH(WildException){

			ECHO(("inside_catch_block\n"));

			result = 456;

		}E4C_FINALLY{

			ECHO(("inside_FINALLY_block\n"));

		}

		ECHO(("before_endexctx\n"));

	}

	ECHO(("after_usingexctx\n"));

	ECHO(("the_result_is_%d\n", result));

	return(EXIT_SUCCESS);
}
