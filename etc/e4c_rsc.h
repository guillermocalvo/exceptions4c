/**
 *
 * @file        e4c_rsc.h
 *
 * exceptions4c automatic resource handling header file
 *
 * @version     1.1
 * @author      Copyright (c) 2012 Guillermo Calvo
 *
 * @section e4c_rsc_h exceptions4c automatic resource handling header file
 *
 * This header file needs to be included in order to be able to use any of the
 * automatic resource handling macros:
 *
 * @li @c #e4c_using_memory
 * @li @c #e4c_using_file
 * @li @c #e4c_using_mutex
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
# define e4c_dispose_memory(_buffer_, _failed_) free(_buffer_)

/*
 * Binds the acquisition of file to the standard function fopen
 */
# define e4c_acquire_file fopen

/*
 * Binds the disposal of file to the standard function fclose
 */
# define e4c_dispose_file(_file_, _failed_) \
	if( fclose(_file_) != 0 ) \
		throw(FileCloseError, "Could not close file: " #_file_)

/*
 * Binds the acquisition of mutex to the function pthread_mutex_lock
 */
# define e4c_acquire_mutex pthread_mutex_lock

/*
 * Binds the disposal of mutex to the function pthread_mutex_unlock
 */
# define e4c_dispose_mutex(_mutex_, _failed_) \
	if( pthread_mutex_unlock(_mutex_) != 0 ) \
		throw(MutexUnlockError, "Could not unlock mutex: " #_mutex_)


/**
 * @name Convenience macros for acquiring and disposing resources
 *
 * These macros let you acquire and dispose different kinds of resources
 * according to the <em>dispose pattern</em>.
 *
 * @{
 */

/**
 * Introduces a block of code with automatic acquisition and disposal of a
 * memory buffer
 *
 * @param   _buffer_
 *          The buffer to be acquired, used and then disposed
 * @param   _bytes_
 *          The amount of memory to be allocated (in bytes)
 *
 * This macro lets you acquire and dispose memory buffers according to the
 * <em>dispose pattern</em>:
 *
 * @code
 * void * buffer;
 *
 * e4c_using_memory(buffer, 1024){
 *     // implicit: buffer = malloc(1024);
 *     memset(buffer, 0, 1024);
 *     // implicit: free(buffer);
 * }
 * @endcode
 *
 * @throws  MemoryAllocationException
 *          If @c malloc returns @c NULL
 *
 */
# define e4c_using_memory(_buffer_, _bytes_) \
	E4C_WITH(_buffer_, e4c_dispose_memory){ \
		_buffer_ = e4c_acquire_memory(_bytes_); \
		if(_buffer_ == NULL){ \
			E4C_THROW(MemoryAllocationException, \
			"Could not allocate memory for: " #_buffer_); \
		} \
	}E4C_USE

/**
 * Introduces a block of code with automatic acquisition and disposal of a
 * file stream
 *
 * @param   _file_
 *          The file to be acquired, used and then disposed
 * @param   _path_
 *          The path of the file to be opened
 * @param   _mode_
 *          The access mode for the file
 *
 * This macro lets you acquire and dispose (open and close) files according to
 * the <em>dispose pattern</em>:
 *
 * @code
 * FILE * file;
 *
 * e4c_using_file(file, "log.txt", "a"){
 *     // implicit: file = fopen("log.txt", "a");
 *     fputs("hello, world!\n", file);
 *     // implicit: fclose(file);
 * }
 * @endcode
 *
 * @throws  FileOpenException
 *          If @c fopen returns @c NULL
 * @throws  FileCloseException
 *          If @c fclose does not return @c zero
 *
 */
# define e4c_using_file(_file_, _path_, _mode_) \
	E4C_WITH(_file_, e4c_dispose_file){ \
		_file_ = e4c_acquire_file(_path_, _mode_); \
		if(_file_ == NULL){ \
			E4C_THROW(FileOpenException, "Could not open file: " #_path_); \
		} \
	}E4C_USE

/**
 * Introduces a block of code with automatic acquisition and disposal of a mutex
 *
 * @param   _mutex_
 *          The mutex to be locked, used and then unlocked
 *
 * This macro lets you lock and unlock mutexes according to the
 * <em>dispose pattern</em>:
 *
 * @code
 * pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 * int counter = 0;
 *
 * e4c_using_mutex(&mutex){
 *     // implicit: pthread_mutex_lock(&mutex);
 *     counter++;
 *     // implicit: pthread_mutex_unlock(&mutex);
 * }
 * @endcode
 *
 * @throws  MutexLockException
 *          If @c pthread_mutex_lock does not return @c zero
 * @throws  MutexUnlockException
 *          If @c pthread_mutex_unlock does not return @c zero
 */
# define e4c_using_mutex(_mutex_) \
	E4C_WITH(_mutex_, e4c_dispose_mutex){ \
		int result = e4c_acquire_mutex(_mutex_); \
		if(result != 0){ \
			E4C_THROW(MutexLockException, "Could not lock mutex: " #_mutex_); \
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
 * @c MemoryAllocationException is thrown by @c #e4c_using_memory when @c malloc
 * returns @c NULL for whatever reason.
 *
 * @par     Extends:
 *          #NotEnoughMemoryException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(MemoryAllocationException);

/**
 * This exception is thrown when a file error occurs
 *
 * @c FileException is the general type of exceptions produced by failed file
 * operations.
 *
 * @note
 * The specific cause of the error can be determined by checking the
 * @c error_number of the exception; it captures the value of @c errno at the
 * time the exception was thrown (i. e. right after @c fopen or @c fclose).
 *
 * @par     Extends:
 *          #InputOutputException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(FileException);

/**
 * This exception is thrown when a file cannot be opened
 *
 * @c FileOpenException is thrown by @c #e4c_using_file when @c fopen returns
 * @c NULL for whatever reason.
 *
 * @par     Extends:
 *          FileException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(FileOpenException);

/**
 * This exception is thrown when a file cannot be closed
 *
 * @c FileCloseException is thrown by @c #e4c_using_file when @c fclose does not
 * return @c zero for whatever reason.
 *
 * @par     Extends:
 *          FileException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(FileCloseException);

/**
 * This exception is thrown when a mutex error occurs
 *
 * @c MutexException is the general type of exceptions produced by failed mutex
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
 * @c MutexLockException is thrown by @c #e4c_using_mutex when @c fopen returns
 * @c NULL for whatever reason.
 *
 * @par     Extends:
 *          MutexException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(MutexLockException);

/**
 * This exception is thrown when a mutex cannot be unlocked
 *
 * @c MutexUnlockException is thrown by @c #e4c_using_mutex when @c fclose does
 * not return @c zero for whatever reason.
 *
 * @par     Extends:
 *          MutexException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(MutexUnlockException);

/** @} */


/*@=exportany@*/


# endif
