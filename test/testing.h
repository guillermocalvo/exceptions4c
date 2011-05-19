
# ifndef TESTING_FRAMEWORK_H

# define TESTING_FRAMEWORK_H

# include <stdio.h>
# include "e4c.h"

# define EXIT_WHATEVER			54321
# define ERROR_WHATEVER			(void *)54321
# define OUTPUT_WHATEVER		(void *)54321

# define SEVERITY_CRITICAL		E4C_TRUE
# define SEVERITY_NOT_CRITICAL	E4C_FALSE

# define TYPE_REQUIREMENT		E4C_TRUE
# define TYPE_UNIT_TEST			E4C_FALSE

# define STATUS_PASSED			0
# define STATUS_WARNING			1
# define STATUS_FAILED			2


/*
	TESTS
	________________________________________________________________
*/

# define DEFINE_UNIT_TEST(IS_REQUIREMENT, CODE, TITLE, DESCRIPTION1, DESCRIPTION2, IS_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR) \
	int test_##CODE##_function(void); \
	\
	unit_test test_##CODE = { \
		/* is_requirement */		IS_REQUIREMENT, \
		/* code */					#CODE, \
		/* title */					TITLE, \
		/* description1 */			DESCRIPTION1, \
		/* description2 */			DESCRIPTION2, \
		/* is_critical */			IS_CRITICAL, \
		/* at_failure */			AT_FAILURE, \
		/* function */				test_##CODE##_function, \
		/* expected_exit_code */	EXIT_CODE, \
		/* expected_output */		OUT, \
		/* expected_error */		ERR, \
		/* found_exit_code */		0, \
		/* found_output */			{ 0 }, \
		/* found_error */			{ 0 }, \
		/* unexpected_exit_code */	0, \
		/* unexpected_output */		0, \
		/* unexpected_error */		0, \
		/* status */				0 \
	}; \
	int test_##CODE##_function(void)

# define DEFINE_TEST(CODE, TITLE, DESCRIPTION, AT_FAILURE, EXIT_CODE, OUT, ERR) \
	DEFINE_UNIT_TEST(TYPE_UNIT_TEST, CODE, TITLE, DESCRIPTION, NULL, SEVERITY_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR)

# define DEFINE_REQUIREMENT(CODE, TITLE, DESCRIPTION, IS_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR) \
	DEFINE_UNIT_TEST(TYPE_REQUIREMENT, CODE, TITLE, DESCRIPTION, NULL, IS_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR)

# define DEFINE_TEST_LONG_DESCRIPTION(CODE, TITLE, DESCRIPTION1, DESCRIPTION2, AT_FAILURE, EXIT_CODE, OUT, ERR) \
	DEFINE_UNIT_TEST(TYPE_UNIT_TEST, CODE, TITLE, DESCRIPTION1, DESCRIPTION2, SEVERITY_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR)

# define DEFINE_REQUIREMENT_LONG_DESCRIPTION(CODE, TITLE, DESCRIPTION1, DESCRIPTION2, IS_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR) \
	DEFINE_UNIT_TEST(TYPE_REQUIREMENT, CODE, TITLE, DESCRIPTION1, DESCRIPTION2, IS_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR)


E4C_DECLARE_EXCEPTION(WildException);
E4C_DECLARE_EXCEPTION(TamedException);

E4C_DECLARE_EXCEPTION(ChildException);
E4C_DECLARE_EXCEPTION(SiblingException);
E4C_DECLARE_EXCEPTION(ParentException);
E4C_DECLARE_EXCEPTION(GrandparentException);

# define ECHO(args) \
	printf args; \
	fflush(stdout);

/*
	SUITES
	________________________________________________________________
*/

# define TEST_DECLARATION(ID) extern unit_test test_##ID;
# define TEST_ENUMERATION(ID) &test_##ID,

# define SUITE(IS_REQUIREMENT, SUITE_CODE, SUITE_TITLE, SUITE_DESCRIPTION1, SUITE_DESCRIPTION2) \
	\
	COLLECTION(TEST_DECLARATION) \
	\
	static unit_test * test_array_##SUITE_CODE[] = { \
		COLLECTION(TEST_ENUMERATION) \
	\
	}; \
	\
	static test_collection test_collection_##SUITE_CODE = { \
		/* test */	test_array_##SUITE_CODE, \
		/* count*/	sizeof(test_array_##SUITE_CODE) / sizeof(test_array_##SUITE_CODE[0]) \
	}; \
	\
	test_suite suite_##SUITE_CODE = { \
		/* is_requirement */	IS_REQUIREMENT, \
		/* title */				SUITE_TITLE, \
		/* description1 */		SUITE_DESCRIPTION1, \
		/* description2 */		SUITE_DESCRIPTION2, \
		/* tests */				&test_collection_##SUITE_CODE, \
		/* stats */				{ \
			/* total */				0, \
			/* passed */			0, \
			/* warnings */			0, \
			/* failed */			0 \
								}, \
		/* status */			0 \
	};


# define END_SUITE NEW

# define TEST_SUITE(SUITE_CODE, SUITE_TITLE, SUITE_DESCRIPTION) \
	SUITE(TYPE_UNIT_TEST, SUITE_CODE, SUITE_TITLE, SUITE_DESCRIPTION, NULL)

# define REQUIREMENT_SUITE(SUITE_CODE, SUITE_TITLE, SUITE_DESCRIPTION) \
	SUITE(TYPE_REQUIREMENT, SUITE_CODE, SUITE_TITLE, SUITE_DESCRIPTION, NULL)

# define TEST_SUITE_LONG_DESCRIPTION(SUITE_CODE, SUITE_TITLE, SUITE_DESCRIPTION1, SUITE_DESCRIPTION2) \
	SUITE(TYPE_UNIT_TEST, SUITE_CODE, SUITE_TITLE, SUITE_DESCRIPTION1, SUITE_DESCRIPTION2)

# define REQUIREMENT_SUITE_LONG_DESCRIPTION(SUITE_CODE, SUITE_TITLE, SUITE_DESCRIPTION1, SUITE_DESCRIPTION2) \
	SUITE(TYPE_REQUIREMENT, SUITE_CODE, SUITE_TITLE, SUITE_DESCRIPTION1, SUITE_DESCRIPTION2)

/*
	RUNNERS
	________________________________________________________________
*/

# define SUITE_DECLARATION(ID) extern test_suite suite_##ID;
# define SUITE_ENUMERATION(ID) &suite_##ID,

# define SUITE_COLLECTION(COLLECTION_NAME) \
	\
	COLLECTION(SUITE_DECLARATION) \
	\
	static test_suite * COLLECTION_NAME##_suites[] = { \
		COLLECTION(SUITE_ENUMERATION) \
	}; \
	\
	test_suite_collection COLLECTION_NAME = { \
		/* suite */ COLLECTION_NAME##_suites, \
		/* count */ sizeof(COLLECTION_NAME##_suites) / sizeof(COLLECTION_NAME##_suites[0]) \
	};

# define END_SUITE_COLLECTION NEW


typedef int (*test_function)(void);

typedef struct unit_test_struct				unit_test;
typedef struct test_suite_struct			test_suite;

typedef struct test_collection_struct		test_collection;
typedef struct test_suite_collection_struct	test_suite_collection;

typedef struct test_runner_struct			test_runner;
typedef struct statistics_struts			statistics;

struct statistics_struts{

	int						total;
	int						passed;
	int						warnings;
	int						failed;
};

struct unit_test_struct{

	E4C_BOOL				is_requirement;
	const char *			code;
	const char *			title;
	const char *			description1;
	const char *			description2;
	E4C_BOOL				is_critical;
	const char *			at_failure;
	test_function			function;
	int						expected_exit_code;
	const char *			expected_output;
	const char *			expected_error;
	int						found_exit_code;
	char					found_output[640];
	char					found_error[1024 * 2];
	E4C_BOOL				unexpected_exit_code;
	E4C_BOOL				unexpected_output;
	E4C_BOOL				unexpected_error;
	int						status;
};

struct test_suite_struct{

	E4C_BOOL				is_requirement;
	const char *			title;
	const char *			description1;
	const char *			description2;
	test_collection *		tests;
	statistics				stats;
	int						status;
};

struct test_collection_struct{
	unit_test * *			test;
	int						count;
};

struct test_suite_collection_struct{

	test_suite * *			suite;
	int						count;
};

struct test_runner_struct{

	const char *			file_path;
	int						suite_number;
	int						test_number;
	char					buffer[1024];

	const char *			out;
	const char *			err;
	const char *			report;

	test_suite_collection *	suites;

	struct{
		statistics			tests;
		statistics			suites;
		statistics			requirements;
	}						stats;
};

extern int parse_command_line(
	int						argc,
	char *					argv[],
	test_suite_collection *	suite_collection,
	const char *			report,
	const char *			out,
	const char *			err
);

# endif
