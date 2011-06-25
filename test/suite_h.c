
# include "testing.h"

/*@-exportheadervar@*/

# define NEW TEST_SUITE(h, "Integration", "\
\
This suite of tests checks the behavior of the library when \
<code>e4c_reusing_context</code> is used. This integration mechanism lets \
library developers make use of exceptions4c, regardless of whether the clients \
are unaware of the exception handling system. Some of the tests will propagate \
exceptions to the caller, some others will return a status code. \
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

/*@=exportheadervar@*/
