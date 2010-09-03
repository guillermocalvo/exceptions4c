
# include "testing.h"


DEFINE_TEST(
	b02,
	"throw(...) after having ended",
	"This test uses the library in an inconsistent way, by attempting to <strong>throw an exception</strong>, after having called <code>e4c_context_end()</code>. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	EXIT_FAILURE,
	"before_THROW",
	"ContextHasNotBegunYet"
){

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("before_THROW\n");

	throw(WildException, "Actually, I can't throw an exception anymore.");

	printf("after_THROW\n");

	return(EXIT_SUCCESS);
}
