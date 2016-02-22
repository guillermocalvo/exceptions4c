
# include "testing.h"


DEFINE_TEST(
	b15,
	"catch(NULL)",
	"This test uses the library in an inconsistent way, by attempting to pass <code>NULL</code> to a <code>catch</code> block. The library must signal the misuse by throwing the exception <code>NullPointerException</code>.",
	NULL,
	EXIT_WHATEVER,
	"inside_TRY_block",
	"NullPointerException"
){

	ECHO(("before_CONTEXT_BEGIN_block\n"));

	e4c_context_begin(E4C_TRUE);

	try{

		ECHO(("inside_TRY_block\n"));

		E4C_THROW(WildException, "You must not pass NULL to a catch block.");

	}catch(  *( (e4c_exception_type *)NULL )  ){

		ECHO(("inside_CATCH_block\n"));
	}

	ECHO(("before_CONTEXT_END_block\n"));

	e4c_context_end();

	return(EXIT_SUCCESS);
}
