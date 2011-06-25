
# include <stdio.h>

# include "testing.h"


/*

These tests can be compiled with any of the following compile-time parameters:

Library parameters:

	E4C_THREADSAFE
		Compiles the library in multi-thread mode.

	NDEBUG
		Disables some of the integrity checks of the library. In addition, the
		function e4c_print_exception prints out less information.


Unit testing parameters:

	RUN
		Can be defined with the name of the suite collection to run:
			* ALL_SUITES
			* SUITE_BEGINNING
			* SUITE_CONSISTENCY
			* SUITE_ENDING
			* SUITE_UNCAUGHT
			* SUITE_FINALLY
			* SUITE_CAUGHT
			* SUITE_SIGNALS
			* SUITE_INTEGRATION
			* PLATFORM_REQUIREMENTS
		Default value: ALL_SUITES

	REPORT_FILE
		Can be defined with the name of the file to be created.
		If not NULL, then the HTML report file will be generated an opened.
		Default value: "report.html"

	TMP_OUT_FILE
		Can be defined with the name of the temporary file to use for stdout.
		Default value: "out.tmp"
		The file will be delted when all the tests are executed.

	TMP_ERR_FILE
		Can be defined with the name of the temporary file to use for stderr.
		Default value: "err.tmp"
		The file will be deleted when all the tests are executed.

*/


# ifndef RUN
#	define RUN				ALL_SUITES
# endif

# ifndef REPORT_FILE
#	define REPORT_FILE		"report.html"
# endif

# ifndef TMP_OUT_FILE
#	define TMP_OUT_FILE		"out.tmp"
# endif

# ifndef TMP_ERR_FILE
#	define TMP_ERR_FILE		"err.tmp"
# endif


int main(int argc, char * argv[])
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	return( parse_command_line(argc, argv, &RUN, REPORT_FILE, TMP_OUT_FILE, TMP_ERR_FILE) );
}
