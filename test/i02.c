
# include "testing.h"


DEFINE_TEST(
	i02,
	"Retrying a block of code",
	"This test <em>retries</em> a <code>try</code> block up to three times. The <code>retry</code> is performed from within a <code>finally</code> block, but it could also be done from a <code>catch</code> block when an exception is caught.",
	NULL,
	EXIT_SUCCESS,
	"before_CONTEXT_END",
	"RETRIES_3"
){
	volatile int retries = 0;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

		if(retries == 0){

			fprintf(stderr, "FIRST_TRY\n");

		}else{

			fprintf(stderr, "RETRIES_%d\n", retries);
		}

		retries++;

	}E4C_FINALLY{

		ECHO(("before_RETRY\n"));

		retry(3);
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
