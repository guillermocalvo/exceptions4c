
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

END_SUITE

/*@=exportheadervar@*/
