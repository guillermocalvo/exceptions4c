
# include <stdio.h>
# include <string.h>
# include <time.h>

# include <signal.h>

# include "testing.h"
# include "html.h"

# ifdef NDEBUG
#	define IS_NDEBUG_DEFINED e4c_true
# else
#	define IS_NDEBUG_DEFINED e4c_false
# endif

e4c_bool NDEBUG_is_defined = IS_NDEBUG_DEFINED;

E4C_DEFINE_EXCEPTION(WildException, "This is a wild exception.", RuntimeException);
E4C_DEFINE_EXCEPTION(TamedException, "This is a tamed exception.", RuntimeException);

E4C_DEFINE_EXCEPTION(GrandparentException, "This is a grandparent exception.", RuntimeException);
E4C_DEFINE_EXCEPTION(ParentException, "This is a parent exception.", GrandparentException);
E4C_DEFINE_EXCEPTION(ChildException, "This is a child exception.", ParentException);
E4C_DEFINE_EXCEPTION(SiblingException, "This is a sibling exception.", ParentException);




static int cannotRead(const char * filePath){

	printf("Error: Cannot read file: %s\nPlease check the file system.\n", filePath);
	return(EXIT_FAILURE);
}

static int cannotWrite(const char * filePath){

	printf("Error: Cannot write file: %s\nPlease check the file system.\n", filePath);
	return(EXIT_FAILURE);
}

static const char * loadFile(const char * filePath, char * store, int bytes){

	FILE *			file;
	int				tmp				= ' ';
	int				read			= 0;
	const char *	lastToken		= NULL;
	e4c_bool		newToken		= e4c_true;
	e4c_bool		lookingForToken = e4c_true;
# ifdef NDEBUG
	e4c_bool		readyForToken	= e4c_false;
# endif

	file = fopen(filePath, "r");

	if(file == NULL) exit( cannotRead(filePath) );

	for(read = 0; read < bytes; read++){

		tmp = getc(file);

		if(tmp == EOF) break;

		if(tmp == '<')			store[read] = '{';
		else if(tmp == '>')		store[read] = '}';
		else					store[read] = tmp;

		if(lookingForToken){
			if(tmp == ':'){
# ifdef NDEBUG
				readyForToken	= e4c_true;
# else
				lookingForToken = e4c_false;
# endif
			}else if(tmp <= ' '){
				newToken = e4c_true;
			}else if(newToken){
				newToken = e4c_false;
				lastToken = store + read;
# ifdef NDEBUG
				if(readyForToken) lookingForToken = e4c_false;
# endif
			}
		}
	}

	fclose(file);

	if(read < bytes){
		store[read] = '\0';
	}else{
		store[read - 1] = '\0';
		store[read - 2] = '.';
		store[read - 3] = '.';
		store[read - 4] = '.';
	}

	return(lastToken);
}

static e4c_bool isUnexpectedToken(const char * expectedToken, const char * token){

	int index;

	if(expectedToken == NULL){
		return(token != NULL);
	}

	if(token == NULL) return(e4c_true);

	for(index = 0; expectedToken[index] != '\0'; index++){
		if( token[index] != expectedToken[index] ){
			return(e4c_true);
		}
	}
	return(e4c_false);
}

static const char * getCommandLine(TestRunner * runner){

	sprintf(
		runner->buffer,
		"%s %d %d > %s 2> %s",
		runner->filePath,
		runner->suiteNumber,
		runner->testNumber,
		runner->out,
		runner->err
	);

	return(runner->buffer);
}

static void runTest(UnitTest * test, TestRunner * runner){

	const char *	command;
	const char *	lastOutput;
	const char *	lastError;

	command = getCommandLine(runner);

	printf("	Test %s: ", test->code);

	test->foundExitCode = system(command);

	lastOutput	= loadFile( runner->out, test->foundOutput, sizeof(test->foundOutput) );
	lastError	= loadFile( runner->err, test->foundError, sizeof(test->foundError) );

	test->unexpectedExitCode	= ( test->expectedExitCode != EXIT_WHATEVER && test->foundExitCode != test->expectedExitCode );
	test->unexpectedOutput		= ( test->expectedOutput != NULL			&& isUnexpectedToken(test->expectedOutput, lastOutput) );
	test->unexpectedError		= ( isUnexpectedToken(test->expectedError, lastError) );

	test->passed = (!test->unexpectedExitCode && !test->unexpectedOutput && !test->unexpectedError);

	printf("%s\n", test->passed ? "passed" : "FAILED!");
}

static void runTestSuite(TestRunner * runner, TestSuite * testSuite){

	int tests;

	printf("Running test suite %s...\n", testSuite->title);

	tests = testSuite->tests->count;

	testSuite->stats.passed	= 0;
	testSuite->stats.failed	= 0;

	for(runner->testNumber = 0; runner->testNumber < tests; runner->testNumber++){

		UnitTest * test = testSuite->tests->test[runner->testNumber];

		runTest(test, runner);

		if(test->passed){
			testSuite->stats.passed++;
		}else{
			testSuite->stats.failed++;
		}
	}

	testSuite->stats.total		= testSuite->stats.passed + testSuite->stats.failed;
	testSuite->passed			= ( testSuite->stats.failed == 0 );
}

static void generateReport(TestRunner * runner){

	FILE *		report;

	report = fopen(runner->report, "w");

	if(report == NULL) exit( cannotWrite(runner->report) );

	html(runner, report);

	fclose(report);
}

int runAllTestSuites(TestRunner * runner){

	printf("Running all the tests...\n\n");

	for(runner->suiteNumber = 0; runner->suiteNumber < runner->suites->count; runner->suiteNumber++){

		TestSuite * suite = runner->suites->suite[runner->suiteNumber];

		runTestSuite(runner, suite);

		if(suite->passed){
			runner->stats.suites.passed++;
		}else{
			runner->stats.suites.failed++;
		}

		runner->stats.tests.passed += suite->stats.passed;
		runner->stats.tests.failed += suite->stats.failed;
	}

	runner->stats.tests.total	= runner->stats.tests.passed	+ runner->stats.tests.failed;
	runner->stats.suites.total	= runner->stats.suites.passed	+ runner->stats.suites.failed;

	/* delete temporary files */
	remove(runner->out);
	remove(runner->err);

	if(runner->report != NULL){

		printf("\n\nGenerating report....\n");

		generateReport(runner);

		/* hopefully this will open the report in the default browser */
		system(runner->report);
	}

	printf("\tDone.\n");

	return(EXIT_SUCCESS);
}

TestRunner newTestRunner(const char * filePath, const char * out, const char * err, const char * report, TestSuiteCollection * suites){

	TestRunner runner;

	runner.filePath = filePath;
	runner.out		= out;
	runner.err		= err;
	runner.report	= report;
	runner.suites	= suites;

	runner.stats.suites.total	= 0;
	runner.stats.suites.passed	= 0;
	runner.stats.suites.failed	= 0;

	runner.stats.tests.total	= 0;
	runner.stats.tests.passed	= 0;
	runner.stats.tests.failed	= 0;

	return(runner);
}
