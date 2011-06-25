
# ifndef MACROS_H

# define MACROS_H


/*@-exportany@*/


# define MACRO_DEFINED			1
# define MACRO_NOT_DEFINED		0

# define IS_DEFINED(macro)		( macro.is_defined == MACRO_DEFINED )
# define IS_NOT_DEFINED(macro)	( macro.is_defined == MACRO_NOT_DEFINED )


struct stateless_macro{
	/*@observer@*/
	const char *	name;
	int				is_defined;
	/*@observer@*/ /*@null@*/
	const char *	detail;
};

struct stateful_macro{
	/*@observer@*/
	const char *	name;
	int				is_defined;
	long			value;
	/*@observer@*/
	const char *	description;
	/*@observer@*/ /*@null@*/
	const char *	detail;
};

struct macro_collection{

	struct stateless_macro	e4c_threadsafe;
	struct stateless_macro	e4c_nokeywords;
	struct stateless_macro	ndebug;
	struct stateless_macro	stdc;
	struct stateless_macro	strict_ansi;
	struct stateless_macro	isoc99_source;
	struct stateless_macro	posix_source;
	struct stateless_macro	xopen_source_extended;
	struct stateless_macro	bsd_source;
	struct stateless_macro	svid_source;
	struct stateless_macro	gnu_source;
	struct stateless_macro	reentrant;
	struct stateless_macro	thread_safe;

	struct stateful_macro	stdc_version;
	struct stateful_macro	cplusplus;
	struct stateful_macro	posix_c_source;
	struct stateful_macro	posix_version;
	struct stateful_macro	posix2_c_version;
	struct stateful_macro	xopen_source;
	struct stateful_macro	xopen_version;
};

/*@unchecked@*/
extern struct macro_collection macros;


/*@=exportany@*/


# endif
