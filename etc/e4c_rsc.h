/**
 *
 * @file        e4c_rsc.h
 *
 * exceptions4c automatic resource handling header file
 *
 * @version     2.1
 * @author      Copyright (c) 2012 Guillermo Calvo
 *
 * @section e4c_rsc_h exceptions4c automatic resource handling header file
 *
 * This header file needs to be included in order to be able to use any of the
 * automatic resource handling macros:
 *
 *   - `#e4c_using_memory`
 *   - `#e4c_using_file`
 *   - `#e4c_using_mutex`
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


# ifndef EXCEPTIONS4C_RESOURCES
# define EXCEPTIONS4C_RESOURCES


# ifndef EXCEPTIONS4C
#	include "e4c.h"
# endif


/*@-exportany@*/


/*
 * Binds the acquisition of memory to the standard function malloc
 */
# define e4c_acquire_memory malloc

/*
 * Binds the disposal of memory to the standard function malloc
 */
# define e4c_dispose_memory(buffer, failed) free(buffer)

/*
 * Binds the acquisition of file to the standard function fopen
 */
# define e4c_acquire_file fopen

/*
 * Binds the disposal of file to the standard function fclose
 */
# define e4c_dispose_file(file, failed) \
	if( fclose(file) != 0 ) \
		throw(FileCloseError, "Could not close file: " #file)

/*
 * Binds the acquisition of mutex to the function pthread_mutex_lock
 */
# define e4c_acquire_mutex pthread_mutex_lock

/*
 * Binds the disposal of mutex to the function pthread_mutex_unlock
 */
# define e4c_dispose_mutex(mutex, failed) \
	if( pthread_mutex_unlock(mutex) != 0 ) \
		throw(MutexUnlockError, "Could not unlock mutex: " #mutex)


/**
 * @name Convenience macros for acquiring and disposing resources
 *
 * These macros let you acquire and dispose different kinds of resources
 * according to the *dispose pattern*.
 *
 * @{
 */

/**
 * Introduces a block of code with automatic acquisition and disposal of a
 * memory buffer
 *
 * @param   buffer
 *          The buffer to be acquired, used and then disposed
 * @param   bytes
 *          The amount of memory to be allocated (in bytes)
 *
 * This macro lets you acquire and dispose memory buffers according to the
 * *dispose pattern*:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
 *   void * buffer;
 *
 *   e4c_using_memory(buffer, 1024){
 *       // implicit: buffer = malloc(1024);
 *       memset(buffer, 0, 1024);
 *       // implicit: free(buffer);
 *   }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @throws  MemoryAllocationException
 *          If `malloc` returns `NULL`
 *
 */
# define e4c_using_memory(buffer, bytes) \
	E4C_WITH(buffer, e4c_dispose_memory){ \
		buffer = e4c_acquire_memory(bytes); \
		if(buffer == NULL){ \
			E4C_THROW(MemoryAllocationException, \
			"Could not allocate memory for: " #buffer); \
		} \
	}E4C_USE

/**
 * Introduces a block of code with automatic acquisition and disposal of a
 * file stream
 *
 * @param   file
 *          The file to be acquired, used and then disposed
 * @param   path
 *          The path of the file to be opened
 * @param   mode
 *          The access mode for the file
 *
 * This macro lets you acquire and dispose (open and close) files according to
 * the *dispose pattern*:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
 *   FILE * file;
 *
 *   e4c_using_file(file, "log.txt", "a"){
 *       // implicit: file = fopen("log.txt", "a");
 *       fputs("hello, world!\n", file);
 *       // implicit: fclose(file);
 *   }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @throws  FileOpenException
 *          If `fopen` returns `NULL`
 * @throws  FileCloseException
 *          If `fclose` does not return *zero*
 *
 */
# define e4c_using_file(file, path, mode) \
	E4C_WITH(file, e4c_dispose_file){ \
		file = e4c_acquire_file(path, mode); \
		if(file == NULL){ \
			E4C_THROW(FileOpenException, "Could not open file: " #path); \
		} \
	}E4C_USE

/**
 * Introduces a block of code with automatic acquisition and disposal of a mutex
 *
 * @param   mutex
 *          The mutex to be locked, used and then unlocked
 *
 * This macro lets you lock and unlock mutexes according to the *dispose
 * pattern*:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
 *   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 *   int counter = 0;
 *
 *   e4c_using_mutex(&mutex){
 *       // implicit: pthread_mutex_lock(&mutex);
 *       counter++;
 *       // implicit: pthread_mutex_unlock(&mutex);
 *   }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @throws  MutexLockException
 *          If `pthread_mutex_lock` does not return *zero*
 * @throws  MutexUnlockException
 *          If `pthread_mutex_unlock` does not return *zero*
 */
# define e4c_using_mutex(mutex) \
	E4C_WITH(mutex, e4c_dispose_mutex){ \
		int result = e4c_acquire_mutex(mutex); \
		if(result != 0){ \
			E4C_THROW(MutexLockException, "Could not lock mutex: " #mutex); \
		} \
	}E4C_USE

/** @} */

/**
 * @name Resource handling exceptions
 *
 * @{
 */

/**
 * This exception is thrown when malloc returns a null pointer
 *
 * `MemoryAllocationException` is thrown by `#e4c_using_memory` when `malloc`
 * returns `NULL` for whatever reason.
 *
 * @par     Extends:
 *          #NotEnoughMemoryException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(MemoryAllocationException);

/**
 * This exception is thrown when a file error occurs
 *
 * `FileException` is the general type of exceptions produced by failed file
 * operations.
 *
 * @note
 * The specific cause of the error can be determined by checking the
 * `error_number` of the exception; it captures the value of `errno` at the
 * time the exception was thrown (i. e. right after `fopen` or `fclose`).
 *
 * @par     Extends:
 *          #InputOutputException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(FileException);

/**
 * This exception is thrown when a file cannot be opened
 *
 * `FileOpenException` is thrown by `#e4c_using_file` when `fopen` returns
 * `NULL` for whatever reason.
 *
 * @par     Extends:
 *          FileException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(FileOpenException);

/**
 * This exception is thrown when a file cannot be closed
 *
 * `FileCloseException` is thrown by `#e4c_using_file` when `fclose` does not
 * return *zero* for whatever reason.
 *
 * @par     Extends:
 *          FileException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(FileCloseException);

/**
 * This exception is thrown when a mutex error occurs
 *
 * `MutexException` is the general type of exceptions produced by failed mutex
 * operations.
 *
 * @par     Extends:
 *          #RuntimeException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(MutexException);

/**
 * This exception is thrown when a mutex cannot be locked
 *
 * `MutexLockException` is thrown by `#e4c_using_mutex` when `fopen` returns
 * `NULL` for whatever reason.
 *
 * @par     Extends:
 *          MutexException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(MutexLockException);

/**
 * This exception is thrown when a mutex cannot be unlocked
 *
 * `MutexUnlockException` is thrown by `#e4c_using_mutex` when `fclose` does
 * not return *zero* for whatever reason.
 *
 * @par     Extends:
 *          MutexException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(MutexUnlockException);

/** @} */


/*@=exportany@*/


# endif
