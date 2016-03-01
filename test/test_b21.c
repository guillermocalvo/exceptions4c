
# include "testing.h"
# include <signal.h>


DEFINE_TEST(
	b21,
	"Repeating an invalid stage",
	"This test tries to repeat an invalid stage. The library must signal the misuse by throwing the exception <code>ExceptionSystemFatalError</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_FRAME_REPEAT",
	"ExceptionSystemFatalError"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_FALSE);

	E4C_TRY{

		ECHO(("before_FRAME_REPEAT\n"));

		/* This function must not be called like this */
		e4c_frame_repeat_(10, e4c_done_ + 1, "misuse.c", 0xBADC0DE, NULL);

		ECHO(("after_FRAME_REPEAT\n"));
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	return(EXIT_SUCCESS);
}
