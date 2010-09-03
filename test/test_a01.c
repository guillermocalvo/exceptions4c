
# include "testing.h"


DEFINE_TEST(
	a01,
	"try{...} without beginning",
	"This test uses the library improperly, by attempting to <strong>start a <code>try</code> block</strong>, without calling <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	EXIT_FAILURE,
	"before_TRY_block",
	"ContextHasNotBegunYet"
){

	printf("before_TRY_block\n");

	try{
		printf("inside_TRY_block\n");
	}

	printf("after_TRY_block\n");

	return(EXIT_SUCCESS);
}
