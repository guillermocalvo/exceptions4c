
# include "testing.h"


typedef struct{ int id; const char * name; } custom_data_t;
static custom_data_t initial_data = {123, "FOOBAR"};


/**
 * Defining custom data
 *
 * This test defines the custom data that will be used every time a new
 * exception is thrown. Then *throws* an exception; the `catch` block inspects
 * the exception's *custom data*.
 *
 */
TEST_CASE{

    e4c_context_begin(E4C_FALSE);

    e4c_context_set_handlers(NULL, &initial_data, NULL, NULL);

    E4C_TRY{

        E4C_THROW(RuntimeException, "Initialize my custom data with a default value");

    }E4C_CATCH(RuntimeException){

        custom_data_t * data = e4c_get_exception()->custom_data;

        TEST_ASSERT_EQUALS(data->id, initial_data.id);
        TEST_ASSERT_STRING_EQUALS(data->name, initial_data.name);
    }

    e4c_context_end();
}
