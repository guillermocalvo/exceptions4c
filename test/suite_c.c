
# include "testing.h"

/*@-exportheadervar@*/

# define NEW TEST_SUITE(c, "Ending", "\
\
This suite of tests checks the behavior of the library when it is used \
improperly. The tests try to exit the program without having ended the \
exception context before. Consequently, the library must throw the \
exception <code>ContextNotEnded</code> in order to signal the misuse to the \
client.\
\
")

# define COLLECTION(TEST) \
			TEST(c01) \
			TEST(c02) \

END_SUITE

/*@=exportheadervar@*/
