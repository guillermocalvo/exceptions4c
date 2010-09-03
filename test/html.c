
# include <stdio.h>
# include <string.h>
# include <time.h>

# include "html.h"

# define SUMMARY_MAX_ITEMS 6

# define MIN(x, y) (x < y ? x : y )
# define ELIDED(x) (x > SUMMARY_MAX_ITEMS ? e4c_true : e4c_false )

# define AFTER_SUMMARY_ITEM(index, total) \
	( \
		index + 1 < MIN(SUMMARY_MAX_ITEMS, total)  \
		? \
		( ELIDED(total) || index + 2 < MIN(SUMMARY_MAX_ITEMS, total) ? ", " : " and " ) \
		: \
		( ELIDED(total) ? "..." : "." ) \
	)

# define HUMAN_EXIT_CODE(v)	\
	( v == EXIT_WHATEVER ? "(any value)" : (v == EXIT_SUCCESS ? "EXIT_SUCCESS" : ( v == EXIT_FAILURE ? "EXIT_FAILURE" : "(a specific value)") ) )

# ifdef NDEBUG
#	define IS_NDEBUG_DEFINED e4c_true
# else
#	define IS_NDEBUG_DEFINED e4c_false
# endif


e4c_bool NDEBUG_is_defined = IS_NDEBUG_DEFINED;


static void printPercentage(char * buffer, int fraction, int total){

	char *	dotPos	= NULL;
	float	value	= 100 * fraction / total;

	sprintf(buffer, "%.2f", value);

	while(*buffer != '\0'){
		if(dotPos == NULL && *buffer == '.'){
			dotPos = buffer;
		}else if(dotPos != NULL && *buffer == '0'){
			if(dotPos + 1 == buffer) buffer = dotPos;
			break;
		}
		buffer++;
	}

	buffer[0] = '%';
	buffer[1] = '\0';
}

static int printExitCode(char * buffer, int exitCode){

	if(exitCode == EXIT_SUCCESS){
		return( sprintf(buffer, "EXIT_SUCCESS") );
	}

	if(exitCode == EXIT_FAILURE){
		return( sprintf(buffer, "EXIT_FAILURE") );
	}

	return( sprintf(buffer, "%d", exitCode) );
}

static void htmlGraph(FILE * report, Statistics stats, const char * what){

	char		percentPassed[8];
	char		percentFailed[8];

	printPercentage(percentPassed, stats.passed, stats.total);
	printPercentage(percentFailed, stats.failed, stats.total);

	fprintf(report,
		"<div class=\"paragraph\">"
			"<div title=\"%d %s failed (%s)\" class=\"shadowed graph percent_failed\">"
				"<div title=\"%d %s passed (%s)\" class=\"graph percent_passed\" style=\"width: %s;\"></div>"
			"</div>"
			"%s passed"
		"</div>",
		stats.failed,		what,		percentFailed,
		stats.passed,		what,		percentPassed,		percentPassed,
		percentPassed
	);
}

static void unitTest(TestSuite * suite, UnitTest * test, FILE * report){

	char exitCode[128];
	char found[64];
	char expected[64];
	char etc[64];

	printExitCode(found, test->foundExitCode);

	if(test->unexpectedExitCode){

		printExitCode(expected, test->expectedExitCode);

		sprintf(etc, " (expecting: <span class=\"console\">%s</span>)", expected);
	}else if(test->expectedExitCode == EXIT_WHATEVER){

		sprintf(etc, " (any value is OK)");
	}else{

		*etc = '\0';
	}

	sprintf(exitCode, "<span class=\"console\">%s</span>%s", found, etc);

	fprintf(report,
		"<div>"
			"<div class=\"unit_test_%s image icon24\"></div>"
			"<h3>"
				"<a name=\"test_%s\">test_%s: %s</a>"
			"</h3>"
		"</div>"
		"<div class=\"test_section\">"

			"<div class=\"test_section_content\">"

				"<div class=\"description paragraph\">"
					"%s"
				"</div>",
	(test->passed ? "passed" : "failed"), test->code, test->code, test->title, test->description);

	if(!test->passed){
		fprintf(report,
				"<div class=\"paragraph\">"
					"<div class=\"bug image icon24\"></div>"
					"<a href=\"http://code.google.com/p/exceptions4c/issues/entry?"
						"template=Defect%%20report%%20from%%20testing&amp;"
						"summary=test_%s%%20failed&amp;"
						"comment="
							"test_%s%%20failed.%%0A"
							"%%0A"
							"%%20Library%%20version:%%20%ld%%0A"
							"%%20%%20NDEBUG%%20defined:%%20%s%%0A"
							"%%20%%20%%20%%20Architecture:%%20...%%0A"
							"Operating%%20system:%%20...%%0A"
							"%%20%%20%%20%%20IDE/Compiler:%%20...%%0A"
							"%%0A"
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
		test->code,
		test->code, E4C_VERSION_NUMBER,
		(NDEBUG_is_defined ? "YES" : "NO"),
		(test->unexpectedExitCode	? "NOK" : "OK"), found,
		(test->unexpectedOutput		? "NOK%20...%20(please%20copy%20and%20paste%20it%20below)" : "OK"),
		(test->unexpectedError		? "NOK%20...%20(please%20copy%20and%20paste%20it%20below)" : "OK")
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

						"<tr>"
							"<td>Suite</td>"
							"<td>"
								"<div class=\"test_suite_%s image icon24\"></div>"
								"<a href=\"#%s\">%s</a>"
							"</td>"
						"</tr>"
						"<tr>"
							"<td>Exit code</td>"
							"<td>"
								"<div class=\"%s image icon24\"></div>"
								"%s"
							"</td>"
						"</tr>"
						"<tr>"
							"<td>Output</td>"
							"<td>"
								"<div class=\"%s image icon24\"></div>"
								"Expecting: "
								"<span class=\"console\">"
									"%s"
								"</span>"
								"<br/>"
								"<textarea cols=\"80\" rows=\"8\" class=\"console\">%s</textarea>"
							"</td>"
						"</tr>"
						"<tr>"
							"<td>Error</td>"
							"<td>"
								"<div class=\"%s image icon24\"></div>"
								"Expecting: "
								"<span class=\"console\">"
									"%s"
								"</span>"
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
					"</table>"
				"</div>"
				"<div class=\"paragraph\">"
					"<div class=\"overall image icon24\"></div>"
					"<a href=\"#top\">"
						"Return to top"
					"</a>"
				"</div>"
			"</div>"
		"</div>",
	test->code, test->code, test->code, (test->passed ? "Passed" : "Failed"), test->code,
	(suite->passed ? "passed" : "failed"), suite->title, suite->title,
	(test->unexpectedExitCode ? "failed" : "passed"),
	exitCode,
	(test->unexpectedOutput ? "failed" : "passed"),
	test->expectedOutput,
	test->foundOutput,
	(test->unexpectedError ? "failed" : "passed"),
	(test->expectedError	!= NULL ? test->expectedError	: " (no error)"),
	(*test->foundError ? "console" : "hidden"), test->foundError,
	(test->passed ? "passed" : "failed"),
	(test->passed ? "Passed" : "Failed")
	);

}

static void testSuite(TestSuite * suite, FILE * report){

	int tests;
	int testIndex;

	fprintf(report,
		"<div>"
			"<div class=\"test_suite_%s image icon24\"></div>"
			"<h3>"
				"<a name=\"%s\">%s</a>"
			"</h3>"
		"</div>"
		"<div class=\"suite_section\">"

			"<div class=\"description paragraph\">"
				"%s"
			"</div>"

			"<div class=\"paragraph\">"
				"<strong>%d</strong> unit tests; <strong>%d</strong> passed; <strong>%d</strong> failed."
			"</div>",
	( suite->passed ? "passed" : "failed" ), suite->title, suite->title,
	suite->description,
	suite->stats.total, suite->stats.passed, suite->stats.failed);

	htmlGraph(report, suite->stats, "unit test(s)");

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
	for(testIndex = 0; testIndex < tests && testIndex < SUMMARY_MAX_ITEMS; testIndex++){

		UnitTest * test = suite->tests->test[testIndex];

		fprintf(report,
						"<div class=\"unit_test_%s image icon24\"></div>"
						"<a href=\"#test_%s\" title=\"%s\">"
							"test_%s"
						"</a>%s",
		(test->passed ? "passed" : "failed"), test->code, test->title, test->code, AFTER_SUMMARY_ITEM(testIndex, tests) );
	}

	fprintf(report,
				"</div>"
			"</div>"
	);

	fprintf(report,
		"<div id=\"div_%s_detailed\" class=\"%shidden details\">",
	suite->title, tests > 32 ? "_8_columns " : ( tests > 16 ? "_4_columns " : ( tests > 8 ? "_2_columns " : "" ) ) );

	for(testIndex = 0; testIndex < tests; testIndex++){

		UnitTest * test = suite->tests->test[testIndex];

		fprintf(report,

					"<div title=\"%s\">"
						"<div class=\"unit_test_%s image icon24\"></div>"
						"<a href=\"#test_%s\">"
							"test_%s"
						"</a>"
					"</div>",
		test->title, (test->passed ? "passed" : "failed"), test->code, test->code);
	}

	fprintf(report,
				"</div>"
			"</div>"
		"</div>"
	);
}

static void testSuites(TestSuiteCollection * suites, FILE * report){

	int index;

	fprintf(report,
		"<h2 class=\"shadowed\">Test Suites</h2>"
		"<div id=\"suite_stats\" class=\"shadowed\">"
	);

	for(index = 0; index < suites->count; index++){
		testSuite(suites->suite[index], report);
	}

	fprintf(report,
		"</div>"
	);
}

static void unitTests(TestSuiteCollection * suites, FILE * report){

	int suiteIndex;
	int testIndex;

	fprintf(report,
		"<h2 class=\"shadowed\">Unit Tests</h2>"
		"<div id=\"test_stats\" class=\"shadowed\">"
	);

	for(suiteIndex = 0; suiteIndex < suites->count; suiteIndex++){

		TestSuite * suite = suites->suite[suiteIndex];

		for(testIndex = 0; testIndex < suite->tests->count; testIndex++){

			UnitTest * test = suite->tests->test[testIndex];

			unitTest(suite, test, report);
		}
	}

	fprintf(report,
		"</div>"
	);
}

static void overallStatistics(Statistics stats, FILE * report){

	time_t			now		= time(NULL);
	const char *	date	= ctime(&now);
	const char *	overall	= "All tests passed successfully";
	const char *	status	= "passed";

	if(stats.failed != 0){
		overall = "Some of the tests failed";
		status	= "failed";
	}

	fprintf(report,
		"<div>"
			"<div class=\"overall image icon24\"></div>"
			"<h3>"
				"Overall"
			"</h3>"
		"</div>"
		"<div class=\"global_section\">"
			"<div class=\"paragraph\">"
				"<div class=\"%s image icon24\"></div>"
				"%s."
			"</div>"
			"<div class=\"paragraph\">"
				"<div class=\"book image icon24\"></div>"
				"exceptions4c version %s"
			"</div>"
			"<div class=\"paragraph\">"
				"<div class=\"bug image icon24\"></div>"
				"<code>NDEBUG</code> %s."
			"</div>"
			"<div class=\"paragraph\">"
				"<div class=\"datetime image icon24\"></div>"
				"%s"
			"</div>"
		"</div>",
	status, overall, E4C_VERSION_STRING, (NDEBUG_is_defined ? "defined" : "undefined"), date);
}

static void testSuiteStatistics(Statistics stats, TestSuiteCollection * suites, FILE * report){

	int index;

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
				"<strong>%d</strong> test suites; <strong>%d</strong> passed; <strong>%d</strong> failed."
			"</div>",
	stats.total, stats.passed, stats.failed);

			htmlGraph(report, stats, "test suite(s)");

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

		TestSuite * suite = suites->suite[index];

		fprintf(report,
						"<div class=\"test_suite_%s image icon24\"></div>"
						"<a href=\"#%s\" title=\"%s suite contains %d unit tests\">"
							"%s"
						"</a>%s",
		(suite->passed ? "passed" : "failed"), suite->title, suite->title, suite->tests->count, suite->title,
		AFTER_SUMMARY_ITEM(index, suites->count)
		);
	};

	fprintf(report,
					"</div>"
				"</div>"
				"<div id=\"suite_list_detailed\" class=\"hidden details\">"
	);

	for(index = 0; index < suites->count; index++){

		TestSuite * suite = suites->suite[index];

		fprintf(report,
					"<div title=\"%s suite contains %d unit tests\">"
						"<div class=\"test_suite_%s image icon24\"></div>"
						"<a href=\"#%s\">"
							"%s"
						"</a>"
					"</div>",
		suite->title, suite->tests->count, (suite->passed ? "passed" : "failed"), suite->title, suite->title);
	}

	fprintf(report,
				"</div>"
			"</div>"
		"</div>"
	);
}

static void unitTestStatistics(Statistics testStats, TestSuiteCollection * suites, FILE * report){

	int suiteIndex;
	int testIndex;
	int index;

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
				"<strong>%d</strong> unit tests; <strong>%d</strong> passed; <strong>%d</strong> failed."
			"</div>",
	testStats.total, testStats.passed, testStats.failed);

			htmlGraph(report, testStats, "unit test(s)");

	fprintf(report,
			"<div class=\"shadowed details_box\">"
				"<div onclick=\"toggleDetails('test_list');\">"
					"<div id=\"test_list_button\" class=\"unfold image icon24\"></div>"
					"<div class=\"details_heading\">"
						"All unit tests:"
					"</div>"
					"<div id=\"test_list_summary\" class=\"summary\">"
	);

	for(index = 0, suiteIndex = 0; index < SUMMARY_MAX_ITEMS && suiteIndex < suites->count; suiteIndex++){

		TestSuite *	suite	= suites->suite[suiteIndex];
		int			tests	= suite->tests->count;

		for(testIndex = 0; index < SUMMARY_MAX_ITEMS && testIndex < tests; testIndex++, index++){

			UnitTest * test = suite->tests->test[testIndex];

			fprintf(report,
						"<div class=\"unit_test_%s image icon24\"></div>"
						"<a href=\"#test_%s\" title=\"%s (%s suite)\">"
							"test_%s"
						"</a>%s",
			(test->passed ? "passed" : "failed"), test->code, test->title, suite->title, test->code,
			AFTER_SUMMARY_ITEM(index, testStats.total)
			);
		}
	};

	fprintf(report,
					"</div>"
				"</div>"
				"<div id=\"test_list_detailed\" class=\"%shidden details\">",
	testStats.total > 32 ? "_8_columns " : ( testStats.total > 16 ? "_4_columns " : ( testStats.total > 8 ? "_2_columns " : "" ) )	);

	for(suiteIndex = 0; suiteIndex < suites->count; suiteIndex++){

		TestSuite *	suite	= suites->suite[suiteIndex];
		int			tests	= suite->tests->count;

		for(testIndex = 0; testIndex < tests; testIndex++, index++){

			UnitTest * test = suite->tests->test[testIndex];

			fprintf(report,
					"<div title=\"%s (%s suite)\">"
						"<div class=\"unit_test_%s image icon24\"></div>"
						"<a href=\"#test_%s\">"
							"test_%s"
						"</a>"
					"</div>",
			test->title, suite->title, (test->passed ? "passed" : "failed"), test->code, test->code);
		}
	};

	fprintf(report,
				"</div>"
			"</div>"
		"</div>"
	);
}

static void statistics(Statistics suiteStats, Statistics testStats, TestSuiteCollection * suites, FILE * report){

	fprintf(report,
		"<h2 class=\"shadowed\">Global Statistics</h2>"
		"<div id=\"global_stats\" class=\"shadowed\">"
	);

			overallStatistics(suiteStats, report);
			testSuiteStatistics(suiteStats, suites, report);
			unitTestStatistics(testStats, suites, report);

	fprintf(report,
		"</div>"
	);
}

static void content(TestRunner * runner, FILE * report){

	fprintf(report, "<div id=\"content\">");

		statistics(runner->stats.suites, runner->stats.tests, runner->suites, report);
		testSuites(runner->suites, report);
		unitTests(runner->suites, report);

	fprintf(report, "</div>");
}

static void header(FILE * report){

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

static void footer(FILE * report){

	fprintf(report,
		"<div id=\"footer\">"
			"<a href=\"http://code.google.com/p/exceptions4c/\">exceptions4c</a>"
			"<div class=\"e4c image icon24\"></div>"
			"Copyleft Guillermo Calvo 2010"
		"</div>"
	);
}

static void body(TestRunner * runner, FILE * report){

	fprintf(report, "<body>");

		header(report);
		content(runner, report);
		footer(report);

	fprintf(report, "</body>");
}

static void stylesheet(FILE * report){

	fprintf(report, "<style type=\"text/css\">");

	fprintf(report,
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
			"background:"				"url(\"data:image/png;base64,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\") repeat-y right 32px #fff;"
			"margin:"					"-1.5em 32px 32px 1.5em;"
			"padding:"					"3em 1.5em 1.5em 1.5em;"
		"}"
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
		".percent_failed,"
		".percent_passed{"
			"background:"				"url(\"data:image/gif;base64,R0lGODlhEwAgALMAAHvGAITOCIzOGJTWOZzWSqXeWtYAAN4QCN4QEN4hIedCQudSSudjY+9ra////////ywAAAAAEwAgAAAElZCx1uRSOOclZ2WXpnESZYljp6KpCrKb+8IK6dK1PLO2ehnAIBCRKBqLwuTheEwKl0ykEwiNHqaGKvM61R65Tq8RrIwWyUICocBWD95wuFrNLrjj8fm6TcDn9W1+f4B9gm96eoaHiHeGjHOKj24AlJWUAQKZmpmWnZibmp2Wn6ACopWkoKeXpZyrqZurALChr62mqxEAADs=\") repeat-x;"
		"}"
		".percent_failed{"
			"border:"					"solid 1px #444;"
			"width:"					"80%%;"
			"min-width:"				"160px;"
		"}"
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
			"background-color:"			"#FFFFE0;"
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
			"background:"				"url(\"data:image/png;base64,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\") no-repeat;"
		"}"
		".test_suite{"
			"background-position:"		"-48px 0;"
		"}"
		".unit_test{"
			"background-position:"		"-48px -24px;"
		"}"
		".test_suite_passed{"
			"background-position:"		"-72px 0;"
		"}"
		".unit_test_passed{"
			"background-position:"		"-72px -24px;"
		"}"
		".test_suite_failed{"
			"background-position:"		"-96px 0;"
		"}"
		".unit_test_failed{"
			"background-position:"		"-96px -24px;"
		"}"
		".passed{"
			"background-position:"		"-120px 0;"
		"}"
		".failed{"
			"background-position:"		"-120px -24px;"
		"}"
		".unfold{"
			"background-position:"		"-144px 0;"
		"}"
		".fold{"
			"background-position:"		"-144px -24px;"
		"}"
		".overall{"
			"background-position:"		"-168px 0;"
		"}"
		".book{"
			"background-position:"		"-168px -24px;"
		"}"
		".datetime{"
			"background-position:"		"-192px 0;"
		"}"
		".stats{"
			"background-position:"		"-192px -24px;"
		"}"
		".bug{"
			"background-position:"		"-216px 0;"
		"}"
		".e4c{"
			"background-position:"		"-216px -24px;"
		"}"
	"</style>"
	);
}

static void javascript(FILE * report){

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

static void head(FILE * report){

	fprintf(report, "<head>");

		fprintf(report,
			"<title>exception4c - Test Report</title>"
			"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
			"<link rel=\"icon\" type=\"image/png\" href=\"data:image/gif;base64,R0lGODlhEAAQALMAAAAAAAgICBwcHDExMUlJSXBwcIyMjKSkpLW1tb29vcbGxtbW1ufn5+/v7/f39////ywAAAAAEAAQAAAEdfDJSetsB1mrRiCIs0mLAJxBcShGaxxHQ5w0TRj1sSgm8cyExAPgI/h+gOEvkUz6BpJCUmKYHIYA6AOHpCmfEsVH4iX+jo/DACUInAYCwXXCIAwSDBaIMWhQGAYFFAYLD4IUCgiHDw6CgBUJGBMLhQgMI5gRAAA7\" />"
		);
		stylesheet(report);
		javascript(report);

	fprintf(report, "</head>");
}

void html(TestRunner * runner, FILE * report){

	fprintf(report,
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
		"<html xmlns=\"http://www.w3.org/1999/xhtml\">"
	);

		head(report);
		body(runner, report);

	fprintf(report, "</html>");
}
