
# include "testing.h"


DEFINE_TEST(
	i15,
	"Finding the cause of an exception",
	"This test <em>throws</em> an exception from within an inner <catch>block</catch> and an outter <code>catch</code> block inspects the <em>cause</em>.",
	NULL,
	EXIT_SUCCESS,
	"TamedException_IS_THE_CAUSE_OF_WildException",
	OUTPUT_WHATEVER
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_FALSE);

	ECHO(("before_outter_TRY_block\n"));

	E4C_TRY{

		ECHO(("before_inner_TRY_block\n"));

		E4C_TRY{

			ECHO(("before_THROWF\n"));

			e4c_exception_throw_verbatim_(&TamedException, "cause.c", 123, NULL, "This is the original cause of the issue");

		}E4C_CATCH(RuntimeException){

			ECHO(("inside_inner_CATCH_block\n"));

			e4c_exception_throw_verbatim_(&WildException, "wrapper.c", 123, NULL, "This is the wrapper exception");
		}

	}E4C_CATCH(RuntimeException){

		const e4c_exception * e;

		ECHO(("inside_outter_CATCH_block\n"));

		e = e4c_get_exception();

		ECHO(("before_PRINT_EXCEPTION\n"));

		e4c_print_exception(e);

		ECHO(("%s_IS_THE_CAUSE_OF_%s\n", e->cause->name, e->name));
	}

	e4c_context_end();

	return(EXIT_SUCCESS);
}
