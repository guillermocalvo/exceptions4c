
# ifndef MACROS_H

# define MACROS_H


# define IS_DEFINED			1
# define IS_NOT_DEFINED		0


struct stateless_macro{
	const char *	name;
	int				is_defined;
	const char *	detail;
};

struct stateful_macro{
	const char *	name;
	int				is_defined;
	long			value;
	const char *	description;
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


extern struct macro_collection macros;


# endif
