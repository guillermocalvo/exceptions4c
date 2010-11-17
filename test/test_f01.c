
# include "testing.h"


DEFINE_TEST(
	f01,
	"Catching a specific exception",
	"This test starts a <code>try</code> block, throws <code>ChildException</code> and catches it with a <code>catch(ChildException)</code> block.",
	NULL,
	EXIT_SUCCESS,
	"exception_WAS_caught",
	NULL
){

	e4c_bool caught = e4c_false;

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_TRY_block\n");

	E4C_TRY{

		printf("before_THROW\n");

		E4C_THROW(ChildException, "I'm going to be caught.");

	}E4C_CATCH(ChildException){

		caught = e4c_true;

		printf("inside_CATCH_block\n");

		printf("catching__%s\n", e4c_get_exception()->name);
	}

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	if(caught){

		printf("exception_WAS_caught\n");

	}else{

		printf("exception_WAS_NOT_caught\n");

	}

	return(EXIT_SUCCESS);
}
