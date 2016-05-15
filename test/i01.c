
# include "testing.h"


DEFINE_TEST(
	i01,
	"Print exception type",
	"This test calls <code>e4c_print_exception_type</code> passing a user-defined exception type.",
	NULL,
	EXIT_SUCCESS,
	"after_PRINT_EXCEPTION_TYPE",
	"________________________________________________________________"
){

	ECHO(("before_PRINT_EXCEPTION_TYPE\n"));

	e4c_print_exception_type(&WildException);

	ECHO(("after_PRINT_EXCEPTION_TYPE\n"));

	return(EXIT_SUCCESS);
}
