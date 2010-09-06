
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
		Can be defined with the name of the suite collection to run.
		Default value: allSuites

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
		The file will be delted when all the tests are executed.

*/


extern TestSuiteCollection
	allSuites,
	beginningSuite,
	consistencySuite,
	endingSuite,
	uncaughtSuite,
	finallySuite,
	caughtSuite,
	signalsSuite,
	integrationSuite;


# ifndef RUN
#	define RUN				allSuites
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


static int usage(){
	printf("Error: please run this program without arguments.\n");
	return(EXIT_FAILURE);
}

static int wrongVersion(long version){
	printf(
		"Error: the library and header version numbers don't match.\n"
		"\n"
		" * run-time version number:	 %ld\n"
		" * compile-time version number: %ld\n"
		"\n"
		"Please compile the library again with its corresponding header file.\n",
		version, E4C_VERSION_NUMBER
	);
	return(EXIT_FAILURE);
}

int main(int argc, char * argv[]){

	long		version;
	int			suiteNumber;
	int			testNumber;
	UnitTest *	test;
	TestSuite *	suite;

	TestSuiteCollection
				suiteCollection = RUN;

	/* check library version numbers */
	version = e4c_library_version();
	if(version != E4C_VERSION_NUMBER) return( wrongVersion(version) );

	/* run all the tests */
	if(argc == 1){
		TestRunner runner;
		int result;

		runner = newTestRunner(argv[0], TMP_OUT_FILE, TMP_ERR_FILE, REPORT_FILE, &suiteCollection);

		result = runAllTestSuites(&runner);

		return(result);
	}

	/* wrong number of arguments */
	if(argc != 3) return( usage() );

	/* run a specific test */
	suiteNumber = atoi(argv[1]);
	testNumber	= atoi(argv[2]);

	if(suiteNumber < 0 || suiteNumber >= suiteCollection.count)		return( usage() );

	suite	= suiteCollection.suite[suiteNumber];

	if(testNumber < 0 || testNumber >= suite->tests->count)			return( usage() );

	test	= suite->tests->test[testNumber];

	return( test->function() );
}
