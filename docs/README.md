
<div style="justify-items: center">
<img style="display: block" src="exceptions4c.svg">
</div>

# Introduction

Bring the power of exceptions to your C applications!

![][EXAMPLE]

> [!NOTE]
> This library provides you with a set of macros and functions that map the exception handling semantics you are
> probably already used to.


# Getting Started

## Adding Exceptions to Your Project

This library consists of two files:

- exceptions4c.h
- exceptions4c.c

To use it in your project, include the header file in your source code files.

```c
#include <exceptions4c.h>
```

And then link your program against the library code.

> [!TIP]
> There is also a lightweight version of this library, intended for small projects and embedded systems.
> [exceptions4c-lite][EXCEPTIONS4C_LITE] is a header-only library that provides the core functionality of exceptions4c
> in just one file.

## Defining Exception Types

Create meaningful exceptions that reflect problematic situations in the program.

@snippet pet-store.c exception_types

An exception type is [a simple structure](#e4c_exception_type) with an optional supertype and a default error message.

> [!NOTE]
> Exception types create a hierarchy, where a more specific type can be built upon a more generic one.

# Basic Usage

Exception handling lets a program deal with errors without crashing. When something goes wrong, the program pauses its
normal flow, jumps to code that handles the issue, and then either recovers or exits cleanly.

This library provides the following macros that are used to handle exceptions:

- #THROW
- #TRY
- #CATCH
- #FINALLY

## Throwing Exceptions

When we #THROW an exception, the flow of the program moves to the appropriate #CATCH block. If the exception is not
handled by any of the blocks in the current function, it propagates up the call stack to the function that called the
current function. This continues until the top level of the program is reached. If no block handles the exception, the
program terminates and an error message is printed to the console.

Use #THROW to trigger an exception when something goes wrong.

@snippet pet-store.c throw

When we #THROW an exception, the flow of the program moves from the #TRY block to the appropriate #CATCH block. If the
exception is not handled by any of the blocks in the current function, it propagates up the call stack to the function
that called the current function. This continues until the top level of the program is reached. If no block handles the
exception, the program terminates and an error message is printed to the console.

> [!NOTE]
> Error messages can be formatted, just as you would with `printf`. Additionally, if you don't provide an error message,
> the default one for that exception type will be used.

## Trying Risky Code

Use a #TRY block to wrap code that might cause an exception.

@snippet pet-store.c try

These code blocks, by themselves, don't do anything special. But they allow the introduction of other blocks that do
serve specific purposes.

> [!TIP]
> A single #TRY block must be followed by one or more #CATCH blocks to handle the errors, and an optional #FINALLY block
> to execute cleanup code.

> [!CAUTION]
> Never exit these blocks using `goto`, `break`, `continue`, or `return`.

## Catching Exceptions

To prevent the program from crashing, exceptions need to be handled properly in designated sections of the code.

### Handling Specific Types of Exceptions

Use a #CATCH block to handle a specific type of exceptions when they occur.

@snippet pet-store.c catch

If the type in the #CATCH block is the same as (or a supertype of) the thrown exception, then the block will be used to
handle it.

One or more #CATCH blocks can follow a #TRY block. Each #CATCH block must specify the type of exception it handles. If
its type doesn't match the thrown exception, then that block is ignored, and the exception may be caught by the
following blocks.

> [!IMPORTANT]
> When looking for a match, #CATCH blocks are inspected in the order they appear. If you place a generic handler before
> a more specific one, the second block will be unreachable.

### Handling All Kinds of Exceptions

On the other hand, the #CATCH_ALL block is a special block that can handle all types of exceptions.

@snippet pet-store.c catch_all

Only one #CATCH_ALL block is allowed per #TRY block, and it must appear after all type-specific #CATCH blocks if any are
present.

### Retrieving the Current Exception

Use #e4c_get_exception to retrieve the exception currently being handled.

@snippet pet-store.c catch_all_get_exception

> [!TIP]
> This allows for inspection and further handling of the error, based on both its type and the detailed context of the
> situation.

### Trying Again

Use #RETRY to restart the previous #TRY block that threw an exception.

@snippet pet-store.c retry

> [!NOTE]
> If the block has already been restarted too many times, then #RETRY will throw the provided exception.

## Ensuring Cleanup

A #FINALLY block always runs, no matter whether an exception happens or not.

@snippet pet-store.c finally

This block is optional. And, for each #TRY block, there can be only one #FINALLY block. If an exception occurs, the
#FINALLY block is executed after the #CATCH or block that can handle it. Otherwise, it is executed after the #TRY block.

> [!TIP]
> Use #e4c_is_uncaught to determine whether the thrown exception hasn't been handled yet.


# Advanced Usage

## Dispose Pattern

This is a powerful design pattern for resource management. It is a clean and terse way to handle the acquisition and
disposal of all kinds of resources.

These macros will help you make sure that no resource is leaked in your program.

- #USING
- #WITH ... #USE

### Simple Resource Acquisition

A #USING block allows you to easily acquire and dispose of a resource. It is similar to a `for` statement, because it
receives three comma-separated expressions that will be evaluated in order.

- An **acquisition** expression that will try to acquire the resource.
- A **test** expression that defines the condition for using the resource.
- A **disposal** expression that will dispose of the resource.

Both these expressions and the code block that uses the resource are free to throw exceptions.

@snippet pet-store.c using

1. The resource will be acquired, using the expression `pet = pet_find(id)`.
2. If the expression `pet != NULL` holds true, then the #USING block will be executed.
3. The resource `pet` will be disposed of, using the expression `pet_free(pet)`, no matter whether an exception happens
   or not.

You can append #CATCH blocks to deal with exceptions that may happen during the manipulation of the resource. Just
remember: by the time the #CATCH block is executed, the resource will already have been disposed of.

@snippet pet-store.c using_catch

> [!TIP]
> You can even append a #FINALLY block for cleanup code other than disposing of the resource.

### Complex Resource Acquisition

The #WITH and #USE blocks come in pairs. They are used when the steps to acquire a resource are more complex than simply
evaluating an expression. It works exactly like the #USING block, except that you can write the code block responsible
for acquiring the resource.

@snippet pet-store.c with_use

> [!TIP]
> You can also append #CATCH blocks and an optional #FINALLY block.

### Trying Acquisition Again

Use #REACQUIRE to restart the previous #USING or #WITH block that threw an exception.

@snippet pet-store.c reacquire

> [!NOTE]
> If the block has already been restarted too many times, then #RETRY will throw the provided exception.

## Customization

To customize the way this library behaves you may configure a structure that represents the
[exception context](#e4c_context) of the program.

### Retrieving the Exception Context

Use #e4c_get_context to retrieve the current [exception context](#e4c_context) of the program.

@snippet customization.c get_context

Then use this object to set up different handlers.

### Custom Exception Initializer

Exceptions support [custom data](#e4c_exception.data). By default, this data is left uninitialized when an exception is
thrown.

You can set a custom [exception initializer](#e4c_context.initialize_exception) and your function will be executed
whenever an exception is thrown.

@snippet customization.c initialize_exception

> [!TIP]
> For example, you could use this opportunity to capture the entire stacktrace of your program.

### Custom Exception Finalizer

You can also set a [exception finalizer](#e4c_context.finalize_exception) to execute your function whenever an exception
is deleted.

@snippet customization.c finalize_exception

> [!TIP]
> This allows you to free any resources you acquired when you initialized an exception's custom data.

### Custom Uncaught Handler

By default, when an exception reaches the top level of the program, it gets printed to the standard error stream.

You can customize this behavior by setting the [uncaught handler](#e4c_context.uncaught_handler) to a custom function
that will be executed in the event of an uncaught exception.

@snippet uncaught-handler.c uncaught_handler

> [!TIP]
> Instead of simply using `stderr` you could save an error report in a local file.

### Custom Termination Handler

After the uncaught handler has been executed, the program is terminated by calling `exit(EXIT_FAILURE)`.

You can make the library do anything else by setting the [termination handler](#e4c_context.termination_handler) to
execute a function in the event of program termination.

@snippet customization.c termination_handler

> [!TIP]
> In a multithreaded program, you may want to cancel the current thread, instead of terminating the whole program.

### Exception Context Supplier

By default, a predefined exception context is provided and used by the library. But you can create a supplying function
and pass it to #e4c_set_context_supplier so you are in full control of your program's exception context.

@snippet customization.c set_context_supplier

> [!TIP]
> This mechanism can be useful to provide a concurrent exception handler. For example, your custom context supplier
> could return different instances, depending on which thread is active.

## Multithreading

There is an extension for this library, intended for multithreaded programs.
[exceptions4c-pthreads][EXCEPTIONS4C_PTHREADS] allows you to safely and concurrently use exceptions.

All you have to do is set the exception context supplier, so that each POSIX thread gets its own exception context.

@snippet pthreads.c setup

In the event of an uncaught exception, instead of terminating the program, only the current thread will be canceled.

> [!NOTE]
> [Read the docs][EXCEPTIONS4C_PTHREADS_DOCS] for more information about this extension.

## Signal Handling

You can turn some standard signals such as [`SIGTERM`][SIGTERM], [`SIGFPE`][SIGFPE], and [`SIGSEGV`][SIGSEGV] into
exceptions so they can be handled in a regular #CATCH block. For example, you could do that to prevent your program from
crashing when a null pointer is dereferenced.

@snippet signals.c null_pointer

However, it's easy to enter undefined behavior territory, due to underspecified behavior and significant implementation
variations regarding signal delivery while a signal handler is executed, so use this technique with caution.

> [!IMPORTANT]
> Keep in mind that the behavior is undefined when [`signal`][SIGNAL] is used in a multithreaded program.


# Additional Info

## Compatibility

This library relies on modern C features such as [designated initializers][DESIGNATED_INITIALIZERS],
[compound literals][COMPOUND_LITERALS], and [`__VA_OPT__`][__VA_OPT__].

> [!TIP]
> If you need support for older compilers, you can try [exceptions4c-lite][EXCEPTIONS4C_LITE]. It's header-only and
> fully compatible with ANSI C. And if you're looking for a cleaner, safer, and more modern approach to error handling
> that doesn't involve throwing or catching exceptions, you may want to take a look at [Result Library][RESULT_LIBRARY].

## Caveat

Exception handling is based on standard C library functions [`setjmp`][SETJMP] to save the current execution context
and [`longjmp`][LONGJMP] to restore it. According to the documentation:

> Upon return to the scope of [`setjmp`][SETJMP]:
> - all accessible objects, floating-point status flags, and other components of the abstract machine have the same
>   values as they had when [`longjmp`][LONGJMP] was executed,
> - except for the non-[volatile][VOLATILE] local variables in the function containing the invocation of
>   [`setjmp`][SETJMP], whose values are indeterminate if they have been changed since the [`setjmp`][SETJMP]
>   invocation.

Since each #TRY block invokes [`setjmp`][SETJMP], modified local variables in scope must be [`volatile`][VOLATILE].

## Similar Projects

There are other exception handling implementations and libraries.

<details>
  <summary>Here you can find some of them, in no particular order.</summary>

- [Exceptions in C](https://web.archive.org/web/20141228062604/http://ldeniau.home.cern.ch/ldeniau/html/exception/exception.html)
  By Laurent Deniau

- [Implementing Exceptions in C Programming language](http://adomas.org/excc/)
  By Adomas Paltanavičius

- [Exceptions in C](https://web.archive.org/web/20090627200944/https://math.umd.edu/~asnowden/c-except.html)
  and
  [LIBXC](http://web.archive.org/web/20051217142403/http://www.math.umd.edu/~asnowden/libxc/ "A C-library which wraps LIBC and implements exceptions")
  By Andrew Snowden

- [Handling exceptions in C code](https://www.swig.org/Doc1.1/HTML/Exceptions.html#n2)
  SWIG (Simplified Wrapper and Interface Generator)

- [Exception Handling in C without C++](http://www.on-time.com/ddj0011.htm)
  By Tom Schotland and Peter Petersen

- [Trycatch library](http://llg.cubic.org/trycatch/)
  By Michael Stickel and Dirk Jagdmann

- [XXL](https://web.archive.org/web/20120106082715/http://zork.org/xxl/)
  By Matt Messier and John Viega

- [libex](http://code.google.com/p/libex/ "Exception handling and RAII for C")
  By Sandro Magi

- [libexcept](http://sourceforge.net/projects/libexcept/ "An exceptions framework for the C programming language")
  By Simon Howard

- [cexcept](http://www.nicemice.net/cexcept/ "try/catch/throw exception-handling interface for C")
  By Adam M. Costello and Cosmin Truta

- [A Manual of C Style](http://www.cs.uiowa.edu/~jones/syssoft/style.html#except "Exception Handling")
  By Douglas W. Jones

- [Ex package](http://cexcept.sourceforge.net/ex.txt.gz "Exception handling package")
  By Douglas A. Gwyn

- [GEF](http://web.archive.org/web/20071210184301/http://home.rochester.rr.com/bigbyofrocny/GEF/ "General Exception-Handling Facility")
  By Bruce W. Bigby

- [cexception](http://sourceforge.net/apps/trac/cexception/ "Simple exception handling in C")
  and
  [ThrowTheSwitch](https://github.com/ThrowTheSwitch/CException "Simple exception handling in C")
  By Mark VanderVoord

- [EXC](https://www.ibiblio.org/pub/historic-linux/ftp-archives/sunsite.unc.edu/Sep-29-1996/devel/lang/c/exc-1.0.tar.gz "An Exception Handling Library for ANSI-C")
  By Taj Khattra

- [libexception](https://github.com/hollow/libexception "Exception handling library for C")
  By Benedikt Böhm

- [cii](http://code.google.com/p/cii/source/browse/trunk/include/except.h "C Interfaces and Implementations")
  By David R. Hanson

- [Exceptions in C with Longjmp and Setjmp](http://www.di.unipi.it/~nids/docs/longjump_try_trow_catch.html)
  By Francesco Nidito

- [sauf](https://gist.github.com/kulp/1348052 "A toy exception handling system for C")
  By Darren Kulp

- [errhdl](http://www.public-software-group.org/errhdl "Error Handler for C")
  By Jan Behrens

- [A Proposal for Exception Handling in C](https://web.archive.org/web/20120122022844/http://lyngvig.org/a-proposal-for-exception-handling-in-c.ashx)
  By Mikael Lyngvig

- [Implementing Exceptions in C](http://bitsavers.informatik.uni-stuttgart.de/pdf/dec/tech_reports/SRC-RR-40.pdf)
  By Eric S. Roberts

- [Complete exception handling for pure C code](https://github.com/takanuva/ANSI-ISO-C-Exception-Handling)
  By Paulo H. "Taka" Torrens

- [OSSP ex](http://www.ossp.org/pkg/lib/ex/)
  By Ralf S. Engelschall

</details>

## Releases

This library adheres to [Semantic Versioning][SEMVER]. All notable changes for each version are documented in a
[change log][CHANGELOG].

Head over to GitHub for the [latest release][LATEST_RELEASE].

[![Latest Release][BADGE_LATEST_RELEASE]][LATEST_RELEASE]

## Source Code

The source code is [available on GitHub][SOURCE_CODE].

[![Fork me on GitHub][BADGE_GITHUB]][SOURCE_CODE]


[BADGE_GITHUB]:                 https://img.shields.io/badge/Fork_me_on_GitHub-black?logo=github
[BADGE_LATEST_RELEASE]:         https://img.shields.io/github/v/release/guillermocalvo/exceptions4c
[CHANGELOG]:                    https://github.com/guillermocalvo/exceptions4c/blob/main/CHANGELOG.md
[COMPOUND_LITERALS]:            https://gcc.gnu.org/onlinedocs/gcc-3.3/gcc/Compound-Literals.html
[DESIGNATED_INITIALIZERS]:      https://gcc.gnu.org/onlinedocs/gcc-3.3/gcc/Designated-Inits.html
[EXAMPLE]:                      example.png
[EXCEPTIONS4C_LITE]:            https://github.com/guillermocalvo/exceptions4c-lite
[EXCEPTIONS4C_PTHREADS]:        https://github.com/guillermocalvo/exceptions4c-pthreads
[EXCEPTIONS4C_PTHREADS_DOCS]:   https://exceptions4c-pthreads.guillermo.dev
[LATEST_RELEASE]:               https://github.com/guillermocalvo/exceptions4c/releases/latest
[LONGJMP]:                      https://devdocs.io/c/program/longjmp
[RESULT_LIBRARY]:               https://result.guillermo.dev/
[SEMVER]:                       https://semver.org/
[SETJMP]:                       https://devdocs.io/c/program/setjmp
[SIGFPE]:                       https://devdocs.io/c/program/sig_types "Erroneous arithmetic operation (such as divide by zero)"
[SIGNAL]:                       https://devdocs.io/c/program/signal
[SIGSEGV]:                      https://devdocs.io/c/program/sig_types "Invalid memory access (segmentation fault)"
[SIGTERM]:                      https://devdocs.io/c/program/sig_types "Termination request, sent to the program"
[SOURCE_CODE]:                  https://github.com/guillermocalvo/exceptions4c
[TYPEOF]:                       https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2899.htm
[VOLATILE]:                     https://devdocs.io/c/language/volatile
[__VA_OPT__]:                   https://open-std.org/JTC1/SC22/WG14/www/docs/n3033.htm
