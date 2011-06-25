
# include "macros.h"
# include "stdio.h"


struct macro_collection macros = {
	{
		"E4C_THREADSAFE",
#	ifdef E4C_THREADSAFE
		MACRO_DEFINED, "Multi-thread version of exceptions4c"
#	else
		MACRO_NOT_DEFINED, "Single-thread version of exceptions4c"
#	endif
	},{
		"E4C_NOKEYWORDS",
#	ifdef E4C_NOKEYWORDS
		MACRO_DEFINED, "Only uppercase keywords (<code>E4C_TRY</code>, <code>E4C_CATCH</code>, <code>E4C_FINALLY</code>)"
#	else
		MACRO_NOT_DEFINED, "Lowercase keywords (<code>try</code>, <code>catch</code>, <code>finally</code>...)"
#	endif
	},{
		"NDEBUG",
#	ifdef NDEBUG
		MACRO_DEFINED, "No debug <em>(assertions disabled)</em>"
#	else
		MACRO_NOT_DEFINED, "Debug mode <em>(assertions enabled)</em>"
#	endif
	},{
		"__STDC__",
#	ifdef __STDC__
		MACRO_DEFINED, "C89 <em>(ANSI X3.159-1989)</em>"
#	else
		MACRO_NOT_DEFINED, "Possibly Pre-C89"
#	endif
	},{
		"__STRICT_ANSI__",
#	ifdef __STRICT_ANSI__
		MACRO_DEFINED, "Strictly ISO standard C"
#	else
		MACRO_NOT_DEFINED, "Possibly relaxed standard C, with some platform-specific features"
#	endif
	},{
		"_ISOC99_SOURCE",
#	ifdef _ISOC99_SOURCE
		MACRO_DEFINED, "C99 <em>(ISO/IEC 9899:1999)</em>"
#	else
		MACRO_NOT_DEFINED, "Possibly Pre-C99"
#	endif
	},{
		"_POSIX_SOURCE",
#	ifdef _POSIX_SOURCE
		MACRO_DEFINED, "POSIX.1-1990 <em>(includes C90)</em>"
#	else
		MACRO_NOT_DEFINED, "Possibly not POSIX compliant"
#	endif
	},{
		"_XOPEN_SOURCE_EXTENDED",
#	ifdef _XOPEN_SOURCE_EXTENDED
		MACRO_DEFINED, "X/Open Portability Guide, plus all necessary functions for the X/Open Unix brand."
#	else
		MACRO_NOT_DEFINED, "Not all necessary functions for the X/Open Unix brand are included."
#	endif
	},{
		"_BSD_SOURCE",
#	ifdef _BSD_SOURCE
		MACRO_DEFINED, "Functionality derived from 4.3 BSD Unix is included as well as the ISO C, POSIX.1, and POSIX.2 material"
#	else
		MACRO_NOT_DEFINED, "Functionality derived from 4.3 BSD Unix is not included"
#	endif
	},{
		"_SVID_SOURCE",
#	ifdef _SVID_SOURCE
		MACRO_DEFINED, "Functionality derived from System V Interface Definition is included as well as the ISO C, POSIX.1, POSIX.2, and X/Open material"
#	else
		MACRO_NOT_DEFINED, "Functionality derived from System V Interface Definition is not included"
#	endif
	},{
		"_GNU_SOURCE",
#	ifdef _GNU_SOURCE
		MACRO_DEFINED, "All functionality included: ISO C89, ISO C99, POSIX.1, POSIX.2, BSD, SVID, X/Open, LFS, and GNU extensions."
#	else
		MACRO_NOT_DEFINED, "Not all GCC functionality is included."
#	endif
	},{
		"_REENTRANT",
#	ifdef _REENTRANT
		MACRO_DEFINED, "Reentrant versions of several functions are included"
#	else
		MACRO_NOT_DEFINED, "Reentrant versions of functions are not included"
#	endif
	},{
		"_THREAD_SAFE",
#	ifdef _THREAD_SAFE
		MACRO_DEFINED, "Reentrant versions of several functions are included"
#	else
		MACRO_NOT_DEFINED, "Reentrant versions of functions are not included"
#	endif
	},{
		"__STDC_VERSION__",
#	ifdef __STDC_VERSION__
#		if __STDC_VERSION__ == 199901L
			MACRO_DEFINED, __STDC_VERSION__, "C99", "ISO/IEC 9899:1999"
#		elif __STDC_VERSION__ == 199409L
			MACRO_DEFINED, __STDC_VERSION__, "C94", "ISO/IEC 9899-1:1994"
#		else
			MACRO_DEFINED, __STDC_VERSION__, "C90", "ISO/IEC 9899:1990"
#		endif
#	else
		MACRO_NOT_DEFINED, 0L, "Possibly Pre-C90", NULL
#	endif
	},{
		"__cplusplus",
#	ifdef __cplusplus
#		if __cplusplus == 199707L
			MACRO_DEFINED, __cplusplus, "C++98", "ISO/IEC 14882:1998"
#		else
			MACRO_DEFINED, __cplusplus, "C++", "Unknown specification"
#		endif
#	else
		MACRO_NOT_DEFINED, 0L, "Possibly not C++", NULL
#	endif
	},{
		"_POSIX_C_SOURCE",
#	ifdef _POSIX_C_SOURCE
#		if _POSIX_C_SOURCE >= 200809L
			MACRO_DEFINED, _POSIX_C_SOURCE, "POSIX.1-2008", "Includes C90, real-time, threads; excludes XSI"
#		elif _POSIX_C_SOURCE >= 200112L
			MACRO_DEFINED, _POSIX_C_SOURCE, "POSIX.1-2001", "Includes C90, real-time, threads; excludes XSI"
#		elif _POSIX_C_SOURCE >= 199506L
			MACRO_DEFINED, _POSIX_C_SOURCE, "POSIX.1c", "Includes C90, real-time, threads"
#		elif _POSIX_C_SOURCE >= 199309L
			MACRO_DEFINED, _POSIX_C_SOURCE, "POSIX.1b", "Includes C90, real-time"
#		elif _POSIX_C_SOURCE >= 2
			MACRO_DEFINED, _POSIX_C_SOURCE, "POSIX.2-1992", "Includes C90"
#		else
			MACRO_DEFINED, _POSIX_C_SOURCE, "POSIX.1-1990", "Includes C90"
#		endif
#	else
		MACRO_NOT_DEFINED, 0L, "Possibly not POSIX compliant", NULL
#	endif
	},{
		"_POSIX_VERSION",
#	ifdef _POSIX_VERSION
#		if _POSIX_VERSION == 200112L
			MACRO_DEFINED, _POSIX_VERSION, "POSIX.1-2001", "IEEE 1003.1-2001"
#		elif _POSIX_VERSION == 199506L
			MACRO_DEFINED, _POSIX_VERSION, "POSIX.1-1996", "IEEE 1003.1-1996"
#		elif _POSIX_VERSION == 199309L
			MACRO_DEFINED, _POSIX_VERSION, "POSIX.1b-1993", "IEEE 1003.1b-1993"
#		elif _POSIX_VERSION == 199009L
			MACRO_DEFINED, _POSIX_VERSION, "POSIX.1-1990", "ISO/IEC 9945-1:1990"
#		elif _POSIX_VERSION == 198808L
			MACRO_DEFINED, _POSIX_VERSION, "POSIX.1-1988", "No specification"
#		else
			MACRO_DEFINED, _POSIX_VERSION, "POSIX.1", "Unknown specification"
#		endif
#	else
		MACRO_NOT_DEFINED, 0L, "Possibly not POSIX compliant", NULL
#	endif
	},{
		"_POSIX2_C_VERSION",
#	ifdef _POSIX2_C_VERSION
#		if _POSIX2_C_VERSION == 199209L
			MACRO_DEFINED, _POSIX2_C_VERSION, "POSIX.2",	"ISO/IEC 9945-2:1993"
#		else
			MACRO_DEFINED, _POSIX2_C_VERSION, "POSIX.2", "Unknown specification"
#		endif
#	else
		MACRO_NOT_DEFINED, 0L, "Possibly not POSIX.2 compliant", NULL
#	endif
	},{
		"_XOPEN_SOURCE",
#	ifdef _XOPEN_SOURCE
#		if _XOPEN_SOURCE == 700
			MACRO_DEFINED, _XOPEN_SOURCE, "SUSv4", "UNIX04: Open Group Single UNIX Specification, Version 4"
#		elif _XOPEN_SOURCE == 600
			MACRO_DEFINED, _XOPEN_SOURCE, "SUSv3", "UNIX03: Open Group Single UNIX Specification, Version 3"
#		elif _XOPEN_SOURCE == 500
			MACRO_DEFINED, _XOPEN_SOURCE, "SUSv2", "UNIX98: X/Open Single UNIX Specification, Version 2"
#		elif _XOPEN_SOURCE == 4
#			if defined(_XOPEN_UNIX)
				MACRO_DEFINED, _XOPEN_SOURCE, "SUS", "UNIX95: X/Open Single UNIX Specification"
#			else
				MACRO_DEFINED, _XOPEN_SOURCE, "XPG4", "X/Open Portability Guide 4, 1992"
#			endif
#		elif _XOPEN_SOURCE == 3
			MACRO_DEFINED, _XOPEN_SOURCE, "XPG3", "X/Open Portability Guide 3, 1989"
#		else
			MACRO_DEFINED, _XOPEN_SOURCE, "X/Open", "Unknown specification"
#		endif
#	else
		MACRO_NOT_DEFINED, 0L, "Possibly not X/Open compliant", NULL
#	endif
	},{
		"_XOPEN_VERSION",
#	ifdef _XOPEN_VERSION
#		if _XOPEN_VERSION == 700
			MACRO_DEFINED, _XOPEN_VERSION, "SUSv4", "UNIX04: Open Group Single UNIX Specification, Version 4"
#		elif _XOPEN_VERSION == 600
			MACRO_DEFINED, _XOPEN_VERSION, "SUSv3", "UNIX03: Open Group Single UNIX Specification, Version 3"
#		elif _XOPEN_VERSION == 500
			MACRO_DEFINED, _XOPEN_VERSION, "SUSv2", "UNIX98: X/Open Single UNIX Specification, Version 2"
#		elif _XOPEN_VERSION == 4
#			if defined(_XOPEN_UNIX)
				MACRO_DEFINED, _XOPEN_VERSION, "SUS", "UNIX95: X/Open Single UNIX Specification"
#			else
				MACRO_DEFINED, _XOPEN_VERSION, "XPG4", "X/Open Portability Guide 4, 1992"
#			endif
#		elif _XOPEN_VERSION == 3
			MACRO_DEFINED, _XOPEN_VERSION, "XPG3", "X/Open Portability Guide 3, 1989"
#		else
			MACRO_DEFINED, _XOPEN_VERSION, "X/Open", "Unknown specification"
#		endif
#	else
		MACRO_NOT_DEFINED, 0L, "Possibly not X/Open compliant", NULL
#	endif
	}
};
