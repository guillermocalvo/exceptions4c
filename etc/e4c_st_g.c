/*
 *
 * @file		e4c_st_g.c
 *
 * exceptions4c gcc stack trace source code file
 *
 * @version		2.0
 * @author		Copyright (c) 2012 Guillermo Calvo
 *
 * This is free software: you can redistribute it and/or modify it under the
 * terms of the **GNU Lesser General Public License** as published by the
 * *Free Software Foundation*, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * **WITHOUT ANY WARRANTY**; without even the implied warranty of
 * **MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**. See the
 * [GNU Lesser General Public License](http://www.gnu.org/licenses/lgpl.html)
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 * this file is undocumented on purpose (everything is documented in the header)
 */


# include <stdio.h>
# include <string.h>
# include "e4c_st_g.h"


# ifndef E4C_STACK_TRACE_ADDR2LINE_PATH
#	define E4C_STACK_TRACE_ADDR2LINE_PATH			"addr2line"
# endif

# ifndef E4C_STACK_TRACE_ADDR2LINE_OPTIONS
#	define E4C_STACK_TRACE_ADDR2LINE_OPTIONS		"-s -f -e"
# endif

# ifndef E4C_STACK_TRACE_ADDR2LINE_BUFFER_SIZE
#	define E4C_STACK_TRACE_ADDR2LINE_BUFFER_SIZE	512
# endif

# ifndef E4C_STACK_TRACE_PRINTF_PRINT_POINTER
#	define E4C_STACK_TRACE_PRINTF_PRINT_POINTER		"%p"
# endif

# ifndef E4C_STACK_TRACE_PRINTF_POINTER_SIZE
#	define E4C_STACK_TRACE_PRINTF_POINTER_SIZE		( 2 * sizeof(void *) )
# endif

# ifndef E4C_STACK_TRACE_MAX_FUNCTION_CALLS
#	define E4C_STACK_TRACE_MAX_FUNCTION_CALLS		256
# endif

# ifndef E4C_STACK_TRACE_MAX_FILE_PATH_LENGTH
#	define E4C_STACK_TRACE_MAX_FILE_PATH_LENGTH		260
# endif

# ifndef E4C_STACK_TRACE_MAX_FUNCTION_NAME_LENGTH
#	define E4C_STACK_TRACE_MAX_FUNCTION_NAME_LENGTH	48
# endif

# ifndef E4C_STACK_TRACE_EXCLUDE_FUNCTIONS
#	define E4C_STACK_TRACE_EXCLUDE_FUNCTIONS		"e4c_", "_e4c_", "__", "??"
# endif


typedef struct call_site_array e4c_call_site_array;
typedef struct debug_info_struct e4c_debug_info;
typedef struct call_frame_struct e4c_call_frame;
typedef struct call_stack_struct e4c_call_stack;

struct call_site{

	/*@observer@*/ /*@null@*/
	const void *		caller;
	/*@observer@*/ /*@null@*/
	const void *		callee;
};

struct call_site_array{

	/*@observer@*/ /*@notnull@*/
	const char *		binary_path;
	int					size;
	struct call_site	call_site[E4C_STACK_TRACE_MAX_FUNCTION_CALLS];
};

struct debug_info_struct{

	/*@observer@*/ /*@null@*/
	const void *		address;
	int					line_number;
	char				function_name[E4C_STACK_TRACE_MAX_FUNCTION_NAME_LENGTH];
	char				file_path[E4C_STACK_TRACE_MAX_FILE_PATH_LENGTH];
};

struct call_frame_struct{

	e4c_debug_info		caller;
	e4c_debug_info		callee;
	/*@observer@*/ /*@null@*/
	e4c_call_frame *	next;
	/*@observer@*/ /*@null@*/
	e4c_call_frame *	previous;
};

struct call_stack_struct{

	/*@observer@*/ /*@notnull@*/
	const char *		binary_path;
	int					size;
	/*@null@*/
	e4c_call_frame *	first_frame;
	/*@null@*/
	e4c_call_frame *	last_frame;
};


# define STRING_BEGINS_WITH(STRING, PREFIX)			( strstr(STRING, PREFIX) == STRING )
# define STR_EQUALS(STR1, STR2)						( strcmp(STR1, STR2) == 0 )
# define DO_NOT_TRACE_FUNCTION						__attribute__ ((no_instrument_function))
# define UNUSED_FUNCTION							__attribute__ ((unused))
# define PRINTF_PRINT_2_POINTERS					" " E4C_STACK_TRACE_PRINTF_PRINT_POINTER " " E4C_STACK_TRACE_PRINTF_PRINT_POINTER
# define SCANF_IGNORE_NEW_LINE						"%*[\n\r]"
# define SCANF_IGNORE_COLON							"%*[:]"
# define SCANF_READ_LINE							"%255[^\n\r]"
# define SCANF_READ_STRING							"%255s"
# define SCANF_READ_TWO_CHARS						"%2c"
# define SCANF_READ_UP_TO_COLON						"%253[^:]"
# define SCANF_READ_INTEGER							"%d"
# define SCANF_PATTERN \
			SCANF_READ_LINE SCANF_IGNORE_NEW_LINE \
			SCANF_READ_TWO_CHARS SCANF_READ_UP_TO_COLON SCANF_IGNORE_COLON \
			SCANF_READ_INTEGER SCANF_IGNORE_NEW_LINE
# define PARSE_DEBUG_INFO(FILE, DEBUG_INFO) \
			( \
				fscanf(FILE, SCANF_PATTERN, \
				(DEBUG_INFO)->function_name, \
				(DEBUG_INFO)->file_path, \
				&(DEBUG_INFO)->file_path[2], \
				&(DEBUG_INFO)->line_number) == 4 \
			)

# ifdef S_SPLINT_S
#	define popen	fopen
#	define pclose	fclose
# endif


static e4c_call_site_array call_site_array = {
	"",
	0,
	/*@-initallelements@*/
	{
		{NULL, NULL}
	}
	/*@=initallelements@*/
};


static
void
__cyg_profile_func_enter(
	/*@observer@*/
	void * callee,
	/*@observer@*/
	void * caller
) DO_NOT_TRACE_FUNCTION UNUSED_FUNCTION
/*@globals
	call_site_array
@*/
/*@modifies
	call_site_array
@*/
;

static
void
__cyg_profile_func_exit(
	/*@observer@*/
	void * callee,
	/*@observer@*/
	void * caller
) DO_NOT_TRACE_FUNCTION UNUSED_FUNCTION
/*@globals
	call_site_array
@*/
/*@modifies
	call_site_array
@*/
;

static
/*@null@*/
e4c_call_frame *
_e4c_parse_call_frame_array(
	e4c_call_site_array * call_site_array
) DO_NOT_TRACE_FUNCTION
/*@globals
	fileSystem
@*/
/*@modifies
	fileSystem
@*/
/*@requires maxRead(call_site_array->call_site) >= call_site_array->size @*/
;

inline static
/*@null@*/
e4c_call_stack *
_e4c_parse_call_stack(
	e4c_call_site_array * call_site_array
) DO_NOT_TRACE_FUNCTION
/*@globals
	fileSystem
@*/
/*@modifies
	fileSystem
@*/
/*@requires maxRead(call_site_array->call_site) >= call_site_array->size @*/
;

inline static
/*@null@*/
e4c_call_site_array *
_e4c_capture_call_site_array(
	/*@observer@*/ /*@notnull@*/
	const char * binary_path
) DO_NOT_TRACE_FUNCTION
/*@globals
	call_site_array
@*/
;

inline static
int
_e4c_print_call_frame(
	/*@temp@*/ /*@notnull@*/
	const char * binary_path,
	/*@temp@*/ /*@notnull@*/
	e4c_debug_info * debug_info,
	/*@temp@*/ /*@notnull@*/
	const char * * prefix_exclude,
	int print_line
) DO_NOT_TRACE_FUNCTION
/*@globals
	fileSystem
@*/
/*@modifies
	fileSystem
@*/
/*@requires
	maxRead(debug_info->function_name) >= 0
	/\
	maxRead(prefix_exclude) >= 0
@*/
;

inline static
void
_e4c_print_call_stack(
	/*@temp@*/ /*@notnull@*/
	e4c_call_stack * call_stack,
	/*@temp@*/ /*@notnull@*/
	const char * * prefix_exclude,
	int max
)
/*@globals
	fileSystem
@*/
/*@modifies
	fileSystem
@*/
;

inline static
void
_e4c_print_exception(
	/*@temp@*/ /*@notnull@*/
	const e4c_exception * exception,
	int is_cause,
	/*@temp@*/ /*@notnull@*/
	const char * * prefix_exclude,
	int max
) DO_NOT_TRACE_FUNCTION
/*@globals
	fileSystem
@*/
/*@modifies
	fileSystem
@*/
;


static void __cyg_profile_func_enter(void * callee, void * caller){

	if(call_site_array.size < E4C_STACK_TRACE_MAX_FUNCTION_CALLS){
		call_site_array.call_site[call_site_array.size].callee = callee;
		call_site_array.call_site[call_site_array.size].caller = caller;
	}

	call_site_array.size++;
}

static void __cyg_profile_func_exit(void * callee, void * caller){

	if(call_site_array.size == 0){
		return;
	}

	call_site_array.size--;
}

static e4c_call_frame * _e4c_parse_call_frame_array(e4c_call_site_array * call_site_array){

	e4c_call_frame * call_frame = NULL;

	if(call_site_array->size > 0){

		size_t	command_size;
		char *	command;

		command_size= 2 * ( /*@-sizeoftype@*/ E4C_STACK_TRACE_PRINTF_POINTER_SIZE /*@=sizeoftype@*/ + 1) * call_site_array->size + E4C_STACK_TRACE_ADDR2LINE_BUFFER_SIZE;
		command		= calloc( command_size, (size_t)1 );

		if(command != NULL){

			call_frame	= calloc( (size_t)call_site_array->size, sizeof(*call_frame) );

			if(call_frame != NULL){

				FILE *	pipe;
				int		index;
				int		written;

				written = snprintf(command, command_size, "%s %s \"%s\"", E4C_STACK_TRACE_ADDR2LINE_PATH, E4C_STACK_TRACE_ADDR2LINE_OPTIONS, call_site_array->binary_path);

				for(index = 0; index < call_site_array->size; index++){

					written += snprintf(&command[written], command_size - written, PRINTF_PRINT_2_POINTERS, call_site_array->call_site[index].caller, call_site_array->call_site[index].callee);

					call_frame[index].caller.address		= call_site_array->call_site[index].caller;
					call_frame[index].callee.address		= call_site_array->call_site[index].callee;
					call_frame[index].caller.line_number	= 0;
					call_frame[index].callee.line_number	= 0;
					*call_frame[index].caller.function_name	= '\0';
					*call_frame[index].callee.function_name	= '\0';
					*call_frame[index].caller.file_path		= '\0';
					*call_frame[index].callee.file_path		= '\0';
					call_frame[index].previous				= ( index == 0 ? NULL : &call_frame[index - 1] );
					call_frame[index].next					= ( index + 1 == call_site_array->size ? NULL : &call_frame[index + 1] );
				}

				(void)snprintf(&command[written], command_size - written, " 2>&1");

				pipe = popen(command, "r");
				if(pipe != NULL){

					for(index = 0; index < call_site_array->size; index++){

						if( !PARSE_DEBUG_INFO(pipe, &call_frame[index].caller) || !PARSE_DEBUG_INFO(pipe, &call_frame[index].callee) ){

							break;
						}
					}

					(void)pclose(pipe);
				}
			}

			free(command);
		}

	}

	return(call_frame);
}

inline static int _e4c_print_call_frame(const char * binary_path, e4c_debug_info * debug_info, const char * * prefix_exclude, int print_line){

	if(*debug_info->function_name != '\0'){

		if(prefix_exclude != NULL){
			for(; *prefix_exclude != NULL; prefix_exclude++){
				if( STRING_BEGINS_WITH(debug_info->function_name, *prefix_exclude) ){
					return(0);
				}
			}
		}

		if(print_line){
			printf("    from %s (%s:%d)\n",
				debug_info->function_name,
				debug_info->file_path,
				debug_info->line_number
			);
		}

	}else if(print_line){
		printf("    from %s @ %p\n", binary_path, debug_info->address);
	}

	return(1);
}

inline static e4c_call_stack * _e4c_parse_call_stack(e4c_call_site_array * call_site_array){

	e4c_call_stack *	call_stack = NULL;

	if(call_site_array != NULL){

		call_stack				= malloc( sizeof(*call_stack) );

		if(call_stack != NULL){

			call_stack->binary_path = call_site_array->binary_path;
			call_stack->size		= call_site_array->size;
			call_stack->first_frame = _e4c_parse_call_frame_array(call_site_array);
			call_stack->last_frame	= ( call_stack->first_frame == NULL ? NULL : call_stack->first_frame + call_site_array->size - 1);
		}
	}

	return(call_stack);
}

inline static e4c_call_site_array * _e4c_capture_call_site_array(const char * binary_path){

	e4c_call_site_array * tmp = malloc( sizeof(*tmp) );

	if(tmp != NULL){

		memcpy( tmp, &call_site_array, sizeof(*tmp) );

		tmp->size			= ( tmp->size > E4C_STACK_TRACE_MAX_FUNCTION_CALLS ? E4C_STACK_TRACE_MAX_FUNCTION_CALLS : tmp->size );
		tmp->binary_path	= binary_path;
	}

	return(tmp);
}

inline static void _e4c_print_call_stack(e4c_call_stack * call_stack, const char * * prefix_exclude, int max){

	e4c_call_frame *	call_frame;
	int					index = 0;

	if(call_stack == NULL){
		return;
	}

	for(call_frame = call_stack->last_frame; call_frame != NULL; call_frame = call_frame->previous){

		int print_line	= ( max == 0 || index < max );
		int mismatched	= (call_frame->next != NULL && !STR_EQUALS(call_frame->callee.function_name, call_frame->next->caller.function_name) );

		if(mismatched){
			index += _e4c_print_call_frame(call_stack->binary_path, &call_frame->callee, prefix_exclude, print_line);
		}

		index += _e4c_print_call_frame(call_stack->binary_path, &call_frame->caller, prefix_exclude, print_line);
	}

	if( max != 0 && index > max ){

		printf("    ... %d more\n", index - max);
	}
}

inline static void _e4c_print_exception(const e4c_exception * exception, int is_cause, const char * * prefix_exclude, int max){

	fprintf(stderr, "%s %s: %s\n\n", (is_cause ? "Caused by" : "\n\nUncaught"), exception->name, exception->message);

	if(exception->file != NULL){

		if(exception->function != NULL){

			fprintf(stderr, "    thrown at %s (%s:%d)\n", exception->function, exception->file, exception->line);
		}else{

			fprintf(stderr, "    thrown at %s:%d\n", exception->file, exception->line);
		}
	}

	if(exception->custom_data != NULL){

		e4c_call_stack *	call_stack	= _e4c_parse_call_stack(exception->custom_data);

		_e4c_print_call_stack(call_stack, prefix_exclude, max);
	}

	fprintf(stderr, "\n");
}

void e4c_stack_trace_print_exception(const e4c_exception * exception){

	const char *	exclude[]	= {E4C_STACK_TRACE_EXCLUDE_FUNCTIONS, NULL};
	int				max			= 16;

	if(exception == NULL){
		throw(NullPointerException, "Null exception.");
	}

	_e4c_print_exception(exception, 0, exclude, 0);

	for(exception = exception->cause; exception != NULL; exception = exception->cause){

		_e4c_print_exception(exception, 1, exclude, max);

		max = ( max == 4 ? 4 : max / 2 );
	}
}

void * e4c_stack_trace_initialize(const e4c_exception * exception){

	e4c_call_site_array * call_site_array = NULL;

	if(exception != NULL){

		const char * binary_path = exception->custom_data;

		call_site_array = _e4c_capture_call_site_array(binary_path);
	}

	return(call_site_array);
}

void e4c_stack_trace_finalize(void * custom_data){

	free(custom_data);
}
