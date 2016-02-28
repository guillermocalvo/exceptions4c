
# include "testing.h"


DEFINE_TEST(
	i11,
	"Getting status (failed)",
	"This test retrieves the completeness of a code block. There was an uncaught exceptions during the <code>try</code> block, so the status is <code>e4c_failed</code>. Finally, the outter block took care of the exception.",
	NULL,
	EXIT_SUCCESS,
	"before_CONTEXT_END",
	"status_FAILED"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_outter_TRY\n"));

	E4C_TRY{

		ECHO(("before_inner_TRY\n"));

		E4C_TRY{

			ECHO(("inside_TRY_block\n"));

			E4C_THROW(WildException, "You can't catch me!");

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

	}E4C_CATCH(RuntimeException){

		ECHO(("inside_outter_CATCH_block\n"));
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	return(EXIT_SUCCESS);
}
