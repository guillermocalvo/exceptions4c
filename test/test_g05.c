
# include "testing.h"


DEFINE_TEST(
	g05,
	"Catching a null pointer exception",
	"This test attempts to dereference a null pointer; the library signal handling is enabled; the exception is caught (<code>catch(BadPointerException)</code>) and then the program exits.",
	EXIT_SUCCESS,
	"the_signal_WAS_CAUGHT",
	NULL
){

	e4c_bool	caught		= e4c_false;
	int *		nullPointer = NULL;
	int			integer;

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_TRY_block\n");

	try{

		printf("before_NULL_POINTER\n");
		fflush(stdout);

		/* some smartypants compilers might need to be fooled */
		/* if(nullPointer != &integer) nullPointer = NULL; */

		integer = *nullPointer;

		printf("after_NULL_POINTER\n");
		fflush(stdout);

	}catch(BadPointerException){

		printf("inside_CATCH_block\n");

		caught = e4c_true;

		printf("catching__%s\n", e4c_get_exception()->name);

	}

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	if(caught){
		printf("the_signal_WAS_CAUGHT\n");
	}else{
		printf("the_signal_WAS_NOT_CAUGHT\n");
	}

	return(EXIT_SUCCESS);

}
