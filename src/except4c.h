/**
 *
 * @file except4c.h
 *
 * <code>exceptions4c</code> header file
 *
 * @version 1.2
 * @author Copyright (c) 2009 Guillermo Calvo
 *
 * @section e4c_h <code>exceptions4c</code> header file
 *
 * <p>
 * This header file needs to be included from other parts of the program in
 * order to be able to use any of the keywords of the the exception handling
 * system:
 * <ul>
 * <li><code>try</code></li>
 * <li><code>catch</code></li>
 * <li><code>finally</code></li>
 * <li><code>throw</code></li>
 * </ul>
 * </p>
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


# include <stdlib.h>
# include <setjmp.h>
# if __STDC_VERSION__ >= 199901L
# include <stdbool.h>
# else
# define	bool								unsigned char
# define	false								0
# define	true								1
# endif

/*
 * Next macros are undocumented on purpose, because they shouldn't be used
 * directly (but through the macros: try, catch, finalize and throw).
 */

# ifdef _POSIX_C_SOURCE
# define	EXCEPT4C_SETJMP(address)			sigsetjmp(address)
# define	EXCEPT4C_LONGJMP(address, value)	siglongjmp(address, 1)
# define	EXCEPT4C_JMP_BUF					sigjmp_buf
# else
# define	EXCEPT4C_SETJMP(address)			setjmp(address)
# define	EXCEPT4C_LONGJMP(address)			longjmp(address, 1)
# define	EXCEPT4C_JMP_BUF					jmp_buf
# endif

# ifdef DEBUG
# define	EXCEPT4C_FILE						(const char *)__FILE__
# define	EXCEPT4C_LINE						__LINE__
# else
# define	EXCEPT4C_FILE						(const char *)NULL
# define	EXCEPT4C_LINE						0
# endif

/** @name Exception handling keywords
 *
 * <p>
 * This library provides you a simple set of keywords which map the semantics of
 * exception handling you're probably already used to.
 * </p>
 *
 * @{
 */

/**
 * Announce a block of code aware of exceptions
 */
# define	try																\
				EXCEPT4C_SETJMP(  *( except4c_firstStep() )  );				\
				while( e4c_nextStep() )										\
					if( e4c_isOkToBreak() )									\
						break;												\
					else if( e4c_isOkToTry() )

/**
 * Announce a block of code capable of handling some kind of exceptions
 *
 * @param _exception_ The kind of exceptions to be handled
 */
# define	catch(_exception_)												\
				else if(													\
					e4c_isOkToCatch(_exception_)							\
				)

/**
 * Announce a block of code responsible for cleaning up a <code>try</code> block
 */
# define	finally															\
				else if(													\
					e4c_isOkToFinalize()									\
				)

/**
 * Signal an exceptional situation
 *
 * @param _exception_ The exception to be thrown
 */
# define	throw(_exception_)												\
				e4c_throwException(											\
					_exception_,											\
					EXCEPT4C_FILE,											\
					EXCEPT4C_LINE											\
				)

/*@}*/

/**
 * @name Convenience macros
 *
 * <p>
 * These macros let you represent and define ::Exception and ::SignalMapping
 * literals and variables.
 * </p>
 *
 * @{
 */

/**
 * Define a new const exception
 *
 * @param _name_ Name of the new exception
 * @param _description_ Description of the new exception
 * @param _super_ Supertype of the new exception
 */
# define	DEFINE_EXCEPTION(_name_, _description_, _super_)				\
				const Exception _name_ = EXCEPTION_LITERAL(					\
					_name_,													\
					_description_,											\
					&_super_												\
				)

/**
 * Convenience macro for representing an exception literal
 *
 * @param _name_ Name of the exception literal
 * @param _description_ Description of the exception literal
 * @param _super_ Address of the supertype of the exception literal
 */
# define	EXCEPTION_LITERAL(_name_, _description_, _super_)				\
				{															\
					name:			#_name_,								\
					description:	_description_,							\
					super:			_super_									\
				}

/**
 * Convenience macro for representing a mapping literal
 *
 * @param _signal_ Signal to be converted
 * @param _exception_ Exception representing the signal
 */
# define	SIGNAL_MAPPING(_signal_, _exception_)							\
				{															\
					signal:			_signal_,								\
					exception:		&_exception_,							\
				}

/*@}*/

typedef struct Exception Exception;
/**
 *
 * This type represents an exception in the exception handling system.
 *
 * <p>
 * Exceptions are objects with a <code>#name</code>, a <code>#description</code>
 * and a <code>#super-type</code>.
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
 * a set of global predefined exceptions built into the framework, amongst
 * others:
 * </p>
 *
 * <ul>
 * <li>#RuntimeException</li>
 * <li>#NotEnoughMemoryException</li>
 * <li>#NullPointerException</li>
 * <li>#AbortException</li>
 * <li>#ArithmeticException</li>
 * <li>#IllegalInstructionException</li>
 * <li>#BadPointerException</li>
 * <li>#TerminationException</li>
 * <li>#UserInterruptionException</li>
 * </ul>
 *
 * <p>
 * <code>#RuntimeException</code> is the root of the exceptions
 * <em>pseudo-hierarchy</em>.
 * </p>
 *
 */
struct Exception{

	/** The exception's name */
	const char *			name;

	/** The exception's description */
	const char *			description;

	/** The exception's supertype */
	const Exception *		super;
};

/**
 * This type represents a map between a signal and an exception.
 *
 * <p>
 * <strong>exceptions4c</strong> can make your life easier by converting each
 * signals to a ::Exception. Most of these signals would crash your program as
 * soon as they were raised. Now you can <code>#catch</code> signals and avoid
 * core dumps.
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
 * extern void initializeExceptionHandling(bool handleSignals);</pre>
 *
 * <p>
 * <code>initializeExceptionHandling</code> will set up for you the default
 * handlers for the available signals in the platform if you pass
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
 *     /<span>* ... *</span>/
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
 */
typedef struct SignalMapping SignalMapping;
struct SignalMapping{

	/** The signal to be converted */
	int						signal;

	/** The exception representing the signal */
	const Exception *		exception;
};

/** @name Predefined pseudo-variable
 *
 * <p>
 * The current exception being thrown can be accessed through the global
 * <em>pseudo-variable</em> <code>#exception</code>. This global variable is not
 * <code>const</code>, but you should treat it as <em>read-only</em>.
 * </p>
 *
 * <p>
 * <code>exception</code> must only be accessed from within <code>#catch</code>
 * or <code>#finally</code> blocks.
 * </p>
 *
 * @see Exception
 *
 * @{
 */

/**
 * The current thrown exception.
 */
extern Exception exception;

/*@}*/

/** @name Predefined mappings
 *
 * <p>
 * These are the default settings for handling the available signals in the
 * platform.
 * </p>
 *
 * @see SignalMapping
 *
 * @{
 */

/**
 * The default array of mappings between the signals to be handled and the
 * corresponding exception to be thrown
 */
const extern SignalMapping defaultSignalMapping[];

/**
 * The number of elements in the array
 */
const extern int defaultSignalMappings;

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
 * This exception is thrown when when an unexpected null pointer is found.
 *
 * NullPointerException is thrown when when some part of the program gets a
 * pointer which was expected or required to contain a valid memory address.
 */
extern const Exception NullPointerException;

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
 * Create a new exception
 *
 * This is a convenience function to create a new exception.
 *
 * @param name Name of the new exception
 * @param description Description of the new exception
 * @param super Address of the supertype of the new exception
 */
extern const Exception newException(
	const char * name, const char * description, const Exception * super);

/**
 * Print an error message at uncaught exception
 *
 * This is a convenience function to print an error message when the program
 * ends with an uncaught exception.
 *
 * It should be registered by calling <code>atexit</code>.
 */
extern void atUncaughtException();

/**
 * Print an exception's hierarchy graph
 *
 * This is a convenience function to print an exception's <em>hierarchy</em>
 * graph with simple <code>ASCII</code> characters.
 *
 * @param _exception_ The exception whose hierarchy is to be printed
 */
extern void printExceptionHierarchy(Exception _exception_);

/**
 * Set up the signal handling system
 *
 * This function receives an array of mappings between the signals to be handled
 * and the corresponding exception to be thrown.
 *
 * @param mapping The array of mappings
 * @param mappings The number of elements in the array
 */
extern void setSignalHandlers(const SignalMapping * mapping, int mappings);

/**
 * Initialize the exception handling system
 *
 * This function registers <code>atUncaughtException</code> to be executed when
 * the program ends (by calling <code>atexit</code>).
 *
 * In addition, it sets up the default mapping to convert signals into
 * exceptions, if <code>handleSignals</code> is <code>true</code>.
 *
 * @param handleSignals If <code>true</code>, the signal handling system will be
 * set up with the default mapping.
 */
extern void initializeExceptionHandling(bool handleSignals);

/*
 * Next functions are undocumented on purpose, because they shouldn't be called
 * directly (but through the macros: try, catch, finalize and throw).
 */

extern void e4c_throwException(
	const Exception _exception_, const char * file, int line);
extern EXCEPT4C_JMP_BUF * except4c_firstStep();
extern bool e4c_nextStep();
extern bool e4c_isOkToTry();
extern bool e4c_isOkToCatch(const Exception _exception_);
extern bool e4c_isOkToFinalize();
extern bool e4c_isOkToBreak();


# endif
