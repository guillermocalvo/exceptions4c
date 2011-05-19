--------------------------------
 EXCEPTIONS4C TESTING FRAMEWORK
--------------------------------

The `exceptions4c` testing framework ensures the library's reliability and
checks that it works as expected. There are many unit tests designed for
many circumstances, and every combination of the library's features.

= Platform Requirements =

Some of the library's functionality relies on the behavior of the platform, so
the test results may vary from one specific platform to another.

For example, the operating system and compiler have to support signal handling,
otherwise the library won't be able to map signals to exceptions. Some of the
platforms the library has been tested on, fail to handle some of the signals.

In addition, some `POSIX` systems need to use the functions `sigsetjmp` and
`siglongjmp` (instead of standard `setjmp` and `longjmp`) in order to save and
restore the current set of blocked signals. The library will try to use those
functions when `_POSIX_C_SOURCE` is defined at compilation time.

Some of the unit tests check the behavior of the platform, in order to determine
which of the functionalities will not be available (and therefore will cause
the tests to fail).

= How the Tests are Organized =

The unit tests are grouped into test suites; each of one with a specific
purpose:

== Test Suite `Beginning` ==

This suite of tests checks the behavior of the library when it is used
improperly. The tests try to interact with the exception handling system without
having begun the exception context before. Consequently, the library must throw
the exception `ContextHasNotBegunYet` in order to signal the misuse to the
client.

== Test Suite `Consistency` ==

This suite of tests checks the behavior of the library when it is used in an
inconsistent way. For example, the tests try to:

  * begin twice in a row the exception context
  * end twice in a row the exception context
  * interact with the exception handling system after having ended the exception
    context
  * perform some others illegal operations, such as breaking out of a `try`
    block, etc.

Consequently, the library must throw the appropriate exception, in order to
signal the misuse to the client.

== Test Suite `Ending` ==

This suite of tests checks the behavior of the library when it is used
improperly. The tests try to exit the program without having ended the exception
context before. Consequently, the library must throw the exception
`ContextNotEnded` in order to signal the misuse to the client.

== Test Suite `Uncaught` ==

This suite of tests checks the behavior of the library when an exception is
thrown and no `catch` block is capable of handling it. The library must prevent
the execution of the next instructions following the `throw` clause, execute all
pending `finally` blocks, call the uncaught handler (if any), and then either:

  * exit the program with the status code `EXIT_FAILURE`.
  * terminate the current thread with a `PTHREAD_CANCELED` status code.

Depending on whether the library is in _thread-unsafe_ or _thread-safe_ mode.

== Test Suite `Finally` ==

This suite of tests checks the behavior of the library when an exception is
thrown and no `catch` block is capable of handling it, more specifically, they
check the execution of all pending `finally` blocks, before exiting the program.

== Test Suite `Caught` ==

This suite of tests checks the behavior of the library when an exception is
thrown and there is some `catch` block capable of handling it. The library must
prevent the execution of the next instructions following the throw clause,
execute all pending `finally` blocks, execute the appropriate `catch` block, and
then execute the last `finally` block (if there was any, next to the `catch`
block).

== Test Suite `Signals` ==

This suite of tests checks the behavior of the library when the program receives
a signal from the operating system. If no signal mapping is provided, then the
behavior is implementation-defined. However, if the signal mappings have been
set up, then the library must map the received signal to the appropriate
exception. Some of the tests will `catch` it, some others won't.

== Test Suite `Integration` ==

This suite of tests checks the behavior of the library when
`e4c_reusing_context` is used. This integration mechanism lets library
developers make use of `exceptions4c`, regardless of whether the clients are
unaware of the exception handling system. Some of the tests will propagate
exceptions to the caller, some others will return a status code.

== Test Suite `PlatformRequirements` ==

This is special test suite to check the platform on which the library is to be
compiled and executed.

= How the Source Code is Organized =

The source code is divided into:

  * test framework files
  * unit tests / test suites files

The test framework consists in 8 main files:

  * `main.c`
  * `testing.h`
  * `testing.c`
  * `html.h`
  * `html.c`
  * `macros.h`
  * `macros.c`
  * `platform.h`

The rest of the files are named after the specific unit test or test suite they
stand for:

  * `test_@??.c`
    These files stand for a single unit test inside a test suite. The test
    suite is identified by the letter and the number of the test inside the
    test is identified by the two-digit code next to the letter.

  * `suite_@.c`
    These files stand for a single test suite which contains several unit
    tests. The test suite is identified by the letter.

  * `run_@.c`
    These files stand for a testing configuration which contains only one
    specific test suite to be run. The test suite is identified by the
    letter.

  * `run__all.c`
    This file stands for a testing configuration which contains all of the test
    suites to be run.

= How to Compile the Tests =

All the files inside the
[http://code.google.com/p/exceptions4c/source/browse/#svn/trunk/test test]
directory have to be compiled and linked into a single binary executable file.

  _Note that the library itself has to be compiled along with the test files
  (it's not included in the `test` directory)._

`exceptions4c` is designed for portability and should compile without warnings
by any compiler. For example, it does on `GCC` using the flags:
  * `-ansi`
  * `-Wall`
  * `-Wextra`
  * `-pedantic`

Please report any errors or warnings, along with your platform details
(operating system, compiler).

== Makefile ==

There is a convenience `Makefile` inside the test directory. It will download
(through `wget`) any missing files and then compile them altogether.

  * http://exceptions4c.googlecode.com/svn/trunk/test/Makefile

= How to Run the Tests =

Once compiled, the executable file has to be run without any parameters.

The main program will run each unit test in a separated process. It will collect
the data and then create a file `report.html` with the results of the testing
process.

The program will also create two files `out.tmp` and `err.tmp` to temporarily
store the standard output and error for each unit test. These files will be
deleted when the process is finished.
