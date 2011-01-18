
# include "testing.h"


DEFINE_TEST(
	g01,
	"Signal SIGSEGV",
	"This test attempts to dereference a null pointer; the library signal handling is disabled, so the behavior of the program will be implementation-defined (typically the program will terminate abruptly).",
	"This functionality relies on the <a href=\"#requirement_z04\"><strong>platform's behavior when dereferencing a null pointer</strong></a>.",
	EXIT_WHATEVER,
	"before_NULL_POINTER",
	ERROR_WHATEVER
){
	int * null_pointer = NULL;
	int integer;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(e4c_false, e4c_print_exception);

	ECHO(("before_NULL_POINTER\n"));

	/* some smartypants compilers might need to be fooled */
	/* if(null_pointer != &integer) null_pointer = NULL; */

	integer = *null_pointer;

	ECHO(("after_NULL_POINTER_%d\n", integer));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
