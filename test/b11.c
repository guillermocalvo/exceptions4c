
# include "testing.h"


DEFINE_TEST(
	b11,
	"use{...} right next to a try{...} block",
	"This test uses the library in an inconsistent way, by attempting to <strong>place a <code>use</code> block right next to a <code>try</code> block</strong>. This is plain wrong, because a <code>use</code> block must be preceded by a <code>with</code> block, so the library must ignore the <code>use</code> block.",
	NULL,
	EXIT_SUCCESS,
	"USE_block_was_ignored",
	NULL
){

	E4C_BOOL is_executed = E4C_FALSE;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{
		ECHO(("inside_TRY_block\n"));
	/* } */ E4C_USE{
		is_executed = E4C_TRUE;
		ECHO(("inside_USE_block\n"));
	}

	ECHO(("after_TRY_block\n"));

	/*

	In order to be able to compile a 'use' block preceded by a 'try', we have to
	remove the ending curly brace, otherwise we get a compiler error.

	Anyway, this test proves that the 'use' block will be ignored.

	*/

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	if(is_executed){
		ECHO(("USE_block_was_NOT_ignored\n"));
	}else{
		ECHO(("USE_block_was_ignored\n"));
	}

	return(EXIT_SUCCESS);
}
