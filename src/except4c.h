/*
	----------------------------------------------------------------
	except4c.h

	exceptions4c
	version 1.0
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

# ifdef		DEBUG
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

/**
 * Convenience function to create a new exception.
 */
extern const Exception newException(const char * name, const char * description, const Exception * super);

/**
 * Convenience function to print an error message at uncaught exception.
 * (should be registered by calling atexit)
 */
extern void atUncaughtException();

/**
 * Convenience function to print an exception's hierarchy.
 */
extern void printExceptionHierarchy(Exception exception);

/**
 * Next functions shouldn't be called directly (but through the macros).
 */
extern void except4c_throwException(const Exception exception_, const char * file, int line);
extern EXCEPT4C_JMP_BUF * except4c_firstStep();
extern bool except4c_nextStep();
extern bool except4c_isOkToTry();
extern bool except4c_isOkToCatch(const Exception _exception);
extern bool except4c_isOkToFinalize();
extern bool except4c_isOkToBreak();


# endif
