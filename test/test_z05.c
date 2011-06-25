
# include "testing.h"


static int set_zero_z05(int dummy)
/*@*/
{

	if(dummy == 0){
		return(1);
	}

	return(0);
}


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

	int		divisor			= 10;
	int		integer			= 100;

	/* some smartypants compilers might need to be fooled */
	/* divisor = 0; */

	ECHO(("before_SET_ZERO\n"));

	divisor = set_zero_z05(divisor);

	ECHO(("before_DIVISION_BY_ZERO\n"));

	integer = integer / divisor;

	ECHO(("after_DIVISION_BY_ZERO_%d\n", integer));

	return(EXIT_SUCCESS);
}
