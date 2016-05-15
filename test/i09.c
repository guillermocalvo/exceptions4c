
# include "testing.h"


DEFINE_TEST(
	i09,
	"Getting status (succeeded)",
	"This test retrieves the completeness of a code block. There were no exceptions during the <code>try</code> block, so the status is <code>e4c_succeeded</code>.",
	NULL,
	EXIT_SUCCESS,
	"before_CONTEXT_END",
	"status_SUCCEEDED"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_TRY\n"));

	E4C_TRY{

		ECHO(("inside_TRY_block\n"));

	}E4C_FINALLY{

		ECHO(("before_GET_STATUS\n"));

		switch( e4c_get_status() ){

			case e4c_succeeded:

				fprintf(stderr, "status_SUCCEEDED\n");
				break;

			case e4c_recovered:

				fprintf(stderr, "status_RECOVERED\n");
				break;

			case e4c_failed:
			default:

				fprintf(stderr, "status_FAILED\n");
		}

		(void)fflush(stderr);
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	return(EXIT_SUCCESS);
}
