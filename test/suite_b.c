
# include "testing.h"

/*@-exportheadervar@*/

# define NEW TEST_SUITE_LONG_DESCRIPTION(b, "Consistency", "\
\
This suite of tests checks the behavior of the library when it is used in an \
inconsistent way. For example, the tests try to:", " \
<ul>\
	<li>begin twice in a row the exception context</li>\
	<li>end twice in a row the exception context</li>\
	<li>interact with the exception handling system after having ended the \
	exception context</li>\
	<li>perform some others illegal operations, such as breaking out of a \
	<code>try</code> block, etc.</li>\
</ul>\
Consequently, the library must throw the appropriate exception, in order to \
signal the misuse to the client.\
\
")

# define COLLECTION(TEST) \
			TEST(b01) \
			TEST(b02) \
			TEST(b03) \
			TEST(b04) \
			TEST(b05) \
			TEST(b06) \
			TEST(b07) \
			TEST(b08) \
			TEST(b09) \
			TEST(b10) \
			TEST(b11) \
			TEST(b12) \
			TEST(b13) \
			TEST(b14) \

END_SUITE

/*@=exportheadervar@*/
