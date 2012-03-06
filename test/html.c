
# include <stdio.h>
# include <string.h>
# include <time.h>

# include "html.h"
# include "testing.h"
# include "platform.h"
# include "macros.h"


# define PRINT_STATELESS_MACRO(report, macro) \
		if( IS_DEFINED(macro) ){ \
			fprintf(report, "%s:%%20%s%%0A", macro.name, ( IS_DEFINED(macro) ? "defined" : "undefined") ); \
		}

# define PRINT_STATEFUL_MACRO(report, macro) \
		if( IS_DEFINED(macro) ){ \
			fprintf( report, "%s:%%20%s%%20(%ld)%%0A", macro.name, macro.description, macro.value ); \
		}

# define PRINT_UNDEFINED_MACRO(report, macro) \
		if( IS_NOT_DEFINED(macro) ){ \
			fprintf(report, "%%20%s", macro.name); \
		}

# define SUMMARY_MAX_ITEMS (size_t)5

# define MIN(x, y) (x < y ? x : y )
# define ELIDED(x) (x > SUMMARY_MAX_ITEMS)

# define AFTER_SUMMARY_ITEM(index, total) \
	( \
		index + 1 < MIN(SUMMARY_MAX_ITEMS, total) \
		? \
		( ELIDED(total) || index + 2 < MIN(SUMMARY_MAX_ITEMS, total) ? ", " : " and " ) \
		: \
		( ELIDED(total) ? "..." : "." ) \
	)

# define HUMAN_EXIT_CODE(v)	\
	( v == EXIT_WHATEVER ? "(any value)" : (v == EXIT_SUCCESS ? "EXIT_SUCCESS" : ( v == EXIT_FAILURE ? "EXIT_FAILURE" : "(a specific value)") ) )

# define HUMAN_STATUS(status) \
	( (status) == STATUS_PASSED ? "passed" : ((status) == STATUS_WARNING ? "warning" : "failed") )

# define HUMAN_TYPE(is_requirement, otherwise) \
	(is_requirement ? "requirement" : otherwise)


static void print_stateful_macro(/*@notnull@*/ FILE * report, struct stateful_macro macro)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	if( IS_DEFINED(macro) ){
		fprintf(report,
			"<div>"
				"<div class=\"info image icon24\"></div>"
				"<code>%s</code> is defined as <code>%ld</code>, i.e. <em>%s (%s)</em>"
			"</div>",
		macro.name, macro.value, macro.description, (macro.detail == NULL ? "" : macro.detail) );
	}else{
		fprintf(report,
			"<div>"
				"<div class=\"info image icon24\"></div>"
				"<code>%s</code> is not defined, i.e. <em>%s</em>"
			"</div>",
		macro.name, macro.description);
	}
}

static void print_stateless_macro(/*@notnull@*/ FILE * report, struct stateless_macro macro)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	fprintf(report,
		"<div>"
			"<div class=\"info image icon24\"></div>"
			"<code>%s</code> is%s defined, i.e. <em>%s</em>"
		"</div>",
	macro.name, ( IS_DEFINED(macro) ? "" : " not"), (macro.detail == NULL ? "" : macro.detail) );
}

static void print_percentage(/*@out@*/ char * SAFE_BUFFER_ARG(buffer, size), size_t fraction, size_t total)
/*@requires
	maxSet(buffer) >= (size - 1)
@*/
/*@modifies
	buffer
@*/
{

	size_t value		= ( (size_t)10000 * fraction) / total;
	size_t percentage	= value / 100;
	size_t decimal		= value % 100;

	if(decimal == 0){

		SAFE_SPRINTF(SAFE_BUFFER(buffer, size), "%u%%", (unsigned int)percentage);

	}else{

		SAFE_SPRINTF(SAFE_BUFFER(buffer, size), "%u.%02u%%", (unsigned int)percentage, (unsigned int)decimal);

	}

}

static void print_exit_code(/*@out@*/ char * SAFE_BUFFER_ARG(buffer, size), int exit_code)
/*@requires maxSet(buffer) >= (size - 1)@*/
/*@modifies
	buffer
@*/
{

	if(exit_code == EXIT_SUCCESS){

		SAFE_SPRINTF(SAFE_BUFFER(buffer, size), "EXIT_SUCCESS");

	}else if(exit_code == EXIT_FAILURE){

		SAFE_SPRINTF(SAFE_BUFFER(buffer, size), "EXIT_FAILURE");

	}else{

		SAFE_SPRINTF(SAFE_BUFFER(buffer, size), "%d", exit_code);
	}
}

static void print_graph(/*@notnull@*/ FILE * report, statistics stats, /*@notnull@*/ const char * what)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	char	percent_passed[8];
	char	percent_failed[8];
	size_t	passed	= stats.passed;
	size_t	total	= stats.total;
	size_t	failed	= stats.failed + stats.warnings;

	if(total > 0){

		print_percentage(SAFE_BUFFER( percent_passed, sizeof(percent_passed) ), passed, total);
		print_percentage(SAFE_BUFFER( percent_failed, sizeof(percent_failed) ), failed, total);

		fprintf(report,
			"<div class=\"paragraph\">"
				"<div title=\"%u %s failed (%s)\" class=\"shadowed graph percent_failed\">"
					"<div title=\"%u %s passed (%s)\" class=\"graph percent_passed\" style=\"width: %s;\"></div>"
				"</div>"
				"%s passed"
			"</div>",
			(unsigned int)failed,			what,		percent_failed,
			(unsigned int)passed,			what,		percent_passed,		percent_passed,
			percent_passed
		);
	}
}

static void print_unit_test(/*@notnull@*/ const test_suite * suite, /*@notnull@*/ const unit_test * test, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	char	expected_value[128];
	char	expected_stdin[128];
	char	expected_stderr[128];
	char	found[64];
	char	expected[64];

	print_exit_code(SAFE_BUFFER( found, sizeof(found) ), test->found_exit_code);

	if(test->expected_exit_code == EXIT_WHATEVER){

		SAFE_SPRINTF(SAFE_ARRAY(expected_value), "<span class=\"console\">%s</span> <em>(any value is OK)</em>", found);

	}else if(test->unexpected_exit_code){

		print_exit_code(SAFE_BUFFER( expected, sizeof(expected) ), test->expected_exit_code);

		SAFE_SPRINTF(SAFE_ARRAY(expected_value), "<span class=\"console\">%s</span> (expecting <span class=\"console\">%s</span>)", found, expected);

	}else{

		SAFE_SPRINTF(SAFE_ARRAY(expected_value), "Expecting <span class=\"console\">%s</span>", found);
	}

	if(test->expected_output == OUTPUT_WHATEVER){

		SAFE_SPRINTF(SAFE_ARRAY(expected_stdin), "<em>Any output is OK</em>");

	}else if(test->expected_output == NULL){

		SAFE_SPRINTF(SAFE_ARRAY(expected_stdin), "Expecting no output");

	}else{

		SAFE_SPRINTF(SAFE_ARRAY(expected_stdin), "Expecting <span class=\"console\">%s</span>", test->expected_output);
	}

	if(test->expected_error == ERROR_WHATEVER){

		SAFE_SPRINTF(SAFE_ARRAY(expected_stderr), "<em>Anything is OK</em>");

	}else if(test->expected_error == NULL){

		SAFE_SPRINTF(SAFE_ARRAY(expected_stderr), "Expecting no error");

	}else{

		SAFE_SPRINTF(SAFE_ARRAY(expected_stderr), "Expecting <span class=\"console\">%s</span>", test->expected_error);

	}

	fprintf(report,
		"<div>"
			"<div class=\"%s_%s image icon24\"></div>"
			"<h3>"
				"<a name=\"%s_%s\">%s_%s: %s</a>"
			"</h3>"
		"</div>"
		"<div class=\"test_section\">"

			"<div class=\"test_section_content\">"

				"<div class=\"description paragraph\">"
					"%s%s"
				"</div>",
	HUMAN_TYPE(test->is_requirement, "unit_test"),
	HUMAN_STATUS(test->status),
	HUMAN_TYPE(test->is_requirement, "unit_test"),
	test->code,
	HUMAN_TYPE(test->is_requirement, "unit_test"),
	test->code,
	test->title,
	test->description1,
	(test->description2 == NULL ? "" : test->description2)
	);

	if(test->status != STATUS_PASSED){

		if(test->at_failure != NULL){
			fprintf(report,
					"<div class=\"paragraph\">"
						"<div class=\"%s image icon24\"></div>"
						"%s"
					"</div>",
			( test->status == STATUS_WARNING ? "warning" : "failed" ),
			test->at_failure);
		}

		fprintf(report,
				"<div class=\"paragraph\">"
					"<div class=\"bug image icon24\"></div>"
					"<a href=\"http://code.google.com/p/exceptions4c/issues/entry?"
						"template=Defect%%20report%%20from%%20testing&amp;"
						"summary=%s_%s%%20failed&amp;"
						"comment="
							"%s_%s%%20failed.%%0A"
							"%%0A"
							"%%20Library%%20version:%%20%ld%%0A"
							"%%20%%20%%20%%20Architecture:%%20%s%%0A"
							"Operating%%20system:%%20%s%%0A"
							"%%20%%20%%20%%20IDE/Compiler:%%20%s%%20%s(%ld)%%0A%%0A",
		HUMAN_TYPE(test->is_requirement, "unit_test"), test->code,
		HUMAN_TYPE(test->is_requirement, "unit_test"), test->code,
		E4C_VERSION_NUMBER,
		PLATFORM_ARCH,
		PLATFORM_OS,
		PLATFORM_COMPILER,
		(PLATFORM_COMPILER_VERSION == NULL ? "" : PLATFORM_COMPILER_VERSION),
		(long)PLATFORM_COMPILER_VERSION_NUMBER
		);

		PRINT_STATELESS_MACRO(report,	macros.e4c_nokeywords);
		PRINT_STATELESS_MACRO(report,	macros.e4c_threadsafe);
		PRINT_STATELESS_MACRO(report,	macros.reentrant);
		PRINT_STATELESS_MACRO(report,	macros.thread_safe);
		PRINT_STATELESS_MACRO(report,	macros.ndebug);
		PRINT_STATELESS_MACRO(report,	macros.stdc);
		PRINT_STATELESS_MACRO(report,	macros.strict_ansi);
		PRINT_STATEFUL_MACRO(report,	macros.stdc_version);
		PRINT_STATELESS_MACRO(report,	macros.isoc99_source);
		PRINT_STATEFUL_MACRO(report,	macros.cplusplus);
		PRINT_STATELESS_MACRO(report,	macros.gnu_source);
		PRINT_STATELESS_MACRO(report,	macros.posix_source);
		PRINT_STATEFUL_MACRO(report,	macros.posix_c_source);
		PRINT_STATEFUL_MACRO(report,	macros.posix_version);
		PRINT_STATEFUL_MACRO(report,	macros.posix2_c_version);
		PRINT_STATEFUL_MACRO(report,	macros.xopen_source);
		PRINT_STATEFUL_MACRO(report,	macros.xopen_version);
		PRINT_STATELESS_MACRO(report,	macros.xopen_source_extended);
		PRINT_STATELESS_MACRO(report,	macros.bsd_source);
		PRINT_STATELESS_MACRO(report,	macros.svid_source);

		fprintf(report, "%%0AUndefined%%20macros:");

		PRINT_UNDEFINED_MACRO(report,	macros.e4c_nokeywords);
		PRINT_UNDEFINED_MACRO(report,	macros.e4c_threadsafe);
		PRINT_UNDEFINED_MACRO(report,	macros.reentrant);
		PRINT_UNDEFINED_MACRO(report,	macros.thread_safe);
		PRINT_UNDEFINED_MACRO(report,	macros.ndebug);
		PRINT_UNDEFINED_MACRO(report,	macros.stdc);
		PRINT_UNDEFINED_MACRO(report,	macros.strict_ansi);
		PRINT_UNDEFINED_MACRO(report,	macros.stdc_version);
		PRINT_UNDEFINED_MACRO(report,	macros.isoc99_source);
		PRINT_UNDEFINED_MACRO(report,	macros.cplusplus);
		PRINT_UNDEFINED_MACRO(report,	macros.gnu_source);
		PRINT_UNDEFINED_MACRO(report,	macros.posix_source);
		PRINT_UNDEFINED_MACRO(report,	macros.posix_c_source);
		PRINT_UNDEFINED_MACRO(report,	macros.posix_version);
		PRINT_UNDEFINED_MACRO(report,	macros.posix2_c_version);
		PRINT_UNDEFINED_MACRO(report,	macros.xopen_source);
		PRINT_UNDEFINED_MACRO(report,	macros.xopen_version);
		PRINT_UNDEFINED_MACRO(report,	macros.xopen_source_extended);
		PRINT_UNDEFINED_MACRO(report,	macros.bsd_source);
		PRINT_UNDEFINED_MACRO(report,	macros.svid_source);

		fprintf(report,
							"%%0A%%0A"
							"Exit%%20code:%%20%s%%20(%s)%%0A"
							"%%20%%20%%20Output:%%20%s%%0A"
							"%%20%%20%%20%%20Error:%%20%s%%0A"
							"%%0A"
							"Please%%20provide%%20any%%20additional%%20information%%20below:%%0A"
							"...%%0A"
					"\">"
						"Create a new issue"
					"</a>"
				"</div>",
		(test->unexpected_exit_code	? "NOK" : "OK"), found,
		(test->unexpected_output	? "NOK%20...%20(please%20copy%20and%20paste%20it%20below)" : "OK"),
		(test->unexpected_error		? "NOK%20...%20(please%20copy%20and%20paste%20it%20below)" : "OK")
		);
	}

	fprintf(report,
				"<div class=\"shadowed details_box\">"
					"<div onclick=\"toggleDetails('div_%s');\">"
						"<div id=\"div_%s_button\" class=\"unfold image icon24\"></div>"
						"<div class=\"details_heading\">"
							"Result:"
						"</div>"
						"<div id=\"div_%s_summary\" class=\"summary\">"
							"%s"
						"</div>"
					"</div>"
					"<table id=\"div_%s_detailed\" class=\"hidden details test_section_parameters\">"
						"<tbody>"
							"<tr>"
								"<td>Suite</td>"
								"<td>"
									"<div class=\"%s_%s image icon24\"></div>"
									"<a href=\"#test_suite_%s\">%s</a>"
								"</td>"
							"</tr>"
							"<tr>"
								"<td>Exit code</td>"
								"<td>"
									"<div class=\"%s image icon24\"></div>"
									"%s"
								"</td>"
							"</tr>",
	test->code, test->code, test->code,
	HUMAN_STATUS(test->status),
	test->code,
	HUMAN_TYPE(suite->is_requirement, "test_suite"),
	HUMAN_STATUS(suite->status), suite->title, suite->title,
	(test->unexpected_exit_code ? "failed" : "passed"),
	expected_value
	);

	fprintf(report,
							"<tr>"
								"<td>Output</td>"
								"<td>"
									"<div class=\"%s image icon24\"></div>"
										"%s"
									"<br/>"
									"<textarea cols=\"80\" rows=\"8\" class=\"%s\">%s</textarea>"
								"</td>"
							"</tr>"
							"<tr>"
								"<td>Error</td>"
								"<td>"
									"<div class=\"%s image icon24\"></div>"
										"%s"
									"<br/>"
									"<textarea cols=\"80\" rows=\"8\" class=\"%s\">%s</textarea>"
								"</td>"
							"</tr>"
							"<tr>"
								"<td>Status</td>"
								"<td>"
									"<div class=\"%s image icon24\"></div>"
									"%s"
								"</td>"
							"</tr>"
						"</tbody>"
					"</table>"
				"</div>",
	(test->unexpected_output ? "failed" : "passed"),
	expected_stdin,
	(*test->found_output != '\0' ? "console" : "hidden"), test->found_output,
	(test->unexpected_error ? "failed" : "passed"),
	expected_stderr,
	(*test->found_error != '\0' ? "console" : "hidden"), test->found_error,
	HUMAN_STATUS(test->status),
	HUMAN_STATUS(test->status)
	);

	fprintf(report,
				"<div class=\"paragraph\">"
					"<div class=\"return image icon24\"></div>"
					"<a href=\"#top\">"
						"Return to top"
					"</a>"
				"</div>"
			"</div>"
		"</div>"
	);

}

static void print_test_suite(/*@notnull@*/ const test_suite * suite, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	size_t tests;
	size_t test_index;

	fprintf(report,
		"<div>"
			"<div class=\"%s_%s image icon24\"></div>"
			"<h3>"
				"<a name=\"test_suite_%s\">%s</a>"
			"</h3>"
		"</div>"
		"<div class=\"suite_section\">"

			"<div class=\"description paragraph\">"
				"%s%s"
			"</div>"

			"<div class=\"paragraph\">"
				"<div class=\"stats image icon24\"></div>"
				"<strong>%u</strong> %s; <strong>%u</strong> passed; <strong>%u</strong> failed."
			"</div>",
	HUMAN_TYPE(suite->is_requirement, "test_suite"),
	HUMAN_STATUS(suite->status),
	suite->title, suite->title,
	suite->description1,
	(suite->description2 == NULL ? "" : suite->description2),
	(unsigned int)suite->stats.total,
	(suite->is_requirement ? "requirements" : "unit test"),
	(unsigned int)suite->stats.passed,
	(unsigned int)(suite->stats.warnings +	suite->stats.failed) );

	print_graph( report, suite->stats, (suite->is_requirement ? "requirement(s)" : "unit test(s)") );

	fprintf(report,
		"<div class=\"shadowed details_box\">"
			"<div onclick=\"toggleDetails('div_%s');\">"
				"<div id=\"div_%s_button\" class=\"unfold image icon24\"></div>"
				"<div class=\"details_heading\">"
					"Unit tests:"
				"</div>"
				"<div id=\"div_%s_summary\" class=\"summary\">",
	suite->title, suite->title, suite->title);

	tests = suite->tests->count;
	for(test_index = 0; test_index < tests && test_index < SUMMARY_MAX_ITEMS; test_index++){

		/*@-boundsread@*/
		const unit_test * test = suite->tests->test[test_index];
		/*@=boundsread@*/

		fprintf(report,
						"<div class=\"%s_%s image icon24\"></div>"
						"<a href=\"#%s_%s\" title=\"%s\">"
							"%s_%s"
						"</a>%s",
		HUMAN_TYPE(test->is_requirement, "unit_test"),
		HUMAN_STATUS(test->status),
		HUMAN_TYPE(test->is_requirement, "unit_test"),
		test->code, test->title,
		HUMAN_TYPE(test->is_requirement, "unit_test"), test->code,
		AFTER_SUMMARY_ITEM(test_index, tests) );
	}

	fprintf(report,
				"</div>"
			"</div>"
	);

	fprintf(report,
		"<div id=\"div_%s_detailed\" class=\"%shidden details\">",
	suite->title, tests >= (size_t)32 ? "_8_columns " : ( tests >= (size_t)16 ? "_4_columns " : ( tests >= (size_t)8 ? "_2_columns " : "" ) ) );

	for(test_index = 0; test_index < tests; test_index++){

		/*@-boundsread@*/
		const unit_test * test = suite->tests->test[test_index];
		/*@=boundsread@*/

		fprintf(report,

					"<div title=\"%s\">"
						"<div class=\"%s_%s image icon24\"></div>"
						"<a href=\"#%s_%s\">"
							"%s_%s"
						"</a>"
					"</div>",
		test->title,
		HUMAN_TYPE(test->is_requirement, "unit_test"),
		HUMAN_STATUS(test->status),
		HUMAN_TYPE(test->is_requirement, "unit_test"),
		test->code,
		HUMAN_TYPE(test->is_requirement, "unit_test"), test->code);
	}

	fprintf(report,
				"</div>"
			"</div>"
		"</div>"
	);
}

static void print_test_suites(/*@notnull@*/ const test_suite_collection * suites, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	size_t index;

	fprintf(report,
		"<h2 class=\"shadowed\">Test Suites</h2>"
		"<div id=\"suite_stats\" class=\"shadowed\">"
	);

	for(index = 0; index < suites->count; index++){
		/*@-boundsread@*/
		print_test_suite(suites->suite[index], report);
		/*@=boundsread@*/
	}

	fprintf(report,
		"</div>"
	);
}

static void print_unit_tests(/*@notnull@*/ const test_suite_collection * suites, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	size_t suite_index;
	size_t test_index;

	fprintf(report,
		"<h2 class=\"shadowed\">Unit Tests</h2>"
		"<div id=\"test_stats\" class=\"shadowed\">"
	);

	for(suite_index = 0; suite_index < suites->count; suite_index++){

		/*@-boundsread@*/
		const test_suite * suite = suites->suite[suite_index];
		/*@=boundsread@*/

		for(test_index = 0; test_index < suite->tests->count; test_index++){

			/*@-boundsread@*/
			const unit_test * test = suite->tests->test[test_index];
			/*@=boundsread@*/

			print_unit_test(suite, test, report);
		}
	}

	fprintf(report,
		"</div>"
	);
}

static void print_overall_statistics(statistics requirement_stats, statistics suite_stats, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	time_t			now		= time(NULL);
	const char *	date	= ctime(&now);
	const char *	compiler_version = PLATFORM_COMPILER_VERSION;

	fprintf(report,
		"<div>"
			"<div class=\"overall image icon24\"></div>"
			"<h3>"
				"Overall"
			"</h3>"
		"</div>"
		"<div class=\"global_section\">"
	);

	if(suite_stats.total > 0){

		const char * summary1;
		const char * summary2;

		if(suite_stats.failed > 0){
			summary1 = "Some of the tests failed.";
		}else if(suite_stats.warnings > 0){
			summary1 = "Some <em>non-critical</em> test didn't pass.";
		}else{
			summary1 = "All tests passed successfully.";
		}

		if(suite_stats.failed == 0 && suite_stats.warnings == 0){
			summary2 = "";
		}else if(requirement_stats.total == 0){
			summary2 = " Please verify platform requirements.";
		}else if(requirement_stats.failed > 0 || requirement_stats.warnings > 0){
			summary2 = " It could be related to some error on <strong>platform requirements</strong>.";
		}else{
			summary2 = " Please report it by clicking on the links (<em>\"create a new issue\"</em>).";
		}

		fprintf(report,
			"<div class=\"paragraph\">"
				"<div class=\"%s image icon24\"></div>"
				"%s%s"
			"</div>",
		( suite_stats.failed > 0 ? "failed" : (suite_stats.warnings > 0 ? "warning" : "passed") ),
		summary1, summary2);
	}

	if(requirement_stats.total > 0){

		const char * summary;

		if(requirement_stats.failed > 0){
			summary = "Some of the library's functionality is broken on this platform.";
		}else if(requirement_stats.warnings > 0){
			summary = "This platform shows some unexpected behavior. Please check platform requirements.";
		}else{
			summary = "All platform requirements are fulfilled.";
		}

		fprintf(report,
				"<div class=\"paragraph\">"
					"<div class=\"%s image icon24\"></div>"
					"%s"
				"</div>",
		( requirement_stats.failed > 0 ? "failed" : (requirement_stats.warnings > 0 ? "warning" : "passed") ),
		summary);
	}

	fprintf(report,
			"<div class=\"shadowed details_box\">"
				"<div onclick=\"toggleDetails('overall');\">"
					"<div id=\"overall_button\" class=\"unfold image icon24\"></div>"
					"<div class=\"details_heading\">"
						"Additional information:"
					"</div>"
					"<div id=\"overall_summary\" class=\"summary\">"
						"exceptions4c version %s"
					"</div>"
				"</div>",
	E4C_VERSION_STRING);

	fprintf(report,
				"<div id=\"overall_detailed\" class=\"hidden details\">"
					"<div>"
						"<div class=\"book image icon24\"></div>"
						"exceptions4c version %s"
					"</div>"
					"<div>"
						"<div class=\"datetime image icon24\"></div>"
						"%s"
					"</div>",
	E4C_VERSION_STRING, date);

	if(compiler_version == NULL){
		fprintf(report,
					"<div>"
						"<div class=\"platform image icon24\"></div>"
						"Compiler: %s (%ld)"
					"</div>",
		PLATFORM_COMPILER, (long)PLATFORM_COMPILER_VERSION_NUMBER);
	}else{
		fprintf(report,
					"<div>"
						"<div class=\"platform image icon24\"></div>"
						"Compiler: %s %s"
					"</div>",
		PLATFORM_COMPILER, PLATFORM_COMPILER_VERSION);
	}

	fprintf(report,
					"<div>"
						"<div class=\"platform image icon24\"></div>"
						"Operating system: %s"
					"</div>",
	PLATFORM_OS);

	fprintf(report,
					"<div>"
						"<div class=\"platform image icon24\"></div>"
						"Architecture: %s"
					"</div>",
	PLATFORM_ARCH);

	print_stateless_macro(report,	macros.e4c_nokeywords);
	print_stateless_macro(report,	macros.e4c_threadsafe);

	if( IS_DEFINED(macros.reentrant) ){
		print_stateless_macro(report, macros.reentrant);
	}

	if( IS_DEFINED(macros.thread_safe) || IS_NOT_DEFINED(macros.reentrant) ){
		print_stateless_macro(report, macros.thread_safe);
	}

	print_stateless_macro(report, macros.ndebug);

	if( IS_NOT_DEFINED(macros.stdc_version) ){
		print_stateless_macro(report, macros.stdc);
	}

	if( IS_DEFINED(macros.strict_ansi) ){
		print_stateless_macro(report, macros.strict_ansi);
	}

	if( IS_DEFINED(macros.stdc_version) ||  IS_NOT_DEFINED(macros.stdc) ){
		print_stateful_macro(report, macros.stdc_version);
	}

	if( IS_DEFINED(macros.isoc99_source) || (  IS_NOT_DEFINED(macros.gnu_source) &&  IS_NOT_DEFINED(macros.stdc_version) &&  IS_NOT_DEFINED(macros.stdc) ) ){
		print_stateless_macro(report, macros.isoc99_source);
	}

	if( IS_DEFINED(macros.cplusplus) ){
		print_stateful_macro(report, macros.cplusplus);
	}

	if( IS_DEFINED(macros.gnu_source) ){
		print_stateless_macro(report, macros.gnu_source);
	}

	if( IS_DEFINED(macros.posix_source) &&  IS_NOT_DEFINED(macros.posix_c_source) ){
		print_stateless_macro(report, macros.posix_source);
	}

	if( IS_DEFINED(macros.posix_c_source) ||  IS_NOT_DEFINED(macros.posix_source) ){
		print_stateful_macro(report, macros.posix_c_source);
	}

	if( IS_DEFINED(macros.posix_version) ){
		print_stateful_macro(report, macros.posix_version);
	}

	if( IS_DEFINED(macros.posix_source) || IS_DEFINED(macros.posix_c_source) || IS_DEFINED(macros.posix_version) ){
		print_stateful_macro(report, macros.posix2_c_version);
	}

	if( IS_NOT_DEFINED(macros.xopen_source) && ( IS_DEFINED(macros.xopen_version) || IS_DEFINED(macros.gnu_source) ) ){
		print_stateful_macro(report, macros.xopen_version);
	}

	if( IS_DEFINED(macros.xopen_source) && (  IS_NOT_DEFINED(macros.xopen_version) &&  IS_NOT_DEFINED(macros.gnu_source) ) ){
		print_stateful_macro(report, macros.xopen_source);
	}

	if( IS_DEFINED(macros.xopen_source_extended) ){
		print_stateless_macro(report, macros.xopen_source_extended);
	}

	if( IS_DEFINED(macros.bsd_source)  ||  IS_NOT_DEFINED(macros.gnu_source) ){
		print_stateless_macro(report, macros.bsd_source);
	}

	if( IS_DEFINED(macros.svid_source) ||  IS_NOT_DEFINED(macros.gnu_source) ){
		print_stateless_macro(report, macros.svid_source);
	}

	fprintf(report,
				"</div>"
			"</div>"
		"</div>"
	);
}

static void print_requirement(/*@notnull@*/ const unit_test * test, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	fprintf(report,
		"<div>"
			"<div class=\"requirement_%s image icon24\"></div>"
			"<a href=\"#requirement_%s\">%s</a>"
		"</div>",
	HUMAN_STATUS(test->status),
	test->code,
	test->title
	);
}

static void print_requirements(statistics stats, const test_suite_collection * suites, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	size_t				suite_index;
	size_t				tests;
	size_t				test_index;
	const unit_test *	test;

	fprintf(report,
		"<div>"
			"<div class=\"requirement image icon24\"></div>"
			"<h3>"
				"Platform Requirements"
			"</h3>"
		"</div>"
		"<div class=\"global_section\">"
			"<div class=\"paragraph\">"
				"<div class=\"stats image icon24\"></div>"
				"<strong>%u</strong> requirements; <strong>%u</strong> passed; <strong>%u</strong> failed."
			"</div>",
	(unsigned int)stats.total,
	(unsigned int)stats.passed,
	(unsigned int)(stats.warnings + stats.failed) );

			print_graph(report, stats, "requirement(s)");

	fprintf(report,
			"<div class=\"shadowed details_box\">"
				"<div onclick=\"toggleDetails('platform_list');\">"
					"<div id=\"platform_list_button\" class=\"unfold image icon24\"></div>"
					"<div class=\"details_heading\">"
						"All requirements:"
					"</div>"
					"<div id=\"platform_list_summary\" class=\"summary\">"
					"The detected platform is <em>%s</em> on <em>%s</em> on <em>%s</em>.",
		PLATFORM_COMPILER, PLATFORM_OS, PLATFORM_ARCH
	);

	fprintf(report,
					"</div>"
				"</div>"
				"<div id=\"platform_list_detailed\" class=\"hidden details\">"
	);

	for(suite_index = 0; suite_index < suites->count; suite_index++){

		/*@-boundsread@*/
		const test_suite * suite = suites->suite[suite_index];
		/*@=boundsread@*/

		tests = suite->tests->count;

		for(test_index = 0; test_index < tests; test_index++){

			/*@-boundsread@*/
			test = suite->tests->test[test_index];
			/*@=boundsread@*/

			if(test->is_requirement){
				print_requirement(test, report);
			}
		}

	}

	fprintf(report,
				"</div>"
			"</div>"
		"</div>"
	);

	/*
	fprintf(report,
		"</div>");
	*/
}

static void print_test_suite_statistics(statistics stats, const test_suite_collection * suites, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	size_t index;

	fprintf(report,
		"<div>"
			"<div class=\"test_suite image icon24\"></div>"
			"<h3>"
				"Test Suites"
			"</h3>"
		"</div>"
		"<div class=\"global_section\">"
			"<div class=\"paragraph\">"
				"<div class=\"stats image icon24\"></div>"
				"<strong>%u</strong> test suites; <strong>%u</strong> passed; <strong>%u</strong> failed."
			"</div>",
	(unsigned int)stats.total,
	(unsigned int)stats.passed,
	(unsigned int)(stats.warnings + stats.failed) );

			print_graph(report, stats, "test suite(s)");

	fprintf(report,
			"<div class=\"shadowed details_box\">"
				"<div onclick=\"toggleDetails('suite_list');\">"
					"<div id=\"suite_list_button\" class=\"unfold image icon24\"></div>"
					"<div class=\"details_heading\">"
						"All test suites:"
					"</div>"
					"<div id=\"suite_list_summary\" class=\"summary\">"
	);

	for(index = 0; index < suites->count && index < SUMMARY_MAX_ITEMS; index++){

		/*@-boundsread@*/
		const test_suite * suite = suites->suite[index];
		/*@=boundsread@*/

		if(!suite->is_requirement){

			fprintf(report,
							"<div class=\"test_suite_%s image icon24\"></div>"
							"<a href=\"#test_suite_%s\" title=\"%s suite contains %u unit tests\">"
								"%s"
							"</a>%s",
			HUMAN_STATUS(suite->status),
			suite->title,
			suite->title,
			(unsigned int)suite->tests->count,
			suite->title,
			AFTER_SUMMARY_ITEM(index, suites->count)
			);
		}
	};

	fprintf(report,
					"</div>"
				"</div>"
				"<div id=\"suite_list_detailed\" class=\"%shidden details\">",
	suites->count >= (size_t)32 ? "_8_columns " : ( suites->count >= (size_t)16 ? "_4_columns " : ( suites->count >= (size_t)8 ? "_2_columns " : "" ) ) );

	for(index = 0; index < suites->count; index++){

		/*@-boundsread@*/
		const test_suite * suite = suites->suite[index];
		/*@=boundsread@*/

		if(!suite->is_requirement){
			fprintf(report,
						"<div title=\"%s suite contains %u unit tests\">"
							"<div class=\"test_suite_%s image icon24\"></div>"
							"<a href=\"#test_suite_%s\">"
								"%s"
							"</a>"
						"</div>",
			suite->title,
			(unsigned int)suite->tests->count,
			HUMAN_STATUS(suite->status),
			suite->title,
			suite->title);
		}
	}

	fprintf(report,
				"</div>"
			"</div>"
		"</div>"
	);
}

static void print_unit_test_statistics(statistics test_stats, const test_suite_collection * suites, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	size_t suite_index;
	size_t test_index;
	size_t index;

	fprintf(report,
		"<div>"
			"<div class=\"test_suite image icon24\"></div>"
			"<h3>"
				"Unit Tests"
			"</h3>"
		"</div>"
		"<div class=\"global_section\">"
			"<div class=\"paragraph\">"
				"<div class=\"stats image icon24\"></div>"
				"<strong>%u</strong> unit tests; <strong>%u</strong> passed; <strong>%u</strong> failed."
			"</div>",
	(unsigned int)test_stats.total,
	(unsigned int)test_stats.passed,
	(unsigned int)(test_stats.warnings + test_stats.failed) );

			print_graph(report, test_stats, "unit test(s)");

	fprintf(report,
			"<div class=\"shadowed details_box\">"
				"<div onclick=\"toggleDetails('test_list');\">"
					"<div id=\"test_list_button\" class=\"unfold image icon24\"></div>"
					"<div class=\"details_heading\">"
						"All unit tests:"
					"</div>"
					"<div id=\"test_list_summary\" class=\"summary\">"
	);

	for(index = 0, suite_index = 0; index < SUMMARY_MAX_ITEMS && suite_index < suites->count; suite_index++){

		/*@-boundsread@*/
		const test_suite *	suite	= suites->suite[suite_index];
		/*@=boundsread@*/
		size_t				tests	= suite->tests->count;

		for(test_index = 0; index < SUMMARY_MAX_ITEMS && test_index < tests; test_index++, index++){

			/*@-boundsread@*/
			const unit_test * test = suite->tests->test[test_index];
			/*@=boundsread@*/

			if(!test->is_requirement){

				fprintf(report,
							"<div class=\"unit_test_%s image icon24\"></div>"
							"<a href=\"#unit_test_%s\" title=\"%s (%s suite)\">"
								"unit_test_%s"
							"</a>%s",
				HUMAN_STATUS(test->status),
				test->code, test->title, suite->title, test->code,
				AFTER_SUMMARY_ITEM(index, test_stats.total)
				);
			}
		}
	};

	fprintf(report,
					"</div>"
				"</div>"
				"<div id=\"test_list_detailed\" class=\"%shidden details\">",
	test_stats.total >= (size_t)32 ? "_8_columns " : ( test_stats.total >= (size_t)16 ? "_4_columns " : ( test_stats.total >= (size_t)8 ? "_2_columns " : "" ) )	);

	for(suite_index = 0; suite_index < suites->count; suite_index++){

		/*@-boundsread@*/
		const test_suite *	suite	= suites->suite[suite_index];
		/*@=boundsread@*/
		size_t				tests	= suite->tests->count;

		for(test_index = 0; test_index < tests; test_index++, index++){

			/*@-boundsread@*/
			const unit_test * test = suite->tests->test[test_index];
			/*@=boundsread@*/

			if(!test->is_requirement){
				fprintf(report,
						"<div title=\"%s (%s suite)\">"
							"<div class=\"unit_test_%s image icon24\"></div>"
							"<a href=\"#unit_test_%s\">"
								"unit_test_%s"
							"</a>"
						"</div>",
				test->title, suite->title,
				HUMAN_STATUS(test->status),
				test->code, test->code);
			}
		}
	};

	fprintf(report,
				"</div>"
			"</div>"
		"</div>"
	);
}

static void print_statistics(statistics requirement_stats, statistics suite_stats, statistics test_stats, const test_suite_collection * suites, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	fprintf(report,
		"<h2 class=\"shadowed\">Global Statistics</h2>"
		"<div id=\"global_stats\" class=\"shadowed\">"
	);

			print_overall_statistics(requirement_stats, suite_stats, report);

			if(requirement_stats.total > 0){
				print_requirements(requirement_stats, suites, report);
			}

			if(suite_stats.total > 0){
				print_test_suite_statistics(suite_stats, suites, report);
			}

			if(test_stats.total > 0){
				print_unit_test_statistics(test_stats, suites, report);
			}

	fprintf(report,
		"</div>"
	);
}

static void print_content(/*@notnull@*/ const test_runner * runner, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	fprintf(report, "<div id=\"content\">");

		print_statistics(runner->stats.requirements, runner->stats.suites, runner->stats.tests, runner->suites, report);
		print_test_suites(runner->suites, report);
		print_unit_tests(runner->suites, report);

	fprintf(report, "</div>");
}

static void print_header(/*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	fprintf(report,
		"<div id=\"header\">"
			"<a id=\"exceptions4c_title\" href=\"http://code.google.com/p/exceptions4c/\">"
				"<span id=\"logo\" class=\"image\"></span>"
				"<span id=\"title\">exceptions4c</span>"
				"<span id=\"slogan\">An exception handling framework for C</span>"
			"</a>"
		"</div>"
		"<h1>"
			"<a name=\"top\">"
				"Test Report"
			"</a>"
		"</h1>"
	);
}

static void print_footer(/*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	time_t			now		= time(NULL);
	struct tm *		date	= localtime(&now);
	int				year	= date->tm_year + 1900;

	fprintf(report,
		"<div id=\"footer\">"
			"<a href=\"http://code.google.com/p/exceptions4c/\">exceptions4c</a>"
			"<div class=\"e4c image icon24\"></div>"
			"Copyleft Guillermo Calvo %d"
		"</div>",
		year
	);
}

static void print_body(/*@notnull@*/ const test_runner * runner, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	fprintf(report, "<body>");

		print_header(report);
		print_content(runner, report);
		print_footer(report);

	fprintf(report, "</body>");
}

static void print_stylesheet(/*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	fprintf(report,
		"<style type=\"text/css\">"
		"body{"
			"font-family:"				"Helvetica, Verdana, Arial;"
			"background:"				"#efefef url(\"data:image/png;base64,R0lGODlhCAAIAIAAAMDAwP///ywAAAAACAAIAAACDIxhgZq7DGOb0dH3CgA7\");"
			"cursor:"					"default;"
		"}"
		"h1{"
			"display:"					"inline-block;"
			"margin-top:"				"0;"
			"text-shadow:"				"2px 2px 2px #888;"
			"-moz-text-shadow:"			"2px 2px 2px #888;"
			"-webkit-text-shadow:"		"2px 2px 2px #888;"
		"}"
	);

	fprintf(report,
		"h2{"
			"border:"					"solid 1px #000;"
			"color:"					"#fff;"
			"background-color:"			"#0033CD;"
			"display:"					"inline-block;"
			"margin:"					"0;"
			"padding:"					"0.5em 1em 0.5em 1em;"
			"-moz-border-radius:"		"16px;"
			"-webkit-border-radius:"	"16px;"
			"border-radius:"			"16px;"
			"text-shadow:"				"2px 2px 0 #004;"
			"-moz-text-shadow:"			"2px 2px 0 #004;"
			"-webkit-text-shadow:"		"2px 2px 0 #004;"
		"}"
		"h3{"
			"display:"					"inline-block;"
			"margin:"					"0;"
		"}"
		"a{"
			"text-decoration:"			"none;"
			"color:"					"#000;"
		"}"
		"a:hover{"
			"text-decoration:"			"underline;"
		"}"
		"#header{"
			"display:"					"inline-block;"
			"height:"					"48px;"
			"width:"					"32%%;"
			"float:"					"left;"
		"}"
		"#exceptions4c_title{"
			"text-decoration:"			"none;"
			"color:"					"#000;"
		"}"
	);

	fprintf(report,
		"#logo{"
			"display:"					"block;"
			"float:"					"left;"
			"margin-right:"				"1em;"
			"border:"					"0;"
			"background-position:"		"0 0;"
			"width:"					"48px;"
			"height:"					"48px;"
		"}"
		"#title{"
			"display:"					"block;"
			"font-weight:"				"bold;"
			"font-size:"				"128%%;"
		"}"
		"#slogan{"
			"display:"					"block;"
			"font-style:"				"italic;"
		"}"
		"#global_stats,"
		"#suite_stats,"
		"#test_stats{"
			"border:"					"solid 1px #888;"
			"background:"				"url(\"data:image/png;base64,%s%s%s%s%s%s%s%s%s\") repeat-y right 32px #fff;"
			"margin:"					"-1.5em 32px 32px 1.5em;"
			"padding:"					"3em 1.5em 1.5em 1.5em;"
		"}",
		"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"
	);

	fprintf(report,
		"#footer{"
			"text-align:"				"center;"
		"}"
		".paragraph{"
			"margin:"					"1em 0 1em 0;"
		"}"
		".shadowed{"
			"-moz-box-shadow:"			"2px 2px 2px #888;"
			"-webkit-box-shadow:"		"2px 2px 2px #888;"
			"box-shadow:"				"2px 2px 2px #888;"
		"}"
		".global_section,"
		".suite_section,"
		".test_section{"
			"margin:"					"0 0 2em 2em;"
		"}"
		".description{"
			"font-style:"				"italic;"
		"}"
		".hidden{"
			"display:"					"none !important;"
		"}"
		".graph{"
			"padding-left:"				"0;"
			"border:"					"0;"
			"height:"					"16px;"
			"display:"					"inline-block;"
			"margin-right:"				"8px;"
			"-moz-border-radius:"		"4px;"
			"-webkit-border-radius:"	"4px;"
			"border-radius:"			"4px;"
		"}"
	);

	fprintf(report,
		".percent_failed,"
		".percent_passed{"
			"background:"				"url(\"data:image/gif;base64,R0lGODlhEwAgALMAAHvGAITOCIzOGJTWOZzWSqXeWtYAAN4QCN4QEN4hIedCQudSSudjY+9ra////////ywAAAAAEwAgAAAElZCx1uRSOOclZ2WXpnESZYljp6KpCrKb+8IK6dK1PLO2ehnAIBCRKBqLwuTheEwKl0ykEwiNHqaGKvM61R65Tq8RrIwWyUICocBWD95wuFrNLrjj8fm6TcDn9W1+f4B9gm96eoaHiHeGjHOKj24AlJWUAQKZmpmWnZibmp2Wn6ACopWkoKeXpZyrqZurALChr62mqxEAADs=\") repeat-x;"
		"}"
		".percent_failed{"
			"border:"					"solid 1px #444;"
			"width:"					"80%%;"
			"min-width:"				"160px;"
		"}"
	);

	fprintf(report,
		".percent_passed{"
			"margin:"					"-1px;"
			"border:"					"solid 1px #444;"
			"background-position:"		"0 -16px;"
		"}"
		".fold,"
		".unfold{"
			"cursor:"					"pointer;"
		"}"
		".details_box{"
			"background-color:"			"rgb(255, 255, 224);"
			"background-color:"			"rgba(255, 255, 224, 0.5);"
			"border:"					"solid 1px #000;"
			"margin:"					"2px;"
			"padding:"					"2px;"
		"}"
		".details_heading{"
			"font-weight:"				"bold;"
			"margin-right:"				"8px;"
			"display:"					"inline-block;"
		"}"
		".summary{"
			"display:"					"inline-block;"
		"}"
		".details{"
			"padding-left:"				"2em;"
		"}"
	);

	fprintf(report,
		"._2_columns,"
		"._4_columns,"
		"._8_columns{"
			"-moz-column-gap:"			".5em;"
			"-webkit-column-gap:"		".5em;"
			"column-gap:"				".5em;"
		"}"
		"._8_columns{"
			"-moz-column-count:"		"8;"
			"-webkit-column-count:"		"8;"
			"column-count:"				"8;"
		"}"
		"._4_columns{"
			"-moz-column-count:"		"4;"
			"-webkit-column-count:"		"4;"
			"column-count:"				"4;"
		"}"
		"._2_columns{"
			"-moz-column-count:"		"2;"
			"-webkit-column-count:"		"2;"
			"column-count:"				"2;"
		"}"
		".test_section_parameters{"
			"width:"					"100%%;"
			"margin-left:"				"1em;"
		"}"
		".console{"
			"font-family:"				"\"Lucida Console\", \"Courier New\", Courier, monospace;"
		"}"
	);

	fprintf(report,
		".test_section_parameters textarea{"
			"font-size:"				"80%%;"
			"max-width:"				"90%%;"
		"}"
		".test_section_parameters td:first-child{"
			"font-weight:"				"bold;"
			"width:"					"1%%;"
			"white-space:"				"nowrap;"
			"text-align:"				"right;"
			"vertical-align:"			"top;"
		"}"
		".test_section_parameters td:first-child:after{"
			"content:"					"\":\";"
		"}"
		".icon24{"
			"width:"					"24px;"
			"height:"					"24px;"
			"display:"					"inline-block;"
			"vertical-align:"			"middle;"
			"margin:"					"0;"
		"}"
		".image{"
			"background:"				"url(\"data:image/png;base64,%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\") no-repeat;"
		"}",
		"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"
	);

	fprintf(report,
		".unfold{"
			"background-position:"		"0 -48px;"
		"}"
		".fold{"
			"background-position:"		"-24px -48px;"
		"}"
		".test_suite{"
			"background-position:"		"-48px 0;"
		"}"
		".unit_test{"
			"background-position:"		"-48px -24px;"
		"}"
		".requirement{"
			"background-position:"		"-48px -48px;"
		"}"
		".test_suite_passed{"
			"background-position:"		"-72px 0;"
		"}"
		".unit_test_passed{"
			"background-position:"		"-72px -24px;"
		"}"
		".requirement_passed{"
			"background-position:"		"-72px -48px;"
		"}"
		".test_suite_failed{"
			"background-position:"		"-96px 0;"
		"}"
		".unit_test_failed{"
			"background-position:"		"-96px -24px;"
		"}"
	);

	fprintf(report,
		".requirement_failed{"
			"background-position:"		"-96px -48px;"
		"}"
		".test_suite_warning{"
			"background-position:"		"-120px 0;"
		"}"
		".unit_test_warning{"
			"background-position:"		"-120px -24px;"
		"}"
		".requirement_warning{"
			"background-position:"		"-120px -48px;"
		"}"
		".passed{"
			"background-position:"		"-144px 0;"
		"}"
		".failed{"
			"background-position:"		"-144px -24px;"
		"}"
		".warning{"
			"background-position:"		"-144px -48px;"
		"}"
		".overall{"
			"background-position:"		"-168px 0;"
		"}"
		".book{"
			"background-position:"		"-168px -24px;"
		"}"
		".info{"
			"background-position:"		"-168px -48px;"
		"}"
	);

	fprintf(report,
		".datetime{"
			"background-position:"		"-192px 0;"
		"}"
		".stats{"
			"background-position:"		"-192px -24px;"
		"}"
		".platform{"
			"background-position:"		"-192px -48px;"
		"}"
		".bug{"
			"background-position:"		"-216px 0;"
		"}"
		".return{"
			"background-position:"		"-216px -24px;"
		"}"
		".e4c{"
			"background-position:"		"-216px -48px;"
		"}"
	"</style>"
	);
}

static void print_javascript(/*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	fprintf(report,
		"<script type=\"text/javascript\">"
			"function toggleDetails(id){"
				"var details"	"= document.getElementById(id + '_detailed');"
				"var summary"	"= document.getElementById(id + '_summary');"
				"var button"	"= document.getElementById(id + '_button');"
				"button.classList.toggle('fold');"
				"button.classList.toggle('unfold');"
				"details.classList.toggle('hidden');"
				"summary.classList.toggle('hidden');"
			"}"
		"</script>"
	);
}

static void print_head(/*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
{

	fprintf(report, "<head>");

		fprintf(report,
			"<title>exception4c - Test Report</title>"
			"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
			"<link rel=\"icon\" type=\"image/png\" href=\"data:image/gif;base64,R0lGODlhEAAQALMAAAAAAAgICBwcHDExMUlJSXBwcIyMjKSkpLW1tb29vcbGxtbW1ufn5+/v7/f39////ywAAAAAEAAQAAAEdfDJSetsB1mrRiCIs0mLAJxBcShGaxxHQ5w0TRj1sSgm8cyExAPgI/h+gOEvkUz6BpJCUmKYHIYA6AOHpCmfEsVH4iX+jo/DACUInAYCwXXCIAwSDBaIMWhQGAYFFAYLD4IUCgiHDw6CgBUJGBMLhQgMI5gRAAA7\" />"
		);
		print_stylesheet(report);
		print_javascript(report);

	fprintf(report, "</head>");
}

void print_html(/*@notnull@*/ const test_runner * runner, /*@notnull@*/ FILE * report){

	fprintf(report,
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
		"<html xmlns=\"http://www.w3.org/1999/xhtml\">"
	);

		print_head(report);
		print_body(runner, report);

	fprintf(report, "</html>");
}
