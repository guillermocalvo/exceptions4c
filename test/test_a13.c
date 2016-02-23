
# include "testing.h"


DEFINE_TEST(
	a13,
	"catch() without beginning",
	"This test uses the library improperly, by attempting to <strong>call <code>catch()</code></strong>, without having called <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_CATCH",
	"ContextHasNotBegunYet"
){
	int dummy = 1;

	ECHO(("before_CATCH\n"));

	if(--dummy){

		ECHO(("inside_IF\n"));

	}catch(NullPointerException){

		ECHO(("inside_CATCH\n"));
	}

	ECHO(("after_CATCH\n"));

	return(EXIT_SUCCESS);
}
