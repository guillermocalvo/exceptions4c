/**
 *
 * @file e4c.h
 *
 * exceptions4c header file
 *
 * @version 2.3
 * @author Copyright (c) 2010 Guillermo Calvo
 *
 * @section e4c_h exceptions4c header file
 *
 * <p>
 * This header file needs to be included from in order to be able to use any of
 * the keywords of the the exception handling system:
 * </p>
 *
 * <ul>
 * <li><code>try</code></li>
 * <li><code>catch</code></li>
 * <li><code>finally</code></li>
 * <li><code>throw</code></li>
 * <li><code>with</code></li>
 * <li><code>using</code></li>
 * </ul>
 *
 * <p>
 * In order to stop defining this keywords, there exists a
 * <code>E4C_NOKEYWORDS</code> <em>compile-time</em> parameter. When the
 * keywords are not defined, the next set of alternative macros can be used to
 * achieve the same functionality:
 * </p>
 *
 * <ul>
 * <li><code>E4C_TRY</code></li>
 * <li><code>E4C_CATCH</code></li>
 * <li><code>E4C_FINALLY</code></li>
 * <li><code>E4C_THROW</code></li>
 * <li><code>E4C_WITH</code></li>
 * <li><code>E4C_USING</code></li>
 * </ul>
 *
 * @section license License
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


# ifndef _E4C_H_
# define _E4C_H_

# define _E4C_VERSION(version)			version(2, 3, 2)

# if !defined(E4C_THREADSAFE) && ( \
		defined(HAVE_PTHREAD_H) \
	||	defined(PTHREAD_H) \
	||	defined(PTHREAD_BARRIER_SERIAL_THREAD) \
	||	defined(PTHREAD_CANCEL_ASYNCHRONOUS) \
	||	defined(PTHREAD_CANCEL_ENABLE) \
	||	defined(PTHREAD_CANCEL_DEFERRED) \
	||	defined(PTHREAD_CANCEL_DISABLE) \
	||	defined(PTHREAD_CANCELED) \
	||	defined(PTHREAD_CREATE_DETACHED) \
	||	defined(PTHREAD_CREATE_JOINABLE) \
	)
# error Please define E4C_THREADSAFE at compiler level to enable the \
multi-thread version of exceptions4c.
# endif

/*
	The _E4C_FUNCTION_NAME compile-time parameter
	could be defined in order to work with some specific compiler.
*/
# ifndef _E4C_FUNCTION_NAME
#	if __STDC_VERSION__ < 199901L
#		if __GNUC__ < 2
#			define _E4C_FUNCTION_NAME	NULL
#		else
#			define _E4C_FUNCTION_NAME	__extension__ __FUNCTION__
#		endif
#	else
#		define _E4C_FUNCTION_NAME		__func__
#	endif
# endif

/* All of these are C99 standard features */
# if __STDC_VERSION__ >= 199901L

#	ifndef HAVE_STD_BOOL_H
#		define HAVE_STD_BOOL_H
#	endif

#	ifndef HAVE_C99_VARIADIC_MACROS
#		define HAVE_C99_VARIADIC_MACROS
#	endif

#	ifndef HAVE_VSNPRINTF
#		define HAVE_VSNPRINTF
#	endif

# endif

# include <stdlib.h>
# include <setjmp.h>

# ifdef HAVE_STD_BOOL_H
#	include <stdbool.h>
# endif

# ifdef __bool_true_false_are_defined
#	define e4c_bool						bool
#	define e4c_false					false
#	define e4c_true						true
# else
#	define e4c_bool						int
#	define e4c_false					0
#	define e4c_true						1
# endif

# ifdef _POSIX_C_SOURCE
#	define _E4C_SETJMP(_address_)		sigsetjmp(_address_, e4c_true)
#	define _E4C_LONGJMP(_address_)		siglongjmp(_address_, 1)
#	define _E4C_JMP_BUF					sigjmp_buf
# else
#	define _E4C_SETJMP(_address_)		setjmp(_address_)
#	define _E4C_LONGJMP(_address_)		longjmp(_address_, 1)
#	define _E4C_JMP_BUF					jmp_buf
# endif

# ifndef NDEBUG
#	define _E4C_FILE_INFO				__FILE__
#	define _E4C_LINE_INFO				__LINE__
#	define _E4C_FUNC_INFO				_E4C_FUNCTION_NAME
#	define _E4C_ASSERT(_condition_) ( \
		(_condition_) \
		? (void)0 \
		: E4C_THROW(AssertionException, "Assertion failed: " #_condition_) \
	)
# else
#	define _E4C_FILE_INFO				NULL
#	define _E4C_LINE_INFO				0
#	define _E4C_FUNC_INFO				NULL
#	define _E4C_ASSERT(_ignore_)		( (void)0 )
# endif

# define _E4C_INFO \
			_E4C_FILE_INFO, \
			_E4C_LINE_INFO, \
			_E4C_FUNC_INFO

# define _E4C_PASTE(_x_, _y_, _z_)		_x_ ## _ ## _y_ ## _ ## _z_
# define _E4C_MANGLE(_pre_, _id_, _post_) _E4C_PASTE(_pre_, _id_, _post_)
# define _E4C_AUTO(_id_)				_E4C_MANGLE(_implicit, _id_, __LINE__)


# ifdef E4C_THREADSAFE
#	define _E4C_V_THREADSAFE			( (int) 1 )
#	define _E4C_V_THREADSAFE_STRING		" (single-thread)"
# else
#	define _E4C_V_THREADSAFE			( (int) 0 )
#	define _E4C_V_THREADSAFE_STRING		" (multi-thread)"
# endif


# define _E4C_V_STRING(_major_, _minor_, _revision_) \
	#_major_ "." #_minor_ "." #_revision_ _E4C_V_THREADSAFE_STRING
# define _E4C_V_NUMBER(_major_, _minor_, _revision_) ( \
	( (long)_E4C_V_THREADSAFE	* 10000000) +	\
	( (long)_major_				* 1000000) +	\
	( (long)_minor_				* 1000) +		\
	( (long)_revision_			* 1)			\
)
# define _E4C_V_MAJOR(_major_, _minor_, _revision_)		( (int)_major_		)
# define _E4C_V_MINOR(_major_, _minor_, _revision_)		( (int)_minor_		)
# define _E4C_V_REVISION(_major_, _minor_, _revision_)	( (int)_revision_	)


/*
 * These undocumented macros hide implementation details from documentation.
 */

# define _E4C_FRAME_LOOP(_stage_) \
	(void)_E4C_SETJMP( *( e4c_frame_init(_stage_, _E4C_INFO) ) ); \
	while( e4c_frame_step() )

# define E4C_TRY \
	_E4C_FRAME_LOOP(_e4c_acquiring) \
	if( e4c_frame_hook(_e4c_trying, NULL, _E4C_INFO) && e4c_frame_step() )
	/* simple optimization: e4c_frame_step() will avoid disposing stage */

# define E4C_CATCH(_exception_type_) \
	else if( e4c_frame_hook(_e4c_catching, (_exception_type_).type, _E4C_INFO) )

# define E4C_FINALLY \
	else if( e4c_frame_hook(_e4c_finalizing, NULL, _E4C_INFO) )

# define E4C_THROW(_exception_type_, _message_) \
	e4c_throw_exception( (_exception_type_).type, _E4C_INFO, \
	e4c_true, _message_ )

# define E4C_WITH(_resource_, _dispose_) \
	_E4C_FRAME_LOOP(_e4c_beginning) \
	if( e4c_frame_hook(_e4c_disposing, NULL, _E4C_INFO) ){ \
		_dispose_( (_resource_), (e4c_get_status() == e4c_failed) ); \
	}else if( e4c_frame_hook(_e4c_acquiring, NULL, _E4C_INFO) ){

# define E4C_USE \
	}else if( e4c_frame_hook(_e4c_trying, NULL, _E4C_INFO) )

# define E4C_USING(_type_, _resource_, _args_) \
	E4C_WITH( (_resource_), e4c_dispose_##_type_){ \
		(_resource_) = e4c_acquire_##_type_ _args_; \
	}E4C_USE

# define _E4C_REUSING_CONTEXT(_thrown_exception_) \
	\
	e4c_bool		_E4C_AUTO(BEGIN)	= !e4c_context_is_ready(); \
	e4c_bool		_E4C_AUTO(DONE)		= e4c_false; \
	e4c_exception	_E4C_AUTO(EXCEPTION); \
	\
	(_thrown_exception_) = NULL; \
	\
	if( _E4C_AUTO(BEGIN) ){ \
		e4c_context_begin(e4c_false, NULL); \
		E4C_TRY{ \
			goto _E4C_AUTO(PAYLOAD); \
			_E4C_AUTO(CLEANUP): \
			( (void)0 ); \
		}E4C_CATCH(RuntimeException){ \
			(_thrown_exception_)		= &_E4C_AUTO(EXCEPTION); \
			_E4C_AUTO(EXCEPTION)		= *e4c_get_exception(); \
			_E4C_AUTO(EXCEPTION).cause	= NULL; \
		} \
		e4c_context_end(); \
		_E4C_AUTO(DONE)		= e4c_true; \
		_E4C_AUTO(BEGIN)	= e4c_false; \
	} \
	\
	_E4C_AUTO(PAYLOAD): \
	for(; !_E4C_AUTO(DONE) || _E4C_AUTO(BEGIN); _E4C_AUTO(DONE) = e4c_true) \
		if( _E4C_AUTO(DONE) ){ \
			goto _E4C_AUTO(CLEANUP); \
		}else

# define _E4C_USING_CONTEXT(_handle_signals_, _uncaught_handler_) \
	\
	for( \
		e4c_context_begin( (_handle_signals_), (_uncaught_handler_) ); \
		e4c_context_is_ready(); \
		e4c_context_end() \
	)

# ifdef HAVE_C99_VARIADIC_MACROS
#	define E4C_THROWF(_exception_type_, _format_, ...) \
		e4c_throw_exception( \
			(_exception_type_).type, _E4C_INFO, \
			e4c_false, _format_, __VA_ARGS__ \
		)
# endif

# define E4C_RETHROW(_message_) \
	e4c_throw_exception( \
		( e4c_get_exception() == NULL ? NULL : e4c_get_exception()->type), \
		_E4C_INFO, e4c_true, _message_ \
	)

# ifdef HAVE_C99_VARIADIC_MACROS
#	define E4C_RETHROWF(_format_, ...) \
		e4c_throw_exception( \
			( e4c_get_exception() == NULL ? NULL : e4c_get_exception()->type), \
			_E4C_INFO, e4c_false, _format_, __VA_ARGS__ \
		)
# endif

# define E4C_RETRY(_max_retry_attempts_) \
	e4c_frame_repeat(_max_retry_attempts_, _e4c_acquiring, _E4C_INFO)

# define E4C_REACQUIRE(_max_acquire_attempts_) \
	e4c_frame_repeat(_max_acquire_attempts_, _e4c_beginning, _E4C_INFO)

# define _E4C_DECLARE_EXCEPTION(_name_) \
	extern const e4c_exception _name_

# define _E4C_DEFINE_EXCEPTION(_name_, _message_, _super_) \
	const e4c_exception _name_ = { \
		/* name			*/	#_name_, \
		/* message		*/	_message_, \
		/* super		*/	&_super_, \
		/* file			*/	_E4C_FILE_INFO, \
		/* line			*/	_E4C_LINE_INFO, \
		/* function		*/	NULL, \
		/* error_number	*/	0, \
		/* type			*/	&_name_, \
		/* cause		*/	NULL \
	}

# define _E4C_SIGNAL_MAPPING(_signal_number_, _exception_) \
	{_signal_number_, &_exception_}

# define _E4C_NULL_SIGNAL_MAPPING \
	{0, NULL}


/**
 * @name Exception handling keywords
 *
 * <p>
 * This set of keywords express the semantics of exception handling.
 * </p>
 *
 * @{
 */

/**
 * Introduces a block of code aware of exceptions
 */
# ifndef E4C_NOKEYWORDS
# define try E4C_TRY
# endif

/**
 * Introduces a block of code capable of handling a specific kind of exceptions
 *
 * @param _exception_type_ The type of exceptions to be handled
 *
 * <p>
 * <code>catch</code> blocks are optional code blocks that must be preceded by
 * <code>try</code>, <code>with/use</code> or <code>using</code> blocks. Several
 * <code>catch</code> blocks can be placed next to one another.
 * </p>
 *
 * <p>
 * When an exception is thrown from a <code>try</code> block, the system looks
 * for a <code>catch</code> block to handle it. The first capable block (in
 * order of appearance) will be executed and the exception is said to be
 * <em>caught</em>.
 * </p>
 *
 * <p>
 * The caught exception can be accessed through the function
 * <code>e4c_get_exception</code>.
 * </p>
 *
 * <p>
 * If a <code>catch</code> block handles (at <em>compile-time</em>) a generic
 * type of exceptions, the specific type of the actual exception can be
 * determined (at <em>run-time</em>) by comparing the <code>type</code> of
 * the caught exception against the type of another exceptions previously
 * defined in the program.
 * </p>
 *
 * <pre class="fragment">
 * #try{
 *    ...
 * }#catch(#RuntimeException){
 *    const #e4c_exception * exception = #e4c_get_exception();
 *    if(exception->type == SignalException.type){
 *        ...
 *    }else if(exception->type == NotEnoughMemoryException.type){
 *        ...
 *    }
 * }
 * </pre>
 *
 * <p>
 * However, this check compares the exception against a specific type. So, if
 * the thrown exception was a <em>subtype</em> of the given exception type, this
 * comparison would then yield <code>false</code>. For example, in the previous
 * example, if the thrown exception was of type
 * <code>#BadPointerException</code>: it would be caught by the
 * <code>catch</code> block, because an instance of a
 * <code>#BadPointerException</code> is also an instance of a
 * <code>#RuntimeException</code>, but the comparison
 * <code>(exception->type == SignalException.type)</code> would
 * yield <code>false</code> because the type of the thrown exception was not
 * <em>strictly speaking</em> <code>#SignalException</code>, but
 * <code>#BadPointerException</code>.
 * </p>
 *
 * <p>
 * There is a more powerful way to find out if the thrown exception <strong>is
 * an instance of</strong> a given type of exception <em>or any subtype</em>.
 * The function <code>#e4c_is_instance_of</code> can determines that:
 * </p>
 *
 * <pre class="fragment">
 * #try{
 *    ...
 * }#catch(#RuntimeException){
 *    const #e4c_exception * exception = #e4c_get_exception();
 *    if( #e4c_is_instance_of(exception, SignalException.type) ){
 *        ...
 *    }else if(exception->type == NotEnoughMemoryException.type){
 *        ...
 *    }
 * }
 * </pre>
 *
 * <p>
 * In this example, the <code>if</code> condition would evaluate to
 * <code>true</code> because a <code>#BadPointerException</code>
 * <strong>is an instance of a</strong> <code>#RuntimeException</code>.
 * </p>
 *
 * <p>
 * After the <code>catch</code> block completes, the <code>finally</code> block
 * (if any) is executed. Then the program continues with the next line following
 * the set of <code>try/catch/finally</code> blocks.
 * </p>
 *
 * <p>
 * However, if an exception is thrown in a <code>catch</code> block, then the
 * <code>finally</code> will be executed right away and the system will look
 * for an outter <code>catch</code> block to handle it.
 * </p>
 *
 * <p>
 * Only one of all the <code>catch</code> blocks will be executed for each
 * <code>try</code> block, even though the executed <code>catch</code> block
 * throws another exception.
 * </p>
 *
 * @see e4c_exception
 * @see e4c_get_exception
 */
# ifndef E4C_NOKEYWORDS
# define catch(_exception_type_) E4C_CATCH(_exception_type_)
# endif

/**
 * Introduces a block of code responsible for cleaning up the previous
 * <code>try</code> block
 *
 * <p>
 * <code>finally</code> blocks are optional code blocks that must be preceded by
 * <code>try</code>, <code>with/use</code> or <code>using</code> blocks. It is
 * allowed to place, at most, one <code>finally</code> block.
 * </p>
 *
 * <p>
 * The <code>finally</code> block can determine the completeness of the
 * <code>try/catch</code> set of blocks through the function
 * <code>e4c_get_status</code>. The thrown exception (if any) can also be
 * accessed through the function <code>e4c_get_exception</code>.
 * </p>
 *
 * <pre class="fragment">
 * #try{
 *    ...
 * }#finally{
 *    switch( #e4c_get_status() ){
 * &nbsp;
 *        case #e4c_succeeded:
 *            ...
 *            break;
 * &nbsp;
 *        case #e4c_recovered:
 *            ...
 *            break;
 * &nbsp;
 *        case #e4c_failed:
 *            ...
 *            break;
 *    }
 * }
 * </pre>
 *
 * <p>
 * Only one of all the <code>catch</code> blocks will be executed for each
 * <code>try</code> block, even though the executed <code>catch</code> block
 * throws another exception.
 * </p>
 *
 * @see e4c_exception
 * @see e4c_get_exception
 * @see e4c_get_status
 * @see e4c_status
 */
# ifndef E4C_NOKEYWORDS
# define finally E4C_FINALLY
# endif

/**
 * Repeats the previous E4C_TRY (or E4C_USE) block entirely
 *
 * <p>
 * This macro discards any thrown exception (if any) and repeats the previous
 * <code>try</code> or <code>use</code> block, up to a specified maximum number
 * of attempts.
 * </p>
 *
 * <p>
 * This macro is intended to be used in <code>catch</code> or
 * <code>finally</code> blocks as a quick way to fix an error condition and try
 * again.
 * </p>
 *
 * <pre class="fragment">
 * const char * file_path = config_get_user_defined_file_path();
 * try{
 *     config = read_config(file_path);
 * }catch(ConfigException){
 *     file_path = config_get_default_file_path();
 *     retry(1);
 *     rethrow("Wrong defaults.");
 * }
 * </pre>
 *
 * <p>
 * If the specified maximum number of attempts is zero, then the
 * block can eventually be attempted an unlimited number of times. Care must be
 * taken in order not to create an <em>infinite loop</em>.
 * </p>
 *
 * <p>
 * This macro won't return control unless the block has already been attempted,
 * at least, the specified maximum number of times.
 * </p>
 *
 * <p>
 * At a <code>catch</code> block, the current exception is considered caught,
 * whether the <code>retry</code> takes place or not. If you want the exception
 * to be propagated when the maximum number of attempts has been reached, then
 * you must throw it again.
 * </p>
 *
 * <pre class="fragment">
 * int dividend = 100;
 * int divisor = 0;
 * int result = 0;
 * try{
 *     result = dividend / divisor;
 *     do_something(result);
 * }catch(RuntimeException){
 *     divisor = 1;
 *     retry(1);
 *     rethrow("Error (not a division by zero).");
 * }
 * </pre>
 *
 * <p>
 * At a <code>finally</code> block, the current exception (if any) will be
 * propagated if the <code>retry</code> does not take place, so you don't need
 * to throw it again.
 * </p>
 *
 * <pre class="fragment">
 * int dividend = 100;
 * int divisor = 0;
 * int result = 0;
 * try{
 *     result = dividend / divisor;
 *     do_something(result);
 * }finally{
 *     if( e4c_get_status() == e4c_failed ){
 *         divisor = 1;
 *         retry(1);
 *         /<span>* when we get here, the exception will be propagated *</span>/
 *     }
 * }
 * </pre>
 *
 * @see reacquire
 * @see try
 *
 * @param _max_retry_attempts_ The maximum number of attempts to retry
 */
# ifndef E4C_NOKEYWORDS
#	define retry(_max_retry_attempts_) E4C_RETRY(_max_retry_attempts_)
# endif

/**
 * Signals an exceptional situation represented by an exception object
 *
 * <p>
 * Creates a new instance of the specified type of exception and then throws it.
 * The provided message is copied into the thrown exception, so it can be freely
 * deallocated. If <code>NULL</code> is passed, then the default message for
 * that type of exception will be used.
 * </p>
 *
 * <p>
 * When an exception is thrown, the exception handling framework looks for the
 * appropriate <code>catch</code> block that can handle the exception. The
 * system unwinds the call chain of the program and executes the
 * <code>finally</code> blocks it finds.
 * </p>
 *
 * <p>
 * When no <code>catch</code> block is able to handle an exception, the system
 * eventually gets to the main function of the program. This situation is called
 * an <strong><em>uncaught exception</em></strong>.
 * </p>
 *
 * @see throwf
 * @see rethrow
 * @see e4c_exception
 * @see e4c_uncaught_handler
 * @see e4c_get_exception
 *
 * @param _exception_type_ The type of exception to be thrown
 * @param _message_ The <em>ad hoc</em> message describing the exception. If
 *        <code>NULL</code>, then the default message for the specified
 *        exception will be used
 */
# ifndef E4C_NOKEYWORDS
# define throw(_exception_type_, _message_) \
	E4C_THROW(_exception_type_, _message_)
# endif

/**
 * Throws again the currently thrown exception, with a new message
 *
 * <p>
 * This macro creates a new instance of the thrown exception, with a more
 * specific message.
 * </p>
 *
 * <p>
 * <code>rethrow</code> is intended to be used in a <code>catch</code> block and
 * the purpose is to refine the message of the currently caught exception. The
 * previous exception (and its message) will be stored as the <em>cause</em> of
 * the newly thrown exception.
 * </p>
 *
 * <pre class="fragment">
 * try{
 *     image = read_file(file_path);
 * }catch(FileOpenException){
 *     rethrow("Image not found.");
 * }
 * </pre>
 *
 * <p>
 * The semantics of this macro are the same as for the <code>throw</code> macro.
 * </p>
 *
 * @see throw
 * @see rethrowf
 *
 * @param _message_ The new message describing the exception. It should be more
 *        specific than the current one
 */
# ifndef E4C_NOKEYWORDS
#	define rethrow(_message_) E4C_RETHROW(_message_)
# endif

/** @} */

/**
 * @name Dispose pattern keywords
 *
 * <p>
 * This set of keywords express the semantics of automatic resource acquisition
 * and disposal.
 * </p>
 *
 * @{
 */

/**
 * Opens a block of code with automatic disposal of a resource
 *
 * @param _resource_ The resource to be disposed
 * @param _dispose_ The name of the disposal function (or macro)
 *
 * <p>
 * The <code>with</code> keyword is used to encapsulate the <em>Dispose
 * Pattern</em>. It must be followed by the <code>use</code> keyword.
 * </p>
 *
 * <p>
 * In addition, the <code>use</code> block can precede <code>catch</code> and
 * <code>finally</code> blocks.
 * </p>
 *
 * <p>
 * This pattern consists of two separate blocks and an implicit call to a
 * given function:
 * </p>
 *
 * <ol>
 * <li>the <code>with</code> block is responsible for the resource
 * acquisition</li>
 * <li>the <code>use</code> block makes use of the resource</li>
 * <li>the disposal function will be called implicitly</li>
 * </ol>
 *
 * <p>
 * The <code>with</code> keyword guarantees that the disposal function will be
 * called <strong>if and only if</strong> the acquisition block
 * <em>completed</em> without an error (i.e. no exteption being thrown from the
 * acquisition block).
 * </p>
 *
 * <p>
 * If the <code>with</code> block does not complete, then neither the disposal
 * function nor the <code>use</code> block will be ever executed.
 * </p>
 *
 * <p>
 * The disposal function is called right after the <code>use</code> block. If an
 * exception was thrown, the <code>catch</code> or <code>finally</code> blocks
 * (if any) will take place <strong>after</strong> the disposal of the resource.
 * </p>
 *
 * <p>
 * When called, the disposal function will receive two arguments:
 * </p>
 *
 * <ol>
 * <li>The resource</li>
 * <li>A boolean flag indicating if the <code>use</code> block did not
 * <strong>complete</strong></li>
 * </ol>
 *
 * <p>
 * This way, different actions can be taken depending on the success or failure
 * of the block. For example, commiting or rolling back a <em>transaction</em>
 * resource.
 * </p>
 *
 * <p>
 * Legacy functions can be reused by defining macros. For example, a file
 * resource needs to be closed regardless of the errors. Since the function
 * <code>fclose</code> only takes one parameter, we could define the next macro:
 * </p>
 *
 * <pre class="fragment">
 * # define e4c_dispose_file(_file_, _failed_) fclose(_file_)
 * </pre>
 *
 * <p>
 * The typical usage of a <code>#with</code> block will be:
 * </p>
 *
 * <pre class="fragment">
 * #with(foo, e4c_dispose_foo){
 *     foo = e4c_acquire_foo(foobar);
 *     someAssertion(foo, bar);
 *     ...
 * }#use{
 *     doSomething(foo);
 *     ...
 * }#catch(FooAcquisitionFailed){
 *     recover1();
 *     ...
 * }#catch(somethingElseFailed){
 *     recover2();
 *     ...
 * }#finally{
 *     cleanup();
 *     ...
 * }
 * </pre>
 *
 * <p>
 * Nonetheless, one-liners fit nicely too:
 * </p>
 *
 * <pre class="fragment">
 * #with(foo, e4c_dispose_foo) foo = e4c_acquire_foo(bar, foobar); use doSomething(foo);
 * </pre>
 *
 * <p>
 * There is a way to lighten up even more this pattern by defining convenience
 * macros, customized for a specific kind of resources. For example,
 * <code>#e4c_using_file</code> or <code>#e4c_using_memory</code>.
 * </p>
 *
 * @see use
 * @see using
 * @see e4c_using_if_not_null
 * @see e4c_using_memory
 * @see e4c_using_file
 */
# ifndef E4C_NOKEYWORDS
# define with(_resource_, _dispose_) E4C_WITH(_resource_, _dispose_)
# endif

/**
 * Closes a block of code with automatic disposal of a resource
 *
 * <p>
 * A <code>use</code> block must always be preceded by a <code>with</code>
 * block. These two macros are designed so the compiler will complain about
 * dangling <code>with</code> or <code>use</code> blocks.
 * </p>
 *
 * <p>
 * A code block introduced by the <code>use</code> keyword will only be executed
 * if the acquisition of the resource completes without any exception.
 * </p>
 *
 * <p>
 * Either if the <code>use</code> block completes or not, the disposal function
 * will be executed right away.
 * </p>
 *
 * @see with
 */
# ifndef E4C_NOKEYWORDS
# define use E4C_USE
# endif

/**
 * Introduces a block of code with automatic acquisition and disposal of a
 * resource
 *
 * @param _type_ The type of the resource
 * @param _resource_ The resource to be acquired, used and then disposed
 * @param _args_ A list of arguments to be passed to the acquisition function
 *
 * <p>
 * The specified resource will be acquired, used and then disposed. The
 * automatic acquisition and disposal is achieved by calling the <em>magic</em>
 * functions:
 * </p>
 *
 * <ul>
 * <li><code>_type_ e4c_acquire_<em>_type_</em>(_args_)</code></li>
 * <li><code>void e4c_dispose<em>_type_</em>(_resource_, _failed_)</code></li>
 * </ul>
 *
 * <p>
 * The resource will be acquired implicitly by assigning to it the result of the
 * <em>magic</em> acquisition function <code>e4c_acquire_<em>_type_</em></code>.
 * </p>
 *
 * <p>
 * The semantics of the automatic acquisition and disposal are the same as for
 * blocks introduced by the keyword <code>with</code>. For example, a
 * <code>using</code> block can also precede <code>catch</code> and
 * <code>finally</code> blocks.
 * </p>
 *
 * @see with
 * @see e4c_using_if_not_null
 * @see e4c_using_memory
 * @see e4c_using_file
 */
# ifndef E4C_NOKEYWORDS
# define using(_type_, _resource_, _args_) E4C_USING(_type_, _resource_, _args_)
# endif

/**
 * Repeats the previous E4C_WITH block entirely
 *
 * <p>
 * This macro discards any thrown exception (if any) and repeats the previous
 * <code>with</code> block, up to a specified maximum number of attempts. If the
 * acquisition completes, then the <code>use</code> block will be executed.
 * </p>
 *
 * <p>
 * This macro is intended to be used in <code>catch</code> or
 * <code>finally</code> blocks, next to a <code>with... use</code> or
 * <code>using</code> block when the resource acquisition failed, as a quick way
 * to fix an error condition and try to acquire the resource again.
 * </p>
 *
 * <pre class="fragment">
 * image_type * image;
 * const char * image_path = image_get_user_avatar();
 * with(image, e4c_image_dispose){
 *     image = e4c_image_acquire(image_path);
 * }use{
 *     image_show(image);
 * }catch(ImageNotFoundException){
 *     image_path = image_get_default_avatar();
 *     reacquire(1);
 * }
 * </pre>
 *
 * <p>
 * If the specified maximum number of attempts is zero, then the
 * <code>with</code> block can eventually be attempted an unlimited number of
 * times. Care must be taken in order not to create an <em>infinite loop</em>.
 * </p>
 *
 * <p>
 * This macro won't return control unless the <code>with</code> block has
 * already been attempted, at least, the specified maximum number of times.
 * </p>
 *
 * <p>
 * Once the resource has been acquired, the <code>use</code> block can also be
 * repeated <em>alone</em> through the <code>retry</code> macro.
 * </p>
 *
 * <pre class="fragment">
 * image_type * image;
 * const char * image_path = image_get_user_avatar();
 * display_type * display = display_get_user_screen();
 * with(image, e4c_image_dispose){
 *     image = e4c_image_acquire(image_path);
 * }use{
 *     image_show(image, display);
 * }catch(ImageNotFoundException){
 *     image_path = image_get_default_avatar();
 *     reacquire(1);
 * }catch(DisplayException){
 *     display = display_get_default_screen();
 *     retry(1);
 * }
 * </pre>
 *
 * @see retry
 * @see with
 * @see use
 *
 * @param _max_acquire_attempts_ The maximum number of attempts to reacquire
 */
# ifndef E4C_NOKEYWORDS
#	define reacquire(_max_acquire_attempts_) \
		E4C_REACQUIRE(_max_acquire_attempts_)
# endif

/** @} */

/*
 * Binds the acquisition of memory to the standard function <code>malloc</code>
 */
# define e4c_acquire_memory malloc

/*
 * Binds the disposal of memory to the standard function <code>malloc</code>
 */
# define e4c_dispose_memory(_buffer_, _failed_) free(_buffer_)

/*
 * Binds the acquisition of file to the standard function <code>fopen</code>
 */
# define e4c_acquire_file fopen

/*
 * Binds the disposal of memory to the standard function <code>fclose</code>
 */
# define e4c_dispose_file(_file_, _failed_) fclose(_file_)

/**
 * @name Convenience macros for acquiring and disposing resources
 *
 * <p>
 * These macros let you acquire and dispose different kinds of resources
 * according to the <em>dispose pattern</em>.
 * </p>
 *
 * @{
 */

/**
 * Introduces a block of code with automatic acquisition and disposal of a
 * memory buffer
 *
 * @param _buffer_ The buffer to be acquired, used and then disposed
 * @param _bytes_ The amount of memory to be allocated (in bytes)
 *
 * <p>
 * This macro lets you acquire and dispose memory according to the
 * <em>dispose pattern</em>:
 * </p>
 *
 * <pre class="fragment">
 * void * buffer;
 * &nbsp;
 * #e4c_using_memory(buffer, 1024){
 *     /<span>* implicit: buffer = malloc(1024); *</span>/
 *     memset(buffer, 0, 1024);
 *     /<span>* implicit: free(buffer); *</span>/
 * }
 * </pre>
 *
 * <p>
 * If <code>malloc</code> returns <code>NULL</code> then the exception
 * <code>NotEnoughMemoryException</code> will be automatically thrown.
 * </p>
 *
 */
# define e4c_using_memory(_buffer_, _bytes_) \
	e4c_using_if_not_null(memory, _buffer_, (_bytes_), \
	NotEnoughMemoryException, "Could not allocate memory for '" #_buffer_ "'.")

/**
 * Introduces a block of code with automatic acquisition and disposal of a
 * file stream
 *
 * @param _file_ The file to be acquired, used and then disposed
 * @param _path_ The path of the file to be opened
 * @param _mode_ The access mode for the file
 *
 * <p>
 * This macro lets you acquire and dispose (open and close) files according to
 * the <em>dispose pattern</em>:
 * </p>
 *
 * <pre class="fragment">
 * FILE * file;
 * &nbsp;
 * #e4c_using_file(file, "log.txt", "a"){
 *     /<span>* implicit: file = fopen("log.txt", "a"); *</span>/
 *     fputs("hello, world!\n", file);
 *     /<span>* implicit: fclose(file); *</span>/
 * }
 * </pre>
 *
 * <p>
 * If <code>fopen</code> returns <code>NULL</code> then the exception
 * <code>FileOpenException</code> will be automatically thrown.
 * </p>
 *
 * <p>
 * The specific cause of the error can be determined by checking the
 * <code>error_number</code> of the thrown exception (it captures the value of
 * <code>errno</code>).
 * </p>
 *
 */
# define e4c_using_file(_file_, _path_, _mode_) \
	e4c_using_if_not_null( file, _file_, (_path_, _mode_), \
		FileOpenException, "Could not open file: " #_path_)

/**
 * Introduces a block of code with automatic disposal of a resource and
 * acquisition, under certain condition
 *
 * @param _type_ The type of the resource
 * @param _resource_ The resource to be acquired, used and then disposed
 * @param _args_ A list of arguments to be passed to the acquisition function
 * @param _cond_ The condition which has to be satisfied in order to
 *        consider the acquisition <em>complete</em>
 * @param _exception_ The exception to be thrown if the acquisition function
 *        does not satisfy the specified condition
 * @param _msg_ The exception message
 *
 * <p>
 * This macro will attempt the acquisition of the resource and then will check
 * the given condition. If the condition evaluates to false, then the specified
 * exception will be thrown, and therefore, the disposal of the resource will
 * not take place.
 * </p>
 *
 * <p>
 * This is a convenience macro for reusing legacy C standard functions which
 * don't throw exceptions when the acquisition fails. For example:
 * </p>
 *
 * <pre class="fragment">
 * # define e4c_acquire_memory malloc
 * # define e4c_dispose_memory(_memory_, _failed_) free(_memory_)
 * # define #e4c_using_memory(_resource_, _bytes_) \
 *          #e4c_using_if(memory, _resource_, ( _bytes_ ), _resource_ != NULL, \
 *          #NotEnoughMemoryException )
 * </pre>
 *
 * @see using
 */
# define e4c_using_if(_type_, _resource_, _args_, _cond_, _exception_, _msg_) \
	E4C_WITH(_resource_, e4c_dispose_##_type_){ \
		_resource_ = e4c_acquire_##_type_ _args_; \
		if( !(_cond_) ) E4C_THROW(_exception_, _msg_); \
	}E4C_USE

/**
 * Introduces a block of code with automatic disposal of a resource and
 * acquisition, preventing null pointers
 *
 * @param _type_ The type of the resource
 * @param _resource_ The resource to be acquired, used and then disposed
 * @param _args_ A list of arguments to be passed to the acquisition function
 * @param _exception_ The exception to be thrown if the acquisition function
 *        yields a <code>NULL</code> pointer
 * @param _msg_ The exception message
 *
 * @see e4c_using_if
 */
# define e4c_using_if_not_null(_type_, _resource_, _args_, _exception_, _msg_) \
	e4c_using_if(_type_, _resource_, _args_, _resource_ != NULL, \
		_exception_, _msg_)

/** @} */

/**
 * @name Integration macros
 *
 * <p>
 * These macros are designed to ease the integration of external libraries which
 * make use of the exception handling system.
 * </p>
 *
 * @{
 */

/**
 * Provides the library version number
 *
 * <p>
 * The library version number is a <code>long</code> value which expresses:
 * </p>
 *
 * <ul>
 *     <li>library <em>thread</em> mode (either <em>single-thread</em> or
 *     <em>multi-thread</em>)</li>
 *     <li>library <em>major</em> version number</li>
 *     <li>library <em>minor</em> version number</li>
 *     <li>library <em>revision</em> number</li>
 * </ul>
 *
 * <p>
 * The multi-thread (or <em>thread-safe</em>) mode can be obtained by compiling
 * the library with the <code>E4C_THREADSAFE</code> <em>compile-time</em>
 * parameter.
 * </p>
 *
 * <p>
 * The formula to encode these version numbers into a single <code>long</code>
 * value is:
 * <pre class="fragment">
 * THREADSAFE * 10000000 + MAJOR * 1000000 + MINOR * 1000 + REVISION
 * </pre>
 * </p>
 *
 * <p>
 * These numbers can be obtained separately through the next macros:
 * </p>
 *
 * <ul>
 *     <li><code>#E4C_VERSION_THREADSAFE</code></li>
 *     <li><code>#E4C_VERSION_MAJOR</code></li>
 *     <li><code>#E4C_VERSION_MINOR</code></li>
 *     <li><code>#E4C_VERSION_REVISION</code></li>
 * </ul>
 *
 * <p>
 * The library version number can be also obtained as a string literal in the
 * format "MAJOR.MINOR.REVISION (THREADSAFE)" through the
 * <code>#E4C_VERSION_STRING</code> macro.
 * </p>
 *
 * <p>
 * This version number can be considered as the <em>compile-time</em> library
 * version number, as opposed to the <em>run-time</em> library version
 * number (associated with the actual, compiled library). This <em>run-time</em>
 * version number can be obtained through the <code>#e4c_library_version</code>
 * function.
 * </p>
 *
 * <p>
 * The library must be compiled with the corresponding header (i.e. library
 * version number should be equal to header version number).
 * </p>
 *
 *
 * @see e4c_library_version
 * @see E4C_VERSION_THREADSAFE
 * @see E4C_VERSION_MAJOR
 * @see E4C_VERSION_MINOR
 * @see E4C_VERSION_REVISION
 * @see E4C_VERSION_STRING
 */
# define E4C_VERSION_NUMBER	\
	\
	_E4C_VERSION(_E4C_V_NUMBER)

/**
 * Provides the library thread mode (either single-thread or multi-thread)
 *
 * <p>
 * When the library is compiled with the <code>E4C_THREADSAFE</code>
 * <em>compile-time</em> parameter, <code>E4C_VERSION_THREADSAFE</code> will
 * yield the <code>int</code> value <code>1</code> (meaning
 * <em>multi-thread</em> mode), otherwise it will yield the <code>int</code>
 * value <code>0</code> (meaning <em>single-thread</em> mode).
 * </p>
 *
 * @see E4C_VERSION_NUMBER
 */
# define E4C_VERSION_THREADSAFE	\
	\
	_E4C_V_THREADSAFE

/**
 * Provides the library major version number
 *
 * <p>
 * The library major version number is an <code>int</code> value which is
 * incremented from one release to another when there are significant changes
 * in functionality.
 * </p>
 *
 * @see E4C_VERSION_NUMBER
 */
# define E4C_VERSION_MAJOR	\
	\
	_E4C_VERSION(_E4C_V_MAJOR)

/**
 * Provides the library minor version number
 *
 * <p>
 * The library minor version number is an <code>int</code> value which is
 * incremented from one release to another when only minor features or
 * significant fixes have been added.
 * </p>
 *
 * @see E4C_VERSION_NUMBER
 */
# define E4C_VERSION_MINOR	\
	\
	_E4C_VERSION(_E4C_V_MINOR)

/**
 * Provides the library revision number
 *
 * <p>
 * The library revision number is an <code>int</code> value which is incremented
 * from one release to another when minor bugs are fixed.
 * </p>
 *
 * @see E4C_VERSION_NUMBER
 */
# define E4C_VERSION_REVISION	\
	\
	_E4C_VERSION(_E4C_V_REVISION)

/**
 * Provides the library version number as a string literal
 *
 * <p>
 * The format of the string literal is: "MAJOR.MINOR.REVISION (THREADSAFE)".
 * </p>
 *
 * @see E4C_VERSION_NUMBER
 */
# define E4C_VERSION_STRING	\
	\
	_E4C_VERSION(_E4C_V_STRING)

/**
 * Provides the maximum length (in bytes) of an exception message
 */
# define E4C_EXCEPTION_MESSAGE_SIZE 128

/**
 * Reuses an existing exception context, otherwise, begins a new one and then
 * ends it.
 *
 * <p>
 * This macro lets library developers use the exception framework, regardless of
 * whether the library clients are unaware of the exception handling system. In
 * a nutshell, function libraries can use <code>#try</code>,
 * <code>#catch</code>, <code>#throw</code>, etc. whether the client previously
 * began an exception context or not.
 * </p>
 *
 * <p>
 * You <strong>must not use this macro</strong> unless you are implementing some
 * functionality which is to be called from another program, potentially unaware
 * of exceptions.
 * </p>
 *
 * <p>
 * A block introduced by <code>e4c_reusing_context</code> is guaranteed to
 * take place <em>inside</em> an execution context. When the block completes,
 * the system returns to its previous status (if it was necessary to open a new
 * exception context, it will be automatically closed).
 * </p>
 *
 * <p>
 * This way, when an external functions encounters an error, it may either throw
 * an exception (when the caller is aware of the exception system), or otherwise
 * return an error code (when the caller did not open an exception context).
 * </p>
 *
 * <p>
 * <code>e4c_reusing_context</code> needs to be given a variable that will point
 * to whichever exception thrown inside the block. This variable must be a
 * pointer to <code>e4c_exception</code> and will be set to <code>NULL</code> if
 * no exception was thrown inside the block.
 * </p>
 *
 * <pre class="fragment">
 * int library_public_function(void * pointer, int number){
 * &nbsp;
 *     /<span>*
 *     We don't know where this function is going to be called from, so:
 *       * We cannot use "try", "throw", etc. right here, because the exception
 *       context COULD be uninitialized at this very moment.
 *       * We cannot call "e4c_context_begin" either, because the
 *       exception context COULD be already initialized.
 *     If we have to make use of the exception handling system, we need to
 *     "reuse" the existing exception context or "use" a new one.
 *     *</span>/
 * &nbsp;
 *     #e4c_exception * exception;
 * &nbsp;
 *     #e4c_reusing_context(exception){
 *         /<span>* Now we can safely use "try", "throw", etc. *</span>/
 *         if(pointer == NULL){
 *             throw(NullPointerException);
 *         }
 * &nbsp;
 *         library_private_function(pointer, number);
 *     }
 * &nbsp;
 *     if(exception != NULL){
 *         /<span>*
 *         We got here because:
 *           * There was no exception context
 *           * An exception was caught during the execution of the block
 *           * The block was closed
 *         Now we should return an error code to the caller, which is unaware of
 *         the exception system (if the caller was aware, the exception would
 *         have been simply propagated).
 *         *</span>/
 * &nbsp;
 *         if(exception->type == #NullPointerException){
 *             return(-3);
 *         }else if(exception->type == #NotEnoughMemoryException){
 *             return(-2);
 *         }
 *         return(-1);
 *     }
 * &nbsp;
 *     /<span>*
 *     If we got here, it means that our block completed successfully.
 *     We need to return a "success" status code.
 *     *</span>/
 *     return(0);
 * }
 * </pre>
 *
 * <p>
 * Next, the semantics of <code>e4c_reusing_context</code> are explained,
 * step by step.
 * </p>
 *
 * <ul>
 *   <li>
 *   If there is an exception context at the time the block starts:
 *   <ol>
 *     <li>The existing exception context will be reused.</li>
 *     <li>The code block will take place.</li>
 *     <li>
 *       If any exception is thrown during the execution of the block:
 *       <ul>
 *       <li>It will be <strong>propagated</strong> upwards to the caller.</li>
 *       <li>The control will be transferred to the nearest surrounding block of
 *         code which is able to handle that exception.</li>
 *       </ul>
 *     </li>
 *     <li>
 *       When the block ends:
 *       <ul>
 *       <li>The block completed successfully.</li>
 *       <li><code>_thrown_exception_</code> will be <code>NULL</code>.</li>
 *       <li>The function may then return a "success" status code.</li>
 *       </ul>
 *     </li>
 *   </ol>
 *   </li>
 *   <li>
 *   If there is no exception context at the time the block starts:
 *   <ol>
 *     <li>A new exception context will be begun; note that the signal handling
 *       system <strong>will NOT be set up.</strong>.</li>
 *     <li>The code block will take place.</li>
 *     <li>
 *       If any exception is thrown during the execution of the block:
 *       <ul>
 *       <li>It will be <strong>caught</strong> and
 *           <code>_thrown_exception_</code> will point to it.</li>
 *       </ul>
 *     </li>
 *     <li>
 *       When the block ends:
 *       <ul>
 *       <li>If <code>_thrown_exception_</code> is <code>NULL</code> then the
 *           block completed successfully. The function may then return a
 *           "success" status code.</li>
 *       <li>Otherwise, an exception was thrown during the execution of the
 *           block. The function may then return an error code to the caller.
 *           </li>
 *       </ul>
 *     </li>
 *   </ol>
 *   </li>
 * </ul>
 *
 * <p>
 * If you need to perform any cleanup, you should place it <em>inside</em> a
 * <code>finally</code> block, for example:
 * </p>
 *
 * <pre class="fragment">
 * ...
 * #e4c_reusing_context(exception){
 * &nbsp;
 *     void * buffer = NULL;
 *     #try{
 *         buffer = malloc(1024);
 *         ...
 *     }#finally{
 *         free(buffer);
 *     }
 * }
 * ...
 * </pre>
 *
 * <p>
 * If you need to rely on the signal handling system, you may call
 * <code>#e4c_context_set_signal_mappings</code> explicitely. You must take into
 * account that you could be <em>hijacking</em> your client's original signal
 * mappings, so you should also call
 * <code>#e4c_context_get_signal_mappings</code> in order to restore the
 * previous signal mappings when you are done.
 * </p>
 *
 * <pre class="fragment">
 * const #e4c_signal_mapping new_mappings[] = {
 *     #E4C_SIGNAL_MAPPING(SIGABRT, Exception1),
 *     #E4C_SIGNAL_MAPPING(SIGINT, Exception2),
 *     ...
 *     #E4C_NULL_SIGNAL_MAPPING
 * };
 * ...
 * #e4c_reusing_context(exception){
 * &nbsp;
 *     const #e4c_signal_mapping * old_mappings = #e4c_context_get_signal_mappings();
 * &nbsp;
 *     #e4c_context_set_signal_mappings (new_mappings);
 * &nbsp;
 *     #try{
 *         ...
 *     }#finally{
 *         #e4c_context_set_signal_mappings (old_mappings);
 *     }
 * }
 * ...
 * </pre>
 *
 * <p>
 * This macro only begins a new exception context <strong>if there is no one,
 * already begun, to be used</strong> whereas
 * <code>#e4c_using_context</code> always attempts to begin a new one.
 * </p>
 *
 * @param _thrown_exception_ A reference to the exception that was thrown inside
 *        the block (if any), <code>NULL</code> otherwise.
 *
 * @see e4c_context_begin
 * @see e4c_context_end
 * @see e4c_context_is_ready
 * @see e4c_using_context
 * @see e4c_exception
 */
# define e4c_reusing_context(_thrown_exception_) \
	_E4C_REUSING_CONTEXT(_thrown_exception_)

/** @} */

/**
 * @name Other convenience macros
 *
 * <p>
 * These macros provide a handy way to: begin (and end) implicitly a new
 * exception context, express <em>assertions</em>, define and declare
 * exceptions, and define arrays of signal mappings.
 * </p>
 *
 * @{
 */

/**
 * Introduces a block of code which will use a new exception context.
 *
 * <p>
 * This macro begins a new exception context to be used by the code block right
 * next to it. When the code completes, <code>#e4c_context_end</code>
 * will be called implicitly.
 * </p>
 *
 * <p>
 * This macro is very convenient when the beginning and the ending of the
 * current exception context are next to each other. For example, there is no
 * semantic difference between this two blocks of code:
 * </p>
 *
 * <pre class="fragment">
 * &nbsp;
 * /<span>* block 1 *</span>/
 * #e4c_context_begin (e4c_true, NULL);
 * /<span>* ... *</span>/
 * #e4c_context_end();
 * &nbsp;
 * /<span>* block 2 *</span>/
 * #e4c_using_context(e4c_true, NULL){
 *     /<span>* ... *</span>/
 * }
 * </pre>
 *
 * <p>
 * This macro <strong>always</strong> attempts to begin a new exception context,
 * whereas <code>#e4c_reusing_context</code> only does if there is no
 * exception context, already begun, to be used.
 * </p>
 *
 * <p>
 * This macro <strong>should be used whenever possible</strong>, rather than
 * doing the explicit, manual calls to <code>#e4c_context_begin</code>
 * and <code>#e4c_context_end</code>, because it is less prone to errors.
 * </p>
 *
 * @param _handle_signals_ If <code>true</code>, the signal handling system will
 *        be set up with the default mapping.
 * @param _uncaught_handler_ If not <code>NULL</code>, this function will be
 *        called in the event of an uncaught exception.
 *
 * @see e4c_context_begin
 * @see e4c_context_end
 * @see e4c_reusing_context
 */
# define e4c_using_context(_handle_signals_, _uncaught_handler_) \
	_E4C_USING_CONTEXT(_handle_signals_, _uncaught_handler_)

/**
 * Throws an exception with a formatted message
 *
 * <p>
 * This is a handy way to compose a formatted message and throw an exception
 * <em>on the fly</em>:
 * </p>
 *
 * <pre class="fragment">
 * int bytes = 1024;
 * void * buffer = malloc(bytes);
 * if(buffer == NULL){
 *     throwf(NotEnoughMemoryException, "Could not allocate %d bytes.", bytes);
 * }
 * </pre>
 *
 * <p>
 * This macro relies on two features that were introduced in the
 * <strong>ISO/IEC 9899:1999</strong> (also known as <em>C99</em>) revision of
 * the C programming language standard in 1999:
 * </p>
 *
 * <ul>
 * <li>Variadic macros</li>
 * <li>Buffer-safe function <code>vsnprintf</code></li>
 * </ul>
 *
 * <p>
 * In order not to create compatibility issues, this macro will only be defined
 * when the <code>__STDC_VERSION__</code> <em>compile-time</em> parameter is
 * defined with a value <em>greater than or equal to</em> <code>199901L</code>,
 * or <code>HAVE_C99_VARIADIC_MACROS</code> is defined.
 * </p>
 *
 * <p>
 * The semantics of this macro are the same as for the <code>throw</code> macro.
 * </p>
 *
 * <p>
 * At least one argument must be passed right after the format string. The
 * message will be composed through the function <code>vsnprintf</code> with the
 * specified format and variadic arguments. For further information on the
 * formatting rules, you may look up the specifications for the function
 * <code>vsnprintf</code>.
 * </p>
 *
 * @see throw
 * @see rethrowf
 *
 * @param _exception_type_ The type of exception to be thrown
 * @param _format_ The string containing the specifications that determine the
 *       output format for the variadic arguments.
 * @param ... The variadic arguments that will be formatted according to the
 *        format control.
 */
# if !defined(E4C_NOKEYWORDS) && defined(HAVE_C99_VARIADIC_MACROS)
#	define throwf(_exception_type_, _format_, ...) \
		\
		E4C_THROWF( (_exception_type_), (_format_), __VA_ARGS__ )
# endif

/**
 * Expresses a program assertion
 *
 * <p>
 * An assertion is a mechanism to express that the developer <em>thinks</em>
 * that a specific condition is always met at some point of the program.
 * </p>
 *
 * <p>
 * <code>e4c_assert</code> is a convenient way to insert debugging assertions
 * into a program. The <code>NDEBUG</code> <em>compile-time</em> parameter
 * determines whether the assumptions will be actually verified by the program
 * at <em>run-time</em>.
 * </p>
 *
 * <p>
 * In absence of <code>NDEBUG</code>, the assertion statements will be ignored
 * and therefore will have no effect on the program, not even evaluating the
 * condition. Therefore expressions passed to <code>e4c_assert</code> must not
 * contain side-effects, since they will not happen when debugging is disabled.
 * </p>
 *
 * <p>
 * In presence of <code>NDEBUG</code>, the assertion statements will verify that
 * the condition is met every time the program reaches that point of the
 * program.
 * </p>
 *
 * <p>
 * If the assertion does not hold at any time, then an
 * <code>AssertionException</code> will be thrown to indicate the programming
 * error. This exception cannot be caught whatsoever. The program (or current
 * thread) will be terminated.
 * </p>
 *
 * <p>
 * The main advantage of using this assertion mechanism (as opposed to the
 * macros provided by the standard header file <code>assert.h</code>) is that
 * before actually exiting the program or thread, all of the pending
 * <code>finally</code> blocks will be executed.
 * </p>
 *
 * @param _condition_ A predicate that must evaluate to <code>true</code>
 *
 * @see AssertionException
 */
# define e4c_assert(_condition_) \
	\
	_E4C_ASSERT(_condition_)

/**
 * Throws again the currently thrown exception, with a new, formatted message
 *
 * <p>
 * This macro creates a new instance of the thrown exception, with a more
 * specific message.
 * </p>
 *
 * <p>
 * This is a handy way to create a new instance of the thrown exception, with a
 * more specific, formatted message.
 * </p>
 *
 * <pre class="fragment">
 * try{
 *     image = read_file(file_path);
 * }catch(FileOpenException){
 *     rethrowf("Image '%s' not found.", title);
 * }
 * </pre>
 *
 * <p>
 * This macro relies on two features that were introduced in the
 * <strong>ISO/IEC 9899:1999</strong> (also known as <em>C99</em>) revision of
 * the C programming language standard in 1999:
 * </p>
 *
 * <ul>
 * <li>Variadic macros</li>
 * <li>Buffer-safe function <code>vsnprintf</code></li>
 * </ul>
 *
 * <p>
 * In order not to create compatibility issues, this macro will only be defined
 * when the <code>__STDC_VERSION__</code> <em>compile-time</em> parameter is
 * defined with a value <em>greater than or equal to</em> <code>199901L</code>,
 * or <code>HAVE_C99_VARIADIC_MACROS</code> is defined.
 * </p>
 *
 * <p>
 * The semantics of this macro are the same as for the <code>throw</code> macro.
 * </p>
 *
 * <p>
 * At least one argument must be passed right after the format string. The
 * message will be composed through the function <code>vsnprintf</code> with the
 * specified format and variadic arguments. For further information on the
 * formatting rules, you may look up the specifications for the function
 * <code>vsnprintf</code>.
 * </p>
 *
 * @see rethrow
 * @see throwf
 *
 * @param _format_ The string containing the specifications that determine the
 *       output format for the variadic arguments.
 * @param ... The variadic arguments that will be formatted according to the
 *        format control.
 */
# if !defined(E4C_NOKEYWORDS) && defined(HAVE_C99_VARIADIC_MACROS)
#	define rethrowf(_format_, ...) \
		\
		E4C_RETHROWF( (_format_), __VA_ARGS__ )
# endif

/**
 * Declares an exception
 *
 * <p>
 * This macro introduces the name of an <code>extern</code>, <code>const</code>
 * exception which will be available to be thrown or caught. It is only a
 * <em>declaration</em> (i.e. the exception has to be <em>defined</em>
 * somewhere else). This macro is intended to be used inside a header file.
 * </p>
 *
 * @param _name_ Name of the new exception
 *
 * @see e4c_exception
 * @see E4C_DEFINE_EXCEPTION
 */
# define E4C_DECLARE_EXCEPTION(_name_) \
	\
	_E4C_DECLARE_EXCEPTION(_name_)

/**
 * Defines an exception
 *
 * <p>
 * This macro allocates a new, <code>const</code> exception.
 * </p>
 *
 * @param _name_ Name of the new exception
 * @param _message_ Default message of the new exception
 * @param _super_ Supertype of the new exception
 *
 * @see e4c_exception
 * @see RuntimeException
 * @see E4C_DECLARE_EXCEPTION
 */
# define E4C_DEFINE_EXCEPTION(_name_, _message_, _super_) \
	\
	_E4C_DEFINE_EXCEPTION(_name_, _message_, _super_)

/**
 * Represents a signal mapping literal
 *
 * <p>
 * This macro comes in handy for initializing arrays of
 * <code>e4c_signal_mapping</code>s.
 * </p>
 *
 * @param _signal_number_ Numeric value of the signal to be converted
 * @param _exception_ Exception representing the signal
 *
 * @see e4c_signal_mapping
 * @see e4c_context_set_signal_mappings
 * @see e4c_context_get_signal_mappings
 * @see E4C_DECLARE_EXCEPTION
 */
# define E4C_SIGNAL_MAPPING(_signal_number_, _exception_) \
	\
	_E4C_SIGNAL_MAPPING(_signal_number_, _exception_)

/**
 * Represents a null signal mapping literal
 *
 * <p>
 * This macro comes in handy for terminating arrays of
 * <code>e4c_signal_mapping</code>s.
 * </p>
 *
 * @see e4c_signal_mapping
 * @see e4c_context_set_signal_mappings
 * @see e4c_context_get_signal_mappings
 * @see E4C_DECLARE_EXCEPTION
 */
# define E4C_NULL_SIGNAL_MAPPING \
	\
	_E4C_NULL_SIGNAL_MAPPING

/** @} */


/**
 * Represents an exception in the exception handling system
 *
 * <p>
 * Exceptions are a means of breaking out of the normal flow of control of a
 * code block in order to handle errors or other exceptional conditions. An
 * exception should be thrown at the point where the error is detected; it may
 * be handled by the surrounding code block or by any code block that directly
 * or indirectly invoked the code block where the error occurred.
 * </p>
 *
 * <p>
 * The types of the exceptions a program will use are defined through the macro
 * <code>#E4C_DEFINE_EXCEPTION</code>:
 * </p>
 *
 * <pre class="fragment">
 * #E4C_DEFINE_EXCEPTION(StackException, "Stack overflow", RuntimeException);
 * #E4C_DEFINE_EXCEPTION(StackOverflowException, "Stack overflow", StackException);
 * #E4C_DEFINE_EXCEPTION(StackUnderflowException, "Stack underflow", StackException);
 * </pre>
 *
 * <p>
 * When defining types of exceptions, they are given a name, a default message
 * and a supertype to organize them into a <em>pseudo-hierarchy</em>. These can
 * be considered as the <em>compile-time</em> attributes of a exception.
 * </p>
 *
 * <p>
 * However, exceptions provide further information regarding the exceptional
 * situation at <em>run-time</em>, such as:
 * </p>
 *
 * <ul>
 * <li>An <em>ad hoc</em> message (as opposed to the <em>default</em> one)</li>
 * <li>The exact point of the program where it was thrown (source code file,
 *     line and function name, if available)</li>
 * <li>The value of the standard error code <code>errno</code> at the time the
 *     exception was thrown</li>
 * <li>The <em>cause</em> of the exception, which is the previous exception (if
 *     any), when the exception was thrown from a <code>catch</code> or
 *     <code>finally</code> block</li>
 * <li>The specific, <em>run-time</em> type of the exception, convenient when
 *     handling an abstract type of exceptions in a <code>catch</code>
 *     block</li>
 * </ul>
 *
 * <p>
 * Exceptions are usually defined as global objects. There is a set of
 * predefined exceptions built into the framework, amongst others:
 * </p>
 *
 * <ul>
 * <li><code>#RuntimeException</code></li>
 * <li><code>#NotEnoughMemoryException</code></li>
 * <li><code>#NullPointerException</code></li>
 * <li><code>#FileOpenException</code></li>
 * <li><code>#AbortException</code></li>
 * <li><code>#ArithmeticException</code></li>
 * <li><code>#IllegalInstructionException</code></li>
 * <li><code>#BadPointerException</code></li>
 * <li><code>#TerminationException</code></li>
 * <li><code>#UserInterruptionException</code></li>
 * </ul>
 *
 * <p>
 * <code>#RuntimeException</code> is the root of the exceptions
 * <em>pseudo-hierarchy</em>. <strong>Any</strong> exception can be caught by a
 * <code>catch(RuntimeException)</code> block, except
 * <code>AssertionException</code>.
 * </p>
 *
 * @see E4C_DEFINE_EXCEPTION
 * @see E4C_DECLARE_EXCEPTION
 * @see RuntimeException
 * @see AssertionException
 * @see throw
 * @see catch
 * @see e4c_get_exception
 */
typedef struct e4c_exception e4c_exception;
struct e4c_exception{

	/** The name of this exception */
	const char *					name;

	/** The message of this exception */
	char							message[E4C_EXCEPTION_MESSAGE_SIZE];

	/** The supertype of this exception */
	const e4c_exception *			super;

	/** The path of the source code file from which the exception was thrown */
	const char *					file;

	/** The number of line from which the exception was thrown */
	int								line;

	/** The function from which the exception was thrown */
	const char *					function;

	/** The value of errno at the time the exception was thrown */
	int								error_number;

	/** The class of this exception */
	const e4c_exception *			type;

	/** The cause of this exception */
	const e4c_exception *			cause;
};

/**
 * Represents a map between a signal and an exception
 *
 * <p>
 * A signal is an asynchronous notification sent by the operating system to a
 * process in order to notify it of an event that occurred. Most of the signals
 * will, by default, crash the program as soon as they are raised.
 * <code><strong>exceptions4c</strong></code> can convert signals to
 * <code>#e4c_exception</code>s, so they can be easily handled.
 * </p>
 *
 * <p>
 * For example, a <em>suspicious</em> or <em>dangerous</em> part of the program
 * could be wrapped up with <code>#try</code> blocks and <code>#catch</code>
 * segmentation faults or divisions by zero. Then the program would clean up and
 * continue normally:
 * </p>
 *
 * <pre class="fragment">
 * #e4c_using_context(true, #e4c_print_exception){
 *     int * pointer = NULL;
 *     #try{
 *         int oops = *pointer;
 *     }#catch(#BadPointerException){
 *         printf("No problem ;-)");
 *     }#finally{
 *         /<span>* clean up... *</span>/
 *     }
 * }
 * </pre>
 *
 * <p>
 * In order to perform the conversion, <strong>exceptions4c</strong>
 * <em>maps</em> signals to exceptions.
 * </p>
 *
 * <p>
 * The simplest way to get this working is by calling the function
 * <code>e4c_context_begin</code>. This function will set up the default
 * mappings for the available signals in the platform, when passed
 * <code>handle_signals=true</code>.
 * </p>
 *
 * <p>
 * If you need to be more specific about which signals get converted to
 * exceptions, you can define an array of <code>e4c_signal_mapping</code>:
 * </p>
 *
 * <pre class="fragment">
 * const #e4c_signal_mapping my_signal_mappings[] = {
 *     #E4C_SIGNAL_MAPPING(SIGABRT, Exception1),
 *     #E4C_SIGNAL_MAPPING(SIGINT, Exception2),
 *     ...
 *     #E4C_NULL_SIGNAL_MAPPING
 * }
 * </pre>
 *
 * <p>
 * An array of <code>e4c_signal_mapping</code>s is defined through the macros
 * <code>E4C_SIGNAL_MAPPING</code> and <code>E4C_NULL_SIGNAL_MAPPING</code>.
 * Every <code>e4c_signal_mapping</code> array needs to be terminated by the
 * <em>null signal mapping</em> element, so the system finds out how many
 * mappings there are in a given array.
 * </p>
 *
 * <p>
 * Once the array is properly defined, it can be passed to the function
 * <code>e4c_context_set_signal_mappings</code>. This way, only the specified
 * signals will be handled as exceptions, and they will be converted to the
 * specified exceptions.
 * </p>
 *
 * <p>
 * These are some of the signals you can handle:
 * </p>
 *
 * <ul>
 * <li><code>SIGFPE</code> when dividing by zero.</li>
 * <li><code>SIGSEGV</code> when dereferencing an invalid pointer.</li>
 * <li><code>SIGINT</code> when a user interrupts the process.</li>
 * <li><code>SIGTERM</code> when a process is requested to be terminated.</li>
 * </ul>
 *
 * @see e4c_context_begin
 * @see e4c_context_set_signal_mappings
 * @see e4c_context_get_signal_mappings
 * @see e4c_default_signal_mappings
 */
typedef struct e4c_signal_mapping e4c_signal_mapping;
struct e4c_signal_mapping{

	/** The signal to be converted */
	int								signal_number;

	/** The exception representing the signal */
	const e4c_exception * const		exception;

};

/**
 * Represents the completeness of a code block aware of exceptions
 *
 * <p>
 * The symbolic values representing the status of a block help to distinguish
 * between different possible situations inside a <code>finally</code> block.
 * For example, different cleanup actions can be taken, depending on the status
 * of the block.
 * </p>
 *
 * <pre class="fragment">
 * #try{
 *    ...
 * }#finally{
 *    switch( #e4c_get_status() ){
 * &nbsp;
 *        case #e4c_succeeded:
 *            ...
 *            break;
 * &nbsp;
 *        case #e4c_recovered:
 *            ...
 *            break;
 * &nbsp;
 *        case #e4c_failed:
 *            ...
 *            break;
 *    }
 * }
 * </pre>
 *
 * @see e4c_get_status
 * @see finally
 */
enum e4c_status{

	/** There was no exception */
	e4c_succeeded,

	/** There was an exception, but it was caught */
	e4c_recovered,

	/** There was an exception and it wasn't caught */
	e4c_failed
};
typedef enum e4c_status e4c_status;

/**
 * This is the signature of a function which will be executed in the event of an
 * uncaught exception
 *
 * <p>
 * These functions are not allowed to try and recover the current exception
 * context. Moreover, the program (or current thread) will terminate right after
 * the function returns.
 * </p>
 *
 * @see e4c_context_begin
 *
 * @param exception The uncaught exception
 */
typedef void (*e4c_uncaught_handler)(const e4c_exception * exception);

/*
 * Next enumeration is undocumented on purpose, in order to hide implementation
 * details, subject to change.
 */
enum _e4c_frame_stage{
	_e4c_beginning,
	_e4c_acquiring,
	_e4c_trying,
	_e4c_disposing,
	_e4c_catching,
	_e4c_finalizing,
	_e4c_done
};


/**
 * @name Predefined signal mappings
 *
 * <p>
 * There is a predefined set of signal mappings. Signal mappings are used to
 * convert signals into exceptions.
 * </p>
 *
 * <p>
 * Common signals are mapped to its corresponding exception, for example:
 * </p>
 *
 * <ul>
 * <li><code>SIGABRT</code> is mapped to <code>#AbortException</code></li>
 * <li><code>SIGFPE</code> is mapped to <code>#ArithmeticException</code></li>
 * <li><code>SIGSEGV</code> is mapped to <code>#BadPointerException</code></li>
 * <li><code>SIGTERM</code> is mapped to <code>#TerminationException</code></li>
 * <li>...and so on</li>
 * </ul>
 *
 * @see e4c_signal_mapping
 * @see e4c_context_begin
 * @see e4c_context_set_signal_mappings
 * @see e4c_context_get_signal_mappings
 * @{
 */

/**
 * The array of predefined signal mappings.
 */
extern const e4c_signal_mapping * e4c_default_signal_mappings;

/** @} */

/**
 * @name Predefined exceptions
 *
 * <p>
 * Built-in exceptions represent usual error conditions that might occur during
 * the course of any program.
 * </p>
 *
 * <p>
 * They are organized into a <em>pseudo-hierarchy</em>, being
 * <code>#RuntimeException</code> the common <em>supertype</em> of all
 * exceptions.
 * </p>
 *
 * @{
 */

/**
 * This is the root of the exception pseudo-hierarchy
 *
 * <p>
 * RuntimeException is the common <em>supertype</em> of all exceptions.
 * </p>
 *
 * <p>
 * This is the <em>hierarchy</em> of runtime exceptions:
 * </p>
 *
 * <ul>
 *     <li><code>#RuntimeException</code><ul>
 *         <li><code>#NotEnoughMemoryException</code></li>
 *         <li><code>#NullPointerException</code></li>
 *         <li><code>#SignalException</code><ul>
 *             <li><code>#SignalAlarmException</code></li>
 *             <li><code>#SignalChildException</code></li>
 *             <li><code>#SignalTrapException</code></li>
 *             <li><code>#ErrorSignalException</code><ul>
 *                 <li><code>#IllegalInstructionException</code></li>
 *                 <li><code>#BadPointerException</code></li>
 *                 <li><code>#ArithmeticException</code></li>
 *                 <li><code>#BrokenPipeException</code></li>
 *             </ul></li>
 *             <li><code>#ControlSignalException</code><ul>
 *                 <li><code>#StopException</code></li>
 *                 <li><code>#KillException</code></li>
 *                 <li><code>#HangUpException</code></li>
 *                 <li><code>#TerminationException</code></li>
 *                 <li><code>#AbortException</code></li>
 *                 <li><code>#CPUTimeException</code></li>
 *                 <li><code>#UserControlSignalException</code><ul>
 *                     <li><code>#UserQuitException</code></li>
 *                     <li><code>#UserInterruptionException</code></li>
 *                     <li><code>#UserBreakException</code></li>
 *                 </ul></li>
 *             </ul></li>
 *             <li><code>#ProgramSignalException</code><ul>
 *                 <li><code>#ProgramSignal1Exception</code></li>
 *                 <li><code>#ProgramSignal2Exception</code></li>
 *             </ul></li>
 *         </ul></li>
 *     </ul></li>
 * </ul>
 */
E4C_DECLARE_EXCEPTION(RuntimeException);

/**
 * This exception is thrown when the system runs out of memory.
 *
 * <p>
 * NotEnoughMemoryException is thrown when there is not enough memory to
 * continue the execution of the program.
 * </p>
 */
E4C_DECLARE_EXCEPTION(NotEnoughMemoryException);

/**
 * This exception is thrown when an unexpected null pointer is found.
 *
 * <p>
 * NullPointerException is thrown when some part of the program gets a pointer
 * which was expected or required to contain a valid memory address.
 * </p>
 */
E4C_DECLARE_EXCEPTION(NullPointerException);

/**
 * This exception is thrown when an assertion does not hold
 *
 * <p>
 * This exception is part of the assertion facility of the library. It will be
 * thrown if the <em>compile-time</em> parameter <code>NDEBUG</code> is present
 * and the conditon of an assertion evaluates to <code>false</code>.
 * </p>
 *
 * <p>
 * This exception cannot be caught whatsoever. The program (or current thread)
 * will be terminated, after the execution of all pending
 * <code>finally</code> blocks.
 * </p>
 *
 * @see e4c_assert
 */
E4C_DECLARE_EXCEPTION(AssertionException);

/**
 * This exception is thrown when a file cannot be opened.
 *
 * <p>
 * FileOpenException is thrown by <code>#e4c_using_file</code> when
 * <code>fopen</code> returns <code>NULL</code> for whatever reason.
 * </p>
 *
 * <p>
 * The specific cause of the error can be determined by checking the
 * <code>error_number</code> of the exception; it captures the value of
 * <code>errno</code> at the time the exception was thrown (right after
 * <code>fopen</code>).
 * </p>
 */
E4C_DECLARE_EXCEPTION(FileOpenException);

/**
 * This exception is the common supertype of all signal exceptions
 *
 * <p>
 * Signal exceptions are thrown when some signal is sent to the process.
 * </p>
 *
 * <p>
 * A signal can be generated by calling <code>raise</code>.
 * </p>
 *
 * <p>
 * This is the <em>hierarchy</em> of signal exceptions:
 * </p>
 *
 * <ul>
 *     <li><code>#SignalException</code><ul>
 *         <li><code>#SignalAlarmException</code></li>
 *         <li><code>#SignalChildException</code></li>
 *         <li><code>#SignalTrapException</code></li>
 *         <li><code>#ErrorSignalException</code><ul>
 *             <li><code>#IllegalInstructionException</code></li>
 *             <li><code>#BadPointerException</code></li>
 *             <li><code>#ArithmeticException</code></li>
 *             <li><code>#BrokenPipeException</code></li>
 *         </ul></li>
 *         <li><code>#ControlSignalException</code><ul>
 *             <li><code>#StopException</code></li>
 *             <li><code>#KillException</code></li>
 *             <li><code>#HangUpException</code></li>
 *             <li><code>#TerminationException</code></li>
 *             <li><code>#AbortException</code></li>
 *             <li><code>#CPUTimeException</code></li>
 *             <li><code>#UserControlSignalException</code><ul>
 *                 <li><code>#UserQuitException</code></li>
 *                 <li><code>#UserInterruptionException</code></li>
 *                 <li><code>#UserBreakException</code></li>
 *             </ul></li>
 *         </ul></li>
 *         <li><code>#ProgramSignalException</code><ul>
 *             <li><code>#ProgramSignal1Exception</code></li>
 *             <li><code>#ProgramSignal2Exception</code></li>
 *         </ul></li>
 *     </ul></li>
 * </ul>
 */
E4C_DECLARE_EXCEPTION(SignalException);

/**
 * This exception is thrown when a time limit has elapsed
 *
 * SignalAlarmException represents <code>SIGALRM</code>, the signal sent to a
 * process when a time limit has elapsed.
 */
E4C_DECLARE_EXCEPTION(SignalAlarmException);

/**
 * This exception is thrown when a child process terminates
 *
 * SignalChildException represents <code>SIGCHLD</code>, the signal sent to a
 * process when a child process terminates (ignored by default).
 */
E4C_DECLARE_EXCEPTION(SignalChildException);

/**
 * This exception is thrown when a condition arises that a debugger has
 * requested to be informed of
 *
 * SignalTrapException represents <code>SIGTRAP</code>, the signal sent to a
 * process when a condition arises that a debugger has requested to be informed
 * of.
 */
E4C_DECLARE_EXCEPTION(SignalTrapException);

/**
 * This exception is the common supertype of all error signal exceptions
 *
 * <p>
 * Error signal exceptions are thrown when some error prevents the program to
 * keep executing its normal flow, for example:
 * </p>
 *
 * <p>
 * This is the <em>hierarchy</em> of error signal exceptions:
 * </p>
 *
 * <ul>
 *     <li><code>#ErrorSignalException</code><ul>
 *         <li><code>#IllegalInstructionException</code></li>
 *         <li><code>#BadPointerException</code></li>
 *         <li><code>#ArithmeticException</code></li>
 *         <li><code>#BrokenPipeException</code></li>
 *     </ul></li>
 * </ul>
 */
E4C_DECLARE_EXCEPTION(ErrorSignalException);

/**
 * This exception is thrown when the process attempts to execute an illegal
 * instruction
 *
 * IllegalInstructionException represents <code>SIGILL</code>, the signal sent
 * to a process when it attempts to execute a malformed, unknown, or privileged
 * instruction.
 */
E4C_DECLARE_EXCEPTION(IllegalInstructionException);

/**
 * This exception is thrown when the process performs an erroneous arithmetic
 * operation
 *
 * ArithmeticException represents <code>SIGFPE</code>, the signal sent to a
 * process when it performs an erroneous arithmetic operation.
 */
E4C_DECLARE_EXCEPTION(ArithmeticException);

/**
 * This exception is thrown when the process tries to dereference an invalid
 * pointer
 *
 * BadPointerException represents <code>SIGSEGV</code>, the signal sent to a
 * process when it makes an invalid memory reference, or segmentation fault.
 */
E4C_DECLARE_EXCEPTION(BadPointerException);

/**
 * This exception is thrown when the process attempts to write to a broken pipe
 *
 * BrokenPipeException represents <code>SIGPIPE</code>, the signal sent to a
 * process when it attempts to write to a pipe without a process connected to
 * the other end.
 */
E4C_DECLARE_EXCEPTION(BrokenPipeException);

/**
 * This exception is the common supertype of all control signal exceptions
 *
 * <p>
 * Control signal exceptions are thrown when the process needs to be controlled
 * by the user or some other process.
 * </p>
 *
 * <p>
 * This is the <em>hierarchy</em> of control signal exceptions:
 * </p>
 *
 * <ul>
 *     <li><code>#ControlSignalException</code><ul>
 *         <li><code>#StopException</code></li>
 *         <li><code>#KillException</code></li>
 *         <li><code>#HangUpException</code></li>
 *         <li><code>#TerminationException</code></li>
 *         <li><code>#AbortException</code></li>
 *         <li><code>#CPUTimeException</code></li>
 *         <li><code>#UserControlSignalException</code><ul>
 *             <li><code>#UserQuitException</code></li>
 *             <li><code>#UserInterruptionException</code></li>
 *             <li><code>#UserBreakException</code></li>
 *         </ul></li>
 *     </ul></li>
 * </ul>
 */
E4C_DECLARE_EXCEPTION(ControlSignalException);

/**
 * This exception is thrown to stop the process for later resumption
 *
 * StopException represents <code>SIGSTOP</code>, the signal sent to a process
 * to stop it for later resumption.
 *
 * Since <code>SIGSTOP</code> is unblock-able, it cannot be handled and
 * converted to this exception automatically.
 */
E4C_DECLARE_EXCEPTION(StopException);

/**
 * This exception is thrown to terminate the process immediately
 *
 * KillException represents <code>SIGKILL</code>, the signal sent to a process
 * to cause it to terminate immediately.
 *
 * Since <code>SIGKILL</code> is unblock-able, it cannot be handled and
 * converted to this exception automatically.
 */
E4C_DECLARE_EXCEPTION(KillException);

/**
 * This exception is thrown when the process' terminal is closed
 *
 * HangUpException represents <code>SIGHUP</code>, the signal sent to a process
 * when its controlling terminal is closed.
 */
E4C_DECLARE_EXCEPTION(HangUpException);

/**
 * This exception is thrown to request the termination of the process
 *
 * TerminationException represents <code>SIGTERM</code>, the signal sent to a
 * process to request its termination.
 */
E4C_DECLARE_EXCEPTION(TerminationException);

/**
 * This exception is thrown to abort the process
 *
 * AbortException represents <code>SIGABRT</code>, the signal sent by computer
 * programs to abort the process.
 */
E4C_DECLARE_EXCEPTION(AbortException);

/**
 * This exception is thrown when the process has used up the CPU for too long
 *
 * CPUTimeException represents <code>SIGXCPU</code>, the signal sent to a
 * process when it has used up the CPU for a duration that exceeds a certain
 * predetermined user-settable value.
 */
E4C_DECLARE_EXCEPTION(CPUTimeException);

/**
 * This exception is the common supertype of all control signal exceptions
 * caused by the user
 *
 * <p>
 * User control signal exceptions are thrown when the process needs to be
 * controlled by the user.
 * </p>
 *
 * <p>
 * This is the <em>hierarchy</em> of control signal exceptions caused by the
 * user:
 * </p>
 *
 * <ul>
 *     <li><code>#UserControlSignalException</code><ul>
 *         <li><code>#UserQuitException</code></li>
 *         <li><code>#UserInterruptionException</code></li>
 *         <li><code>#UserBreakException</code></li>
 *     </ul></li>
 * </ul>
 */
E4C_DECLARE_EXCEPTION(UserControlSignalException);

/**
 * This exception is thrown when the user requests to quit the process
 *
 * UserQuitException represents <code>SIGQUIT</code>, the signal sent to a
 * process by its controlling terminal when the user requests that the process
 * dump core.
 */
E4C_DECLARE_EXCEPTION(UserQuitException);

/**
 * This exception is thrown when the user requests to interrupt the process
 *
 * UserInterruptionException represents <code>SIGINT</code>, the signal sent to
 * a process by its controlling terminal when a user wishes to interrupt it.
 */
E4C_DECLARE_EXCEPTION(UserInterruptionException);

/**
 * This exception is thrown when a user wishes to break the process
 *
 * UserBreakException represents <code>SIGBREAK</code>, the signal sent to a
 * process by its controlling terminal when a user wishes to break it.
 */
E4C_DECLARE_EXCEPTION(UserBreakException);

/**
 * This exception is the common supertype of all user-defined signal exceptions
 *
 * <p>
 * User-defined signal exceptions are thrown to indicate user-defined
 * conditions.
 * </p>
 *
 * <p>
 * This is the <em>hierarchy</em> of user-defined signal exceptions:
 * </p>
 *
 * <ul>
 *     <li><code>#ProgramSignalException</code><ul>
 *         <li><code>#ProgramSignal1Exception</code></li>
 *         <li><code>#ProgramSignal2Exception</code></li>
 *     </ul></li>
 * </ul>
 */
E4C_DECLARE_EXCEPTION(ProgramSignalException);

/**
 * This exception is thrown when user-defined conditions occur
 *
 * ProgramSignal1Exception represents <code>SIGUSR1</code>, the signal sent to a
 * process to indicate user-defined conditions.
 */
E4C_DECLARE_EXCEPTION(ProgramSignal1Exception);

/**
 * This exception is thrown when user-defined conditions occur
 *
 * ProgramSignal2Exception represents <code>SIGUSR1</code>, the signal sent to a
 * process to indicate user-defined conditions.
 */
E4C_DECLARE_EXCEPTION(ProgramSignal2Exception);

/** @} */

/**
 * @name Exception context handling Functions
 *
 * <p>
 * These functions enclose the scope of the exception handling system and
 * retrieve the current exception context.
 * </p>
 *
 * @{
 */

/**
 * Checks if the current exception context is ready
 *
 * <p>
 * This function returns whether there is an actual exception context ready to
 * be used by the program or current thread.
 * </p>
 *
 * @see e4c_context_begin
 * @see e4c_context_end
 * @see e4c_using_context
 * @see e4c_reusing_context
 *
 * @return Whether the current exception context of the program (or current
 *   thread) is ready to be used
 */
extern e4c_bool e4c_context_is_ready(void);

/**
 * Begins an exception context
 *
 * <p>
 * This function begins the current exception context to be used by the program
 * (or current thread), until <code>#e4c_context_end</code> is called.
 * </p>
 *
 * <p>
 * A program (or thread) <strong>must not</strong> use the keywords
 * <code>#try</code>, <code>#catch</code>, <code>#throw</code>, etc. prior to
 * calling <code>e4c_context_begin</code>. Such programming error will
 * lead to an abrupt exit of the program or thread.
 * </p>
 *
 * <p>
 * Calling <code>e4c_context_begin</code> <em>twice</em> is also
 * considered a programming error, and therefore the program (or thread) will
 * exit abruptly too. Nevertheless, <code>e4c_context_begin</code> can
 * be called several times <em>if, and only if,</em>
 * <code>#e4c_context_end</code> is called in between.
 * </p>
 *
 * <p>
 * The signal handling system can be automatically initialized with the default
 * signal mapping via <code>handle_signals</code> parameter when calling
 * <code>e4c_context_begin</code>. This is equivalent to:
 * </p>
 *
 * <pre class="fragment">
 * #e4c_context_set_signal_mappings(#e4c_default_signal_mappings);
 * </pre>
 *
 * <p>
 * Note that the behavior of <code>signal</code> is undefined in a
 * multithreaded program, so use the signal handling system with caution.
 * </p>
 *
 * <p>
 * In addition, a handling function can be specified to be called in the event
 * of an uncaught exception. This function will be called before exiting the
 * program or thread.
 * </p>
 *
 * <p>
 * There exist a convenience function to be used as the default
 * <em>uncaught handler</em>, called <code>#e4c_print_exception</code>. This
 * function simply prints information regarding the exception to
 * <code>stderr</code>, and then exits.
 * </p>
 *
 * @see e4c_context_end
 * @see e4c_context_is_ready
 * @see e4c_using_context
 * @see e4c_reusing_context
 * @see e4c_uncaught_handler
 * @see e4c_print_exception
 *
 * @param handle_signals If <code>true</code>, the signal handling system will
 *        be set up with the default mapping.
 * @param uncaught_handler If not <code>NULL</code>, this function will be
 *        called in the event of an uncaught exception.
 */
extern void e4c_context_begin(
	e4c_bool handle_signals, e4c_uncaught_handler uncaught_handler);

/**
 * Ends the current exception context
 *
 * <p>
 * This function ends the current exception context.
 * </p>
 *
 * <p>
 * A program (or thread) <strong>must</strong> begin an exception context prior
 * to calling <code>e4c_context_end</code>. Such programming error will lead to
 * an abrupt exit of the program (or thread).
 * </p>
 *
 * @see e4c_context_begin
 * @see e4c_context_is_ready
 * @see e4c_using_context
 * @see e4c_reusing_context
 */
extern void e4c_context_end(void);

/**
 * Assigns the specified signal mappings to the exception context
 *
 * <p>
 * This function assigns an array of mappings between the signals to be handled
 * and the corresponding exception to be thrown.
 * </p>
 *
 * <p>
 * A program (or thread) <strong>must</strong> begin an exception context prior
 * to calling <code>e4c_context_set_signal_mappings</code>. Such programming
 * error will lead to an abrupt exit of the program (or thread).
 * </p>
 *
 * <p>
 * Note that the behavior of <code>signal</code> is undefined in a
 * multithreaded program, so use the signal handling system with caution.
 * </p>
 *
 * @see e4c_signal_mapping
 * @see e4c_default_signal_mappings
 *
 * @param mappings The array of mappings
 */
extern void e4c_context_set_signal_mappings(const e4c_signal_mapping * mappings);

/**
 * Retrieves the signal mappings for the current exception context
 *
 * <p>
 * This function retrieves the current array of mappings between the signals to
 * be handled and the corresponding exception to be thrown.
 * </p>
 *
 * <p>
 * A program (or thread) <strong>must</strong> begin an exception context prior
 * to calling <code>e4c_context_get_signal_mappings</code>. Such programming
 * error will lead to an abrupt exit of the program (or thread).
 * </p>
 *
 * @see e4c_signal_mapping
 * @see e4c_default_signal_mappings
 *
 * @return The current array of mappings
 */
extern const e4c_signal_mapping * e4c_context_get_signal_mappings(void);

/**
 * Returns the completeness status of the executing code block
 *
 * <p>
 * Code blocks aware of exceptions have a completeness status regarding the
 * exception handling system. This status determines whether an exception was
 * actually thrown or not, and whether the exception was caught or not.
 * </p>
 *
 * <p>
 * A program (or thread) <strong>must</strong> begin an exception context prior
 * to calling <code>e4c_get_status</code>. Such programming error will lead to
 * an abrupt exit of the program (or thread).
 * </p>
 *
 * <p>
 * The status of the current block can be obtained any time, provided that
 * the exception context has begun at the time of the function call. However,
 * it is sensible to call this function only during the execution of
 * <code>finally</code> blocks.
 * </p>
 *
 * @see e4c_get_status
 * @see finally
 *
 * @return The completeness status of the executing code block
 */
extern e4c_status e4c_get_status(void);

/**
 * Returns the exception that was thrown
 *
 * <p>
 * This function returns a pointer to the exception that was thrown in the
 * surrounding <code>try/catch/finally</code> block, if any, otherwise
 * <code>NULL</code>.
 * </p>
 *
 * <p>
 * A program (or thread) <strong>must</strong> begin an exception context prior
 * to calling <code>e4c_get_exception</code>. Such programming error will lead
 * to an abrupt exit of the program (or thread).
 * </p>
 *
 * <p>
 * The function <code>e4c_is_instance_of</code> will determine if the thrown
 * exception is an instance of any of the defined exception types. The
 * <code>type</code> of the thrown exception can also be compared for an exact
 * type match.
 * </p>
 *
 * <pre class="fragment">
 * #try{
 *    ...
 * }#catch(#RuntimeException){
 *    const #e4c_exception * exception = #e4c_get_exception();
 *    if( #e4c_is_instance_of(exception, SignalException.type) ){
 *        ...
 *    }else if(exception->type == NotEnoughMemoryException.type){
 *        ...
 *    }
 * }
 * </pre>
 *
 * <p>
 * The thrown exception can be obtained any time, provided that the exception
 * context has begun at the time of the function call. However, it is sensible
 * to call this function only during the execution of <code>finally</code> or
 * <code>catch</code> blocks.
 * </p>
 *
 * <p>
 * Moreover, a pointer to the thrown exception obtained <em>inside</em> a
 * <code>finally</code> or <code>catch</code> block <strong>must not</strong> be
 * used from the <em>outside</em>.
 * </p>
 *
 * <p>
 * The exception system objects are dinamically allocated and deallocated, as
 * the program enters or exits <code>try/catch/finally</code> blocks. While it
 * is legal to <strong>copy</strong> the thrown exception and access its name
 * and message outside these blocks, care must be taken in order not to
 * dereference the <code>cause</code> of the exception, unless it is a
 * <strong>deep copy</strong> (as opposed to a <strong>shallow copy</strong>).
 * </p>
 *
 * @see e4c_exception
 * @see e4c_is_instance_of
 * @see throw
 * @see catch
 * @see finally
 *
 * @return the exception that was thrown in the current exception context (if
 *         any) otherwise <code>NULL</code>
 */
extern const e4c_exception * e4c_get_exception(void);

/** @} */

/**
 * @name Other integration and convenience functions
 *
 * @{
 */

/**
 * Gets the library version number
 *
 * <p>
 * This function provides the same information as the
 * <code>E4C_VERSION_NUMBER</code> macro, but the returned version number is
 * associated with the actual, compiled library.
 * </p>
 *
 * <p>
 * This version number can be considered as the <em>run-time</em> library
 * version number, as opposed to the <em>compile-time</em> library version
 * number (specified by the header file).
 * </p>
 *
 * <p>
 * The library must be compiled with the corresponding header (i.e. library
 * version number should be equal to header version number).
 * </p>
 *
 * @see E4C_VERSION_NUMBER
 *
 * @return The version number associated with the library
 */
extern long e4c_library_version(void);

/**
 * Returns whether an exception is of a given exception type
 *
 * <p>
 * <code>e4c_is_instance_of</code> can be used to determine if a thrown
 * exception <strong>is an instance of a given type</strong> defined through
 * <code>E4C_DEFINE_EXCEPTION</code> and/or declared through
 * <code>E4C_DECLARE_EXCEPTION</code>
 * </p>
 *
 * <p>
 * This macro is intended to be used in a <code>catch</code> block, or in a
 * <code>finally</code> block provided that some exception was actually thrown
 * (i.e. <code>e4c_get_status</code> returned <code>e4c_failed</code> or
 * <code>e4c_recovered</code>)
 * </p>
 *
 * <pre class="fragment">
 * #try{
 *    ...
 * }#catch(#RuntimeException){
 *    const #e4c_exception * exception = #e4c_get_exception();
 *    if( #e4c_is_instance_of(exception, SignalException.type) ){
 *        ...
 *    }else if(exception->type == NotEnoughMemoryException.type){
 *        ...
 *    }
 * }
 * </pre>
 *
 * @see e4c_exception
 * @see e4c_get_exception
 *
 * @param instance The thrown exception
 * @param type A previously defined type of exception
 * @return Whether the specified exception is an instance of the given type
 * @throws NullPointerException if either <code>instance</code> or
 *         <code>type</code> is <code>NULL</code>
 */
extern e4c_bool e4c_is_instance_of(const e4c_exception * instance,
	const e4c_exception * type);

/**
 * Prints a fatal error message regarding the specified exception
 *
 * <p>
 * This is a convenience function for showing an error message through the
 * standard error output. It can be passed to
 * <code>#e4c_context_begin</code> or <code>#e4c_using_context</code> as the
 * handler for uncaught exceptions.
 * </p>
 *
 * <p>
 * In absence of <code>NDEBUG</code>, this function prints as much information
 * regarding the exception as it is available, whereas in presence of
 * <code>NDEBUG</code>, only the name and message of the exception are printed.
 * </p>
 *
 * @param exception The uncaught exception
 * @throws NullPointerException if <code>exception</code> is <code>NULL</code>
 */
extern void e4c_print_exception(const e4c_exception * exception);

/** @} */

/*
 * Next functions are undocumented on purpose, because they shouldn't be used
 * directly (but through the 'keyword' macros).
 */

extern _E4C_JMP_BUF * e4c_frame_init(enum _e4c_frame_stage stage,
	const char * file, int line, const char * function);

extern e4c_bool e4c_frame_step(void);

extern e4c_bool e4c_frame_hook(enum _e4c_frame_stage stage,
	const e4c_exception * exception, const char * file, int line,
	const char * function);

extern void e4c_frame_repeat(
	int max_repeat_attempts, enum _e4c_frame_stage stage,
	const char * file, int line, const char * function);

extern void e4c_throw_exception(const e4c_exception * exception,
	const char * file, int line, const char * function,
	e4c_bool verbatim, const char * message, ...)
#ifdef	__GNUC__
	__attribute__ ((noreturn))
#endif
;

# endif
