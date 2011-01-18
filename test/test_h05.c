
# include "testing.h"

static void aux(void * pointer){
	if(pointer == NULL){
		ECHO(("____aux_before_THROW\n"));
		E4C_THROW(WildException, "The REUSING_CONTEXT block will mute me.");
	}else{
		ECHO(("____aux_no_exception_was_thrown\n"));
	}
}

static int ext(){

	const		e4c_exception * error;
	e4c_bool	is_ready1;
	e4c_bool	is_ready2;

	is_ready1 = e4c_context_is_ready();

	if(is_ready1){
		ECHO(("__ext_the_context_WAS_ready\n"));
	}else{
		ECHO(("__ext_the_context_WAS_NOT_ready\n"));
	}

	ECHO(("__ext_before_REUSING_CONTEXT\n"));

	{
		e4c_reusing_context(error){

			ECHO(("__ext_before_TRY_block\n"));

			E4C_TRY{

				ECHO(("__ext_before_CALL_FUNCTION_aux\n"));

				aux(NULL);

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

	if(is_ready1 != is_ready2) return(112233);

	if(error == NULL){

		ECHO(("__ext_there_was_no_error\n"));

		return(0);

	}else{

		ECHO(("__ext_there_was_an_error_%s\n", error->name));

		if(error->type == WildException.type){
			return(123);
		}else{
			return(-123);
		}

	}

}

DEFINE_TEST(
	h05,
	"A library (exception-unaware client) does not catch an exception",
	"This tests simulates a call to an external function (as in a library function). The client code is <em>exception-unaware</em>, but the external function uses the exception framework. So the external function opens a <code>e4c_reusing_context</code> and then a new exception context is created. The external function does not catch an exception, however, the <code>e4c_reusing_context</code> allows the external function to inspect the error and return an status code to its caller.",
	NULL,
	EXIT_SUCCESS,
	"result_was_123",
	NULL
){

	int result;

	ECHO(("before_CALL_FUNCTION_ext\n"));

	result = ext();

	ECHO(("after_CALL_FUNCTION_ext\n"));

	ECHO(("result_was_%d\n", result));

	return(EXIT_SUCCESS);
}
