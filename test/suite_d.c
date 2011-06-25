
# include "testing.h"

/*@-exportheadervar@*/

# define NEW TEST_SUITE_LONG_DESCRIPTION(d, "Uncaught", "\
\
This suite of tests checks the behavior of the library when an exception is \
thrown and no <code>catch</code> block is capable of handling it. The library \
must prevent the execution of the next instructions following the \
<code>throw</code> clause, execute all pending <code>finally</code> blocks, \
call the uncaught handler (if any), and then either:", "\
<ul>\
<li>exit the program with the status code <code>EXIT_FAILURE</code>.</li>\
<li>terminate the current thread with a <code>PTHREAD_CANCELED</code> \
status code.</li>\
</ul>\
Depending on whether the library is in thread-unsafe or thread-safe mode.\
\
")

# define COLLECTION(TEST) \
			TEST(d01) \
			TEST(d02) \
			TEST(d03) \
			TEST(d04) \
			TEST(d05) \

END_SUITE

/*@=exportheadervar@*/
