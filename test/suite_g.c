
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
			TEST(g10) \
			TEST(g11) \
			TEST(g12) \
			TEST(g13) \
			TEST(g14) \
			TEST(g15) \
			TEST(g16) \
			TEST(g17) \
			TEST(g18) \
			TEST(g19) \
			TEST(g20) \
			TEST(g21) \
			TEST(g22) \
			TEST(g23) \
			TEST(g24) \

END_SUITE

/*@=exportheadervar@*/
