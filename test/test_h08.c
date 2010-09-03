
# include "testing.h"


static void aux(void * pointer){
	if(pointer == NULL){
		printf("______aux_before_THROW\n");
		throw(WildException, "I'm not going to be thrown.");
	}else{
		printf("______aux_no_exception_was_thrown\n");
	}
}

static int ext2(void * pointer){

	const e4c_exception * error;
	e4c_bool is_ready1;
	e4c_bool is_ready2;

	is_ready1 = e4c_context_is_ready();

	if(is_ready1)	printf("____ext2_the_context_WAS_ready\n");
	else			printf("____ext2_the_context_WAS_NOT_ready\n");

	printf("____ext2_before_REUSING_CONTEXT\n");

	{
		e4c_reusing_context(error){

			try{

				printf("before_CALL_FUNCTION_aux\n");

				aux(pointer);

				printf("after_CALL_FUNCTION_aux\n");

			}

		}
	}

	printf("after_REUSING_CONTEXT_ext2\n");

	is_ready2 = e4c_context_is_ready();

	if(is_ready2)	printf("____ext2_the_context_IS_ready\n");
	else			printf("____ext2_the_context_IS_NOT_ready\n");

	if(is_ready1 != is_ready2) return(112233);

	if(error == NULL){

		printf("____ext2_there_was_no_error\n");

		return(0);

	}else{

		printf("____ext2_there_was_an_error_%s\n", error->name);

		return(2000);
	}

}

static int ext1(){

	const e4c_exception * error;
	e4c_bool is_ready1;
	e4c_bool is_ready2;
	int status_code;

	is_ready1 = e4c_context_is_ready();

	if(is_ready1)	printf("__ext1_the_context_WAS_ready\n");
	else			printf("__ext1_the_context_WAS_NOT_ready\n");

	printf("__ext1_before_REUSING_CONTEXT\n");

	{
		e4c_reusing_context(error){

			printf("__ext1_before_TRY_block\n");

			try{

				int number;

				printf("__ext1_before_CALL_FUNCTION_ext2\n");

				status_code = ext2(&number);

				printf("__ext1_after_CALL_FUNCTION_ext2\n");

			}finally{

				printf("__ext1_inside_FINALLY_block\n");

			}

			printf("__ext1_after_TRY_block\n");

		}
	}

	printf("__ext1_after_REUSING_CONTEXT\n");

	is_ready2 = e4c_context_is_ready();

	if(is_ready2)	printf("__ext1_the_context_IS_ready\n");
	else			printf("__ext1_the_context_IS_NOT_ready\n");

	if(is_ready1 != is_ready2) return(112233);

	if(error == NULL){

		printf("__ext1_there_was_no_error\n");

		return(status_code);

	}else{

		printf("__ext1_there_was_an_error_%s\n", error->name);

		return(status_code + 123);
	}

}

DEFINE_TEST(
	h08,
	"A library (exception-unaware client) calls another library",
	"This tests simulates a call to an external function (as in a library function). The client code is <em>exception-unaware</em>, but the external function uses the exception framework. So the external function opens a <code>e4c_reusing_context</code> and then a new exception context is created. Then the external function calls some function from another library, which opens another <code>e4c_reusing_context</code>. The exception context is now <em>reused</em>. By the time the original caller continues, the exception context has been ended.",
	EXIT_SUCCESS,
	"result_was_0",
	NULL
){
	int result = -1;
	e4c_bool is_ready;

	printf("before_CALL_FUNCTION_ext1\n");

	result = ext1();

	printf("after_CALL_FUNCTION_ext1\n");

	is_ready = e4c_context_is_ready();
	if(is_ready)	printf("oops_the_exception_context_is_ready_now\n");
	else	        printf("result_was_%d\n", result);

	return(EXIT_SUCCESS);
}
