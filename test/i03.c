
# include "testing.h"

# define DISPOSE_FOO(IGNORE1, IGNORE2) ECHO(("disposing_FOO\n"));

DEFINE_TEST(
	i03,
	"Reacquiring a resource",
	"This test tries to <em>acquire</em> a dummy resource and throws an exception. Then the <code>catch</code> block tries to <code>reacquire</code> the resource up to three times. The third reacquisition succeeds and then the resource is first <em>used</em> and then <em>disposed</em>.",
	NULL,
	EXIT_SUCCESS,
	"before_CONTEXT_END",
	"REACQUISITION_3"
){
	volatile int foo        = 0;
	volatile int reacquires = 0;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_WITH_block\n"));

	E4C_WITH(foo, DISPOSE_FOO){

		if(reacquires == 0){

			fprintf(stderr, "FIRST_ACQUISITION\n");

		}else{

			fprintf(stderr, "REACQUISITION_%d\n", reacquires);
		}

		foo++;
		reacquires++;

		if(reacquires <= 3){

			E4C_THROW(WildException, "Simulates an error while acquiring foo");
		}

	}E4C_USE{

		ECHO(("using_foo_%d\n", foo));

	}E4C_CATCH(WildException){

		ECHO(("before_REACQUIRE\n"));

		reacquire(3);
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
