
# include "testing.h"

/*@-exportheadervar@*/

# define NEW TEST_SUITE(a, "Beginning", "\
\
This suite of tests checks the behavior of the library when it is used \
improperly. The tests try to interact with the exception handling system \
without having begun the exception context before. Consequently, the library \
must throw the exception <code>ContextHasNotBegunYet</code> in order to signal \
the misuse to the client.\
\
")

# define COLLECTION(TEST) \
			TEST(a01) \
			TEST(a02) \
			TEST(a03) \
			TEST(a04) \
			TEST(a05) \
			TEST(a06) \

END_SUITE

/*@=exportheadervar@*/
