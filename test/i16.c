
# include "testing.h"


# ifdef E4C_THREADSAFE
#   define IS_THREAD_SAFE 1L
# else
#   define IS_THREAD_SAFE 0L
# endif


/**
 * Obtaining the version number of the library
 *
 * This test calls `e4c_library_version` to retrieve the version number of
 * exceptions4c.
 *
 */
TEST_CASE{

    long library_version;
    long expected_version =
        10000000L * IS_THREAD_SAFE +
         1000000L * E4C_VERSION_MAJOR +
            1000L * E4C_VERSION_MINOR +
               1L * E4C_VERSION_REVISION;

    library_version = e4c_library_version();

    TEST_DUMP("%ld", library_version);

    TEST_ASSERT_EQUALS(library_version, expected_version);
}
