/**
 *
 * @file        e4c_st_g.h
 *
 * exceptions4c gcc stack trace header file
 *
 * @version     2.0
 * @author      Copyright (c) 2012 Guillermo Calvo
 *
 * @section e4c_st_g_h exceptions4c gcc stack trace header file
 *
 * This extension allows **exceptions4c** to print a trace stack regarding an
 * uncaught exception:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Uncaught RuntimeException: He who foos last, foos best.
 *
 *     thrown at thud (foobar.c:9)
 *     from xyzzy (foobar.c:20)
 *     from plugh (foobar.c:25)
 *     from fred (foobar.c:30)
 *     from waldo (foobar.c:35)
 *     from garply (foobar.c:40)
 *     from grault (foobar.c:45)
 *     from corge (foobar.c:50)
 *     from quux (foobar.c:55)
 *     from qux (foobar.c:63)
 *     from baz (foobar.c:68)
 *     from bar (foobar.c:75)
 *     from foo (foobar.c:84)
 *     from main (foobar.c:96)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * You need to set up the provided exception context handlers through the
 * function `#e4c_context_set_handlers`:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
 * int main(int argc, char *argv[]){
 *
 *     e4c_using_context(true){
 *
 *         e4c_context_set_handlers(
 *             e4c_stack_trace_print_exception,
 *             argv[0],
 *             e4c_stack_trace_initialize,
 *             e4c_stack_trace_finalize
 *         );
 *
 *         // ...
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This module needs to be compiled with GCC (the GNU Compiler Collection). It
 * depends on the *GCC Function Instrumentation* and *Generate debugging
 * information* functionality. They can be enabled by using the compiler
 * parameters:
 *
 *   - `-finstrument-functions`
 *   - `-g3`
 *
 * @section license License
 *
 * > This is free software: you can redistribute it and/or modify it under the
 * > terms of the **GNU Lesser General Public License** as published by the
 * > *Free Software Foundation*, either version 3 of the License, or (at your
 * > option) any later version.
 * >
 * > This software is distributed in the hope that it will be useful, but
 * > **WITHOUT ANY WARRANTY**; without even the implied warranty of
 * > **MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**. See the
 * > [GNU Lesser General Public License](http://www.gnu.org/licenses/lgpl.html)
 * > for more details.
 * >
 * > You should have received a copy of the GNU Lesser General Public License
 * > along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */


# ifndef EXCEPTIONS4C_STACK_TRACE_GCC
# define EXCEPTIONS4C_STACK_TRACE_GCC


# ifndef EXCEPTIONS4C
#	include "e4c.h"
# endif


/*@-exportany@*/


/**
 * Prints a fatal error message and backtrace regarding the uncaught exception
 *
 * @param   exception
 *          The uncaught exception
 *
 * This function prints the exception and its backtrace to the standard error
 * output. The stack trace represents the reverse path of execution at the
 * moment the exception was thrown.
 *
 * It must be passed to `#e4c_context_set_handlers` as the handler for uncaught
 * exceptions, along with `#e4c_stack_trace_initialize` and
 * `#e4c_stack_trace_finalize`.
 *
 * @see     #e4c_context_set_handlers
 * @see     #e4c_stack_trace_initialize
 * @see     #e4c_stack_trace_finalize
 * @see     #e4c_initialize_handler
 * @see     #e4c_finalize_handler
 * @see     #e4c_uncaught_handler
 */
/*@unused@*/ extern
void e4c_stack_trace_print_exception(
	/*@temp@*/ /*@notnull@*/
	const e4c_exception * exception
)
/*@globals
	fileSystem,
	internalState,

	NotEnoughMemoryException,
	NullPointerException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/**
 * Initializes the backtrace of a newly created exception
 *
 * @param   exception
 *          The newly created exception
 * @return  The backtrace to be assigned to the exception
 *
 * This function captures the call stack at the moment the exception is being
 * thrown. It expects that the initial value of the custom data is a text string
 * containing the path to the executing program.
 *
 * It must be passed to `#e4c_context_set_handlers` as the handler for
 * initializing the custom data of an exception, along with
 * `#e4c_stack_trace_finalize` and `#e4c_stack_trace_print_exception`.
 *
 * @see     #e4c_context_set_handlers
 * @see     #e4c_stack_trace_finalize
 * @see     #e4c_stack_trace_print_exception
 * @see     #e4c_initialize_handler
 * @see     #e4c_finalize_handler
 * @see     #e4c_uncaught_handler
 */
/*@unused@*/ extern
void * e4c_stack_trace_initialize(
	/*@temp@*/ /*@notnull@*/
	const e4c_exception * exception
)
/*@globals
	fileSystem,
	internalState,

	NotEnoughMemoryException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/**
 * Finalizes the backtrace of an exception which is about to be destroyed
 *
 * @param   custom_data
 *          The backtrace to be finalized
 *
 * This function disposes of the call stack captured at the moment the exception
 * was thrown.
 *
 * It must be passed to `#e4c_context_set_handlers` as the handler for
 * finalizing the custom data of an exception, along with
 * `#e4c_stack_trace_initialize` and `#e4c_stack_trace_print_exception`.
 *
 * @see     #e4c_context_set_handlers
 * @see     #e4c_stack_trace_initialize
 * @see     #e4c_stack_trace_print_exception
 * @see     #e4c_initialize_handler
 * @see     #e4c_finalize_handler
 * @see     #e4c_uncaught_handler
 */
/*@unused@*/ extern
void e4c_stack_trace_finalize(
	/*@owned@*/ /*@null@*/
	void * custom_data
)
/*@globals
	fileSystem,
	internalState,

	NotEnoughMemoryException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;


/*@=exportany@*/


# endif
