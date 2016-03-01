
# include "testing.h"
# include <signal.h>


DEFINE_TEST(
	b22,
	"throw(NULL)",
	"This test <em>throws</em> <code>NULL</code>. The library must <em>throw</em> <code>NullPointerException</code> instead.",
	NULL,
	EXIT_WHATEVER,
	"before_THROW",
	"NullPointerException"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_FALSE);

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW( *( (const e4c_exception_type *)NULL ), "I see what you did there..." );

		ECHO(("after_THROW\n"));
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	return(EXIT_SUCCESS);
}
