
# include "testing.h"

/*@-exportheadervar@*/

# define NEW TEST_SUITE(f, "Caught", "\
\
This suite of tests checks the behavior of the library when an exception is \
thrown and there is some <code>catch</code> block capable of handling it. The \
library must prevent the execution of the next instructions following the \
<code>throw</code> clause, execute all pending <code>finally</code> blocks, \
execute the appropriate <code>catch</code> block, and then execute the last \
<code>finally</code> block (if there was any, next to the <code>catch</code> \
block).\
\
")

# define COLLECTION(TEST) \
			TEST(f01) \
			TEST(f02) \
			TEST(f03) \
			TEST(f04) \
			TEST(f05) \
			TEST(f06) \
			TEST(f07) \

END_SUITE

/*@=exportheadervar@*/
