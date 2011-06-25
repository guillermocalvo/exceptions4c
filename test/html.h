
# ifndef HTML_H

# define HTML_H


# include "testing.h"


/*@-exportany@*/


extern void print_html(/*@notnull@*/ const test_runner * runner, /*@notnull@*/ FILE * report)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState,

	report
@*/
;


/*@=exportany@*/


# endif
