
# include "testing.h"

# define deallocate_buffer(resource, failed) free(resource)


DEFINE_TEST(
	a03,
	"with-use{...} without beginning",
	"This test uses the library improperly, by attempting to <strong>start a <code>with... use</code> block</strong>, without calling <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	EXIT_FAILURE,
	"before_WITH_block",
	"ContextHasNotBegunYet"
){

	char * buffer;

	printf("before_WITH_block\n");

	E4C_WITH(buffer, deallocate_buffer){
        buffer = malloc(256);
		printf("inside_WITH_block\n");
	}E4C_USE{
		printf("inside_USE_block\n");
	}

	printf("after_USE_block\n");

	return(EXIT_SUCCESS);
}
