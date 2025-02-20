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
 * Implementation of the exception handling library for C.
 *
 * <a href="https://exceptions4c.guillermo.dev/">
 * <img src="exceptions4c.svg">
 * </a>
 *
 * @file        exceptions4c.c
 * @version     4.0.0
 * @author      [Guillermo Calvo]
 * @copyright   Licensed under [Apache 2.0]
 * @see         For more information, visit the [project on GitHub].
 *
 * [Guillermo Calvo]: https://guillermo.dev
 * [Apache 2.0]: http://www.apache.org/licenses/LICENSE-2.0
 * [project on GitHub]: https://github.com/guillermocalvo/exceptions4c
 */

#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <stdnoreturn.h>
#include <exceptions4c.h>

/**
 * @internal
 * @brief Represents the execution stage of the current exception block.
 */
enum block_stage {
    /**
     * @internal
     * The exception block has started.
     */
    BEGINNING,

    /**
     * @internal
     * The exception block is [acquiring a resource](#WITH).
     */
    ACQUIRING,

    /**
     * @internal
     * The exception block is [trying something](#TRY) or [using a resource](#USE).
     */
    TRYING,

    /**
     * @internal
     * The exception block is [disposing of a resource](#WITH).
     */
    DISPOSING,

    /**
     * @internal
     * The exception block is [catching an exception](#CATCH).
     */
    CATCHING,

    /**
     * @internal
     * The exception block is [finalizing](#FINALLY).
     */
    FINALIZING,

    /**
     * @internal
     * The exception block has finished.
     */
    DONE
};

/**
 * @internal
 * @brief Represents an exception block.
 */
struct e4c_block {
    /**
     * A possibly-null pointer to the outer exception block.
     */
    struct e4c_block *outer_block;

    /**
     * The stage of this block.
     */
    enum block_stage stage;

    /**
     * Whether this block currently has an uncaught exception.
     */
    bool uncaught;

    /**
     * A possibly-null pointer to the currently thrown exceptions.
     */
    struct e4c_exception *exception;

    /**
     * Current number of times the #TRY block has been attempted.
     */
    int retry_attempts;

    /**
     * Current number of times the #WITH block has been attempted.
     */
    int reacquire_attempts;

    /**
     * The execution context of this exception block.
     */
    e4c_env env;
};

/* Causes abnormal program termination due to a fatal error. */
static noreturn void panic(const char *error_message, const struct e4c_debug_info *debug);

/* Allocates memory for an object; panics if not enough memory. */
static void *allocate(size_t size, const char *error_message, const struct e4c_debug_info *debug);

/* Retrieves the current exception context; panics if NULL. */
static struct e4c_context *get_context(const struct e4c_debug_info *debug);

/* Retrieves the current exception block (if any). */
static struct e4c_block *e4c_get_block(void);

/* Checks for dangling exception blocks at program exit. */
static void cleanup_default_context(void);

/* Creates and propagates a new exception. */
static void throw(
    const struct e4c_context *context, const struct e4c_exception_type *type, const char *name, int error_number,
    const struct e4c_debug_info *debug, const char *format, va_list arguments_list);

/* Propagates the supplied exception in the supplied context. */
static void propagate(const struct e4c_context *context, struct e4c_exception *exception);

/* Returns the current exception block stage; panics if NULL. */
static enum block_stage get_stage(const struct e4c_debug_info *debug);

/* Deletes the supplied exception, along with its cause. */
static void delete_exception(const struct e4c_context *context, struct e4c_exception *exception);

/* Prints debug info (if available) to the standard error output. */
static void print_debug_info(const struct e4c_debug_info *debug);

/* Prints the supplied exception to the standard error output. */
static void print_exception(const struct e4c_exception *exception, bool is_cause);

/* Determines if the supplied type extends from the supplied supertype. */
static bool extends(const struct e4c_exception_type *type, const struct e4c_exception_type *supertype);

/**
 * Stores the exception context supplier.
 */
static struct e4c_context * (*context_supplier)(void) = NULL;

/**
 * Default exception context of the program when no custom supplier is provided.
 */
static struct e4c_context default_context = {
    ._innermost_block       = NULL,
    .initialize_exception   = NULL,
    .finalize_exception     = NULL,
    .uncaught_handler       = NULL
};

/**
 * Flag that determines if the exception system has been already initialized.
 */
static bool is_cleanup_registered = false;

void e4c_set_context_supplier(struct e4c_context * (*supplier)(void)) {
    context_supplier = supplier;
}

struct e4c_context *e4c_get_context(void) {
    return context_supplier != NULL ? context_supplier() : &default_context;
}

const struct e4c_exception *e4c_get_exception(void) {
    const struct e4c_block *block = e4c_get_block();
    return block != NULL ? block->exception : NULL;
}

bool e4c_is_uncaught(void) {
    const struct e4c_block *block = e4c_get_block();
    return block != NULL && block->uncaught;
}

e4c_env *e4c_start(const bool should_acquire, const struct e4c_debug_info debug) {
    struct e4c_block *new_block = allocate(
        sizeof(*new_block), "Not enough memory to create a new exception block", &debug);
    struct e4c_context *context = get_context(&debug);
    if (context == &default_context && !is_cleanup_registered) {
        if (atexit(cleanup_default_context) != 0) {
            panic("Cleanup function could not be registered.", &debug);
        }
        is_cleanup_registered = true;
    }

    new_block->outer_block          = context->_innermost_block;
    new_block->stage                = should_acquire ? BEGINNING : ACQUIRING;
    new_block->uncaught             = false;
    new_block->reacquire_attempts   = 0;
    new_block->retry_attempts       = 0;
    new_block->exception            = NULL;

    context->_innermost_block = new_block;

    return &new_block->env;
}

bool e4c_next(const struct e4c_debug_info debug) {
    struct e4c_context *context = get_context(&debug);
    struct e4c_block *block = context->_innermost_block;
    if (block == NULL) {
        panic("Invalid exception context state.", &debug);
    }

    /* advance the block to the next stage */
    block->stage++;

    struct e4c_exception *exception = block->exception;
    const bool uncaught = block->uncaught;

    /* simple optimization: skip CATCHING stage if no exception thrown */
    if (block->stage == CATCHING && (exception == NULL || !uncaught)) {
        block->stage++;
    }

    /* carry on until the block is DONE */
    if (block->stage < DONE) {
        return true;
    }

    /* deallocate this block and promote its outer block */
    context->_innermost_block = block->outer_block;
    free(block);

    /* deallocate or propagate its exception */
    if (exception != NULL) {
        if (uncaught) {
            propagate(context, exception);
        } else {
            delete_exception(context, exception);
        }
    }

    /* get out of the loop */
    return false;
}

e4c_env *e4c_get_env(void) {
    struct e4c_block *block = e4c_get_block();
    return block != NULL ? &block->env : NULL;
}

bool e4c_acquire(const struct e4c_debug_info debug) {
    return get_stage(&debug) == ACQUIRING;
}

bool e4c_try(const struct e4c_debug_info debug) {
    return get_stage(&debug) == TRYING;
}

bool e4c_dispose(const struct e4c_debug_info debug) {
    return get_stage(&debug) == DISPOSING;
}

bool e4c_catch(const struct e4c_exception_type *type, const struct e4c_debug_info debug) {
    const struct e4c_context *context = get_context(&debug);
    struct e4c_block *block = context->_innermost_block;
    if (block == NULL) {
        panic("Invalid exception context state.", &debug);
    }
    /* check if this CATCH block can handle current exception */
    if (block->stage == CATCHING
        && block->exception != NULL
        && block->uncaught
        && (type == NULL || extends(block->exception->type, type))) {
        block->uncaught = false;
        return true;
    }
    return false;
}

bool e4c_finally(const struct e4c_debug_info debug) {
    return get_stage(&debug) == FINALIZING;
}

/*
 * Suppressed warning:
 * - `c:S923` Functions should not be defined with a variable number of arguments
 *   <https://next.sonarqube.com/sonarqube/coding_rules?open=c%3AS923&rule_key=c%3AS923>
 */
e4c_env *e4c_throw( /* NOSONAR */
    const struct e4c_exception_type *type, const char *name,
    const struct e4c_debug_info debug, const char *format, ...) {
    const int error_number = errno;
    const struct e4c_context *context = get_context(&debug);

    va_list arguments_list;
    va_start(arguments_list, format);
    throw(context, type, name, error_number, &debug, format, arguments_list);
    va_end(arguments_list);

    return &((struct e4c_block *) context->_innermost_block)->env;
}

/*
 * Suppressed warnings:
 * - `c:S923` Functions should not be defined with a variable number of arguments
 *   <https://next.sonarqube.com/sonarqube/coding_rules?open=c%3AS923&rule_key=c%3AS923>
 */
e4c_env *e4c_restart( /* NOSONAR */
    const bool should_reacquire, const int max,
    const struct e4c_exception_type *type, const char *name,
    const struct e4c_debug_info debug, const char *format, ...) {
    const int error_number = errno;
    const struct e4c_context *context = get_context(&debug);
    struct e4c_block *block = context->_innermost_block;
    if (block == NULL) {
        panic(should_reacquire ? "No WITH block to reacquire." : "No TRY block to retry.", &debug);
    }
    /* update the number of retry/reacquire attempts */
    bool max_reached;
    if (should_reacquire) {
        max_reached = block->reacquire_attempts >= max;
        if (!max_reached) {
            block->reacquire_attempts++;
        }
    } else {
        max_reached = block->retry_attempts >= max;
        if (!max_reached) {
            block->retry_attempts++;
        }
    }
    /* check if maximum number of attempts reached */
    if (max_reached) {
        /* throw a new exception, possibly using the current one as cause */
        va_list arguments_list;
        va_start(arguments_list, format);
        throw(context, type, name, error_number, &debug, format, arguments_list);
        va_end(arguments_list);
    } else {
        /* suppress currently thrown exception; jump back to TRY/WITH block */
        if (block->exception != NULL) {
            delete_exception(context, block->exception);
            block->exception = NULL;
        }
        block->uncaught = false;
        block->stage = should_reacquire ? BEGINNING : ACQUIRING;
    }
    return &block->env;
}

/**
 * Causes abnormal program termination due to a fatal error.
 *
 * @param error_message The message to print to standard error output.
 * @param debug Debug info.
 */
static noreturn void panic(const char *error_message, const struct e4c_debug_info *debug) {
    fprintf(stderr, "[exceptions4c] %s\n", error_message);
    print_debug_info(debug);
    fflush(stderr);
    abort();
}

/**
 * Allocates memory for an object; <em>panics</em> if not enough memory.
 *
 * @param size The size of the new object.
 * @param error_message The message to print to standard error output.
 * @param debug Debug info.
 * @return A pointer to zero-initialized, newly allocated memory.
 */
static void *allocate(const size_t size, const char *error_message, const struct e4c_debug_info *debug) {
    void *object = calloc(1, size);
    if (object == NULL) {
        panic(error_message, debug);
    }
    return object;
}

/**
 * Checks for dangling exception blocks at program exit.
 */
static void cleanup_default_context(void) {
    if (default_context._innermost_block != NULL) {
        const struct e4c_debug_info debug = {
            .file = NULL,
            .line = 0,
            .function = NULL
        };
        panic("Dangling exception block detected.\n"
              "    Some TRY block may have been exited improperly (via goto, break, continue, or return).", &debug);
    }
}

/**
 * Retrieves the current exception context; <em>panics</em> if <tt>NULL</tt>.
 *
 * @param debug Debug info.
 * @return A pointer to the current exception context.
 */
static struct e4c_context *get_context(const struct e4c_debug_info *debug) {
    struct e4c_context *context = e4c_get_context();
    if (context == NULL) {
        panic("Context supplier returned NULL.", debug);
    }
    return context;
}

/**
 * Retrieves the current exception block (if any).
 *
 * @return the current exception block; <tt>NULL</tt> if absent.
 */
static struct e4c_block *e4c_get_block(void) {
    const struct e4c_context *context = e4c_get_context();
    return context != NULL ? context->_innermost_block : NULL;
}

/**
 * Propagates the supplied exception in the supplied context.
 *
 * @note
 * If the exception reached the top level of the program, then the program will
 * be abruptly terminated.
 *
 * @param context The context whose innermost exception block will receive the
 *   exception.
 * @param exception The exception to propagate.
 */
static void propagate(const struct e4c_context *context, struct e4c_exception *exception) {
    struct e4c_block *block = context->_innermost_block;
    if (block == NULL) {
        /* uncaught exception handler */
        if (context->uncaught_handler != NULL) {
            context->uncaught_handler(exception);
        } else {
            print_exception(exception, false);
            (void) fflush(stderr);
        }
        /* delete the exception to avoid memory leaks */
        delete_exception(context, exception);
        /* abrupt termination handler */
        if (context->termination_handler != NULL) {
            context->termination_handler();
            return;
        }
        exit(EXIT_FAILURE);
    }

    /* suppress current exception (if any) */
    if (block->exception != NULL && block->exception != exception) {
        delete_exception(context, block->exception);
    }

    block->exception = exception;
    block->uncaught = true;

    /* skip DISPOSING stage if the acquisition just failed */
    if (block->stage == ACQUIRING) {
        block->stage = DISPOSING;
    }
}

/**
 * Returns the current exception block stage; <em>panics</em> if <tt>NULL</tt>.
 *
 * @param debug Debug info.
 * @return The stage of the current exception block.
 */
static enum block_stage get_stage(const struct e4c_debug_info *debug) {
    const struct e4c_context *context = get_context(debug);
    const struct e4c_block *block = context->_innermost_block;
    if (block == NULL) {
        panic("Invalid exception context state.", debug);
    }
    return block->stage;
}

/**
 * Determines if the supplied type extends from the supplied supertype.
 *
 * @param type The specific type to check.
 * @param supertype The generic type to check against.
 * @return <tt>true</tt> if the <tt>type</tt> extends from <tt>supertype</tt>.
 */
static bool extends(const struct e4c_exception_type *type, const struct e4c_exception_type *supertype) {
    for (; type != NULL;
           type = type != type->supertype ? type->supertype : NULL) {
        if (type == supertype) {
            return true;
        }
    }
    return false;
}

/**
 * Creates and propagates a new exception.
 *
 * @param context The current exception context.
 * @param type The exception type to throw.
 * @param name The name of the exception type.
 * @param error_number The value of <tt>errno</tt> when the exception is thrown.
 * @param debug Debug info.
 * @param format The optional error message.
 * @param arguments_list The list of arguments to be formatted.
 */
static void throw(
    const struct e4c_context *context, const struct e4c_exception_type *type, const char *name, int error_number,
    const struct e4c_debug_info *debug, const char *format, va_list arguments_list) {
    /* allocate and initialize the new exception */
    struct e4c_exception *exception = allocate(
        sizeof(*exception), "Not enough memory to create a new exception", debug);

    exception->name             = name;
    exception->debug.file       = debug->file;
    exception->debug.line       = debug->line;
    exception->debug.function   = debug->function;
    exception->error_number     = error_number;
    exception->type             = type;
    exception->cause            = NULL;
    exception->data             = NULL;

    if (format == NULL && type != NULL) {
        (void) snprintf(exception->message, sizeof(exception->message), "%s", type->default_message);
    } else if (format != NULL) {
        /*
         * Suppressed warning:
         * - `c:S5281` Argument of "printf" should be a format string
         *   <https://next.sonarqube.com/sonarqube/coding_rules?open=c%3AS5281&rule_key=c%3AS5281>
         */
        (void) vsnprintf(exception->message, sizeof(exception->message), format, arguments_list); /* NOSONAR */
    }

    /* capture the cause of this exception */
    for (
        struct e4c_block *block = context->_innermost_block;
        block != NULL;
        block = block->outer_block
    ) {
        if (block->exception != NULL
            && (block->uncaught || block->stage == CATCHING)) {
            exception->cause = block->exception;
            block->exception = NULL;
            break;
        }
    }

    /* initialize custom data */
    if (context->initialize_exception != NULL) {
        context->initialize_exception(exception);
    }

    propagate(context, exception);
}

/**
 * Deletes the supplied exception, along with its cause.
 *
 * @param context The context the supplied exception belongs to.
 * @param exception The exception to delete.
 */
static void delete_exception(const struct e4c_context *context, struct e4c_exception *exception) {
    if (context->finalize_exception != NULL) {
        context->finalize_exception(exception);
    }
    if (exception->cause != NULL) {
        delete_exception(context, exception->cause);
    }
    free(exception);
}

/**
 * Prints debug info (if available) to the standard error output.
 *
 * @param debug The debug info to print if available.
 */
static void print_debug_info(const struct e4c_debug_info *debug) {
    if (debug->file != NULL) {
        (void) fprintf(stderr, "    at %s (%s:%d)\n", debug->function, debug->file, debug->line);
    }
}

/**
 * Prints the supplied exception to the standard error output.
 *
 * @param exception The exception to print.
 * @param is_cause <tt>true</tt> if the supplied exception is the cause of
 *   another one.
 */
static void print_exception(const struct e4c_exception *exception, const bool is_cause) {
    (void) fprintf(stderr, "%s%s: %s\n", is_cause ? "Caused by: " : "\n", exception->name, exception->message);
    print_debug_info(&exception->debug);
    if (exception->cause != NULL) {
        print_exception(exception->cause, true);
    }
}
