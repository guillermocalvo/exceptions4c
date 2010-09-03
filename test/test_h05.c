
# include "testing.h"

static void aux(void * pointer){
	if(pointer == NULL){
		printf("____aux_before_THROW\n");
		throw(WildException, "The REUSING_CONTEXT block will mute me.");
	}else{
		printf("____aux_no_exception_was_thrown\n");
	}
}

static int ext(){

	const e4c_exception * error;
	e4c_bool is_ready1;
	e4c_bool is_ready2;

	is_ready1 = e4c_context_is_ready();

	if(is_ready1)	printf("__ext_the_context_WAS_ready\n");
	else			printf("__ext_the_context_WAS_NOT_ready\n");

	printf("__ext_before_REUSING_CONTEXT\n");

	{
		e4c_reusing_context(error){

			printf("__ext_before_TRY_block\n");

			try{

				printf("__ext_before_CALL_FUNCTION_aux\n");

				aux(NULL);

				printf("__ext_after_CALL_FUNCTION_aux\n");

			}

			printf("__ext_after_TRY_block\n");

		}
	}

	printf("__ext_after_REUSING_CONTEXT\n");

	is_ready2 = e4c_context_is_ready();

	if(is_ready2)	printf("__ext_the_context_IS_ready\n");
	else			printf("__ext_the_context_IS_NOT_ready\n");

	if(is_ready1 != is_ready2) return(112233);

	if(error == NULL){

		printf("__ext_there_was_no_error\n");

		return(0);

	}else{

		printf("__ext_there_was_an_error_%s\n", error->name);

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
	EXIT_SUCCESS,
	"result_was_123",
	NULL
){

	int result;

	printf("before_CALL_FUNCTION_ext\n");

	result = ext();

	printf("after_CALL_FUNCTION_ext\n");

	printf("result_was_%d\n", result);

	return(EXIT_SUCCESS);
}
