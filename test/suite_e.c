
# include "testing.h"

/*@-exportheadervar@*/

# define NEW TEST_SUITE(e, "Finally", "\
\
This suite of tests checks the behavior of the library when an exception is \
thrown and no <code>catch</code> block is capable of handling it, more \
specifically, they check the execution of all pending <code>finally</code> \
blocks, before exiting the program.\
\
")

# define COLLECTION(TEST) \
			TEST(e01) \
			TEST(e02) \
			TEST(e03) \

END_SUITE

/*@=exportheadervar@*/
