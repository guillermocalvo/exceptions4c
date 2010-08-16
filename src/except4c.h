/**
 *
 * @file except4c.h
 *
 * exceptions4c header file
 *
 * @version 1.4
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


# ifndef _EXCEPT4C_H_
# define _EXCEPT4C_H_

# if !defined(E4C_THREAD_SAFE) && ( \
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
# error Please define E4C_THREAD_SAFE at compiler level to enable the \
          thread-safe version of exceptions4c.
# endif

# include <stdlib.h>
# include <setjmp.h>

# if __STDC_VERSION__ >= 199901L
# include <stdbool.h>
# endif

# ifdef __bool_true_false_are_defined
# define e4c_bool						bool
# define e4c_false						false
# define e4c_true						true
# else
# define e4c_bool						unsigned char
# define e4c_false						0
# define e4c_true						1
# endif

# ifdef E4C_IMPLEMENTATION
# define E4C_READ_ONLY
# else
# define E4C_READ_ONLY					const
# endif


# define E4C_PASTE_TOKENS(a, b, c, d, e) a ## b ## c ## d ## e
# define E4C_MANGLE_NAME(_prefix_, _name_, _line_) E4C_PASTE_TOKENS(_prefix_, _name_, _, _line_, _)
# define E4C_RECYCLING(name) E4C_MANGLE_NAME(_recycling_, name, __LINE__)


/*
 * Next macros are undocumented on purpose, because they shouldn't be used
 * directly (but through the macros: try, catch, finalize and throw).
 */

# ifdef _POSIX_C_SOURCE
# define E4C_SETJMP(address)			sigsetjmp(address, e4c_true)
# define E4C_LONGJMP(address)			siglongjmp(address, 1)
# define E4C_JMP_BUF					sigjmp_buf
# else
# define E4C_SETJMP(address)			setjmp(address)
# define E4C_LONGJMP(address)			longjmp(address, 1)
# define E4C_JMP_BUF					jmp_buf
# endif

# ifndef NDEBUG
# define E4C_FILE_INFO					(const char *)__FILE__
# define E4C_LINE_INFO					__LINE__
# else
# define E4C_FILE_INFO					(const char *)NULL
# define E4C_LINE_INFO					0
# endif

/*
 * These undocumented macros hide implementation details from documentation.
 */
# define E4C_LOOP(stage) \
	E4C_SETJMP(  *( e4c_first(stage, E4C_FILE_INFO, E4C_LINE_INFO) )  ); \
	while( e4c_next() )

# define E4C_TRY \
	E4C_LOOP(e4c_acquiring) \
	if( e4c_hook(e4c_trying, INVALID_EXCEPTION) \
		&& e4c_next() )
	/* simple optimization: e4c_next() will avoid e4c_disposing stage */

# define E4C_CATCH(_exception_) \
	else if( \
		e4c_hook(e4c_catching, _exception_) \
	)

# define E4C_FINALLY \
	else if( \
		e4c_hook(e4c_finalizing, INVALID_EXCEPTION) \
	)

# define E4C_THROW(_exception_) \
	e4c_throw( \
		_exception_, E4C_FILE_INFO, E4C_LINE_INFO \
	)

# define E4C_WITH(_resource_, _dispose_) \
	E4C_LOOP(e4c_beginning) \
	if( e4c_hook(e4c_disposing, INVALID_EXCEPTION) ){ \
		_dispose_(_resource_, E4C_CONTEXT->currentFrame->thrown, E4C_CONTEXT); \
	}else if( e4c_hook(e4c_acquiring, INVALID_EXCEPTION) ){

# define E4C_USE \
	}else if( \
		e4c_hook(e4c_trying, INVALID_EXCEPTION) \
	)

# define E4C_USING(_type_, _resource_, _args_) \
	with(_resource_, dispose##_type_){ \
		_resource_ = acquire##_type_ _args_; \
	}use

# define E4C_RECYCLE_EXCEPTION_CONTEXT(_thrownException_) \
	\
	e4c_RecyclingStage	E4C_RECYCLING(stage)	= e4c_beforePayload; \
	e4c_bool			E4C_RECYCLING(recycled)	= (E4C_CONTEXT != NULL); \
	const Exception *	_thrownException_		= NULL; \
	\
	if( !E4C_RECYCLING(recycled) ){ \
        beginExceptionContext(e4c_false, NULL); \
        try{ \
			goto E4C_RECYCLING(payload); \
			E4C_RECYCLING(cleanup): \
            ; \
		}catch(RuntimeException){ \
			_thrownException_ = &EXCEPTION; \
		}finally{ \
			E4C_RECYCLING(stage) = e4c_recyclingDone; \
            e4c_next(); \
			endExceptionContext(); \
			break; \
		} \
	} \
	\
	E4C_RECYCLING(payload): \
	for(; E4C_RECYCLING(stage) < e4c_recyclingDone; E4C_RECYCLING(stage)++) \
		if( E4C_RECYCLING(stage) == e4c_afterPayload){ \
            if( !E4C_RECYCLING(recycled) ){ \
            	goto E4C_RECYCLING(cleanup); \
            }else{ \
            	break; \
        	} \
		}else

/**
 * @name Exception handling keywords
 *
 * <p>
 * This is a set of keywords which map the semantics of exception handling.
 * </p>
 *
 * @{
 */

/**
 * Introduces a block of code aware of exceptions
 */
# define try E4C_TRY

/**
 * Introduces a block of code capable of handling a specific kind of exceptions
 *
 * @param _exception_ The kind of exceptions to be handled
 */
# define catch(_exception_) E4C_CATCH(_exception_)

/**
 * Introduces a block of code responsible for cleaning up the previous
 * <code>try</code> block
 */
# define finally E4C_FINALLY

/**
 * Signals an exceptional situation represented by an exception object
 *
 * @param _exception_ The exception to be thrown
 */
# define throw(_exception_) E4C_THROW(_exception_)

/*@}*/

/**
 * @name Dispose pattern keywords
 *
 * <p>
 * This is a set of keywords which map the semantics of automatic resource
 * acquisition and disposal.
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
 * The <code>with</code> keyword is used to encapsulate the
 * <em>Dispose Pattern</em>. It must be followed by the <code>use</code>
 * keyword.
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
 * <li>the <code>with</code> block is responsible of the resource
 * acquisition</li>
 * <li>the <code>use</code> block makes use of the resource</li>
 * <li>the disposal function is called implicitly</li>
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
 * If the <code>with</code> does not complete, then neither the disposal
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
 * When called, the disposal function will receive three arguments:
 * </p>
 *
 * <ol>
 * <li>The resource</li>
 * <li>A boolean flag indicating if the <code>use</code> block did not
 * <strong>complete</strong></li>
 * <li>The current exception context</li>
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
 * <code>fclose</code> only takes a parameter, we could provide
 * <code>disposeFile</code> as the disposal function and define the next macro:
 * </p>
 *
 * <pre class="fragment">
 * # define disposeFile(_file_, _failed_, _context_) fclose(_file_)
 * </pre>
 *
 * <p>
 * The typical usage of a <code>with</code> block will be:
 * </p>
 *
 * <pre class="fragment">
 * with(foo, disposeFoo){
 *     foo = acquireFoo(foobar);
 *     someAssertion(foo, bar);
 *     ...
 * }use{
 *     fooSomething(foo);
 *     ...
 * }catch(FooAcquisitionFailed){
 *     recover1();
 *     ...
 * }catch(somethingElseFailed){
 *     recover2();
 *     ...
 * }finally{
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
 * with(foo, disposeFoo) foo = acquireFoo(bar, foobar); use fooSomething(foo);
 * </pre>
 *
 * <p>
 * There is a way to lighten up even more this pattern by defining convenience
 * macros, customized for a specific kind of resources. For example,
 * <code>#usingFile</code> or <code>#usingMemory</code>.
 * </p>
 *
 * @see use
 * @see using
 * @see usingIfNotNull
 * @see usingMemory
 * @see usingFile
 */
# define with(_resource_, _dispose_) E4C_WITH(_resource_, _dispose_)

/**
 * Closes a block of code with automatic disposal of a resource
 *
 * <p>
 * A <code>use</code> block must always be preceded by a <code>with</code>
 * block. Dangling <code>with</code> or <code>use</code> blocks will (hopefully)
 * make the compiler complain.
 * </p>
 *
 * @see with
 */
# define use E4C_USE

/**
 * Introduces a block of code with automatic acquisition and disposal of a
 * resource
 *
 * @param _type_ The type of the resource
 * @param _resource_ The resource to be acquired, used and then disposed
 * @param _args_ A list of arguments to be passed to the acquisition function
 *
 * <p>
 * The specified resource will be aquired, used and then disposed. The automatic
 * acquisition and disposal is achieved by calling the <em>magic</em> functions:
 * </p>
 *
 * <ul>
 * <li><code>_type_ acquire<em>_type_</em>(_args_)</code></li>
 * <li><code>void dispose<em>_type_</em>(_resource_, failed,
 * context)</code></li>
 * </ul>
 *
 * <p>
 * The resource will be acquired implicitly by assigning it the result of the
 * <em>magic</em> acquisition function <code>acquire<em>_type_</em></code>.
 * </p>
 *
 * <p>
 * The semantics of the automatic acquisition and disposal are the same as for
 * blocks introduced by the keyword <code>with</code>.
 * </p>
 *
 * @see with
 * @see usingIfNotNull
 * @see usingMemory
 * @see usingFile
 */

# define using(_type_, _resource_, _args_) E4C_USING(_type_, _resource_, _args_)

/*@}*/

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
 * Binds the acquisition of memory to the standard function <code>malloc</code>
 */
# define acquireMemory malloc

/**
 * Binds the disposal of memory to the standard function <code>malloc</code>
 */
# define disposeMemory(_buffer_, _failure_, _context_) free(_buffer_)

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
 * usingMemory(buffer, 1024){
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
# define usingMemory(_buffer_, _bytes_) \
	usingIfNotNull( \
		Memory, _buffer_, (_bytes_), NotEnoughMemoryException \
	)

/**
 * Binds the acquisition of file to the standard function <code>fopen</code>
 */
# define acquireFile fopen

/**
 * Binds the disposal of memory to the standard function <code>fclose</code>
 */
# define disposeFile(_file_, _failure_, _context_) fclose(_file_)

/**
 * Introduces a block of code with automatic acquisition and disposal of a
 * file stream
 *
 * @param _file_ The file to be acquired, used and then disposed
 * @param _path_ The path of the file to be opened
 * @param _mode_ The access mode of the file
 *
 * <p>
 * This macro lets you acquire and dispose (open and close) files according to
 * the <em>dispose pattern</em>:
 * </p>
 *
 * <pre class="fragment">
 * FILE * file;
 * &nbsp;
 * usingFile(file, "log.txt", "a"){
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
 * The macro <code>E4C_ERRNO</code> can be checked in order to find out the
 * specific cause of the error.
 * </p>
 *
 */
# define usingFile(_file_, _path_, _mode_) \
	usingIfNotNull( \
		File, _file_, (_path_, _mode_), FileOpenException \
	)

/**
 * Introduces a block of code with automatic disposal of a resource and
 * acquisition, under certain condition
 *
 * @param _type_ The type of the resource
 * @param _resource_ The resource to be acquired, used and then disposed
 * @param _args_ A list of arguments to be passed to the acquisition function
 * @param _cond_ The condition which has to be satisfied in order to consider
 *        the acquisition <em>complete</em>
 * @param _exception_ The exception to be thrown if the acquisition function
 *        does not satisfy the specified condition
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
 * # define acquireMemory malloc
 * # define disposeMemory free
 * # define usingMemory(_resource_, _bytes_) \
 *          usingIf(Memory, _resource_, ( _bytes_ ), _resource_ != NULL, \
 *          NotEnoughMemoryException )
 * </pre>
 *
 * @see using
 */
# define usingIf(_type_, _resource_, _args_, _cond_, _exception_) \
	with(_resource_, dispose##_type_){ \
		_resource_ = acquire##_type_ _args_; \
		if( !(_cond_) ) throw(_exception_); \
	}use

/**
 * Introduces a block of code with automatic disposal of a resource and
 * acquisition, preventing null pointers
 *
 * @param _type_ The type of the resource
 * @param _resource_ The resource to be acquired, used and then disposed
 * @param _args_ A list of arguments to be passed to the acquisition function
 * @param _exception_ The exception to be thrown if the acquisition function
 *        yields a <code>NULL</code> pointer.
 *
 * @see usingIf
 */
# define usingIfNotNull(_type_, _resource_, _args_, _exception_) \
	usingIf( \
		_type_, _resource_, _args_, _resource_ != NULL, _exception_ \
	)

/*@}*/

/**
 * @name Integration macro
 *
 * <p>
 * This macro is designed to ease the integration of external libraries.
 * </p>
 *
 * @{
 */

/**
 * Reuses an existing exception context, otherwise, opens a new one and then
 * closes it.
 *
 * <p>
 * This macro lets library developers use the exception framework, regardless of
 * the library clients being unaware of the exception handling system. In a
 * nutshell, function libraries can use <code>#try</code>, <code>#catch</code>,
 * <code>#throw</code>, etc. whether the client previously began an exception
 * context or not.
 * </p>
 *
 * <p>
 * You <strong>must not use this macro</strong> unless you are implementing some
 * functionality which is to be called from another program, potentially unaware
 * of exceptions.
 * </p>
 *
 * <p>
 * A block introduced by <code>recycleExceptionContext</code> is guaranteed to
 * take place <em>inside</em> an execution context. When the block completes,
 * the system returns to its previous status (if it was necessary to open a new
 * exception context, it will be automatically closed).
 * </p>
 *
 * <p>
 * This way, when an external functions encounters an error, it may either throw
 * an exception (when the caller did open an exception context), or otherwise
 * return an error code (when the caller did not open an exception context).
 * </p>
 *
 * <p>
 * <code>recycleExceptionContext</code> needs to be given a name to reference
 * whichever exception thrown inside the block. This variable will be implicitly
 * created within the scope; its type will be <code>const #Exception *</code>.
 * </p>
 *
 * <pre class="fragment">
 * int libraryPublicFunction(void * pointer, int number){
 * &nbsp;
 *     /<span>*
 *     We don't know where this function is going to be called from, so:
 *       * We cannot use "try", "throw", etc. right here, because the exception
 *       context COULD be uninitialized at this very moment.
 *       * We cannot call "beginExceptionContext" either, because the exception
 *       context COULD be already initialized.
 *     If we have to make use of the exception handling system, we need to
 *     "recycle" the exception context.
 *     *</span>/
 * &nbsp;
 *     recycleExceptionContext(exception){
 *         /<span>* Now we can safely use "try", "throw", etc. *</span>/
 *         if(pointer == NULL){
 *             throw(NullPointerException);
 *         }
 * &nbsp;
 *         libraryPrivateFunction(pointer, number);
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
 * Next, the semantics of <code>recycleExceptionContext</code> are explained,
 * step by step.
 * </p>
 *
 * <ul>
 *   <li>
 *   If there is an exception context at the time the block starts:
 *   <ol>
 *     <li>The existing exception context will be reused
 *       (<em>recycled</em>).</li>
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
 *       <li><code>_thrownException_</code> will be <code>NULL</code>.</li>
 *       <li>The function may then return a "success" status code.</li>
 *       </ul>
 *     </li>
 *   </ol>
 *   </li>
 *   <li>
 *   If there is no exception context at the time the block starts:
 *   <ol>
 *     <li>A new exception context will be opened; note that the signal handling
 *       system <strong>will NOT be set up</strong>.</li>
 *     <li>The code block will take place.</li>
 *     <li>
 *       If any exception is thrown during the execution of the block:
 *       <ul>
 *       <li>It will be <strong>caught</strong> and
 *           <code>_thrownException_</code> will point to it.</li>
 *       </ul>
 *     </li>
 *     <li>
 *       When the block ends:
 *       <ul>
 *       <li>If <code>_thrownException_</code> is <code>NULL</code> then the
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
 * recycleExceptionContext(exception){
 * &nbsp;
 *     void * buffer = NULL;
 *     try{
 *         buffer = malloc(1024);
 *         ...
 *     }finally{
 *         free(buffer);
 *     }
 * }
 * ...
 * </pre>
 *
 * <p>
 * If you need to rely on the signal handling system, you should call
 * <code>#setSignalHandlers</code> explicitely. You could also call
 * <code>#getSignalHandlers</code>, so you can restore the previous mapping when
 * you are done:
 * </p>
 *
 * <pre class="fragment">
 * ...
 * recycleExceptionContext(exception){
 * &nbsp;
 *     int prevMappings;
 *     const SignalMapping * prevMapping = getSignalHandlers(&prevMappings);
 * &nbsp;
 *     setSignalHandlers(newMapping, newMappings);
 * &nbsp;
 *     try{
 *         ...
 *     }finally{
 *         setSignalHandlers(prevMapping, prevMappings);
 *     }
 * }
 * ...
 * </pre>
 *
 * @param _thrownException_ The name of a pointer to find out whether an
 *        exception was thrown inside the block.
 *
 * @see e4c_ExceptionContext
 * @see beginExceptionContext
 * @see endExceptionContext
 * @see E4C_CONTEXT
 */
# define recycleExceptionContext(_thrownException_) \
	E4C_RECYCLE_EXCEPTION_CONTEXT(_thrownException_)

/*@}*/

/**
 * @name Convenience macros for accessing exception-related information
 *
 * <p>
 * These macros let you obtain data regarding the current exception context,
 * such as the actual exception being thrown, the value of <code>errno</code>,
 * etc.
 * </p>
 *
 * <p>
 * This information is especially useful when recovering from errors
 * (<code>catch</code> blocks) or cleaning up (<code>finally</code> blocks).
 * </p>
 *
 * @{
 */

/**
 * Accesses the current exception context of the program (or current thread)
 * @see ExceptionContext
 * @see getExceptionContext
 * @see beginExceptionContext
 * @see endExceptionContext
 */
# define E4C_CONTEXT			getExceptionContext()

/**
 * Accesses the current frame of the exception context
 * @see ExceptionFrame
 */
# define E4C_FRAME				(E4C_CONTEXT->currentFrame)

/**
 * Accesses the current exception being thrown from the current exception frame
 *
 * <p>
 * If no exception was thrown, then the value of <code>EXCEPTION</code> will be
 * <code>INVALID_EXCEPTION</code>.
 * </p>
 *
 * <p>
 * The flag <code>E4C_STATUS_THROWN</code> can be checked in order to know if an
 * exception was actually thrown.
 * </p>
 *
 * @see ExceptionFrame
 * @see INVALID_EXCEPTION
 * @see E4C_STATUS_THROWN
 */
# define EXCEPTION				(E4C_FRAME->exception)

/**
 * Accesses the error number of the current exception frame
 * @see ExceptionFrame
 */
# define E4C_ERRNO				(E4C_FRAME->errorNumber)

/**
 * Accesses the error file of the current exception frame
 * @see ExceptionFrame
 */
# define E4C_FILE				(E4C_FRAME->file)

/**
 * Accesses the error line of the current exception frame
 * @see ExceptionFrame
 */
# define E4C_LINE				(E4C_FRAME->line)

/**
 * Accesses the <code>thrown</code> flag of the current exception frame
 *
 * <p>
 * The <code>thrown</code> flag is set when some exception is thrown within the
 * current exception frame, regardless of whether it gets caught or not.
 * </p>
 *
 * @see ExceptionFrame
 * @see E4C_STATUS_UNCAUGHT
 */
# define E4C_STATUS_THROWN		(E4C_FRAME->thrown)

/**
 * Accesses the <code>uncaught</code> flag of the current exception frame
 *
 * <p>
 * The <code>uncaught</code> flag is set when some exception is caught within
 * the current exception frame.
 * </p>
 *
 * @see ExceptionFrame
 * @see E4C_STATUS_THROWN
 */
# define E4C_STATUS_UNCAUGHT	(E4C_FRAME->uncaught)

/**
 * Checks whether the current exception frame did complete without an error.
 * @see ExceptionFrame
 */
# define E4C_SUCCEED			(!E4C_STATUS_THROWN)

/**
 * Checks whether the current exception frame did fail and then did recover of
 * the error.
 * @see ExceptionFrame
 */
# define E4C_RECOVERED			(E4C_STATUS_THROWN && !E4C_STATUS_UNCAUGHT)

/**
 * Checks whether the current exception frame did fail and did not recover of
 * the error.
 * @see ExceptionFrame
 */
# define E4C_FAILED				(E4C_STATUS_THROWN && E4C_STATUS_UNCAUGHT)

/*@}*/

/**
 * @name Other convenience macros
 *
 * <p>
 * These macros let you represent and define #Exception and #SignalMapping
 * literals and variables.
 * </p>
 *
 * @{
 */

/**
 * Defines a new, const exception
 *
 * @param _name_ Name of the new exception
 * @param _description_ Description of the new exception
 * @param _super_ Supertype of the new exception
 *
 * @see Exception
 */
# define DEFINE_EXCEPTION(_name_, _description_, _super_) \
	const Exception _name_ = { \
		name: #_name_, description: _description_, super: &_super_ \
	}

/**
 * Represents a mapping literal
 *
 * @param _signalNumber_ Signal to be converted
 * @param _exception_ Exception representing the signal
 *
 * @see SignalMapping
 * @see beginExceptionContext
 * @see setSignalHandlers
 * @see getSignalHandlers
 * @see defaultSignalMapping
 * @see defaultSignalMappings
 */
# define SIGNAL_MAPPING(_signalNumber_, _exception_) \
	{ \
		signalNumber:   _signalNumber_, \
		exception:      &_exception_, \
	}

/*@}*/

# ifdef __DOXYGEN
/* This is a kludge to work around Doxygen */
typedef e4c_Exception Exception;
typedef e4c_SignalMapping SignalMapping;
typedef e4c_ExceptionFrame ExceptionFrame;
typedef e4c_ExceptionContext ExceptionContext;
# endif

/**
 * Represents an exception in the exception handling system
 *
 * <p>
 * Exceptions are objects with a <code>name</code>, a <code>description</code>
 * and a <code>super</code>-type.
 * </p>
 *
 * <p>
 * They are defined through the macro <code>#DEFINE_EXCEPTION</code>:
 * </p>
 *
 * <pre class="fragment">
 * DEFINE_EXCEPTION(StackException, "Stack overflow", RuntimeException);
 * DEFINE_EXCEPTION(StackOverflowException, "Stack overflow", StackException);
 * DEFINE_EXCEPTION(StackUnderflowException, "Stack underflow", StackException);
 * </pre>
 *
 * <p>
 * Exceptions are usually defined as global <code>const</code> objects. There is
 * a set of predefined exceptions built into the framework, amongst others:
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
 * <code>catch(RuntimeException)</code> block.
 * </p>
 *
 * @see RuntimeException
 * @see DEFINE_EXCEPTION
 * @see throw
 * @see catch
 * @see EXCEPTION
 */
typedef struct e4c_Exception Exception;
struct e4c_Exception{

	/** The name of this exception */
	const char *			name;

	/** The description of this exception */
	const char *			description;

	/** The supertype of this exception */
	const Exception *		super;
};

/*
 * Next two enumerations are undocumented on purpose, in order to hide
 * implementation details, subject to change.
 */

/*
 * Represents the current execution status of an exception frame
 *
 * <p>
 * <em>Note: this type is internal to the framework, and not intended to be used
 * by the user of the library.</em>
 * </p>
 *
 * <p>
 * Every group of <code>#try</code>/<code>#catch</code>/<code>#finally</code>
 * blocks goes through a set of execution stages. Each stage determines which
 * one of the different blocks is run.
 * </p>
 */
typedef enum{
	/* No block has been executed yet */
	e4c_beginning,
	/* Acquiring a resource */
	e4c_acquiring,
	/* Executing the "try" or "use" block */
	e4c_trying,
	/* Disposing a resource */
	e4c_disposing,
	/* Catching (or attempting to catch) an exception */
	e4c_catching,
	/* Cleaning up ("finally" block) */
	e4c_finalizing,
	/* The group of blocks has been executed */
	e4c_end
} e4c_Stage;

/*
 * Represents the current execution status of a <em>recycled</em> block
 *
 * <p>
 * <em>Note: this type is internal to the framework, and not intended to be used
 * by the user of the library.</em>
 * </p>
 *
 * @see recycleExceptionContext
 */
typedef enum{
	/* The block has not been executed yet */
	e4c_beforePayload,
	/* The block has just been executed */
	e4c_afterPayload,
	/* The block is about to complete */
	e4c_recyclingDone
} e4c_RecyclingStage;

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
 * @see beginExceptionContext
 *
 * @param exception The uncaught exception
 * @param file The path of the source code file from which the exception was
 *        thrown
 * @param line The number of line from which the exception was thrown
 * @param errorNumber The value of errno at the time the exception was thrown
 */
typedef void (*UncaughtHandler)(
	Exception		exception,
	const char *	file,
	int				line,
	int				errorNumber
);


/**
 * Represents a map between a signal and an exception
 *
 * <p>
 * <strong>exceptions4c</strong> can make your life easier by converting signals
 * to <code>#Exception</code>s. Most of these signals would crash your program
 * as soon as they were raised. Now you can <code>#catch</code> signals and
 * avoid core dumps.
 * </p>
 *
 * <p>
 * For example, you could wrap a <em>suspicious</em> or <em>dangerous</em> part
 * of the code with <code>#try</code> blocks and <code>#catch</code>
 * segmentation faults or divisions by zero. Then you can clean up and keep
 * working:
 * </p>
 *
 * <pre class="fragment">
 * int * pointer = NULL;
 * try{
 *     int oops = *pointer;
 * }catch(BadPointerException){
 *     printf("No problem ;-)");
 * }finally{
 *     /<span>* clean up... *</span>/
 * }
 * </pre>
 *
 * <p>
 * In order to perform the conversion, <strong>exceptions4c</strong> maps
 * signals to exceptions.
 * </p>
 *
 * <p>
 * The simpler way to get this working is by calling function:
 * </p>
 *
 * <pre class="fragment">
 * extern void beginExceptionContext(bool handleSignals);</pre>
 *
 * <p>
 * <code>beginExceptionContext</code> will set up for you the default handlers
 * for the available signals in the platform if you pass
 * <code>handleSignals=true</code>.
 * </p>
 *
 * <p>
 * If you need to be more specific about which signals get converted to
 * exceptions, you can define an array of <code>SignalMapping</code> this way:
 * </p>
 *
 * <pre class="fragment">
 * SignalMapping mapping[] = {
 *     SIGNAL_MAPPING(SIGABRT, YourVeryOwnException),
 *     ...
 * }
 * </pre>
 *
 * <p>
 * ...and then pass it to the function:
 * </p>
 *
 * <pre class="fragment">
 * extern void setSignalHandlers(const SignalMapping * mapping, int mappings);
 * </pre>
 *
 * <p>
 * This way, only the specified signals will be handled as exceptions, and they
 * will be converted to the specified exceptions.
 * </p>
 *
 * <p>
 * These are some of the signals you can handle:
 * </p>
 *
 * <ul>
 * <li><code>SIGFPE</code> when you try to divide by zero.</li>
 * <li><code>SIGSEGV</code> when you make an invalid memory reference, or
 * segmentation fault.</li>
 * <li><code>SIGINT</code> when a user wishes to interrupt the process,
 * Control-c.</li>
 * <li><code>SIGTERM</code> the signal sent to a process to request its
 * termination.</li>
 * </ul>
 *
 * @see SIGNAL_MAPPING
 * @see beginExceptionContext
 * @see setSignalHandlers
 * @see getSignalHandlers
 * @see defaultSignalMapping
 * @see defaultSignalMappings
 */
typedef struct e4c_SignalMapping SignalMapping;
struct e4c_SignalMapping{

	/** The signal to be converted */
	int						signalNumber;

	/** The exception representing the signal */
	const Exception *		exception;
};

/**
 *
 * Represents a frame inside an exception context.
 *
 * <p>
 * A new frame is created every time a group of <code>#try</code>/
 * <code>#catch</code>/<code>#finally</code> blocks starts, and destroyed
 * when the group ends.
 * </p>
 *
 * <p>
 * Frames are stacked and dispatched in a <em>First-In-Last-Out</em> way. When
 * an exception is thrown within a frame, it <em>climbs</em> towards the
 * previous frames until it gets caught, otherwise the program ends.
 * </p>
 */
typedef struct e4c_ExceptionFrame ExceptionFrame;
struct e4c_ExceptionFrame{

	/** The continuation address of the current block */
	E4C_READ_ONLY	E4C_JMP_BUF			address;

	/** The previous (upper) frame */
	ExceptionFrame * E4C_READ_ONLY		previous;

	/** The current stage of this frame */
	E4C_READ_ONLY	e4c_Stage			stage;

	/** A flag that is set when an exception was thrown */
	E4C_READ_ONLY	e4c_bool			thrown;

	/** A flag that is set when the exception was caught */
	E4C_READ_ONLY	e4c_bool			uncaught;

	/** The current exception being thrown */
	E4C_READ_ONLY	Exception			exception;

	/** The path of the source code file from which the exception was thrown */
	const char *	E4C_READ_ONLY		file;

	/** The number of line from which the exception was thrown */
	E4C_READ_ONLY	int					line;

	/** The value of errno at the time the exception was thrown */
	E4C_READ_ONLY	int					errorNumber;
};

/**
 * Represents the exception context of the program
 *
 * <p>
 * Each part of the program that makes use of the exception handling system,
 * needs an exception context. This context holds a stack of
 * <code>#ExceptionFrame</code>s, in which exception information is recorded.
 * </p>
 */
typedef struct e4c_ExceptionContext ExceptionContext;
struct e4c_ExceptionContext{

	/** The current (inner) exception frame of this context */
	E4C_READ_ONLY ExceptionFrame * E4C_READ_ONLY	currentFrame;

	/** The array of mappings between the signals and exceptions */
	const SignalMapping * E4C_READ_ONLY				signalMapping;

	/** The number of elements in the array of mappings */
	E4C_READ_ONLY	int								signalMappings;

	/** The function to be executed in the event of an uncaught exception */
	E4C_READ_ONLY	UncaughtHandler					uncaughtHandler;
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
 * @see SignalMapping
 * @see SIGNAL_MAPPING
 * @see beginExceptionContext
 * @see setSignalHandlers
 * @see getSignalHandlers
 * @{
 */

/**
 * The array of predefined signal mappings.
 */
extern const SignalMapping defaultSignalMapping[];

/**
 * The number of predefined signal mappings.
 */
extern const int defaultSignalMappings;

/*@}*/

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
 * This represents an invalid exception.
 *
 * <p>
 * <code>INVALID_EXCEPTION</code> honors the Null Object pattern:
 * </p>
 *
 * <ul>
 * <li>Throwing it has no effect</li>
 * <li>Catching it does nothing</li>
 * </ul>
 */
extern const Exception INVALID_EXCEPTION;

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
extern const Exception RuntimeException;

/**
 * This exception is thrown when the system runs out of memory.
 *
 * NotEnoughMemoryException is thrown when there is not enough memory to
 * continue the execution of the program.
 */
extern const Exception NotEnoughMemoryException;

/**
 * This exception is thrown when an unexpected null pointer is found.
 *
 * NullPointerException is thrown when some part of the program gets a pointer
 * which was expected or required to contain a valid memory address.
 */
extern const Exception NullPointerException;

/**
 * This exception is thrown when a file cannot be opened.
 *
 * FileOpenException is thrown by <code>#usingFile</code> when
 * <code>fopen</code> returns <code>NULL</code> for whatever reason.
 *
 * The value of <code>errno</code> at the time the exception was thrown (right
 * after <code>fopen</code>) can be accessed through the macro
 * <code>E4C_ERRNO</code>.
 */
extern const Exception FileOpenException;

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
extern const Exception SignalException;

/**
 * This exception is thrown when a time limit has elapsed
 *
 * SignalAlarmException represents <code>SIGALRM</code>, the signal sent to a
 * process when a time limit has elapsed.
 */
extern const Exception SignalAlarmException;

/**
 * This exception is thrown when a child process terminates
 *
 * SignalChildException represents <code>SIGCHLD</code>, the signal sent to a
 * process when a child process terminates (ignored by default).
 */
extern const Exception SignalChildException;

/**
 * This exception is thrown when a condition arises that a debugger has
 * requested to be informed of.
 *
 * SignalTrapException represents <code>SIGTRAP</code>, the signal sent to a
 * process when a condition arises that a debugger has requested to be informed
 * of.
 */
extern const Exception SignalTrapException;

/**
 * This exception is the common supertype of all error signal exceptions.
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
extern const Exception ErrorSignalException;

/**
 * This exception is thrown when the process attempts to execute an illegal
 * instruction.
 *
 * IllegalInstructionException represents <code>SIGILL</code>, the signal sent
 * to a process when it attempts to execute a malformed, unknown, or privileged
 * instruction.
 */
extern const Exception IllegalInstructionException;

/**
 * This exception is thrown when the process performs an erroneous arithmetic
 * operation.
 *
 * ArithmeticException represents <code>SIGFPE</code>, the signal sent to a
 * process when it performs an erroneous arithmetic operation.
 */
extern const Exception ArithmeticException;

/**
 * This exception is thrown when the process tries to dereference an invalid
 * pointer.
 *
 * BadPointerException represents <code>SIGSEGV</code>, the signal sent to a
 * process when it makes an invalid memory reference, or segmentation fault.
 */
extern const Exception BadPointerException;

/**
 * This exception is thrown when the process attempts to write to a broken pipe.
 *
 * BrokenPipeException represents <code>SIGPIPE</code>, the signal sent to a
 * process when it attempts to write to a pipe without a process connected to
 * the other end.
 */
extern const Exception BrokenPipeException;

/**
 * This exception is the common supertype of all control signal exceptions.
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
extern const Exception ControlSignalException;

/**
 * This exception is thrown to stop the process for later resumption.
 *
 * StopException represents <code>SIGSTOP</code>, the signal sent to a process
 * to stop it for later resumption.
 *
 * Since <code>SIGSTOP</code> is unblock-able, it cannot be handled and
 * converted to this exception automatically.
 */
extern const Exception StopException;

/**
 * This exception is thrown to terminate the process immediately.
 *
 * KillException represents <code>SIGKILL</code>, the signal sent to a process
 * to cause it to terminate immediately.
 *
 * Since <code>SIGKILL</code> is unblock-able, it cannot be handled and
 * converted to this exception automatically.
 */
extern const Exception KillException;

/**
 * This exception is thrown when the process' terminal is closed.
 *
 * HangUpException represents <code>SIGHUP</code>, the signal sent to a process
 * when its controlling terminal is closed.
 */
extern const Exception HangUpException;

/**
 * This exception is thrown to request the termination of the process.
 *
 * TerminationException represents <code>SIGTERM</code>, the signal sent to a
 * process to request its termination.
 */
extern const Exception TerminationException;

/**
 * This exception is thrown to abort the process.
 *
 * AbortException represents <code>SIGABRT</code>, the signal sent by computer
 * programs to abort the process.
 */
extern const Exception AbortException;

/**
 * This exception is thrown when the process has used up the CPU for too long.
 *
 * CPUTimeException represents <code>SIGXCPU</code>, the signal sent to a
 * process when it has used up the CPU for a duration that exceeds a certain
 * predetermined user-settable value.
 */
extern const Exception CPUTimeException;

/**
 * This exception is the common supertype of all control signal exceptions
 * caused by the user.
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
extern const Exception UserControlSignalException;

/**
 * This exception is thrown when the user requests to quit the process.
 *
 * UserQuitException represents <code>SIGQUIT</code>, the signal sent to a
 * process by its controlling terminal when the user requests that the process
 * dump core.
 */
extern const Exception UserQuitException;

/**
 * This exception is thrown when the user requests to interrupt the process.
 *
 * UserInterruptionException represents <code>SIGINT</code>, the signal sent to
 * a process by its controlling terminal when a user wishes to interrupt it.
 */
extern const Exception UserInterruptionException;

/**
 * This exception is thrown when a user wishes to break the process.
 *
 * UserBreakException represents <code>SIGBREAK</code>, the signal sent to a
 * process by its controlling terminal when a user wishes to break it.
 */
extern const Exception UserBreakException;

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
extern const Exception ProgramSignalException;

/**
 * This exception is thrown when user-defined conditions occur.
 *
 * ProgramSignal1Exception represents <code>SIGUSR1</code>, the signal sent to a
 * process to indicate user-defined conditions.
 */
extern const Exception ProgramSignal1Exception;

/**
 * This exception is thrown when user-defined conditions occur.
 *
 * ProgramSignal2Exception represents <code>SIGUSR1</code>, the signal sent to a
 * process to indicate user-defined conditions.
 */
extern const Exception ProgramSignal2Exception;

/*@}*/

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
 * Begins an exception context
 *
 * <p>
 * This function begins the current exception context to be used by the program
 * (or current thread), until <code>#endExceptionContext</code> is called.
 * </p>
 *
 * <p>
 * A program (or thread) <strong>must not</strong> use the keywords
 * <code>#try</code>, <code>#catch</code>, <code>#throw</code>, etc. prior to
 * calling <code>beginExceptionContext</code>. Such programming error will lead
 * to an abrupt exit of the program (or thread).
 * </p>
 *
 * <p>
 * Calling <code>beginExceptionContext</code> <em>twice</em> is also considered
 * a programming error, and therefore the program (or thread) will exit abruptly
 * too. Nevertheless, <code>beginExceptionContext</code> can be called several
 * times <em>if, and only if,</em> <code>#endExceptionContext</code> is called
 * in between.
 * </p>
 *
 * <p>
 * The signal handling system can be automatically initialized with the default
 * signal mapping via <code>handleSignals</code> parameter when calling
 * <code>beginExceptionContext</code>. This is equivalent to:
 * </p>
 *
 * <pre class="fragment">
 * setSignalHandlers(defaultSignalMapping, defaultSignalMappings);
 * </pre>
 *
 * <p>
 * Note that the behaviour of <code>signal</code> is undefined in a
 * multithreaded program, so use the signal handling system with caution.
 * </p>
 *
 * <p>
 * In addition, a handling function can be specified to be called in the event
 * of an uncaught exception. This function will be called before exiting the
 * program (or thread).
 * </p>
 *
 * <p>
 * There exist a convenience function to be used as the default
 * <em>uncaught handler</em>, called <code>#dumpException</code>. This function
 * simply prints information regarding the exception to <code>stderr</code>, and
 * then exits.
 * </p>
 *
 * @see endExceptionContext
 * @see setSignalHandlers
 * @see defaultSignalMapping
 * @see defaultSignalMappings
 * @see UncaughtHandler
 * @see dumpException
 *
 * @param handleSignals If <code>true</code>, the signal handling system will be
 *        set up with the default mapping.
 * @param uncaughtHandler If not <code>NULL</code>, this function will be called
 *        in the event of an uncaught exception.
 */
extern void beginExceptionContext(e4c_bool handleSignals,
	UncaughtHandler uncaughtHandler);

/**
 * Retrieves the current exception context of the program (or current thread)
 *

 * <p>
 * This function retrieves the current exception context, to access its
 * properties.
 * </p>
 *
 * <p>
 * The macro <code>#E4C_CONTEXT</code> is an <em>alias</em> to call this
 * function.
 * </p>
 *
 * <p>
 * This function returns <code>NULL</code> if either the exception context has
 * not yet begun, or has already been ended.
 * </p>
 *
 * @see ExceptionContext
 * @see beginExceptionContext
 * @see endExceptionContext
 * @see E4C_CONTEXT
 *
 * @return The current exception context of the program (or current thread),
 *   otherwise <code>NULL</code>
 */
extern ExceptionContext * getExceptionContext();

/**
 * Ends the current exception context
 *
 * <p>
 * This function ends the current exception context.
 * </p>
 *
 * <p>
 * A program (or thread) <strong>must</strong> call
 * <code>#endExceptionContext</code> when the exception handling system is no
 * longer needed. Exiting a program (or thread) without calling before
 * <code>#endExceptionContext</code> is a programming error.
 * </p>
 *
 * @see beginExceptionContext
 */
extern void endExceptionContext();

/*@}*/

/**
 * @name Signal handling system functions
 *
 * <p>
 * These functions retrieve or assign the mapping between signals and exceptions
 * for the program (or thread).
 * </p>
 *
 * @{
 */

/**
 * Assigns the signal mapping for the program (or thread)
 *
 * <p>
 * This function assigns an array of mappings between the signals to be handled
 * and the corresponding exception to be thrown.
 * </p>
 *
 * <p>
 * A program (or thread) <strong>must</strong> call
 * <code>#beginExceptionContext</code> prior to calling
 * <code>setSignalHandlers</code>. Such programming error will lead to an abrupt
 * exit of the program (or thread).
 * </p>
 *
 * <p>
 * Note that the behaviour of <code>signal</code> is undefined in a
 * multithreaded program, so use the signal handling system with caution.
 * </p>
 *
 * @see SignalMapping
 * @see SIGNAL_MAPPING
 * @see defaultSignalMapping
 * @see defaultSignalMappings
 *
 * @param mapping The array of mappings
 * @param mappings The number of elements in the array
 */
extern void setSignalHandlers(const SignalMapping * mapping, int mappings);

/**
 * Retrieves the current signal mapping for the program (or thread)
 *
 * <p>
 * This function retrieves the current array of mappings between the signals to
 * be handled and the corresponding exception to be thrown.
 * </p>
 *
 * @see SignalMapping
 * @see SIGNAL_MAPPING
 * @see defaultSignalMapping
 * @see defaultSignalMappings
 *
 * @param mappings Will be set to the current number of mappings
 * @return The current array of mappings
 */
extern const SignalMapping * getSignalHandlers(int * mappings);

/*@}*/

/**
 * @name Convenience functions for printing out exception information
 *
 * <p>
 * These functions might come in handy when dealing with exceptions.
 * </p>
 *
 * @{
 */

/**
 * Prints a fatal error message regarding the specified exception
 *
 * <p>
 * This is a convenience function for showing an error message through the
 * standard error output. It can be passed to
 * <code>#beginExceptionContext</code> as the handler for uncaught exceptions.
 * </p>
 *
 * @param exception The uncaught exception
 * @param file The path of the source code file from which the exception was
 *        thrown
 * @param line The number of line from which the exception was thrown
 * @param errorNumber The value of errno at the time the exception was thrown
 */
extern void dumpException(Exception exception,
	const char * file, int line, int errorNumber);

/**
 * Prints an exception's hierarchy graph
 *
 * <p>
 * This is a convenience function for printing an exception's <em>hierarchy</em>
 * graph with simple <code>ASCII</code> characters.
 * </p>
 *
 * @param exception The exception whose hierarchy is to be printed
 */
extern void printExceptionHierarchy(Exception exception);

/*@}*/

/*
 * Next functions are undocumented on purpose, because they shouldn't be used
 * directly (but through the 'keyword' macros).
 */
extern E4C_JMP_BUF * e4c_first(e4c_Stage stage, const char * file, int line);
extern e4c_bool e4c_next();
extern e4c_bool e4c_hook(e4c_Stage stage, const Exception exception);
extern void e4c_throw(const Exception exception, const char * file, int line);

# undef E4C_READ_ONLY

# endif
