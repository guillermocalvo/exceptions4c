
# include "testing.h"


DEFINE_TEST(
	g04,
	"Bad pointer exception",
	"This test attempts to dereference a null pointer; the library signal handling is enabled; there is no <code>catch</code> block. The behavior of the program will no longer be undefined: it will terminate because of the uncaught exception <code>BadPointerException</code>.",
	"This functionality relies on the <a href=\"#requirement_z07\"><strong>platform's ability to handle signal <code>SIGSEGV</code></strong></a>.",
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"before_NULL_POINTER",
	"BadPointerException"
){

	int * null_pointer = NULL;
	int integer;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(e4c_true, e4c_print_exception);

	ECHO(("before_NULL_POINTER\n"));

	/* some smartypants compilers might need to be fooled */
	/* if(null_pointer != &integer) null_pointer = NULL; */

	integer = *null_pointer;

	ECHO(("after_NULL_POINTER_%d\n", integer));

	if(integer) integer = 0;

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
