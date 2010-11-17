
# include "testing.h"


DEFINE_TEST(
	f06,
	"An exception will only be caught by the first capable catch{...} block",
	"This test starts a <code>try</code> block, throws <code>ChildException</code> and attempts to catch it with a <code>catch(ChildException)</code> block, but there is a previous <code>catch(ParentException)</code> block which will eventually handle it.",
	NULL,
	EXIT_SUCCESS,
	"caught_by_GENERIC_block",
	NULL
){

	e4c_bool caught1 = e4c_false;
	e4c_bool caught2 = e4c_false;

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_TRY_block\n");

	E4C_TRY{

		printf("before_THROW\n");

		E4C_THROW(ChildException, "I'm going to be caught by the first (generic) catch block.");

	}E4C_CATCH(ParentException){

		caught1 = e4c_true;

		printf("inside_GENERIC_CATCH_block\n");

	}E4C_CATCH(ChildException){

		caught2 = e4c_true;

		printf("inside_SPECIFIC_CATCH_block\n");

	}

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	if(caught1 && caught2){

		printf("caught_by_BOTH\n");

	}else if(caught1){

		printf("caught_by_GENERIC_block\n");

	}else if(caught2){

		printf("caught_by_SPECIFIC_block\n");

	}else{

		printf("caught_by_NONE\n");

	}

	return(EXIT_SUCCESS);
}
