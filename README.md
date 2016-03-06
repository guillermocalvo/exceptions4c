
# exceptions4c

[![Build status][BUILD_STATUS_BADGE]][BUILD_STATUS_LINK]
[![Code coverage][COVERAGE_BADGE]][COVERAGE_LINK]
[![Static analysis][COVERITY_BADGE]][COVERITY_LINK]

> Bring the power of exceptions to your C applications
> with this tiny, portable library.

![][EXAMPLE]


## An exception handling framework for C

This library provides you with a simple set of keywords (*macros*, actually)
which map the semantics of exception handling you're probably already used to:

- `try`
- `catch`
- `finally`
- `throw`

You can use exceptions in C by writing `try/catch/finally` blocks:

```c
    #include "e4c.h"

    int foobar(){

        int foo;
        void * buffer = malloc(1024);

        if(buffer == NULL){
            throw(NotEnoughMemoryException, "Could not allocate buffer.");
        }

        try{
            foo = get_user_input(buffer, 1024);
        }catch(BadUserInputException){
            foo = 123;
        }finally{
            free(buffer);
        }

        return(foo);
    }
```

This way you will never have to deal again with boring error codes, or check
return values every time you call a function.


## Exception Hierarchies

The possible exceptions in a program are organized in a *pseudo-hierarchy* of
exceptions. `RuntimeException` is the root of the exceptions *pseudo-hierarchy*.
**Any** exception can be caught by a `catch(RuntimeException)` block, **except**
`AssertionException`.

When an exception is thrown, control is transferred to the nearest
dynamically-enclosing `catch` code block that handles the exception. Whether a
particular `catch` block handles an exception is found out by comparing the type
(and supertypes) of the actual thrown exception against the specified exception
in the `catch` clause.

A `catch` block is given an exception as a parameter. This parameter determines
the set of exceptions that can be handled by the code block. A block handles an
actual exception that was thrown if the specified parameter is either:

- the same type of that exception.
- the same type of any of the *supertypes* of that exception.

If you write a `catch` block that handles an exception with no defined
*subtype*, it will only handle that very specific exception. By grouping
exceptions in *hierarchies*, you can design generic `catch` blocks that deal
with several exceptions:

```c
    /*                   Name             Default message   Supertype */
    E4C_DEFINE_EXCEPTION(ColorException, "Colorful error.", RuntimeException);
    E4C_DEFINE_EXCEPTION(RedException,   "Red error.",      ColorException);
    E4C_DEFINE_EXCEPTION(GreenException, "Green error.",    ColorException);
    E4C_DEFINE_EXCEPTION(BlueException,  "Blue error.",     ColorException);

    ...

    try{
        int color = chooseColor();
        if(color == 0xff0000) throw(RedException, "I don't like it.");
        if(color == 0x00ff00) throw(GreenException, NULL);
        if(color == 0x0000ff) throw(BlueException, "It's way too blue.");
        doSomething(color);
    }catch(GreenException){
        printf("You cannot use green.");
    }catch(ColorException){
        const e4c_exception * e = e4c_get_exception();
        printf("You cannot use that color: %s (%s).", e->name, e->message);
    }
```

When looking for a match, `catch` blocks are inspected in the order they appear
*in the code*. If you place a handler for a superclass before a subclass
handler, the second block will in fact be **unreachable**.


## Dispose Pattern

There are other keywords related to resource handling:

- `with... use`
- `using`

They allow you to express the *Dispose Pattern* in your code:

```c
    /* syntax #1 */
    FOO f;
    with(f, e4c_dispose_FOO) f = e4c_acquire_FOO(foo, bar); use do_something(f);

    /* syntax #2 (relies on 'e4c_acquire_BAR' and 'e4c_dispose_BAR') */
    BAR bar;
    using(BAR, bar, ("BAR", 123) ){
        do_something_else(bar);
    }

    /* syntax #3 (customized to specific resource types) */
    FILE * report;
    e4c_using_file(report, "log.txt", "a"){
        fputs("hello, world!\n", report);
    }
```

This is a clean and terse way to handle all kinds of resources with implicit
acquisition and automatic disposal.


## Signal Handling

In addition, signals such as `SIGHUP`, `SIGFPE` and `SIGSEGV` can be handled in
an *exceptional* way. Forget about scary segmentation faults, all you need is to
catch `BadPointerException`:

```c
    int * pointer = NULL;

    try{
        int oops = *pointer;
    }catch(BadPointerException){
        printf("No problem ;-)");
    }
```

## Multithreading

If you are using threads in your program, you must enable the *thread-safe*
version of the library by defining `E4C_THREADSAFE` at compiler level.

The usage of the framework does not vary between single and multithreaded
programs. The same semantics apply. The only caveat is that **the behavior of
signal handling is undefined in a multithreaded program** so use this feature
with caution.


## Integration

Whether you are developing a standalone application, or an external library that
provides services to independent programs, you can integrate `exceptions4c` in
your code very easily.

The system provides a mechanism for implicit initialization and finalization of
the exception framework, so that it is safe to use `try`, `catch`, `throw`, etc.
from any external function, even if its caller is not exception-aware
whatsoever.


## Portability

This library should compile in any ANSI C compiler. It uses nothing but standard
C functions. In order to use exceptions4c you have to drop the two files
(`e4c.h` and `e4c.c`) in your project and remember to `#include` the header file
from your code.

In case your application uses threads, `exceptions4c` relies on pthreads, the
**POSIX** application programming interface for writing multithreaded
applications. This *API* is available for most operating systems and platforms.


## Lightweight Version

If you have the feeling that the standard version of **exceptions4c** may be
*a bit overkill* for your specific needs, there exists a *lightweight version*,
targeted at small projects and embedded systems. Use it when you just want to
handle error conditions that may occur in your program through a simple yet
powerful exception handling mechanism. It provides the
**core functionality of exceptions4c in less than 200 source lines of code**.


## License

This is free software: you can redistribute it and/or modify it under the terms
of the **GNU Lesser General Public License** as published by the
*Free Software Foundation*, either version 3 of the License, or (at your option)
any later version.

This software is distributed in the hope that it will be useful, but
**WITHOUT ANY WARRANTY**; without even the implied warranty of
**MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**. See the
[GNU Lesser General Public License](http://www.gnu.org/licenses/lgpl.html) for
more details.

You should have received a copy of the GNU Lesser General Public License along
with this software. If not, see <http://www.gnu.org/licenses/>.

### Required

- **License and copyright notice**: Include a copy of the license and copyright
notice with the code.
- **Library usage**: The library may be used within a non-open-source
application.
- **Disclose Source**: Source code for this library must be made available when
distributing the software.

### Permitted

- **Commercial Use**: This software and derivatives may be used for commercial
purposes.
- **Modification**: This software may be modified.
- **Distribution**: You may distribute this software.
- **Sublicensing**: You may grant a sublicense to modify and distribute this
software to third parties not included in the license.
- **Patent Grant**: This license provides an express grant of patent rights from
the contributor to the recipient.

### Forbidden

- **Hold Liable**: Software is provided without warranty and the software
author/license owner cannot be held liable for damages.


----

![LGPL][LGPL_LOGO]
![exceptions4c][E4C_LOGO]

![Code coverage][COVERAGE_GRAPH]


[BUILD_STATUS_LINK]:
https://travis-ci.org/guillermocalvo/exceptions4c

[BUILD_STATUS_BADGE]:
https://travis-ci.org/guillermocalvo/exceptions4c.svg?branch=master

[COVERAGE_LINK]:
https://codecov.io/github/guillermocalvo/exceptions4c?branch=master

[COVERAGE_BADGE]:
https://codecov.io/github/guillermocalvo/exceptions4c/coverage.svg?branch=master

[COVERAGE_GRAPH]:
https://codecov.io/github/guillermocalvo/exceptions4c/branch.svg?branch=master

[COVERITY_LINK]:
https://scan.coverity.com/projects/guillermocalvo-exceptions4c

[COVERITY_BADGE]:
https://scan.coverity.com/projects/8074/badge.svg

[EXAMPLE]:
https://raw.githubusercontent.com/guillermocalvo/exceptions4c/master/etc/img/example.png

[LGPL_LOGO]:
https://raw.githubusercontent.com/guillermocalvo/exceptions4c/master/etc/img/lgpl.png

[E4C_LOGO]:
https://raw.githubusercontent.com/guillermocalvo/exceptions4c/master/etc/img/logo/sheep_064.png
