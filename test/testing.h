
# ifndef TESTING_FRAMEWORK_H

# define TESTING_FRAMEWORK_H

# include <stdio.h>
# include "e4c.h"

# define EXIT_WHATEVER		76543210

/*

	WHAT IS "EXIT_WHATEVER" USED FOR?

	1. When an unhandled signal is received, all bets are off

		The behavior of a program when it receives a signal is undefined,
		specially in a multithreaded program.

		There are a few tests in which I check what happens when a signal is
		received and exceptions4c wasn't told to handle it.

		In these tests, the exit code will be implementation-defined, so there's
		no point in checking it.

	2. pthreads-win32/pthread_exit does not meet the specification

		According to PTHREADS specifications:

			"The process shall exit with an exit status of 0 after the last
			thread has been terminated. The behavior shall be as if the
			implementation called exit() with a zero argument at thread
			termination time."

		So, in the event of an uncaught exception, the program SHOULD be
		returning:

			* EXIT_FAILURE	  if the library is in single-thread mode.
			* EXIT_SUCCESS	  if the library is in multi-thread mode.

		But:

		Since the windows implementation of PTHREADS (at least, pthreads-win32,
		the one I am currently using) returns the value passed as parameter to
		pthread_exit (in this case, PTHREAD_CANCELED) as the actual program exit
		code, I won't check the exit code when the tests are running in
		thread-safe mode.

	That's why I am using EXIT_WHATEVER instead of EXIT_SUCCESS or EXIT_FAILURE
	in some of the unit tests.

*/


/*
	TESTS
	________________________________________________________________
*/

# define DEFINE_TEST(CODE, TITLE, DESCRIPTION, EXIT_CODE, OUT, ERR) \
	int test_##CODE##_function(); \
	\
	UnitTest test_##CODE = { \
		/* code */					#CODE, \
		/* title */					TITLE, \
		/* description */			DESCRIPTION, \
		/* function */				test_##CODE##_function, \
		/* expectedExitCode */		EXIT_CODE, \
		/* expectedOutput */		OUT, \
		/* expectedError */			ERR, \
		/* foundExitCode */			0, \
		/* foundOutput */			{ 0 }, \
		/* foundError */			{ 0 }, \
		/* unexpectedExitCode */	0, \
		/* unexpectedOutput */		0, \
		/* unexpectedError */		0, \
		/* passed */				0 \
	}; \
	int test_##CODE##_function()

extern e4c_bool NDEBUG_is_defined;
extern long STDC_VERSION;

E4C_DECLARE_EXCEPTION(WildException);
E4C_DECLARE_EXCEPTION(TamedException);

E4C_DECLARE_EXCEPTION(ChildException);
E4C_DECLARE_EXCEPTION(SiblingException);
E4C_DECLARE_EXCEPTION(ParentException);
E4C_DECLARE_EXCEPTION(GrandparentException);


/*
	SUITES
	________________________________________________________________
*/

# define TEST_DECLARATION(ID) extern UnitTest test_##ID;
# define TEST_ENUMERATION(ID) &test_##ID,

# define SUITE(SUITE_CODE, SUITE_TITLE, SUITE_DESCRIPTION) \
	\
	COLLECTION(TEST_DECLARATION) \
	\
	static UnitTest * testArray_##SUITE_CODE[] = { \
		COLLECTION(TEST_ENUMERATION) \
	\
	}; \
	\
	static TestCollection testCollection_##SUITE_CODE = { \
		/* test */	testArray_##SUITE_CODE, \
		/* count*/	sizeof(testArray_##SUITE_CODE) / sizeof(testArray_##SUITE_CODE[0]) \
	}; \
	\
	TestSuite suite_##SUITE_CODE = { \
		/* title */			SUITE_TITLE, \
		/* description*/	SUITE_DESCRIPTION, \
		/* tests */			&testCollection_##SUITE_CODE, \
		/* stats */			{ \
			/* total */			0, \
			/* passed */		0, \
			/* failed */		0 \
							}, \
		/* passed */		0 \
	};


# define END_SUITE NEW


/*
	RUNNERS
	________________________________________________________________
*/

# define SUITE_DECLARATION(ID) extern TestSuite suite_##ID;
# define SUITE_ENUMERATION(ID) &suite_##ID,

# define SUITE_COLLECTION(COLLECTION_NAME) \
	\
	COLLECTION(SUITE_DECLARATION) \
	\
	static TestSuite * COLLECTION_NAME##_suites[] = { \
		COLLECTION(SUITE_ENUMERATION) \
	}; \
	\
	TestSuiteCollection COLLECTION_NAME = { \
		/* suite */ COLLECTION_NAME##_suites, \
		/* count */ sizeof(COLLECTION_NAME##_suites) / sizeof(COLLECTION_NAME##_suites[0]) \
	};

# define END_SUITE_COLLECTION NEW


typedef int (*TestFunction)();

typedef struct UnitTestStruct				UnitTest;
typedef struct TestSuiteStruct				TestSuite;

typedef struct TestCollectionStruct			TestCollection;
typedef struct TestSuiteCollectionStruct	TestSuiteCollection;

typedef struct TestRunnerStruct				TestRunner;
typedef struct StatisticsStruts				Statistics;

struct StatisticsStruts{

	int						total;
	int						passed;
	int						failed;
};

struct UnitTestStruct{

	const char *			code;
	const char *			title;
	const char *			description;
	TestFunction			function;
	int						expectedExitCode;
	const char *			expectedOutput;
	const char *			expectedError;
	int						foundExitCode;
	char					foundOutput[640];
	char					foundError[1024];
	e4c_bool				unexpectedExitCode;
	e4c_bool				unexpectedOutput;
	e4c_bool				unexpectedError;
	e4c_bool				passed;
};

struct TestSuiteStruct{

	const char *			title;
	const char *			description;
	TestCollection *		tests;
	Statistics				stats;
	e4c_bool				passed;
};

struct TestCollectionStruct{
	UnitTest * *			test;
	int						count;
};

struct TestSuiteCollectionStruct{

	TestSuite * *			suite;
	int						count;
};

struct TestRunnerStruct{

	const char *			filePath;
	int						suiteNumber;
	int						testNumber;
	char					buffer[1024];

	const char *			out;
	const char *			err;
	const char *			report;

	TestSuiteCollection *	suites;

	struct{
		Statistics				tests;
		Statistics				suites;
	}						stats;
};

extern TestRunner newTestRunner(
	const char *			filePath,
	const char *			out,
	const char *			err,
	const char *			report,
	TestSuiteCollection *	suiteCollection
);

extern int runAllTestSuites(TestRunner * runner);


# endif
