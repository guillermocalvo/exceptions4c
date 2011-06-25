
# include "testing.h"

/*@-exportheadervar@*/

# define NEW TEST_SUITE(g, "Signals", "\
\
This suite of tests checks the behavior of the library when the program \
receives a signal from the operating system. If no signal mapping is provided, \
then the behavior is implementation-defined. However, if the signal mappings \
have been set up, then the library must map the received signal to the \
appropriate exception. Some of the tests will <code>catch</code> it, some \
others won't.\
\
")

# define COLLECTION(TEST) \
			TEST(g01) \
			TEST(g02) \
			TEST(g03) \
			TEST(g04) \
			TEST(g05) \
			TEST(g06) \
			TEST(g07) \
			TEST(g08) \
			TEST(g09) \

END_SUITE

/*@=exportheadervar@*/
