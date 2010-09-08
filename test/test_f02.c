
# include "testing.h"


DEFINE_TEST(
	f02,
	"A child exception can be caught by catching a parent exception",
	"This test starts a <code>try</code> block, throws <code>ChildException</code> and catches it with a <code>catch(GrandparentException)</code> block.",
	EXIT_SUCCESS,
	"exception_WAS_caught",
	NULL
){

	e4c_bool caught = e4c_false;

	printf("before_USING_CONTEXT\n");

	e4c_using_context(e4c_true, e4c_print_exception){

		printf("before_TRY_block\n");

		E4C_TRY{

			printf("before_THROW\n");

			E4C_THROW(ChildException, "I'm going to be caught.");

		}E4C_CATCH(GrandparentException){

            caught = e4c_true;

			printf("inside_CATCH_block\n");

			printf("catching__%s\n", e4c_get_exception()->name);
		}

		printf("after_TRY_block\n");

	}

	printf("after_USING_CONTEXT\n");

	if(caught){

		printf("exception_WAS_caught\n");

	}else{

		printf("exception_WAS_NOT_caught\n");

	}

	return(EXIT_SUCCESS);
}
