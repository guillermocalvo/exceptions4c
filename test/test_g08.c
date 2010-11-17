
# include "testing.h"


DEFINE_TEST(
	g08,
	"Catching a division by zero exception",
	"This test attempts to divide by zero; the library signal handling is enabled; the exception is caught (<code>catch(SignalException)</code>)and then the program exits.",
	"This functionality relies on the <a href=\"#requirement_z08\"><strong>platform's ability to handle signal <code>SIGFPE</code></strong></a>.",
	EXIT_SUCCESS,
	"after_CONTEXT_END",
	"the_signal_WAS_CAUGHT"
){

	e4c_bool	caught		= e4c_false;
	int			zero		= 0;
	int			integer		= 100;

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_TRY_block\n");

	E4C_TRY{

		printf("before_DIVISION_BY_ZERO\n");
		fflush(stdout);

		/* some smartypants compilers might need to be fooled */
		/* if(zero != integer) zero = 0; */

		integer = integer / zero;

		printf("after_DIVISION_BY_ZERO_%d\n", integer);
		fflush(stdout);

	}E4C_CATCH(SignalException){

		printf("inside_CATCH_block\n");

		caught = e4c_true;

		printf("catching__%s\n", e4c_get_exception()->name);

	}

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	if(caught){
		fprintf(stderr, "\nthe_signal_WAS_CAUGHT\n");
	}else{
		fprintf(stderr, "\nthe_signal_WAS_NOT_CAUGHT\n");
	}

	return(EXIT_SUCCESS);

}
