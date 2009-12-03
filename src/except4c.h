/*
	----------------------------------------------------------------
	except4c.h

	exceptions4c
	version 1.1
	An exception handling framework for C.

	Copyright (c) 2009 Guillermo Calvo

	This is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This software is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this software.  If not, see <http://www.gnu.org/licenses/>.

	Credits:
	This little library has been inspired by these previous works:

	"Exceptions in C"
	By Laurent Deniau
	http://ldeniau.home.cern.ch/ldeniau/html/exception/exception.html

	"Implementing Exceptions in C Programming language"
	By Adomas Paltanavicius
	http://adomas.org/excc/

	----------------------------------------------------------------
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


# ifdef _POSIX_C_SOURCE
# define	EXCEPT4C_SETJMP(address)			sigsetjmp(address)
# define	EXCEPT4C_LONGJMP(address, value)	siglongjmp(address, 1)
# define	EXCEPT4C_JMP_BUF					sigjmp_buf
# else
# define	EXCEPT4C_SETJMP(address)			setjmp(address)
# define	EXCEPT4C_LONGJMP(address)			longjmp(address, 1)
# define	EXCEPT4C_JMP_BUF					jmp_buf
# endif

# define	EXCEPT4C_FILE						(const char *)__FILE__
# define	EXCEPT4C_LINE						__LINE__
# else
# define	EXCEPT4C_FILE						(const char *)NULL
# define	EXCEPT4C_LINE						0
# endif

# define	throw(exception_)												\
				except4c_throwException(									\
					exception_,												\
					EXCEPT4C_FILE,											\
					EXCEPT4C_LINE											\
				)
# define	try																\
				EXCEPT4C_SETJMP(  *( except4c_firstStep() )  );				\
				while( except4c_nextStep() )								\
					if( except4c_isOkToBreak() )							\
						break;												\
					else if( except4c_isOkToTry() )

# define	catch(e)														\
				else if( except4c_isOkToCatch(e) )

# define	finally															\
				else if( except4c_isOkToFinalize() )

# define	EXCEPTION_LITERAL(name_, description_, super_)					\
				{															\
					name:			#name_,									\
					description:	description_,							\
					super:			&super_									\
				}

# define	DEFINE_EXCEPTION(name_, description_, super_)					\
				const Exception name_ = EXCEPTION_LITERAL(					\
					name_,													\
					description_,											\
					super_													\
				)

# define	SIGNAL_MAPPING(signalId_, exception_)                           \
				{															\
					signalId:		signalId_,								\
					exception:		&exception_,							\
				}

/**
 * This struct represents an exception in our system.
 */
typedef struct ExceptionStruct Exception;
struct ExceptionStruct{
	const char *			name;
	const char *			description;
	const Exception *		super;
};

/**
 * This type represents a map between a signal and an exception.
 */
typedef struct SignalMappingStruct SignalMapping;
struct SignalMappingStruct{
	int						signalId;
	const Exception *		exception;
};

extern SignalMapping defaultSignalMapping[];
extern int defaultSignalMappings;

/**
 * This "read-only" global variable holds the current exception.
 */
extern Exception exception;

/**
 * RuntimeException is the root of the exceptions pseudo-hierarchy.
 */
extern const Exception RuntimeException;

/**
 * NotEnoughMemoryException is a predefined exception.
 */
extern const Exception NotEnoughMemoryException;

/**
 * NullPointerException is a predefined exception.
 */
extern const Exception NullPointerException;

/*
 * Next exceptions replace the signals received by the process
 */

/*
 * SignalException is the root of all signal exceptions.
 */
extern const Exception SignalException;

/*
 * SignalAlarmException represents SIGALRM, the signal sent to a process when a
 * time limit has elapsed.
 */
extern const Exception SignalAlarmException;

/*
 * SignalChildException represents SIGCHLD, the signal sent to a process when a
 * child process terminates (ignored by default).
 */
extern const Exception SignalChildException;

/*
 * SignalTrapException represents SIGTRAP, the signal sent to a process when a
 * condition arises that a debugger has requested to be informed of.
 */
extern const Exception SignalTrapException;

/*
 * ErrorSignalException is the root of all error signal exceptions.
 */
extern const Exception ErrorSignalException;

/*
 * IllegalInstructionException represents SIGILL, the signal sent to a process
 * when it attempts to execute a malformed, unknown, or privileged instruction.
 */
extern const Exception IllegalInstructionException;

/*
 * ArithmeticException represents SIGFPE, the signal sent to a process when it
 * performs an erroneous arithmetic operation.
 */
extern const Exception ArithmeticException;

/*
 * BadPointerException represents SIGSEGV, the signal sent to a process when it
 * makes an invalid memory reference, or segmentation fault.
 */
extern const Exception BadPointerException;

/*
 * BrokenPipeException represents SIGPIPE, the signal sent to a process when it
 * attempts to write to a pipe without a process connected to the other end.
 */
extern const Exception BrokenPipeException;

/*
 * ControlSignalException is the root of all control signal exceptions.
 */
extern const Exception ControlSignalException;

/*
 * StopException represents SIGSTOP, the signal sent to a process to stop it for
 * later resumption (unblock-able).
 */
extern const Exception StopException;

/*
 * KillException represents SIGKILL, the signal sent to a process to cause it to
 * terminate immediately (unblock-able).
 */
extern const Exception KillException;

/*
 * HangUpException represents SIGHUP, the signal sent to a process when its
 * controlling terminal is closed.
 */
extern const Exception HangUpException;

/*
 * TerminationException represents SIGTERM, the signal sent to a process to
 * request its termination.
 */
extern const Exception TerminationException;

/*
 * AbortException represents SIGABRT, the signal sent by computer programs to
 * abort the process.
 */
extern const Exception AbortException;

/*
 * CPUTimeException represents SIGXCPU, the signal sent to a process when it has
 * used up the CPU for a duration that exceeds a certain predetermined
 * user-settable value.
 */
extern const Exception CPUTimeException;

/*
 * UserControlSignalException is the root of all control signal exceptions
 * caused by the user.
 */
extern const Exception UserControlSignalException;

/*
 * UserQuitException represents SIGQUIT, the signal sent to a process by its
 * controlling terminal when the user requests that the process dump core.
 */
 extern const Exception UserQuitException;

/*
 * UserInterruptionException represents SIGINT, the signal sent to a process by
 * its controlling terminal when a user wishes to interrupt the process.
 */
 extern const Exception UserInterruptionException;

/*
 * UserBreakException represents SIGBREAK, the signal sent to a process by its
 * controlling terminal when a user wishes to break the process.
 */
 extern const Exception UserBreakException;

/*
 * ProgramSignalException is the root of all error user-defined signal
 * exceptions.
 */
 extern const Exception ProgramSignalException;

/*
 * ProgramSignal1Exception represents SIGUSR1, the signal sent to a process to
 * indicate user-defined conditions.
 */
 extern const Exception ProgramSignal1Exception;

/*
 * ProgramSignal2Exception represents SIGUSR2, the signal sent to a process to
 * indicate user-defined conditions.
 */
 extern const Exception ProgramSignal2Exception;

/*
 * This tree structure represents the hierarchy of exceptions used to represent
 * signals.
 *
 * SignalException
 *   |
 *   +-- SignalAlarmException
 *   |
 *   +-- SignalChildException
 *   |
 *   +-- SignalTrapException
 *   |
 *   +-- ErrorSignalException
 *   |     |
 *   |     +-- IllegalInstructionException
 *   |     |
 *   |     +-- BadPointerException
 *   |     |
 *   |     +-- ArithmeticException
 *   |     |
 *   |     +-- BrokenPipeException
 *   |
 *   +-- ControlSignalException
 *   |     |
 *   |     +-- StopException
 *   |     |
 *   |     +-- KillException
 *   |     |
 *   |     +-- HangUpException
 *   |     |
 *   |     +-- TerminationException
 *   |     |
 *   |     +-- AbortException
 *   |     |
 *   |     +-- CPUTimeException
 *   |     |
 *   |     +-- UserControlSignalException
 *   |           |
 *   |           +-- UserQuitException
 *   |           |
 *   |           +-- UserInterruptionException
 *   |           |
 *   |           +-- UserBreakException
 *   |
 *   +-- ProgramSignalException
 *         |
 *         +-- ProgramSignal1Exception
 *         |
 *         +-- ProgramSignal2Exception
 *
 */


/**
 * Convenience function to create a new exception.
 */
extern const Exception newException(
	const char * name, const char * description, const Exception * super);

/**
 * Convenience function to print an error message at uncaught exception.
 * (should be registered by calling atexit)
 */
extern void atUncaughtException();

/**
 * Convenience function to print an exception's hierarchy.
 */
extern void printExceptionHierarchy(Exception exception);

extern void setSignalHandlers(SignalMapping * mapping, int mappings);
extern void initializeExceptionHandling(bool handleSignals);

/**
 * Next functions shouldn't be called directly (but through the macros).
 */
extern void except4c_throwException(
	const Exception exception_, const char * file, int line);
extern EXCEPT4C_JMP_BUF * except4c_firstStep();
extern bool except4c_nextStep();
extern bool except4c_isOkToTry();
extern bool except4c_isOkToCatch(const Exception _exception);
extern bool except4c_isOkToFinalize();
extern bool except4c_isOkToBreak();


# endif
