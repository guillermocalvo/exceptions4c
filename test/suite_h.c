
# include "testing.h"

# define NEW TEST_SUITE(h, "Integration", "\
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
			TEST(h01) \
			TEST(h02) \
			TEST(h03) \
			TEST(h04) \
			TEST(h05) \
			TEST(h06) \
			TEST(h07) \
			TEST(h08) \
			TEST(h09) \
			TEST(h10) \
			TEST(h11) \

END_SUITE
