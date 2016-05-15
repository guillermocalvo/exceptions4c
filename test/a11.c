
# include "testing.h"


DEFINE_TEST(
	a11,
	"e4c_frame_get_stage() without beginning",
	"This test uses the library improperly, by attempting to <strong>call <code>e4c_frame_get_stage()</code></strong>, without having called <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_FRAME_GET_STAGE",
	"ContextHasNotBegunYet"
){

	ECHO(("before_FRAME_GET_STAGE\n"));

	/* This functions must not be called like this */
	(void)e4c_frame_get_stage_(__FILE__, __LINE__, "test");

	ECHO(("after_FRAME_GET_STAGE\n"));

	return(EXIT_SUCCESS);
}
