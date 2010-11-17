
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <signal.h>

# ifdef _POSIX_SOURCE
#	include <sys/wait.h>
#	define GET_EXIT_CODE(status) WEXITSTATUS(status)
# else
#	define GET_EXIT_CODE(status) (status)
# endif

# include "testing.h"
# include "html.h"


E4C_DEFINE_EXCEPTION(WildException, "This is a wild exception.", RuntimeException);
E4C_DEFINE_EXCEPTION(TamedException, "This is a tamed exception.", RuntimeException);

E4C_DEFINE_EXCEPTION(GrandparentException, "This is a grandparent exception.", RuntimeException);
E4C_DEFINE_EXCEPTION(ParentException, "This is a parent exception.", GrandparentException);
E4C_DEFINE_EXCEPTION(ChildException, "This is a child exception.", ParentException);
E4C_DEFINE_EXCEPTION(SiblingException, "This is a sibling exception.", ParentException);


static void calculate_total(statistics * stats){

	stats->total = stats->passed + stats->warnings + stats->failed;
}

static int cannot_read(const char * file_path){

	printf("Error: Cannot read file: %s\nPlease check the file system.\n", file_path);
	return(EXIT_FAILURE);
}

static int cannot_write(const char * file_path){

	printf("Error: Cannot write file: %s\nPlease check the file system.\n", file_path);
	return(EXIT_FAILURE);
}

static const char * load_file(const char * file_path, char * store, int bytes){

	FILE *			file;
	int				tmp				= ' ';
	int				read			= 0;
	const char *	last_token		= NULL;
	e4c_bool		new_token		= e4c_true;
	e4c_bool		looking_for_token = e4c_true;
# ifdef NDEBUG
	e4c_bool		ready_for_token	= e4c_false;
# endif

	file = fopen(file_path, "r");

	if(file == NULL) exit( cannot_read(file_path) );

	for(read = 0; read < bytes; read++){

		tmp = getc(file);

		if(tmp == EOF) break;

		if(tmp == '<')			store[read] = '{';
		else if(tmp == '>')		store[read] = '}';
		else					store[read] = tmp;

		if(looking_for_token){
			if(tmp == ':'){
# ifdef NDEBUG
				ready_for_token	= e4c_true;
# else
				looking_for_token = e4c_false;
# endif
			}else if(tmp <= ' '){
				new_token = e4c_true;
			}else if(new_token){
				new_token = e4c_false;
				last_token = store + read;
# ifdef NDEBUG
				if(ready_for_token) looking_for_token = e4c_false;
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

	return(last_token);
}

static e4c_bool is_unexpected_token(const char * expected_token, const char * token){

	int index;

	if(expected_token == NULL){
		return(token != NULL);
	}

	if(token == NULL) return(e4c_true);

	for(index = 0; expected_token[index] != '\0'; index++){
		if( token[index] != expected_token[index] ){
			return(e4c_true);
		}
	}
	return(e4c_false);
}

static const char * make_command_line(test_runner * runner){

	sprintf(
		runner->buffer,
		"%s %d %d > %s 2> %s",
		runner->file_path,
		runner->suite_number,
		runner->test_number,
		runner->out,
		runner->err
	);

	return(runner->buffer);
}

static void run_test(test_runner * runner, unit_test * test){

	const char *	command;
	const char *	last_output;
	const char *	last_error;

	command = make_command_line(runner);

	printf("	%s_%s: ", (test->is_requirement ? "requirement" : "unit_test"), test->code);

	test->found_exit_code = GET_EXIT_CODE( system(command) );

	last_output	= load_file( runner->out, test->found_output, sizeof(test->found_output) );
	last_error	= load_file( runner->err, test->found_error, sizeof(test->found_error) );

	test->unexpected_exit_code	= ( test->expected_exit_code	!= EXIT_WHATEVER	&& test->found_exit_code != test->expected_exit_code );
	test->unexpected_error		= ( test->expected_error		!= ERROR_WHATEVER	&& is_unexpected_token(test->expected_error, last_error) );
	test->unexpected_output		= ( test->expected_output		!= NULL				&& is_unexpected_token(test->expected_output, last_output) );

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

static void run_test_suite(test_runner * runner, test_suite * suite){

	int				tests;

	printf("%s %s...\n", (suite->is_requirement ? "Checking" : "Running test suite"), suite->title);

	tests = suite->tests->count;

	suite->stats.passed		= 0;
	suite->stats.warnings	= 0;
	suite->stats.failed		= 0;

	for(runner->test_number = 0; runner->test_number < tests; runner->test_number++){

		unit_test * test = suite->tests->test[runner->test_number];

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
					break;

				case STATUS_WARNING:
					if(test->is_requirement){
						runner->stats.requirements.warnings++;
					}
					if(!test->is_requirement || suite->is_requirement){
						suite->stats.warnings++;
					}
					break;

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

static void generate_report(test_runner * runner){

	FILE * report;

	report = fopen(runner->report, "w");

	if(report == NULL) exit( cannot_write(runner->report) );

	print_html(runner, report);

	fclose(report);
}

test_runner new_test_runner(const char * file_path, const char * out, const char * err, const char * report, test_suite_collection * suites){

	test_runner runner;
	statistics empty_stats = {0, 0, 0, 0};

	runner.file_path			= file_path;
	runner.out					= out;
	runner.err					= err;
	runner.report				= report;
	runner.suites				= suites;

	runner.stats.suites			= empty_stats;
	runner.stats.requirements	= empty_stats;
	runner.stats.tests			= empty_stats;

	return(runner);
}

int run_all_test_suites(test_runner * runner){

	printf("Running all the tests...\n\n");

	for(runner->suite_number = 0; runner->suite_number < runner->suites->count; runner->suite_number++){

		test_suite * suite = runner->suites->suite[runner->suite_number];

		run_test_suite(runner, suite);

		if(!suite->is_requirement){

			switch(suite->status){

				case STATUS_PASSED:
					runner->stats.suites.passed++;
					break;

				case STATUS_WARNING:
					runner->stats.suites.warnings++;
					break;

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
	remove(runner->out);
	remove(runner->err);

	if(runner->report != NULL){

		printf("\n\nGenerating report....\n");

		generate_report(runner);

		/* hopefully this will open the report in the default browser */
		system(runner->report);
	}

	printf("\tDone.\n");

	return(EXIT_SUCCESS);
}

int parse_command_line(int argc, char * argv[], test_suite_collection * suite_collection, const char * report, const char * out, const char * err){

	long			version;
	int				suite_number;
	int				test_number;
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

		runner = new_test_runner(argv[0], out, err, report, suite_collection);

		result = run_all_test_suites(&runner);

		return(result);
	}

	/* wrong number of arguments */
	if(argc != 3){
		printf("Error: please run this program without arguments.\n");
		return(EXIT_FAILURE);
	}

	/* run a specific test */

	suite_number		= atoi(argv[1]);
	test_number			= atoi(argv[2]);

	if(suite_number < 0 || suite_number >= suite_collection->count){
		printf("Error: wrong test suite number (%d).\n", suite_number);
		return(EXIT_FAILURE);
	}
	suite = suite_collection->suite[suite_number];

	if(test_number < 0 || test_number >= suite->tests->count){
		printf("Error: wrong unit test number (%d).\n", test_number);
		return(EXIT_FAILURE);
	}

	test = suite->tests->test[test_number];

	return( test->function() );
}
