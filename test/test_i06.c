
# include "testing.h"


typedef struct{ int id; const char * name; } custom_data_t;
static custom_data_t custom_data = {123, "FOOBAR"};


DEFINE_TEST(
	i06,
	"Defining custom data",
	"This test defines the custom data that will be used every time a new exception is thrown. Then <em>throws</em> an exception; the <code>catch</code> block inspects the exception's <em>custom data</em>.",
	NULL,
	EXIT_SUCCESS,
	"before_CONTEXT_END",
	"CUSTOM_DATA_123_FOOBAR"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_SET_HANDLERS\n"));

	e4c_context_set_handlers(NULL, &custom_data, NULL, NULL);

	ECHO(("before_TRY\n"));

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW(WildException, "Initialize my custom data with a default value");

	}E4C_CATCH(WildException){

		custom_data_t * data;

		ECHO(("inside_CATCH_block\n"));

		data = e4c_get_exception()->custom_data;

		fprintf(stderr, "CUSTOM_DATA_%d_%s", data->id, data->name);
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
