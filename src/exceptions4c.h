/*
 * Copyright 2025 Guillermo Calvo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * An exception handling library for C.
 *
 * <a href="https://exceptions4c.guillermo.dev/">
 * <img src="exceptions4c.svg">
 * </a>
 *
 * This library consists of two files:
 * - exceptions4c.h
 * - exceptions4c.c
 *
 * To use it in your project, include the header file in your
 * source code files.
 *
 * ```c
 * #include <exceptions4c.h>
 * ```
 *
 * And then link your program against the library code.
 *
 * @file        exceptions4c.h
 * @version     4.0.0
 * @author      [Guillermo Calvo]
 * @copyright   Licensed under [Apache 2.0]
 * @see         For more information, visit the [project on GitHub]
 *
 * [Guillermo Calvo]: https://guillermo.dev
 * [Apache 2.0]: http://www.apache.org/licenses/LICENSE-2.0
 * [project on GitHub]: https://github.com/guillermocalvo/exceptions4c
 */

#ifndef EXCEPTIONS4C

/**
 * Returns the major version number of this library.
 */
#define EXCEPTIONS4C 4

#include <stdlib.h>
#include <setjmp.h>

#ifndef __bool_true_false_are_defined
#include <stdbool.h>
#endif

/**
 * Introduces a block of code that may throw exceptions during execution.
 *
 * The #TRY block is used to define a section of code where exceptions MAY
 * occur. It allows you to handle exceptions gracefully using other blocks that
 * follow it. If an exception occurs, control is transferred to the appropriate
 * block.
 *
 * A single #TRY block MAY be followed by:
 *
 * 1. One or more #CATCH blocks to handle specific types of exceptions.
 * 2. Optionally, one #CATCH_ALL block to handle all exception types (if
 *    present, it MUST appear after all #CATCH blocks).
 * 3. Optionally, one #FINALLY block to execute cleanup code, regardless of
 *    whether an exception was thrown or caught.
 *
 * @attention
 * These blocks MUST NOT be exited through any of: <tt>goto</tt>,
 * <tt>break</tt>, <tt>continue</tt>, or <tt>return</tt>.
 *
 * @important
 * Local variables in the function containing the #TRY block MUST be
 * <tt>volatile</tt> because these blocks invoke <tt>setjump</tt> and their
 * values would be indeterminate if they had been changed since the invocation.
 *
 * Example: @snippet pet-store.c try
 *
 * @see CATCH
 * @see CATCH_ALL
 * @see FINALLY
 */
#define TRY                                                                 \
                                                                            \
  EXCEPTIONS4C_START_BLOCK(false)                                           \
  if (e4c_try(EXCEPTIONS4C_DEBUG))

/**
 * Introduces a block of code that handles exceptions thrown by a preceding #TRY
 * block.
 *
 * Use this macro to to handle a specific type of exceptions when they occur.
 *
 * If <tt>type</tt> is equal to (or a [supertype](#e4c_exception_type.supertype)
 * of) the type of the [thrown exception](#e4c_get_exception), then this block
 * will be used to handle it.
 *
 * One or more #CATCH blocks MAY follow a #TRY block. If <tt>type</tt> doesn't
 * match the thrown exception, then this block will be ignored, and the
 * exception MAY be caught by the following #CATCH or #CATCH_ALL blocks.
 *
 * @important
 * When looking for a match, #CATCH blocks are inspected in the order they
 * appear. If you place a generic handler before a more specific one, the second
 * block will be unreachable.
 *
 * @attention
 * These blocks MUST NOT be exited through any of: <tt>goto</tt>,
 * <tt>break</tt>, <tt>continue</tt>, or <tt>return</tt>.
 *
 * @param type The type of exception to catch.
 *
 * Example: @snippet pet-store.c catch
 *
 * @see TRY
 * @see CATCH_ALL
 *
 */
#define CATCH(type)                                                         \
                                                                            \
  else if (e4c_catch(&type, EXCEPTIONS4C_DEBUG))

/**
 * Introduces a block of code that handles any exception thrown by a preceding
 * #TRY block, regardless of its type.
 *
 * The #CATCH_ALL block works like a general #CATCH block that does not require
 * specifying the type of exception to handle. It MAY be used as a fallback for
 * catching all exceptions, including those not explicitly declared in other
 * #CATCH blocks.
 *
 * Only one #CATCH_ALL block is allowed per #TRY block, and it MUST appear after
 * all type-specific #CATCH blocks if any are present.
 *
 * @remark
 * Using a #CATCH_ALL block is useful for logging, debugging, or handling
 * unexpected exceptions that don't fit into specific categories. However,
 * specific #CATCH blocks SHOULD be used whenever possible to maintain clarity
 * and precise control over exception handling.
 *
 * @attention
 * These blocks MUST NOT be exited through any of: <tt>goto</tt>,
 * <tt>break</tt>, <tt>continue</tt>, or <tt>return</tt>.
 *
 * Example: @snippet pet-store.c catch_all
 *
 * @see TRY
 * @see CATCH
 */
#define CATCH_ALL                                                           \
                                                                            \
  else if (e4c_catch(NULL, EXCEPTIONS4C_DEBUG))

/**
 * Introduces a block of code that is executed after a #TRY block, regardless of
 * whether an exception was thrown or not.
 *
 * Use this macro to run a block, no matter whether an exception happens or not.
 *
 * A #FINALLY block MUST be preceded by a single #TRY, #USING, or #WITH block,
 * after all the accompanying #CATCH and #CATCH_ALL blocks (if any). Only one
 * #FINALLY block is allowed per block.
 *
 * @attention
 * These blocks MUST NOT be exited through any of: <tt>goto</tt>,
 * <tt>break</tt>, <tt>continue</tt>, or <tt>return</tt>.
 *
 * @remark
 * This macro SHOULD be used to release resources, close files, or perform
 * cleanup tasks.
 *
 * Example: @snippet pet-store.c finally
 *
 * @see TRY
 */
#define FINALLY                                                             \
                                                                            \
  else if (e4c_finally(EXCEPTIONS4C_DEBUG))

/**
 * Throws an exception, interrupting the normal flow of execution.
 *
 * #THROW is used within a #TRY block, a #CATCH block, or any other function to
 * signal that an error has occurred. The thrown exception will be of the
 * specified <tt>type</tt>, and it MAY be handled by a preceding #CATCH block.
 *
 * If a thrown exception is not handled by any of the #CATCH blocks in the
 * current function, it propagates up the call stack to the function that called
 * the current function. This continues until the exception is either handled by
 * a #CATCH block higher in the stack, or it reaches the top level of the
 * program. If no #CATCH block handles the exception, the program terminates and
 * an error message is printed to the console.
 *
 * @remark
 * The error message will be formatted just as it would with <tt>printf</tt>. If
 * no message is specified, then the default message for the supplied
 * <tt>type</tt> will be used.
 *
 * @important
 * Control never returns to the #THROW point.
 *
 * Example: @snippet pet-store.c throw
 *
 * @param type The type of the exception to throw.
 * @param format The error message.
 * @param ... An optional list of arguments that will be formatted according to
 *   <tt>format</tt>.
 *
 * @see CATCH
 * @see CATCH_ALL
 */
#define THROW(type, format, ...)                                            \
                                                                            \
  EXCEPTIONS4C_LONG_JUMP(e4c_throw(                                         \
      &type, #type, EXCEPTIONS4C_DEBUG, (format) __VA_OPT__(,) __VA_ARGS__))

/**
 * Repeats the previous #TRY or #USE block entirely.
 *
 * This macro restarts the previous #TRY or #USE block up to a specified
 * maximum number of attempts, or throws an exception if it has already been
 * restarted too many times.
 *
 * @remark
 * This macro SHOULD be used in the body of a #CATCH or #CATCH_ALL block as a
 * quick way to fix an error condition and try something again. It MAY also be
 * used in the body of a #FINALLY block provided that there is an
 * [uncaught exception](#e4c_is_uncaught) about to propagate up the call stack.
 *
 * @important
 * Control never returns to the #RETRY point.
 *
 * Example: @snippet pet-store.c retry
 *
 * @param max The maximum number of attempts.
 * @param type The type of exception to throw if <tt>max</tt> is exceeded.
 * @param format The error message.
 * @param ... An optional list of arguments that will be formatted according to
 *   <tt>format</tt>.
 *
 * @see REACQUIRE
 * @see TRY
 * @see USE
 * @see THROW
 * @see e4c_is_uncaught
 */
#define RETRY(max, type, format, ...)                                       \
                                                                            \
  EXCEPTIONS4C_LONG_JUMP(e4c_restart(false, (max),                          \
      &type, #type, EXCEPTIONS4C_DEBUG, (format) __VA_OPT__(,) __VA_ARGS__))

/**
 * Introduces a block of code with automatic resource acquisition and disposal.
 *
 * Use this macro to easily acquire, use, and dispose of a resource.
 *
 * A #USING block is similar to a for statement, because it receives three
 * comma-separated expressions that will be evaluated in order:
 * <tt>acquisition</tt>, <tt>test</tt>, and <tt>disposal</tt>.
 *
 * Both these expressions and the code block that uses the resource MAY throw
 * exceptions.
 *
 * A single #USING block MAY be followed by one or more #CATCH, an optional
 * #CATCH_ALL block, and an optional #FINALLY (in this order), just like #TRY
 * blocks.
 *
 * @attention
 * These blocks MUST NOT be exited through any of: <tt>goto</tt>,
 * <tt>break</tt>, <tt>continue</tt>, or <tt>return</tt>.
 *
 * @important
 * Local variables in the function containing the #USING block MUST be
 * <tt>volatile</tt> because these blocks invoke <tt>setjump</tt> and their
 * values would be indeterminate if they had been changed since the invocation.
 *
 * Example: @snippet pet-store.c using_catch
 *
 * @param acquisition The expression to acquire the resource.
 * @param test The condition for executing the code block.
 * @param disposal The expression to dispose of the resource.
 *
 * @see WITH
 * @see USE
 */
#define USING(acquisition, test, disposal)                                  \
                                                                            \
  WITH (disposal) {                                                         \
    (void) (acquisition);                                                   \
  } USE (test)

/**
 * Opens a block of code with automatic resource disposal.
 *
 * You MAY use this macro instead of #USING when the steps to acquire a resource
 * are more complex than simply evaluating an expression.
 *
 * A #WITH block MUST be followed by one #USE block.
 *
 * Any of these blocks MAY throw exceptions.
 *
 * @note
 * The #WITH... #USE combination works exactly the same as the #USING block,
 * except that the #WITH block is responsible for the resource acquisition, and
 * the #USE block manipulates the resource.
 *
 * @attention
 * These blocks MUST NOT be exited through any of: <tt>goto</tt>,
 * <tt>break</tt>, <tt>continue</tt>, or <tt>return</tt>.
 *
 * @important
 * Local variables in the function containing the #WITH block MUST be
 * <tt>volatile</tt> because these blocks invoke <tt>setjump</tt> and their
 * values would be indeterminate if they had been changed since the invocation.
 *
 * Example: @snippet pet-store.c with_use
 *
 * @param disposal The expression to dispose of the resource.
 *
 * @see USE
 * @see USING
 */
#define WITH(disposal)                                                      \
                                                                            \
  EXCEPTIONS4C_START_BLOCK(true)                                            \
  if (e4c_dispose(EXCEPTIONS4C_DEBUG)) {                                    \
    (void) (disposal);                                                      \
  } else if (e4c_acquire(EXCEPTIONS4C_DEBUG)) {

/**
 * Closes a block of code with automatic resource disposal.
 *
 * You MUST use this macro to close a preceding #WITH block.
 *
 * A #USE block MUST be preceded by one #WITH block and MAY be followed by one
 * or more #CATCH, an optional #CATCH_ALL block, and an optional #FINALLY (in
 * this order), just like #TRY blocks.
 *
 * Any of these blocks MAY throw exceptions.
 *
 * @note
 * The #WITH... #USE combination works exactly the same as the #USING block,
 * except that the #WITH block is responsible for resource acquisition, and the
 * #USE block is responsible for resource manipulation.
 *
 * @attention
 * These blocks MUST NOT be exited through any of: <tt>goto</tt>,
 * <tt>break</tt>, <tt>continue</tt>, or <tt>return</tt>.
 *
 * Example: @snippet pet-store.c with_use
 *
 * @param test The condition for executing the code block.
 *
 * @see WITH
 */
#define USE(test)                                                           \
                                                                            \
  } else if (e4c_try(EXCEPTIONS4C_DEBUG) && (test))

/**
 * Repeats the previous #WITH or #USING block entirely.
 *
 * This macro restarts the previous #WITH or #USING block, up to a specified
 * maximum number of attempts, or throws an exception if it has already been
 * restarted too many times.
 *
 * @remark
 * This macro SHOULD be used in the body of a #CATCH or #CATCH_ALL block as a
 * quick way to fix an error condition and reacquire a resource again. It MAY
 * also be used in the body of a #FINALLY block provided that there is an
 * [uncaught exception](#e4c_is_uncaught) about to propagate up the call stack.
 *
 * @remark
 * Once the resource has been acquired, the #USE block MAY also be repeated
 * using the macro #RETRY.
 *
 * @important
 * Control never returns to the #REACQUIRE point.
 *
 * Example: @snippet pet-store.c reacquire
 *
 * @param max The maximum number of attempts.
 * @param type The type of exception to throw if <tt>max</tt> is exceeded.
 * @param format The error message.
 * @param ... An optional list of arguments that will be formatted according to
 *   <tt>format</tt>.
 *
 * @see RETRY
 * @see WITH
 * @see USE
 * @see THROW
 * @see e4c_is_uncaught
 */
#define REACQUIRE(max, type, format, ...)                                   \
                                                                            \
  EXCEPTIONS4C_LONG_JUMP(e4c_restart(true, (max),                           \
      &type, #type, EXCEPTIONS4C_DEBUG, (format) __VA_OPT__(,) __VA_ARGS__))

/**
 * @internal
 * @brief Starts a new exception block.
 *
 * @warning
 * This macro SHOULD be used only by #TRY and #WITH.
 *
 * @param should_acquire If <tt>true</tt>, the exception block will start
 *   #ACQUIRING a resource.
 */
#define EXCEPTIONS4C_START_BLOCK(should_acquire)                            \
                                                                            \
  for (                                                                     \
    EXCEPTIONS4C_SET_JUMP(e4c_start(should_acquire, EXCEPTIONS4C_DEBUG));   \
    e4c_next(EXCEPTIONS4C_DEBUG) || (                                       \
      e4c_is_uncaught() && (EXCEPTIONS4C_LONG_JUMP(e4c_get_env()), true)    \
    );                                                                      \
  )

#ifndef HAVE_SIGSETJMP

/**
 * @internal
 * @brief Saves the current execution context.
 *
 * @param env The variable to store the current [execution context](#e4c_env).
 */
#define EXCEPTIONS4C_SET_JUMP(env) setjmp(*(env))

/**
 * @internal
 * @brief Loads the supplied execution context.
 *
 * @param env The [execution context](#e4c_env) to load.
 */
#define EXCEPTIONS4C_LONG_JUMP(env) longjmp(*(env), 1)

/**
 * @internal
 * @brief Stores information to restore a calling environment.
 */
typedef jmp_buf e4c_env;

#else

/**
 * @internal
 * @brief Saves the current execution context.
 *
 * @param env The variable to store the current [execution context](#e4c_env).
 */
#define EXCEPTIONS4C_SET_JUMP(env) sigsetjmp(*(env), 1)

/**
 * @internal
 * @brief Loads the supplied execution context.
 *
 * @param env The [execution context](#e4c_env) to load.
 */
#define EXCEPTIONS4C_LONG_JUMP(env) siglongjmp(*(env), 1)

/**
 * @internal
 * @brief Stores information to restore a calling environment.
 */
typedef sigjmp_buf e4c_env;

#endif

#ifndef NDEBUG

/**
 * @internal
 * @brief Captures debug information about the running program.
 */
#define EXCEPTIONS4C_DEBUG                                                  \
                                                                            \
  (struct e4c_debug_info) {                                                 \
    .file = __FILE__,                                                       \
    .line = __LINE__,                                                       \
    .function = __func__                                                    \
  }

#else

/**
 * @internal
 * @brief Discards debug information about the running program.
 */
#define EXCEPTIONS4C_DEBUG                                                  \
                                                                            \
  (struct e4c_debug_info) {                                                 \
    .file = NULL,                                                           \
    .line = 0,                                                              \
    .function = NULL                                                        \
  }

#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represents a category of problematic situations in a program.
 *
 * Defines a kind of error or exceptional condition that a program MAY want to
 * #THROW and #CATCH. It serves as a way to group related issues that share
 * common characteristics.
 *
 * Exception types SHOULD be defined as <tt>const</tt>.
 *
 * @snippet pet-store.c exception_types
 *
 * @see THROW
 * @see CATCH
 */
struct e4c_exception_type {
    /** The possibly-null supertype of this type */
    const struct e4c_exception_type *supertype;

    /** The default message for new exceptions of this type */
    const char *default_message;
};

/**
 * Contains debugging information about an exception.
 *
 * @see e4c_exception.debug
 */
struct e4c_debug_info {
    /**
     * The name of the source file that threw the exception, or
     * <tt>NULL</tt> if <tt>NDEBUG</tt> is defined.
     */
    const char *file;

    /**
     * The line number in the source file that threw the exception, or
     * zero if <tt>NDEBUG</tt> is defined.
     */
    int line;

    /**
     * The name of the function that threw the exception, or
     * <tt>NULL</tt> if <tt>NDEBUG</tt> is defined.
     */
    const char *function;
};

/**
 * Represents a specific occurrence of an exceptional situation in a program.
 *
 * An [exception](#e4c_exception) combines a [type](#e4c_exception_type) with a
 * detailed error message and specific information about what went wrong in this
 * particular instance.
 *
 * After an exception is [thrown](#THROW), it propagates through the program and
 * MAY be caught by an appropriate #CATCH or #CATCH_ALL block.
 *
 * @remark
 * When an exception is caught, #e4c_get_exception MAY be used to retrieve the
 * exception currently being handled. This allows for inspection and further
 * handling of the error, based on both its type and the detailed context of the
 * situation.
 *
 * @see e4c_get_exception
 */
struct e4c_exception {
    /**
     * The general nature of the error.
     */
    const struct e4c_exception_type *type;

    /**
     * The name of the exception type.
     */
    const char *name;

    /**
     * A text message describing the specific problem.
     */
    char message[256];

    /**
     * Debugging information about this exception.
     */
    struct e4c_debug_info debug;

    /**
     * The value of [errno](https://devdocs.io/c/error/errno) at the time this
     * exception was thrown.
     */
    int error_number;

    /**
     * A possibly-null pointer to the current exception when this exception was
     * thrown.
     */
    struct e4c_exception *cause;

    /**
     * A possibly-null pointer to custom data associated to this exception.
     */
    void *data;
};

/**
 * Contains the configuration and the current status of exceptions.
 *
 * This structure allows you to configure the way the exception system behaves.
 *
 * - Set <strong>uncaught_handler</strong> to a function that will be executed
 *   when an exception reaches the top level of the program.
 * - Set <strong>termination_handler</strong> to a function that will be
 *   executed when the program abruptly terminates due to an uncaught exception.
 * - Set <strong>initialize_exception</strong> to a function that will be
 *   executed whenever an exception is thrown. This function MAY create and
 *   assign custom data to the exception.
 * - Set <strong>finalize_exception</strong> to a function that will be executed
 *   whenever an exception is deleted. This function MAY delete custom data
 *   previously created.
 *
 * @see e4c_get_context
 * @see e4c_set_context_supplier
 */
struct e4c_context {
    /**
     * @internal
     * The current exception block of the running program.
     */
    void *_innermost_block;

    /** The function to execute in the event of an uncaught exception */
    void (*uncaught_handler)(const struct e4c_exception *exception);

    /** The function to execute in the event of program termination. */
    void (*termination_handler)();

    /** The function to execute whenever a new exception is thrown */
    void (*initialize_exception)(struct e4c_exception *exception);

    /** The function to execute whenever an exception is destroyed */
    void (*finalize_exception)(const struct e4c_exception *exception);
};

/**
 * Sets the exception context supplier.
 *
 * The library relies on this [context](#e4c_context) to handle the current
 * status of exceptions. If no supplier is provided, a default one will be used.
 *
 * @remark
 * Since the default exception context does not support concurrency, this
 * mechanism can be useful to provide a concurrent version. For example, a
 * context supplier could return different instances, depending on which thread
 * is active. In that case, the supplier MUST be responsible for the creation
 * and deletion of those instances.
 *
 * Example: @snippet customization.c set_context_supplier
 *
 * @param supplier The function that supplies the current exception context.
 *
 * @see e4c_context
 */
void e4c_set_context_supplier(struct e4c_context * (*supplier)(void));

/**
 * Supplies the current exception context.
 *
 * @remark
 * The objected returned by this function MAY be used to configure the program's
 * current exception context. For example, you can set a custom handler that
 * will be executed when the program abruptly terminates due to an uncaught
 * exception.
 *
 * Example: @snippet uncaught-handler.c uncaught_handler
 *
 * @return The current exception context.
 *
 * @see e4c_context
 * @see e4c_set_context_supplier
 */
struct e4c_context *e4c_get_context(void);

/**
 * Retrieves the last exception that was thrown.
 *
 * @remark
 * This function SHOULD be used in the body of a #CATCH or #CATCH_ALL block to
 * inspect the exception being handled. It MAY also be used in the body of a
 * #FINALLY block to determine if an exception was thrown in the corresponding
 * #TRY block, or during the execution of a #CATCH or #CATCH_ALL block.
 *
 * Example: @snippet pet-store.c catch_all_get_exception
 *
 * @return The last exception that was thrown.
 *
 * @see e4c_exception
 * @see THROW
 * @see CATCH
 * @see FINALLY
 * @see e4c_is_uncaught
 */
const struct e4c_exception *e4c_get_exception(void);

/**
 * Determines whether the thrown exception was not caught.
 *
 * An exception is considered "uncaught" if no matching #CATCH or #CATCH_ALL
 * block has been executed for it. In other words, this function returns
 * <tt>true</tt> if the exception has bypassed all specific exception-handling
 * logic and is propagating further. And it returns <tt>false</tt> if no
 * exception was thrown in the #TRY block, or if an exception was successfully
 * caught.
 *
 * @remark
 * This function SHOULD be used exclusively in the body of a #FINALLY block to
 * check whether an exception thrown during the #TRY block has propagated past
 * all #CATCH and #CATCH_ALL blocks without being handled.
 *
 * Example: @snippet pet-store.c is_uncaught
 *
 * @return <tt>true</tt> if the current exception (if any) has not yet been
 *   handled by any #CATCH or #CATCH_ALL block; <tt>false</tt> otherwise.
 *
 * @see FINALLY
 * @see e4c_get_exception
 */
bool e4c_is_uncaught(void);

/**
 * @internal
 * @brief Starts a new exception block.
 *
 * @warning
 * This function SHOULD be called only via #EXCEPTIONS4C_START_BLOCK.
 *
 * @param should_acquire if <tt>true</tt>, the exception block will start in the
 *   #ACQUIRING stage; otherwise it will start in the #TRYING stage.
 * @param file The name of the source code file that is calling this function.
 * @param line The number of line that is calling this function.
 * @param function The name of the function that is calling this function.
 * @return The execution context of the new exception block.
 *
 * @see TRY
 * @see WITH
 */
e4c_env *e4c_start(bool should_acquire, struct e4c_debug_info debug);

/**
 * @internal
 * @brief Iterates through the different [stages](#block_stage) of the current
 *   exception block.
 *
 * @warning
 * This function SHOULD be called only via #EXCEPTIONS4C_START_BLOCK.
 *
 * @param file The name of the source code file that is calling this function.
 * @param line The number of line that is calling this function.
 * @param function The name of the function that is calling this function.
 * @return <tt>true</tt> if the current exception block has not completed yet;
 *   <tt>false</tt> otherwise.
 *
 * @see TRY
 * @see WITH
 */
bool e4c_next(struct e4c_debug_info debug);

/**
 * @internal
 * @brief Retrieves the execution context of the current exception block.
 *
 * @warning
 * This function SHOULD be called only via #EXCEPTIONS4C_START_BLOCK.
 *
 * @return The execution context of the current exception block.
 *
 * @see TRY
 * @see WITH
 */
e4c_env *e4c_get_env(void);

/**
 * @internal
 * @brief Checks if the current exception block is in the #ACQUIRING stage.
 *
 * @warning
 * This function SHOULD be called only via #WITH.
 *
 * @param file The name of the source code file that is calling this function.
 * @param line The number of line that is calling this function.
 * @param function The name of the function that is calling this function.
 * @return <tt>true</tt> if the current exception block is in the #ACQUIRING
 *   stage; <tt>false</tt> otherwise.
 *
 * @see WITH
 */
bool e4c_acquire(struct e4c_debug_info debug);

/**
 * @internal
 * @brief Checks if the current exception block is in the #TRYING stage.
 *
 * @warning
 * This function SHOULD be called only via #TRY.
 *
 * @param file The name of the source code file that is calling this function.
 * @param line The number of line that is calling this function.
 * @param function The name of the function that is calling this function.
 * @return <tt>true</tt> if the current exception block is in the #TRYING stage;
 *   <tt>false</tt> otherwise.
 *
 * @see TRY
 */
bool e4c_try(struct e4c_debug_info debug);

/**
 * @internal
 * @brief Checks if the current exception block is in the #DISPOSING stage.
 *
 * @warning
 * This function SHOULD be called only via #WITH.
 *
 * @param file The name of the source code file that is calling this function.
 * @param line The number of line that is calling this function.
 * @param function The name of the function that is calling this function.
 * @return <tt>true</tt> if the current exception block is in the #DISPOSING
 *   stage; <tt>false</tt> otherwise.
 *
 * @see WITH
 */
bool e4c_dispose(struct e4c_debug_info debug);

/**
 * @internal
 * @brief Checks if the current exception can be handled.
 *
 * @warning
 * This function SHOULD be called only via #CATCH or #CATCH_ALL.
 *
 * @param type The type of exceptions to handle.
 * @param file The name of the source code file that is calling this function.
 * @param line The number of line that is calling this function.
 * @param function The name of the function that is calling this function.
 * @return <tt>true</tt> if:
 *   - the current exception block is in the #CATCHING stage, AND
 *   - the supplied <tt>type</tt> is either <tt>NULL</tt> or a supertype of the
 *     thrown exception.
 *   <tt>false</tt> otherwise.
 *
 * @see CATCH
 * @see CATCH_ALL
 */
bool e4c_catch(
    const struct e4c_exception_type *type, struct e4c_debug_info debug);

/**
 * @internal
 * @brief Checks if the current exception block is in the #FINALIZING stage.
 *
 * @warning
 * This function SHOULD be called only via #FINALLY.
 *
 * @param file The name of the source code file that is calling this function.
 * @param line The number of line that is calling this function.
 * @param function The name of the function that is calling this function.
 * @return <tt>true</tt> if the current exception block is in the #FINALIZING
 *   stage; <tt>false</tt> otherwise.
 *
 * @see FINALLY
 */
bool e4c_finally(struct e4c_debug_info debug);

/**
 * @internal
 * @brief Throws a new exception.
 *
 * @warning
 * This function SHOULD be called only via #THROW.
 *
 * @param type The type of exception to throw.
 * @param name The name of the exception type.
 * @param file The name of the source code file that is calling this function.
 * @param line The number of line that is calling this function.
 * @param function The name of the function that is calling this function.
 * @param format The error message.
 * @param ... An optional list of arguments that will be formatted according to
 *   <tt>format</tt>.
 * @return The execution context of the current exception block.
 *
 * @see THROW
 */
e4c_env *e4c_throw(
    const struct e4c_exception_type *type, const char *name,
    struct e4c_debug_info debug, const char *format, ...);

/**
 * @internal
 * @brief Restarts an exception block.
 *
 * @warning
 * This function SHOULD be called only via #RETRY or #REACQUIRE.
 *
 * @param should_reacquire If <tt>true</tt>, the exception block will restart in
 *   the #ACQUIRING stage; otherwise it will start in the #TRYING stage.
 * @param max_attempts
 * @param type The type of exception to throw.
 * @param name The name of the exception type.
 * @param file The name of the source code file that is calling this function.
 * @param line The number of line that is calling this function.
 * @param function The name of the function that is calling this function.
 * @param format The error message.
 * @param ... An optional list of arguments that will be formatted according to
 *   <tt>format</tt>.
 * @return The execution context of the current exception block.
 */
e4c_env *e4c_restart(
    bool should_reacquire, int max_attempts,
    const struct e4c_exception_type *type, const char *name,
    struct e4c_debug_info debug, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
