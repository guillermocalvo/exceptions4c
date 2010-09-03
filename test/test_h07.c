
# include "testing.h"


static void aux(void * pointer){
	if(pointer == NULL){
		printf("____aux_before_THROW\n");
		throw(WildException, "The client of the external function will catch me.");
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

		return(123);
	}

}

DEFINE_TEST(
	h07,
	"A library lets its client catch an exception",
	"This tests simulates a call to an external function (as in a library function). The client code is <em>exception-aware</em>, but the external function cannot simply rely on that. So the external function opens a <code>e4c_reusing_context</code> and then the existing exception context is <em>reused</em>. The external function does not catch an exception so it is propagated to the caller. The caller catches it and then continues.",
	EXIT_SUCCESS,
	"result_was_456",
	NULL
){

	int result = -1;

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_TRY_block\n");

	try{

		printf("before_CALL_FUNCTION_ext\n");

		result = ext();

		printf("after_CALL_FUNCTION_ext\n");

	}catch(WildException){

		printf("inside_catch_block\n");

		result = 456;

	}

	printf("after_TRY_block\n");

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	printf("result_was_%d\n", result);

	return(EXIT_SUCCESS);
}
