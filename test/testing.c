# include <stdio.h>
# include <string.h>
# include <time.h>
# include <signal.h>

# if !defined(MAC) && ( \
		defined(__APPLE__) \
	||	defined(__MACH__) \
	||	defined(macintosh) \
	||	defined(Macintosh) \
)
#	define MAC
# endif

# if !defined(LINUX) && ( \
		defined(_LINUX) \
	||	defined(__LINUX) \
	||	defined(__LINUX__) \
	||	defined(linux) \
	||	defined(_linux) \
	||	defined(__linux) \
	||	defined(__linux__) \
	||	defined(gnu_linux) \
	||	defined(_gnu_linux) \
	||	defined(__gnu_linux) \
	||	defined(__gnu_linux__) \
)
#	define LINUX
# endif

# if !defined(WINDOWS) && ( \
		defined(_WINDOWS) \
	||	defined(__WINDOWS) \
	||	defined(__WINDOWS__) \
	||	defined(WINNT) \
	||	defined(_WINNT) \
	||	defined(__WINNT) \
	||	defined(__WINNT__) \
	||	defined(WIN64) \
	||	defined(_WIN64) \
	||	defined(__WIN64) \
	||	defined(__WIN64__) \
	||	defined(WIN32) \
	||	defined(_WIN32) \
	||	defined(__WIN32) \
	||	defined(__WIN32__) \
)
#	define WINDOWS
# endif

# if !defined(MSDOS) && ( \
		defined(_MSDOS) \
	||	defined(__MSDOS) \
	||	defined(__MSDOS__) \
)
# define MSDOS
# endif

# if defined(MAC)
#	define PLATFORM_OPEN_REPORT "open %s"
# elif defined(LINUX)
#	define PLATFORM_OPEN_REPORT "gnome-open %s"
# elif defined(WINDOWS)
#	define PLATFORM_OPEN_REPORT "start %s"
# endif


/*
	When we run a test (through the function `system`), we need to get the exit
	code returned by `main` to check whether it succeeded or failed.

	On most platforms, `system` does not return the *exit code*, but a
	*termination status*. This termination status must be passed to the macro
	`WEXITSTATUS` in order to get the actual exit code. `WEXITSTATUS` is defined
	in the header file <sys/wait.h>.

	By default, we will try and include this header, unless GET_EXIT_CODE is
	defined at compile time.

	If the platform is well known not to have that header, then we won't include
	it (unless HAVE_SYS_WAIT_H is defined).

	We will also hide this include from Splint to prevent configuration issues
	related to header inclusion paths.
*/
# ifndef GET_EXIT_CODE
#	if !defined(HAVE_SYS_WAIT_H) && ( \
			defined(WINDOWS) \
		||	defined(MSDOS) \
		||	defined(S_SPLINT_S) \
	)
#		define GET_EXIT_CODE(status) (status)
#	else
/*
		In case your platform does not have <sys/wait.h>, please define
		GET_EXIT_CODE at compile time.

		This macro must take the status value returned by the `system`
		function, and return the actual exit status of the executed process.

		If your platform's `system` function already returns the actual exit
		status, you can define GET_EXIT_CODE(status) just like this:

			# define GET_EXIT_CODE(status) (status)
*/
#		include <sys/wait.h>
#		define GET_EXIT_CODE(status) WEXITSTATUS(status)
#	endif
# endif


# include "testing.h"
# include "html.h"


E4C_DEFINE_EXCEPTION(WildException, "This is a wild exception.", RuntimeException);
E4C_DEFINE_EXCEPTION(TamedException, "This is a tamed exception.", RuntimeException);

E4C_DEFINE_EXCEPTION(GrandparentException, "This is a grandparent exception.", RuntimeException);
E4C_DEFINE_EXCEPTION(ParentException, "This is a parent exception.", GrandparentException);
E4C_DEFINE_EXCEPTION(ChildException, "This is a child exception.", ParentException);
E4C_DEFINE_EXCEPTION(SiblingException, "This is a sibling exception.", ParentException);


# ifdef PLATFORM_OPEN_REPORT

	static void open_report(/*@notnull@*/ test_runner * runner)
	/*@globals
		fileSystem,
		internalState
	@*/
	/*@modifies
		fileSystem,
		internalState,

		runner->buffer
	@*/
	{

		SAFE_SPRINTF(SAFE_ARRAY(runner->buffer), PLATFORM_OPEN_REPORT, runner->report);

		(void)system(runner->buffer);
	}

# else

	static void open_report(/*@notnull@*/ test_runner * runner)
	/*@globals
		fileSystem,
		internalState
	@*/
	/*@modifies
		fileSystem,
		internalState
	@*/
	{

		printf("\tPlease read \"%s\" for further information.\n", runner->report);
	}

# endif

static void calculate_total(statistics * stats)
/*@modifies
	stats
@*/
{

	stats->total = stats->passed + stats->warnings + stats->failed;
}

static int cannot_read(const char * file_path)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	printf("Error: Cannot read file: %s\nPlease check the file system.\n", file_path);

	return(EXIT_FAILURE);
}

static int cannot_write(const char * file_path)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	printf("Error: Cannot write file: %s\nPlease check the file system.\n", file_path);

	return(EXIT_FAILURE);
}

static /*@null@*/ /*@temp@*/ const char * load_file(const char * file_path, /*@returned@*/ char * store, size_t bytes)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	store
@*/
{

	FILE *			file;
	int				tmp				= (int)' ';
	size_t			read			= 0;
	const char *	last_token		= NULL;
	E4C_BOOL		new_token		= E4C_TRUE;
	E4C_BOOL		looking_for_token = E4C_TRUE;
# ifdef NDEBUG
	E4C_BOOL		ready_for_token	= E4C_FALSE;
# endif

	file = fopen(file_path, "r");

	if(file == NULL){
		exit( cannot_read(file_path) );
	}

	for(read = 0; read < bytes; read++){

		tmp = getc(file);

		if(tmp == EOF){
			break;
		}

		/*@-boundswrite@*/
		if(tmp == (int)'<'){
			store[read] = '{';
		}else if(tmp == (int)'>'){
			store[read] = '}';
		}else{
			store[read] = (char)tmp;
		}
		/*@=boundswrite@*/

		if(looking_for_token){
			if(tmp == (int)':'){
# ifdef NDEBUG
				ready_for_token	= E4C_TRUE;
# else
				looking_for_token = E4C_FALSE;
# endif
			}else if(tmp <= (int)' '){
				new_token = E4C_TRUE;
			}else if(new_token){
				new_token = E4C_FALSE;
				/*@-ptrarith@*/
				last_token = store + read;
				/*@=ptrarith@*/
# ifdef NDEBUG
				if(ready_for_token) looking_for_token = E4C_FALSE;
# endif
			}
		}
	}

	(void)fclose(file);

	/*@-boundswrite@*/
	if(read < bytes){
		store[read] = '\0';
	}else{
		store[read - 1] = '\0';
		store[read - 2] = '.';
		store[read - 3] = '.';
		store[read - 4] = '.';
	}
	/*@=boundswrite@*/

	return(last_token);
}

static E4C_BOOL is_unexpected_token(/*@null@*/ const char * expected_token, /*@null@*/ const char * token)
/*@*/
{

	int index;

	if(expected_token == NULL){
		return(token != NULL);
	}

	if(token == NULL){
		return(E4C_TRUE);
	}

	for(index = 0; expected_token[index] != '\0'; index++){
		/*@-boundsread@*/
		if( token[index] != expected_token[index] ){
			return(E4C_TRUE);
		}
		/*@=boundsread@*/
	}

	return(E4C_FALSE);
}

static void make_command_line(/*@notnull@*/ test_runner * runner)
/*@modifies
	runner->buffer
@*/
{

	SAFE_SPRINTF(
		SAFE_ARRAY(runner->buffer),
		"%s %u %u > %s 2> %s",
		runner->file_path,
		(unsigned int)runner->suite_number,
		(unsigned int)runner->test_number,
		runner->out,
		runner->err
	);
}

static void run_test(/*@notnull@*/ test_runner * runner, /*@notnull@*/ unit_test * test)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	runner->buffer,
	test
@*/
{

	const char *	last_output;
	const char *	last_error;
	int				termination_status;

	make_command_line(runner);

	printf("	%s_%s: ", (test->is_requirement ? "requirement" : "unit_test"), test->code);

	termination_status			= system(runner->buffer);
	test->found_exit_code		= GET_EXIT_CODE(termination_status);

	last_output	= load_file( runner->out, test->found_output, sizeof(test->found_output) );
	last_error	= load_file( runner->err, test->found_error, sizeof(test->found_error) );

	test->unexpected_exit_code	= ( test->expected_exit_code	!= EXIT_WHATEVER	&& test->found_exit_code != test->expected_exit_code );
	test->unexpected_error		= ( test->expected_error		!= ERROR_WHATEVER	&& is_unexpected_token(test->expected_error, last_error) );
	test->unexpected_output		= ( test->expected_output		!= OUTPUT_WHATEVER	&& is_unexpected_token(test->expected_output, last_output) );

	if(!test->unexpected_exit_code && !test->unexpected_output && !test->unexpected_error){
		test->status = STATUS_PASSED;
	}else if(!test->is_critical){
		test->status = STATUS_WARNING;
	}else{
		test->status = STATUS_FAILED;
	}

	if(test->status == STATUS_PASSED){
		printf("%s\n",
			(test->is_requirement ? "fulfilled" : "passed")
		);
	}else{
		printf("%s [%c%c%c]\n",
			(test->is_requirement
				? (test->is_critical ? "NOT FULFILLED!"	: "not fulfilled")
				: (test->is_critical ? "FAILED!"		: "failed!")
			),
			(test->unexpected_exit_code	? 'X' : '1'),
			(test->unexpected_output	? 'O' : '2'),
			(test->unexpected_error		? 'E' : '3')
		);
	}
}

static void run_test_suite(test_runner * runner, test_suite * suite)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	runner,
	runner->buffer,
	suite
@*/
{

	size_t tests;

	printf("%s %s...\n", (suite->is_requirement ? "Checking" : "Running test suite"), suite->title);

	tests = suite->tests->count;

	suite->stats.passed		= 0;
	suite->stats.warnings	= 0;
	suite->stats.failed		= 0;

	for(runner->test_number = 0; runner->test_number < tests; runner->test_number++){

		/*@-boundsread@*/
		unit_test * test = suite->tests->test[runner->test_number];
		/*@=boundsread@*/

		run_test(runner, test);

		{

			switch(test->status){

				case STATUS_PASSED:
					if(test->is_requirement){
						runner->stats.requirements.passed++;
					}
					if(!test->is_requirement || suite->is_requirement){
						suite->stats.passed++;
					}
					/*@switchbreak@*/ break;

				case STATUS_WARNING:
					if(test->is_requirement){
						runner->stats.requirements.warnings++;
					}
					if(!test->is_requirement || suite->is_requirement){
						suite->stats.warnings++;
					}
					/*@switchbreak@*/ break;

				default: /* STATUS_FAILED */
					if(test->is_requirement){
						runner->stats.requirements.failed++;
					}
					if(!test->is_requirement || suite->is_requirement){
						suite->stats.failed++;
					}
			}
		}
	}

	calculate_total(&suite->stats);

	suite->status = (suite->stats.failed > 0 ? STATUS_FAILED : (suite->stats.warnings > 0 ? STATUS_WARNING : STATUS_PASSED) );
}

static void generate_report(/*@notnull@*/ test_runner * runner)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	FILE * report;

	report = fopen(runner->report, "w");

	if(report == NULL){
		exit( cannot_write(runner->report) );
	}

	print_html(runner, report);

	(void)fclose(report);
}

static test_runner new_test_runner(const char * file_path, const char * out, const char * err, const char * report, test_suite_collection * suites)
/*@globals
	internalState
@*/
/*@modifies
	internalState
@*/
{

	test_runner runner;
	statistics empty_stats = {0, 0, 0, 0};

	runner.file_path			= file_path;
	runner.suite_number			= 0;
	runner.test_number			= 0;

	runner.buffer[0]			= '\0';

	runner.out					= out;
	runner.err					= err;
	runner.report				= report;
	runner.suites				= suites;

	runner.stats.suites			= empty_stats;
	runner.stats.requirements	= empty_stats;
	runner.stats.tests			= empty_stats;

	return(runner);
}

static int run_all_test_suites(/*@notnull@*/ test_runner * runner)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	runner,
	runner->buffer,
	runner->suite_number,
	runner->suites->suite,
	runner->stats
@*/
{

	const char * summary1;
	const char * summary2;

	printf("Running all the tests...\n\n");

	for(runner->suite_number = 0; runner->suite_number < runner->suites->count; runner->suite_number++){

		/*@-boundsread@*/
		test_suite * suite = runner->suites->suite[runner->suite_number];
		/*@=boundsread@*/

		run_test_suite(runner, suite);

		if(!suite->is_requirement){

			switch(suite->status){

				case STATUS_PASSED:
					runner->stats.suites.passed++;
					/*@switchbreak@*/ break;

				case STATUS_WARNING:
					runner->stats.suites.warnings++;
					/*@switchbreak@*/ break;

				default: /* STATUS_FAILED */
					runner->stats.suites.failed++;
			}

			runner->stats.tests.passed		+= suite->stats.passed;
			runner->stats.tests.warnings	+= suite->stats.warnings;
			runner->stats.tests.failed		+= suite->stats.failed;
		}
	}

	calculate_total(&runner->stats.requirements);
	calculate_total(&runner->stats.tests);
	calculate_total(&runner->stats.suites);

	/* delete temporary files */
	(void)remove(runner->out);
	(void)remove(runner->err);

	if(runner->report != NULL){

		printf("\n\nGenerating report...\n");

		generate_report(runner);

		printf("\tTest report created.\n");

		open_report(runner);
	}

	if(runner->stats.suites.failed > 0){
		summary1 = "Some of the tests failed.";
	}else if(runner->stats.suites.warnings > 0){
		summary1 = "Some *non-critical* test didn't pass.";
	}else{
		summary1 = "All tests passed successfully.";
	}

	if(runner->stats.suites.failed == 0 && runner->stats.suites.warnings == 0){
		summary2 = "";
	}else if(runner->stats.requirements.total == 0){
		summary2 = "\n\tPlease verify platform requirements.";
	}else if(runner->stats.requirements.failed > 0 || runner->stats.requirements.warnings > 0){
		summary2 = "\n\tIt could be related to some error on **platform requirements**.";
	}else{
		summary2 = "\n\tPlease report it at: http://code.google.com/p/exceptions4c/";
	}

	printf("\n%s%s\n\n", summary1, summary2);

	return(EXIT_SUCCESS);
}

int parse_command_line(int argc, char * argv[], test_suite_collection * suite_collection, const char * report, const char * out, const char * err){

	long			version;
	size_t			suite_number;
	size_t			test_number;
	test_suite *	suite;
	unit_test *		test;

	/* check library version numbers */
	version = e4c_library_version();
	if(version != E4C_VERSION_NUMBER){
		printf(
			"Error: the library and header version numbers don't match.\n"
			"\n"
			" * run-time version number:     %ld\n"
			" * compile-time version number: %ld\n"
			"\n"
			"Please compile the library again with its corresponding header file.\n",
			version, E4C_VERSION_NUMBER
		);
		return(EXIT_FAILURE);
	}

	/* run all the tests */
	if(argc == 1){
		test_runner runner;
		int result;

		printf("\n--------------------------------\n EXCEPTIONS4C TESTING FRAMEWORK \n--------------------------------\n\n");
		printf("library version: %ld\n\n", version);

		/*@-boundsread@*/
		runner = new_test_runner(argv[0], out, err, report, suite_collection);
		/*@=boundsread@*/

		result = run_all_test_suites(&runner);

		return(result);
	}

	/* wrong number of arguments */
	if(argc != 3){
		printf("Error: please run this program without arguments.\n");
		return(EXIT_FAILURE);
	}

	/* run a specific test */

	/*@-boundsread@*/
	suite_number		= (size_t)atoi(argv[1]);
	test_number			= (size_t)atoi(argv[2]);
	/*@=boundsread@*/

	if(suite_number >= suite_collection->count){
		printf("Error: wrong test suite number (%u).\n", (unsigned int)suite_number);
		return(EXIT_FAILURE);
	}

	/*@-boundsread@*/
	suite = suite_collection->suite[suite_number];
	/*@=boundsread@*/

	if(test_number >= suite->tests->count){
		printf("Error: wrong unit test number (%u).\n", (unsigned int)test_number);
		return(EXIT_FAILURE);
	}

	/*@-boundsread@*/
	test = suite->tests->test[test_number];
	/*@=boundsread@*/

	return( test->function() );
}
