
# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/).


## [Unreleased]

- Changed license from LGPL to Apache 2.
- Renamed public macros and "keywords".
- Extracted multithreading support to a separate library
  [exceptions4c-pthreads](https://github.com/guillermocalvo/exceptions4c-pthreads).
- Extracted lightweight version to a separate library
  [exceptions4c-lite](https://github.com/guillermocalvo/exceptions4c-lite).
- Removed non-essential public functions and typedefs.
- Removed built-in exceptions and signal handling.
- Removed support for legacy compilers.

### Added

- Type `e4c_exception_type`
- Type `e4c_debug_info`
- Type `e4c_exception`
- Type `e4c_context`
- Macro `EXCEPTIONS4C`
- Macro `TRY`
- Macro `CATCH`
- Macro `CATCH_ALL`
- Macro `FINALLY`
- Macro `THROW`
- Macro `RETRY`
- Macro `USING`
- Macro `WITH`
- Macro `USE`
- Macro `REACQUIRE`
- Function `e4c_set_context_supplier`
- Function `e4c_get_context`
- Function `e4c_get_exception`
- Function `e4c_is_uncaught`


## [3.0.5]

- Added lightweight version of the library.
- Added support for generating backtraces via execinfo.
- Minor bugfixes, changes and optimizations.
- Migrated repo to GitHub.


## [3.0.0]

- Changed license from GPL to LGPL.
- Minor internal changes.


## [2.11.0]

- Added custom uncaught handler mechanism.
- Added extern "C" block for C++ compatibility.
- Improved current thread termination.
- Other internal changes and bugfixes.


## [2.10.0]

- Added support for generating backtraces via addr2line.
- Fixed minor bug (dangling contexts).
- Other internal changes.
- Renamed some public macros.


## [2.9.0]

- Added custom data to thrown exception.
- Improved detection of cause exception.
- Renamed some struct members.
- Other internal changes.


## [2.8.0]

- Changed assertion macro.
- Other internal changes related to multithreading.


## [2.7.0]

- Renamed some public macros and functions.
- Internal changes and optimizations.
- Fixed signal-related bug.


## [2.6.0]

- Added `e4c_exception_type`.
- Added `snprintf` support where available.
- Removed `volatile` qualifier.


## [2.5.0]

- Enhanced specific compiler, OS, C source version, and PTHREADS detection.
- Minor changes to signal handling.
- Fixed minor bugs.


## [2.4.0]

- Enhanced compiler detection.
- Fixed minor bugs.
- Fixed compilation error on some compilers.


## [2.3.0]

- Added macros to rethrow the current exception, retry, and reacquire.


## [2.2.0]

- Added `volatile` qualifier to some struct members.
- Fixed minor bugs.


## [2.1.0]

- Added macro to throw with formatted message.
- Added function name to thrown exception.
- Added test suites.


## [2.0.0]

- Added errno, file, and line info to thrown exception.
- Added detailed message to thrown exception.
- Added cause to thrown exception.
- Added assertion mechanism.


## [1.6.0]

- Added thread-safe mode information to the library version number.
- Improved thread cancelation.


## [1.5.0]

- Added mechanism to retrieve library version.
- Enhanced dangling context detection.
- Improved PTHREADS detection.


## [1.4.0]

- Added integration macro.


## [1.3.0]

- Added dispose pattern (automatic resource disposal).
- Added multithreading support.


## [1.2.0]

- Minor changes.


## [1.1.0]

- Added signal handling.


## [1.0.0]

Initial release.


[Unreleased]: https://github.com/guillermocalvo/exceptions4c/compare/main...develop
[1.0.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v1.0.0
[1.1.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v1.1.0
[1.2.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v1.2.0
[1.3.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v1.3.0
[1.4.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v1.4.0
[1.5.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v1.5.0
[1.6.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v1.6.0
[2.0.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.0.0
[2.1.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.1.0
[2.2.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.2.0
[2.3.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.3.0
[2.4.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.4.0
[2.5.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.5.0
[2.6.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.6.0
[2.7.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.7.0
[2.8.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.8.0
[2.9.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.9.0
[2.10.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.10.0
[2.11.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v2.11.0
[3.0.0]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v3.0.0
[3.0.5]: https://github.com/guillermocalvo/exceptions4c/releases/tag/v3.0.5
