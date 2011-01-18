
# include "testing.h"


DEFINE_REQUIREMENT(
	z05,
	"Platform behavior when dividing by zero",
	"This test attempts to divide an integer by zero in order to verify that the program terminates.",
	SEVERITY_CRITICAL,
	"The platform does not stop your program when dividing by zero. Therefore, the library can't detect integer divisions by zero.",
	EXIT_WHATEVER,
	"before_DIVISION_BY_ZERO",
	ERROR_WHATEVER
){
	int zero = 0;
	int integer = 1010;

	ECHO(("before_DIVISION_BY_ZERO\n"));

	/* some smartypants compilers might need to be fooled */
	/* if(zero != integer) zero = 0; */

	integer = 31416 / zero;

	ECHO(("after_DIVISION_BY_ZERO_%d\n", integer));

	return(EXIT_SUCCESS);
}
