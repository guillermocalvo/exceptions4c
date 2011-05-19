
# ifndef PLATFORM_H

/*
	Platform detection (architecture, OS and compiler) is based on this great project:

		Pre-defined C/C++ Compiler Macros
		http://predef.sourceforge.net/

	Thank you :)
*/

# define PLATFORM_H


/*
	----------------------------------------------------------------
	architecture detection
	----------------------------------------------------------------
*/

# if	defined(__alpha_ev6__)
#	define PLATFORM_ARCH "Alpha EV6"
# elif	defined(__alpha_ev5__)
#	define PLATFORM_ARCH "Alpha EV5"
# elif	defined(__alpha_ev4__)
#	define PLATFORM_ARCH "Alpha EV4"
# elif	defined(__alpha__) \
	||	defined(__alpha) \
	||	defined(_M_ALPHA)
#	define PLATFORM_ARCH "Alpha"
# elif	defined(__amd64) \
	||	defined(__amd64__) \
	||	defined(__x86_64) \
	||	defined(__x86_64__) \
	||	defined(_M_X64)
#	define PLATFORM_ARCH "AMD64"
# elif	defined(__thumb__) \
	||	defined(__TARGET_ARCH_THUMB)
#	define PLATFORM_ARCH "ARM (Thumb mode)"
# elif	defined(__arm__) \
	||	defined(_ARM) \
	||	defined(__TARGET_ARCH_ARM)
#	define PLATFORM_ARCH "ARM"
# elif	defined(__hppa__) \
	||	defined(__hppa)
#	define PLATFORM_ARCH "HP/PA RISC"
# elif	defined(__i686__) \
	||	defined(__tune_i686__) \
	||	defined(__tune_pentiumpro__) \
	||	( defined(__I86__) && (__I86__ == 6) ) \
	||	( defined(_M_IX86) && (_M_IX86 == 600) )
#	define PLATFORM_ARCH "Intel Pentium Pro/II"
# elif	defined(__i586__) \
	||	defined(__tune_i586__) \
	||	defined(__tune_pentium__) \
	||	( defined(__I86__) && (__I86__ == 5) ) \
	||	( defined(_M_IX86) && (_M_IX86 == 500) )
#	define PLATFORM_ARCH "Intel Pentium"
# elif	defined(__i486__) \
	||	defined(__tune_i486__) \
	||	( defined(__I86__) && (__I86__ == 4) ) \
	||	( defined(_M_IX86) && (_M_IX86 == 400) )
#	define PLATFORM_ARCH "Intel 80486"
# elif	defined(__i386__) \
	||	( defined(__I86__) && (__I86__ == 3) ) \
	||	( defined(_M_IX86) && (_M_IX86 == 300) )
#	define PLATFORM_ARCH "Intel 80386"
# elif	defined(_X86_) \
	||	defined(__X86__) \
	||	defined(__INTEL__) \
	||	defined(__THW_INTEL__)
#	define PLATFORM_ARCH "Intel x86"
# elif	defined(__ia64) \
	||	defined(__ia64__) \
	||	defined(_IA64) \
	||	defined(__IA64__) \
	||	defined(_M_IA64)
#	define PLATFORM_ARCH "Intel 64-bit"
# elif	defined(__mc68060__)
#	define PLATFORM_ARCH "Motorola 68060"
# elif	defined(__mc68040__)
#	define PLATFORM_ARCH "Motorola 68040"
# elif	defined(__mc68030__)
#	define PLATFORM_ARCH "Motorola 68030"
# elif	defined(__mc68020__)
#	define PLATFORM_ARCH "Motorola 68020"
# elif	defined(__mc68010__)
#	define PLATFORM_ARCH "Motorola 68010"
# elif	defined(__mc68000__)
#	define PLATFORM_ARCH "Motorola 68000"
# elif	defined(__m68k__) \
	||	defined(M68000)
#	define PLATFORM_ARCH "Motorola 68k"
# elif	defined(_R4000)
#	define PLATFORM_ARCH "MIPS R4000"
# elif	defined(_R3000)
#	define PLATFORM_ARCH "MIPS R3000"
# elif	defined(_MIPS_ISA_MIPS4) \
	||	defined(__MIPS_ISA4__) \
	||	( defined(__mips) && (__mips == 4) )
#	define PLATFORM_ARCH "MIPS R8000 or R10000"
# elif	defined(_MIPS_ISA_MIPS3) \
	||	defined(__MIPS_ISA3__) \
	||	( defined(__mips) && (__mips == 3) )
#	define PLATFORM_ARCH "MIPS R4400"
# elif	defined(_MIPS_ISA_MIPS2) \
	||	defined(__MIPS_ISA2__) \
	||	( defined(__mips) && (__mips == 2) )
#	define PLATFORM_ARCH "MIPS R6000"
# elif	defined(_MIPS_ISA_MIPS1) \
	||	( defined(__mips) && (__mips == 1) )
#	define PLATFORM_ARCH "MIPS R2000 or R3000"
# elif	defined(mips) \
	||	defined(__mips) \
	||	defined(__mips__) \
	||	defined(__MIPS__)
#	define PLATFORM_ARCH "MIPS"
# elif	( defined(_M_PPC) && (_M_PPC == 620) )
#	define PLATFORM_ARCH "PowerPC 620"
# elif	defined(__ppc604__) \
	||	defined(_ARCH_604) \
	||	( defined(_M_PPC) && (_M_PPC == 604) )
#	define PLATFORM_ARCH "PowerPC 604"
# elif	defined(__ppc603__) \
	||	defined(_ARCH_603) \
	||	( defined(_M_PPC) && (_M_PPC == 603) )
#	define PLATFORM_ARCH "PowerPC 603"
# elif	defined(__ppc601__) \
	||	defined(_ARCH_601) \
	||	( defined(_M_PPC) && (_M_PPC == 601) )
#	define PLATFORM_ARCH "PowerPC 601"
# elif	defined(__powerpc) \
	||	defined(__powerpc__) \
	||	defined(__POWERPC__) \
	||	defined(__ppc__) \
	||	defined(_M_PPC) \
	||	defined(_ARCH_PPC)
#	define PLATFORM_ARCH "PowerPC"
# elif	defined(_ARCH_PWR) \
	||	defined(_ARCH_PWR2) \
	||	defined(_POWER) \
	||	defined(_IBMR2) \
	||	defined(__THW_RS6000)
#	define PLATFORM_ARCH "RS/6000"
# elif	defined(__sparcv9)
#	define PLATFORM_ARCH "UltraSPARC"
# elif	defined(__sparcv8)
#	define PLATFORM_ARCH "SuperSPARC"
# elif	defined(__sparc) \
	||	defined(__sparc__)
#	define PLATFORM_ARCH "SPARC"
# elif	defined(__sh1__) \
	||	defined(__SH1__)
#	define PLATFORM_ARCH "SuperH version 1"
# elif	defined(__sh2__) \
	||	defined(__SH2__)
#	define PLATFORM_ARCH "SuperH version 2"
# elif	defined(__sh3__) \
	||	defined(__SH3__)
#	define PLATFORM_ARCH "SuperH version 3"
# elif	defined(__sh4__) \
	||	defined(__SH4__)
#	define PLATFORM_ARCH "SuperH version 4"
# elif	defined(__sh5__) \
	||	defined(__SH5__)
#	define PLATFORM_ARCH "SuperH version 5"
# elif	defined(__sh__) \
	||	defined(__SH__)
#	define PLATFORM_ARCH "SuperH"
# elif	defined(__370__) \
	||	defined(__THW_370__)
#	define PLATFORM_ARCH "System/370"
# elif	defined(__s390__) \
	||	defined(__s390x__)
#	define PLATFORM_ARCH "System/390"
# elif	defined(__SYSC_ZARCH__)
#	define PLATFORM_ARCH "z/Architecture"
# else
#	define PLATFORM_ARCH "Unknown architecture"
# endif


/*
	----------------------------------------------------------------
	operating system detection
	----------------------------------------------------------------
*/

# if	defined(__nucleus__)
#	define PLATFORM_OS "Nucleus OS"
# elif	defined(__minix)
#	define PLATFORM_OS "MINIX"
# elif	defined(__QNXNTO__)
#	define PLATFORM_OS "QNX 6.x"
# elif	defined(QNX)
#	define PLATFORM_OS "QNX 4.x"
# elif	defined(__ANDROID__)
#	define PLATFORM_OS "Android"
# elif	defined(__SYMBIAN32__)
#	define PLATFORM_OS "Symbian OS"
# elif	defined(_XBOX)
#	define PLATFORM_OS "XBox"
# elif	defined(__APPLE__) && defined(__MACH__)
#	define PLATFORM_OS "MacOs X"
# elif	defined(macintosh) \
	||	defined(Macintosh)
#	define PLATFORM_OS "MacOs 9"
# elif	defined(__FreeBSD) \
	||	defined(__FreeBSD__)
#	define PLATFORM_OS "FreeBSD"
# elif	defined(__NetBSD) \
	||	defined(__NetBSD__)
#	define PLATFORM_OS "NetBSD"
# elif	defined(__OpenBSD__)
#	define PLATFORM_OS "OpenBSD"
# elif	defined(__CYGWIN__)
#	define PLATFORM_OS "Cygwin"
# elif	defined(LINUX) \
	||	defined(_LINUX) \
	||	defined(__LINUX) \
	||	defined(__LINUX__) \
	||	defined(linux) \
	||	defined(_linux) \
	||	defined(__linux) \
	||	defined(__linux__) \
	||	defined(gnu_linux) \
	||	defined(_gnu_linux) \
	||	defined(__gnu_linux) \
	||	defined(__gnu_linux__)
#	define PLATFORM_OS "GNU/Linux"
# elif	defined(WINNT) \
	||	defined(_WINNT) \
	||	defined(__WINNT) \
	||	defined(__WINNT__)
#	define PLATFORM_OS "Microsoft Windows NT"
# elif	defined(WIN64) \
	||	defined(_WIN64) \
	||	defined(__WIN64) \
	||	defined(__WIN64__)
#	define PLATFORM_OS "Microsoft Windows 64-bit"
# elif	defined(_WIN32_WCE)
#	define PLATFORM_OS "Microsoft Windows CE"
# elif	defined(WIN32) \
	||	defined(_WIN32) \
	||	defined(__WIN32) \
	||	defined(__WIN32__)
#	define PLATFORM_OS "Microsoft Windows 32-bit"
# elif	defined(WINDOWS) \
	||	defined(_WINDOWS) \
	||	defined(__WINDOWS) \
	||	defined(__WINDOWS__) \
	||	defined(__TOS_WIN__)
#	define PLATFORM_OS "Microsoft Windows"
# elif	defined(MSDOS) \
	||	defined(_MSDOS) \
	||	defined(__MSDOS) \
	||	defined(__MSDOS__) \
	||	defined(__DOS__)
#	define PLATFORM_OS "Microsoft MS-DOS"
# else
#	define PLATFORM_OS "Unknown operating system"
# endif

/*
	----------------------------------------------------------------
	compiler detection
	----------------------------------------------------------------
*/

# if defined(__TURBOC__)
#	define PLATFORM_COMPILER						"Turbo C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__TURBOC__)
#	if __TURBOC__ == 397
#		define PLATFORM_COMPILER_VERSION			"2.0"
#	elif __TURBOC__ == 661
#		define PLATFORM_COMPILER_VERSION			"3.0"
#	else
#		define PLATFORM_COMPILER_VERSION			NULL
#	endif
# elif defined(__BORLANDC__) || defined(__CODEGEARC__)
#	define PLATFORM_COMPILER						"Borland C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__BORLANDC__)
#	if __BORLANDC__ < 0x200
#		define PLATFORM_COMPILER_VERSION			"2.0"
#	elif __BORLANDC__ == 0x400
#		define PLATFORM_COMPILER_VERSION			"3.0"
#	elif __BORLANDC__ == 0x410
#		define PLATFORM_COMPILER_VERSION			"3.1"
#	elif __BORLANDC__ == 0x452
#		define PLATFORM_COMPILER_VERSION			"4.0"
#	elif __BORLANDC__ == 0x500
#		define PLATFORM_COMPILER_VERSION			"5.0"
#	elif __BORLANDC__ == 0x520
#		define PLATFORM_COMPILER_VERSION			"5.02"
#	elif __BORLANDC__ == 0x530
#		define PLATFORM_COMPILER_VERSION			"Builder 3.0"
#	elif __BORLANDC__ == 0x540
#		define PLATFORM_COMPILER_VERSION			"Builder 4.0"
#	elif __BORLANDC__ == 0x550
#		define PLATFORM_COMPILER_VERSION			"Builder 5.0"
#	elif __BORLANDC__ == 0x551
#		define PLATFORM_COMPILER_VERSION			"5.51"
#	elif __BORLANDC__ == 0x613
#		define PLATFORM_COMPILER_VERSION			"Builder 2009"
#	elif __BORLANDC__ == 0x621
#		define PLATFORM_COMPILER_VERSION			"Builder 2010"
#	else
#		define PLATFORM_COMPILER_VERSION			NULL
#	endif
# elif defined(__DECCXX)
#	define PLATFORM_COMPILER						"Compaq C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__DECCXX_VER)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__DECC)
#	define PLATFORM_COMPILER						"Compaq C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__DECC_VER)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(_CRAYC)
#	define PLATFORM_COMPILER						"Cray C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			( (_RELEASE) * 1000 + (_RELEASE_MINOR) )
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__DCC__)
#	define PLATFORM_COMPILER						"Diab C/C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__VERSION_NUMBER__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__SYSC__)
#	define PLATFORM_COMPILER						"Dignus Systems C/C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__SYSC_VER__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__PATHCC__)
#	define PLATFORM_COMPILER						"EKOPath"
#	define PLATFORM_COMPILER_VERSION_NUMBER			( (__PATHCC__) * 1000 + (__PATHCC_MINOR__) )
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__EDG__)
#	define PLATFORM_COMPILER						"EDG C++ Front End"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__EDG_VERSION__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__ghs__)
#	define PLATFORM_COMPILER						"Green Hill C/C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__GHS_VERSION_NUMBER__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__HP_cc)
#	define PLATFORM_COMPILER						"HP ANSI C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__HP_aCC)
#	define PLATFORM_COMPILER						"HP aCC"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__HP_aCC)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__IAR_SYSTEMS_ICC__)
#	define PLATFORM_COMPILER						"IAR C/C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__VER__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__xlc__)
#	define PLATFORM_COMPILER						"IBM XL C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__xlc__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__xlC__)
#	define PLATFORM_COMPILER						"IBM XL C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__xlC__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__IBMC__)
#	define PLATFORM_COMPILER						"IBM z/OS C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__COMPILER_VER__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__IBMCPP__)
#	define PLATFORM_COMPILER						"IBM z/OS C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__xlc__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__IMAGECRAFT__)
#	define PLATFORM_COMPILER						"ImageCraft C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__INTEL_COMPILER)
#	define PLATFORM_COMPILER						"Intel C/C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__INTEL_COMPILER)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__KCC)
#	define PLATFORM_COMPILER						"KAI C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__KCC_VERSION)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__KEIL__) || defined(__CA__)
#	define PLATFORM_COMPILER						"Keil CARM"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__CA__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__C166__)
#	define PLATFORM_COMPILER						"Keil C166"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__C166__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__C51__) || defined(__CX51__)
#	define PLATFORM_COMPILER						"Keil C51"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__C166__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__HIGHC__)
#	define PLATFORM_COMPILER						"MetaWare High C/C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__MWERKS__)
#	define PLATFORM_COMPILER						"Metrowerks CodeWarrior"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__MWERKS__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__sgi) || defined(sgi)
#	define PLATFORM_COMPILER						"MIPSpro"
#	if defined(_SGI_COMPILER_VERSION)
#		define PLATFORM_COMPILER_VERSION_NUMBER		(_SGI_COMPILER_VERSION)
#	else
#		define PLATFORM_COMPILER_VERSION_NUMBER		(_COMPILER_VERSION)
#	endif
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__MRC__)
#	if defined(MPW_CPLUS)
#		define PLATFORM_COMPILER					"MPW C++"
#	else
#		define PLATFORM_COMPILER					"MPW C"
#	endif
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__MRC__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(_MRI)
#	define PLATFORM_COMPILER						"Microtec C/C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__CC_NORCROFT)
#	define PLATFORM_COMPILER						"Norcroft C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__ARMCC_VERSION)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(_PACC_VER)
#	define PLATFORM_COMPILER						"Palm C/C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(_PACC_VER)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__PGI)
#	define PLATFORM_COMPILER						"Portland Group C/C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__CC_ARM)
#	define PLATFORM_COMPILER						"RealView C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__ARMCC_VERSION)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__RENESAS__) || defined(__HITACHI__)
#	define PLATFORM_COMPILER						"Renesas C/C++"
#	if defined(__RENESAS_VERSION__)
#		define PLATFORM_COMPILER_VERSION_NUMBER		(__RENESAS_VERSION__)
#	else
#		define PLATFORM_COMPILER_VERSION_NUMBER		(__HITACHI_VERSION__)
#	endif
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(SASC) || defined(__SASC) || defined(__SASC__)
#	define PLATFORM_COMPILER						"SAS/C"
#	if defined(__SASC__)
#		define PLATFORM_COMPILER_VERSION_NUMBER		(__SASC__)
#	else
#		define PLATFORM_COMPILER_VERSION_NUMBER		( (__VERSION__) * 1000 + (__REVISION__) )
#	endif
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(_SCO_DS)
#	define PLATFORM_COMPILER						"SCO OpenServer"
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(SDCC)
#	define PLATFORM_COMPILER						"Small Device C Compiler"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(SDCC)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__SUNPRO_C)
#	define PLATFORM_COMPILER						"Sun Studio C Compiler"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__SUNPRO_C)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__SUNPRO_CC)
#	define PLATFORM_COMPILER						"Sun Studio C++ Compiler"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__SUNPRO_CC)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__TenDRA__)
#	define PLATFORM_COMPILER						"TenDRA C/C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(_UCC)
#	define PLATFORM_COMPILER						"Ultimate C/C++"
#	define PLATFORM_COMPILER_VERSION_NUMBER			( (_MAJOR_REV) * 1000 + (_MINOR_REV) )
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__USLC__)
#	define PLATFORM_COMPILER						"USL C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__SCO_VERSION)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__WATCOMC__)
#	define PLATFORM_COMPILER						"Open Watcom"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__WATCOMC__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__DMC__)
#	define PLATFORM_COMPILER						"Digital Mars"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__DMC__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__DJGPP__) || defined(__GO32__)
#	define PLATFORM_COMPILER						"DJGPP"
#	define PLATFORM_COMPILER_VERSION_NUMBER			( (__DJGPP__) * 1000 + (__DJGPP_MINOR__) )
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__COMO__)
#	define PLATFORM_COMPILER						"Comeau"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__COMO_VERSION__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__PACIFIC__)
#	define PLATFORM_COMPILER						"Pacific C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__POCC__)
#	define PLATFORM_COMPILER						"Pelles C"
#	define PLATFORM_COMPILER_VERSION_NUMBER			(__POCC__)
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(MSC) || defined(_MSC_VER) || defined(_MSC_FULL_VER)
#	define PLATFORM_COMPILER						"Microsoft Visual C"
#	if defined(_MSC_VER)
#		define PLATFORM_COMPILER_VERSION_NUMBER		(_MSC_VER)
#		if _MSC_VER == 800
#			define PLATFORM_COMPILER_VERSION		"1.0"
#		elif _MSC_VER == 900
#			define PLATFORM_COMPILER_VERSION		"3.0"
#		elif _MSC_VER == 1000
#			define PLATFORM_COMPILER_VERSION		"4.0"
#		elif _MSC_VER == 1020
#			define PLATFORM_COMPILER_VERSION		"4.2"
#		elif _MSC_VER == 1100
#			define PLATFORM_COMPILER_VERSION		"5.0"
#		elif _MSC_VER == 1200
#			if defined(_MSC_FULL_VER) && _MSC_FULL_VER == 12008804
#				define PLATFORM_COMPILER_VERSION	"6.0 SP6"
#			else
#				define PLATFORM_COMPILER_VERSION	"6.0"
#			endif
#		elif _MSC_VER == 1300
#			define PLATFORM_COMPILER_VERSION		"7.0"
#		elif _MSC_VER == 1310
#			define PLATFORM_COMPILER_VERSION		"7.1"
#		elif _MSC_VER == 1400
#			define PLATFORM_COMPILER_VERSION		"8.0"
#		elif _MSC_VER == 1500
#			define PLATFORM_COMPILER_VERSION		"9.0"
#		elif _MSC_VER == 1600
#			define PLATFORM_COMPILER_VERSION		"10.0"
#		else
#			define PLATFORM_COMPILER_VERSION		NULL
#		endif
#	elif defined(_MSC_FULL_VER)
#		define PLATFORM_COMPILER_VERSION_NUMBER		(_MSC_FULL_VER)
#		if _MSC_FULL_VER == 150021022
#			define PLATFORM_COMPILER_VERSION		"9.0"
#		elif _MSC_FULL_VER == 140050727
#			define PLATFORM_COMPILER_VERSION		"8.0"
#		elif _MSC_FULL_VER == 13103077
#			define PLATFORM_COMPILER_VERSION		"7.1"
#		elif _MSC_FULL_VER == 13009466
#			define PLATFORM_COMPILER_VERSION		"7.0"
#		elif _MSC_FULL_VER == 12008804
#			define PLATFORM_COMPILER_VERSION		"6.0 SP6"
#		else
#			define PLATFORM_COMPILER_VERSION		NULL
#		endif
#	else
#		define PLATFORM_COMPILER_VERSION			NULL
#		define PLATFORM_COMPILER_VERSION_NUMBER		-1L
#	endif
# elif defined(__TINYC__)
#	define PLATFORM_COMPILER						"Tiny C Compiler"
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__clang__)
#	define PLATFORM_COMPILER						"Clang"
#	define PLATFORM_COMPILER_VERSION_NUMBER			( (__clang_major__) * 1000 + (__clang_minor__) )
#	define PLATFORM_COMPILER_VERSION				__clang_version__
# elif defined(__llvm__)
#	define PLATFORM_COMPILER						"LLVM"
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__LCC__)
#	define PLATFORM_COMPILER						"Lcc-Win32"
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
#	define PLATFORM_COMPILER_VERSION				NULL
# elif defined(__MINGW32__)
#	define PLATFORM_COMPILER						"MinGW"
#	define PLATFORM_COMPILER_VERSION				__VERSION__
#	define PLATFORM_COMPILER_VERSION_NUMBER			( (__MINGW32_MAJOR_VERSION) * 1000 + (__MINGW32_MINOR_VERSION) )
# elif defined(__GNUC__)
#	define PLATFORM_COMPILER						"GCC"
#	define PLATFORM_COMPILER_VERSION				__VERSION__
#	define PLATFORM_COMPILER_VERSION_NUMBER			( (__GNUC__) * 1000 + (__GNUC_MINOR__) )
# else
#	define PLATFORM_COMPILER						"Unknown compiler"
#	define PLATFORM_COMPILER_VERSION				""
#	define PLATFORM_COMPILER_VERSION_NUMBER			-1L
# endif


# endif
