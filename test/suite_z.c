
# include "testing.h"

/*@-exportheadervar@*/

# define NEW REQUIREMENT_SUITE(z, "PlatformRequirements", "\
\
<div class=\"paragraph\"> \
This is special test suite to check the platform on which the library is to be \
compiled and executed.\
</div> \
<div class=\"paragraph\"> \
Any errors in this test suite will affect some of the library's \
functionality on this specific platform. \
</div> \
\
")

# define COLLECTION(TEST) \
			TEST(z01) \
			TEST(z02) \
			TEST(z03) \
			TEST(z04) \
			TEST(z05) \
			TEST(z06) \
			TEST(z07) \
			TEST(z08) \
			TEST(z09) \
			TEST(z10) \
			TEST(z11) \
			TEST(z12) \

END_SUITE

/*@=exportheadervar@*/
