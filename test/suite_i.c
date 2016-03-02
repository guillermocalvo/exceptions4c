
# include "testing.h"

/*@-exportheadervar@*/

# define NEW TEST_SUITE(i, "Features", "\
\
This suite of tests checks the behavior of the library when \
other features of the library are used. \
\
")

# define COLLECTION(TEST) \
			TEST(i01) \
			TEST(i02) \
			TEST(i03) \
			TEST(i04) \
			TEST(i05) \
			TEST(i06) \
			TEST(i07) \
			TEST(i08) \
			TEST(i09) \
			TEST(i10) \
			TEST(i11) \
			TEST(i12) \
			TEST(i13) \
			TEST(i14) \
			TEST(i15) \

END_SUITE

/*@=exportheadervar@*/
