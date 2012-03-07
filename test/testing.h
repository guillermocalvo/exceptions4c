
# ifndef TESTING_FRAMEWORK_H

# define TESTING_FRAMEWORK_H

# include <stdio.h>
# include "e4c.h"

/*@-exportany@*/

# define EXIT_WHATEVER			54321
# define ERROR_WHATEVER			(void *)54321
# define OUTPUT_WHATEVER		(void *)54321

# if E4C_VERSION_THREADSAFE == 1
#	define IF_NOT_THREADSAFE(EXIT_CODE) EXIT_WHATEVER
# else
#	define IF_NOT_THREADSAFE(EXIT_CODE) EXIT_CODE
# endif

# define SEVERITY_CRITICAL		E4C_TRUE
# define SEVERITY_NOT_CRITICAL	E4C_FALSE

# define TYPE_REQUIREMENT		E4C_TRUE
# define TYPE_UNIT_TEST			E4C_FALSE

# define STATUS_PASSED			0
# define STATUS_WARNING			1
# define STATUS_FAILED			2

# define BOOL_EQUAL(x, y)		( (x) ? (y) : !(x) )
# define BOOL_NOT_EQUAL(x, y)	( (x) ? !(y) : (x) )

# if	defined(HAVE_C99_SNPRINTF) \
	||	defined(HAVE_SNPRINTF) \
	||	defined(S_SPLINT_S)
#	define SAFE_SPRINTF						(void)snprintf
#	define SAFE_BUFFER_ARG(buffer, size)	buffer, size_t size
#	define SAFE_BUFFER(buffer, size)		(buffer), (size)
#	define SAFE_ARRAY(buffer)				(buffer), sizeof(buffer)
# else
#	define SAFE_SPRINTF						(void)sprintf
#	define SAFE_BUFFER_ARG(buffer, size)	buffer
#	define SAFE_BUFFER(buffer, size)		(buffer)
#	define SAFE_ARRAY(buffer)				(buffer)
# endif

/*
	TESTS
	________________________________________________________________
*/

# define DEFINE_UNIT_TEST(IS_REQUIREMENT, CODE, TITLE, DESCRIPTION1, DESCRIPTION2, IS_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR) \
	/*@-globuse@*/ \
	static int test_##CODE##_function(void) \
	/*@globals \
		fileSystem, \
		internalState, \
		\
		e4c_default_signal_mappings, \
		\
		AbortException, \
		AssertionException, \
		BadPointerException, \
		MemoryAllocationException, \
		NotEnoughMemoryException, \
		NullPointerException, \
		RuntimeException, \
		SignalException \
	@*/ \
	/*@modifies \
		fileSystem, \
		internalState \
	@*/; \
	/*@=globuse@*/ \
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
		/* found_output */			/*@-initallelements@*/ { '\0' }, /*@=initallelements@*/ \
		/* found_error */			/*@-initallelements@*/ { '\0' }, /*@=initallelements@*/ \
		/* unexpected_exit_code */	E4C_FALSE, \
		/* unexpected_output */		E4C_FALSE, \
		/* unexpected_error */		E4C_FALSE, \
		/* status */				0 \
	}; \
	\
	static int test_##CODE##_function(void)

# define DEFINE_TEST(CODE, TITLE, DESCRIPTION, AT_FAILURE, EXIT_CODE, OUT, ERR) \
	DEFINE_UNIT_TEST(TYPE_UNIT_TEST, CODE, TITLE, DESCRIPTION, NULL, SEVERITY_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR)

# define DEFINE_REQUIREMENT(CODE, TITLE, DESCRIPTION, IS_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR) \
	DEFINE_UNIT_TEST(TYPE_REQUIREMENT, CODE, TITLE, DESCRIPTION, NULL, IS_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR)

# define DEFINE_TEST_LONG_DESCRIPTION(CODE, TITLE, DESCRIPTION1, DESCRIPTION2, AT_FAILURE, EXIT_CODE, OUT, ERR) \
	DEFINE_UNIT_TEST(TYPE_UNIT_TEST, CODE, TITLE, DESCRIPTION1, DESCRIPTION2, SEVERITY_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR)

# define DEFINE_REQUIREMENT_LONG_DESCRIPTION(CODE, TITLE, DESCRIPTION1, DESCRIPTION2, IS_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR) \
	DEFINE_UNIT_TEST(TYPE_REQUIREMENT, CODE, TITLE, DESCRIPTION1, DESCRIPTION2, IS_CRITICAL, AT_FAILURE, EXIT_CODE, OUT, ERR)

/*@unchecked@*/
E4C_DECLARE_EXCEPTION(WildException);
/*@unchecked@*/
E4C_DECLARE_EXCEPTION(TamedException);

/*@unchecked@*/
E4C_DECLARE_EXCEPTION(ChildException);
/*@unchecked@*/
E4C_DECLARE_EXCEPTION(SiblingException);
/*@unchecked@*/
E4C_DECLARE_EXCEPTION(ParentException);
/*@unchecked@*/
E4C_DECLARE_EXCEPTION(GrandparentException);

# define ECHO(args) \
	(void)printf args; \
	(void)fflush(stdout);

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
	/*@unused@*/ \
	test_collection test_collection_##SUITE_CODE = { \
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

# define SUITE_DECLARATION(ID) \
	/*@-redecl@*/ \
	extern test_suite suite_##ID; \
	/*@=redecl@*/

# define SUITE_ENUMERATION(ID) \
	&suite_##ID,

# define SUITE_COLLECTION(COLLECTION_NAME) \
	\
	COLLECTION(SUITE_DECLARATION) \
	\
	static test_suite * COLLECTION_NAME##_suites[] = { \
		COLLECTION(SUITE_ENUMERATION) \
	}; \
	\
	/*@unused@*/ \
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

	size_t					total;
	size_t					passed;
	size_t					warnings;
	size_t					failed;
};

struct unit_test_struct{

	E4C_BOOL				is_requirement;
	/*@observer@*/
	const char *			code;
	/*@observer@*/
	const char *			title;
	/*@observer@*/
	const char *			description1;
	/*@observer@*/ /*@null@*/
	const char *			description2;
	E4C_BOOL				is_critical;
	/*@observer@*/ /*@null@*/
	const char *			at_failure;
	test_function			function;
	int						expected_exit_code;
	/*@observer@*/ /*@null@*/
	const char *			expected_output;
	/*@observer@*/ /*@null@*/
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
	/*@observer@*/
	const char *			title;
	/*@observer@*/
	const char *			description1;
	/*@observer@*/ /*@null@*/
	const char *			description2;
	/*@shared@*/
	test_collection *		tests;
	statistics				stats;
	int						status;
};

struct test_collection_struct{

	/*@shared@*/
	unit_test * *			test;
	const size_t			count;
};

struct test_suite_collection_struct{

	/*@shared@*/
	test_suite * *			suite;
	const size_t			count;
};

struct test_runner_struct{

	/*@observer@*/
	const char *			file_path;
	size_t					suite_number;
	size_t					test_number;
	char					buffer[1024];

	/*@observer@*/
	const char *			out;
	/*@observer@*/
	const char *			err;
	/*@observer@*/
	const char *			report;

	/*@shared@*/
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
	/*@shared@*/
	test_suite_collection *	suite_collection,
	const char *			report,
	const char *			out,
	const char *			err
)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/*@unchecked@*/
extern test_suite_collection
	ALL_SUITES,
	SUITE_BEGINNING,
	SUITE_CONSISTENCY,
	SUITE_ENDING,
	SUITE_UNCAUGHT,
	SUITE_FINALLY,
	SUITE_CAUGHT,
	SUITE_SIGNALS,
	SUITE_INTEGRATION,
	PLATFORM_REQUIREMENTS;


/*@=exportany@*/


# endif
