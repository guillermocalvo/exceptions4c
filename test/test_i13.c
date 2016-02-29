
# include "testing.h"
# include <signal.h>


static e4c_signal_mapping custom_mappings[] = {

	E4C_SIGNAL_MAPPING(SIGTERM, WildException),
	E4C_NULL_SIGNAL_MAPPING
};

static char foobar[] = "FOOBAR";

static void * custom_initialize_handler(const e4c_exception * exception){

	fprintf(stderr, "CUSTOM_INITIALIZE_HANDLER\n");

	return(foobar);
}


DEFINE_TEST(
	i13,
	"Mixing custom initialization handler and signal mappings",
	"This test sets both custom initialization handler and signal mappings. Then a <em>signal</em> is raised and the <code>catch</code> block inspects the exception.",
	NULL,
	EXIT_SUCCESS,
	"before_CONTEXT_END",
	"WildException_FOOBAR"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_FALSE);

	ECHO(("before_SET_SIGNAL_MAPPINGS\n"));

	e4c_context_set_signal_mappings(custom_mappings);

	ECHO(("before_SET_HANDLERS\n"));

	e4c_context_set_handlers(NULL, NULL, custom_initialize_handler, NULL);

	E4C_TRY{

		ECHO(("before_raise_SIGTERM\n"));

		raise(SIGTERM);

		ECHO(("after_raise_SIGTERM\n"));

	}E4C_CATCH(RuntimeException){

		const e4c_exception * e = e4c_get_exception();

		ECHO(("inside_CATCH_block\n"));

		fprintf(stderr, "%s_%s", e->name, (const char *)e->custom_data);
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
