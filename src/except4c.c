/*
 *
 * @file except4c.c
 *
 * exceptions4c source code file
 *
 * @version 1.6
 * @author Copyright (c) 2010 Guillermo Calvo
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 * except4c.c is undocumented on purpose (everything is documented in except4c.h)
 */


# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <errno.h>

# define E4C_IMPLEMENTATION
# include "except4c.h"

# define IS_TOP_FRAME(frame)			( frame->previous == NULL )
# define STOP_IF(c, e, f, l) \
	if(c){ \
		DEBUG_INITIALIZE_ONCE; \
		fatalErrorFlag = e4c_true; \
		e4c_dumpException(e, f, l, errno); \
		PRINT_MSG_FATAL_ERROR; \
		STOP_EXECUTION; \
	}


# ifdef E4C_THREADSAFE
#	include <pthread.h>
#	define	DESC_INVALID_CONTEXT	"The exception context for this thread is in an invalid state."
#	define	DESC_ALREADY_BEGUN		"The exception context for this thread has already begun."
#	define	DESC_NOT_BEGUN_YET		"The exception context for this thread has not begun yet."
#	define	DESC_NOT_ENDED			"There is at least one thread that did not end its exception context properly."
#	define	THREAD_TYPE				pthread_t
#	define	THREAD_CURRENT			pthread_self()
#	define	THREAD_SAME				pthread_equal
#	define	THREAD_EXIT				pthread_exit
#	define	MUTEX_DEFINE(mutex)		static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER
#	define	MUTEX_LOCK(mutex)		pthread_mutex_lock(&mutex)
#	define	MUTEX_UNLOCK(mutex)		pthread_mutex_unlock(&mutex)
#	define	STOP_EXECUTION			THREAD_EXIT(PTHREAD_CANCELED)
#	define	DANGLING_CONTEXT		(environmentCollection != NULL)
	typedef struct ThreadEnvironmentStruct ThreadEnvironment;
	struct ThreadEnvironmentStruct{
		THREAD_TYPE					self;
		ThreadEnvironment *			next;
		struct e4c_ExceptionContext	context;
	};
	MUTEX_DEFINE(environmentCollectionMutex);
	static ThreadEnvironment * environmentCollection = NULL;
# else
#	undef	E4C_CONTEXT
#	define	E4C_CONTEXT				currentContext
#	define	DESC_INVALID_CONTEXT	"The exception context for this program is in an invalid state."
#	define	DESC_ALREADY_BEGUN		"The exception context for this program has already begun."
#	define	DESC_NOT_BEGUN_YET		"The exception context for this program has not begun yet."
#	define	DESC_NOT_ENDED			"The program did not end its exception context properly."
#	define	MUTEX_LOCK(mutex)
#	define	MUTEX_UNLOCK(mutex)
#	define	STOP_EXECUTION			exit(EXIT_FAILURE)
#	define	DANGLING_CONTEXT		(currentContext != NULL)
	static struct e4c_ExceptionContext		mainContext;
	static struct e4c_ExceptionContext *	currentContext = NULL;
# endif

static e4c_bool fatalErrorFlag = e4c_false;

# ifndef NDEBUG
	static e4c_bool isInitialized = e4c_false;
#	ifdef E4C_THREADSAFE
		MUTEX_DEFINE(isInitializedMutex);
#		define	MSG_FATAL_ERROR			"\n\nThis is an unrecoverable programming error; the thread will be terminated\nimmediately.\n"
#		define	MSG_AT_EXIT_ERROR		"\n\nThe program will now yield a failure exit code due to exception system errors.\n"
#	else
#		define	MSG_FATAL_ERROR			"\n\nThis is an unrecoverable programming error; the application will be terminated\nimmediately.\n"
#		define	MSG_AT_EXIT_ERROR		"\n\nIf this application is making use of threads, please recompile exceptions4c\nwith thread support (by defining the macro E4C_THREADSAFE).\n\n\nThe program will now yield a failure exit code due to exception system errors.\n"
#	endif
#	define	PRINT_MSG_FATAL_ERROR		fprintf(stderr, MSG_FATAL_ERROR)
#	define	DEBUG_INITIALIZE_ONCE		if(!isInitialized) e4c_initialize()
#	define	DEBUG_STOP_IF(c, e, f, l)	STOP_IF(c, e, f, l)
#	define	E4C_FILE_SIGNAL				"<system signal>"
#	define	E4C_FILE_GET_HANDLERS		"exceptions4c.getSignalMappings"
#	define	E4C_FILE_SET_HANDLERS		"exceptions4c.setSignalMappings"
#	define	E4C_FILE_BEGIN				"exceptions4c.beginExceptionContext"
#	define	E4C_FILE_END				"exceptions4c.endExceptionContext"
#	define	E4C_FILE_AT_EXIT			"exceptions4c.atExit"
# else
#	define	PRINT_MSG_FATAL_ERROR
#	define	DEBUG_INITIALIZE_ONCE
#	define	DEBUG_STOP_IF(c, e, f, l)
#	define	E4C_FILE_SIGNAL				E4C_FILE_INFO
#	define	E4C_FILE_GET_HANDLERS		E4C_FILE_INFO
#	define	E4C_FILE_SET_HANDLERS		E4C_FILE_INFO
#	define	E4C_FILE_BEGIN				E4C_FILE_INFO
#	define	E4C_FILE_END				E4C_FILE_INFO
#	define	E4C_FILE_AT_EXIT			E4C_FILE_INFO
# endif

const struct e4c_Exception E4C_NULL_EXCEPTION = {
	name:			(const char *)NULL,
	description:	(const char *)NULL,
	super:			(const struct e4c_Exception *)NULL
};

E4C_DEFINE_EXCEPTION(RuntimeException,				"Runtime exception",				RuntimeException);
E4C_DEFINE_EXCEPTION(NotEnoughMemoryException,		"Not enough memory",				RuntimeException);
E4C_DEFINE_EXCEPTION(NullPointerException,			"Null pointer",						RuntimeException);
E4C_DEFINE_EXCEPTION(FileOpenException,				"Couldn't open file",				RuntimeException);

E4C_DEFINE_EXCEPTION(SignalException,				"Signal received",					RuntimeException);
E4C_DEFINE_EXCEPTION(SignalAlarmException,			"Alarm clock signal received",		SignalException);
E4C_DEFINE_EXCEPTION(SignalChildException,			"Child process signal received",	SignalException);
E4C_DEFINE_EXCEPTION(SignalTrapException,			"Trace trap",						SignalException);
E4C_DEFINE_EXCEPTION(ErrorSignalException,			"Error signal received",			SignalException);
E4C_DEFINE_EXCEPTION(IllegalInstructionException,	"Illegal instruction",				ErrorSignalException);
E4C_DEFINE_EXCEPTION(ArithmeticException,			"Erroneous arithmetic operation",	ErrorSignalException);
E4C_DEFINE_EXCEPTION(BadPointerException,			"Segmentation violation",			ErrorSignalException);
E4C_DEFINE_EXCEPTION(BrokenPipeException,			"Broken pipe",						ErrorSignalException);
E4C_DEFINE_EXCEPTION(ControlSignalException,		"Control signal received",			SignalException);
E4C_DEFINE_EXCEPTION(StopException,					"Stop signal received",				ControlSignalException);
E4C_DEFINE_EXCEPTION(KillException,					"Kill signal received",				ControlSignalException);
E4C_DEFINE_EXCEPTION(HangUpException,				"Hang up signal received",			ControlSignalException);
E4C_DEFINE_EXCEPTION(TerminationException,			"Termination signal received",		ControlSignalException);
E4C_DEFINE_EXCEPTION(AbortException,				"Abort signal received",			ControlSignalException);
E4C_DEFINE_EXCEPTION(CPUTimeException,				"Exceeded CPU time",				ControlSignalException);
E4C_DEFINE_EXCEPTION(UserControlSignalException,	"User control signal received",		ControlSignalException);
E4C_DEFINE_EXCEPTION(UserQuitException,				"Quit signal received",				UserControlSignalException);
E4C_DEFINE_EXCEPTION(UserInterruptionException,		"Interrupt signal received",		UserControlSignalException);
E4C_DEFINE_EXCEPTION(UserBreakException,			"Break signal received",			UserControlSignalException);
E4C_DEFINE_EXCEPTION(ProgramSignalException,		"User-defined signal received",		SignalException);
E4C_DEFINE_EXCEPTION(ProgramSignal1Exception,		"User-defined signal 1 received",	ProgramSignalException);
E4C_DEFINE_EXCEPTION(ProgramSignal2Exception,		"User-defined signal 2 received",	ProgramSignalException);

static
const struct e4c_Exception FatalError = {
	name:			"FatalError",
	description:	"This application has encountered an error from which it cannot recover",
	super:			NULL
};
static
E4C_DEFINE_EXCEPTION(ExceptionSystemFatalError,		DESC_INVALID_CONTEXT,				FatalError);
static
E4C_DEFINE_EXCEPTION(ContextAlreadyBegun,			DESC_ALREADY_BEGUN,					ExceptionSystemFatalError);
static
E4C_DEFINE_EXCEPTION(ContextHasNotBegunYet,			DESC_NOT_BEGUN_YET,					ExceptionSystemFatalError);
static
E4C_DEFINE_EXCEPTION(ContextNotEnded,				DESC_NOT_ENDED,						ExceptionSystemFatalError);

const struct e4c_SignalMapping e4c_defaultSignalMapping[] = {
	{signalNumber: SIGABRT,		exception: &AbortException},
	{signalNumber: SIGFPE,		exception: &ArithmeticException},
	{signalNumber: SIGILL,		exception: &IllegalInstructionException},
	{signalNumber: SIGSEGV,		exception: &BadPointerException},
	{signalNumber: SIGTERM,		exception: &TerminationException},
	{signalNumber: SIGINT,		exception: &UserInterruptionException},
# ifdef SIGALRM
	{signalNumber: SIGALRM,		exception: &SignalAlarmException},
# endif
# ifdef SIGCHLD
	{signalNumber: SIGCHLD,		exception: &SignalChildException},
# endif
# ifdef SIGTRAP
	{signalNumber: SIGTRAP,		exception: &SignalTrapException},
# endif
# ifdef SIGPIPE
	{signalNumber: SIGPIPE,		exception: &BrokenPipeException},
# endif
# ifdef SIGSTOP
	{signalNumber: SIGSTOP,		exception: &StopException},
# endif
# ifdef SIGKILL
	{signalNumber: SIGKILL,		exception: &KillException},
# endif
# ifdef SIGHUP
	{signalNumber: SIGHUP,		exception: &HangUpException},
# endif
# ifdef SIGXCPU
	{signalNumber: SIGXCPU,		exception: &CPUTimeException},
# endif
# ifdef SIGQUIT
	{signalNumber: SIGQUIT,		exception: &UserQuitException},
# endif
# ifdef SIGBREAK
	{signalNumber: SIGBREAK,	exception: &UserBreakException},
# endif
# ifdef SIGUSR1
	{signalNumber: SIGUSR1,		exception: &ProgramSignal1Exception},
# endif
# ifdef SIGUSR2
	{signalNumber: SIGUSR2,		exception: &ProgramSignal2Exception},
# endif
};
const int e4c_defaultSignalMappings = ( sizeof(e4c_defaultSignalMapping) / sizeof(e4c_defaultSignalMapping[0]) );

long e4c_getLibraryVersion(){

	return(E4C_VERSION_NUMBER);
}

# ifndef NDEBUG

static void e4c_atExit(){

	if(DANGLING_CONTEXT){
		e4c_dumpException(ContextNotEnded, E4C_FILE_AT_EXIT, E4C_LINE_INFO, errno);
		fatalErrorFlag = e4c_true;
	}

	if(fatalErrorFlag){
		fprintf(stderr, MSG_AT_EXIT_ERROR);
		/* force failure exit status */
		exit(EXIT_FAILURE);
	}

}

static void e4c_initialize(){

	MUTEX_LOCK(isInitializedMutex);

		if(!isInitialized){
			isInitialized = e4c_true;
			atexit(e4c_atExit);
		}

	MUTEX_UNLOCK(isInitializedMutex);
}

# endif


# ifndef E4C_THREADSAFE

struct e4c_ExceptionContext * e4c_getExceptionContext(){

	return(currentContext);
}

# else

static void e4c_addThreadEnvironment(ThreadEnvironment * environment){

	MUTEX_LOCK(environmentCollectionMutex);

		environment->next		= environmentCollection;
		environmentCollection	= environment;

	MUTEX_UNLOCK(environmentCollectionMutex);
}

static ThreadEnvironment * e4c_removeThreadEnvironment(){

	THREAD_TYPE				self		= THREAD_CURRENT;
	e4c_bool				found		= e4c_false;
	ThreadEnvironment *		previous	= NULL;
	ThreadEnvironment *		environment;

	MUTEX_LOCK(environmentCollectionMutex);

		environment = environmentCollection;

		while(environment != NULL){
			if( THREAD_SAME(self, environment->self) ){
				found = e4c_true;
				break;
			}
			previous	= environment;
			environment	= environment->next;
		}

		if(found){
			if(previous == NULL)	environmentCollection	= environment->next;
			else					previous->next			= environment->next;
		}

	MUTEX_UNLOCK(environmentCollectionMutex);

	return(found ? environment : NULL);
}

static ThreadEnvironment * e4c_getThreadEnvironment(){

	THREAD_TYPE				self		= THREAD_CURRENT;
	e4c_bool				found		= e4c_false;
	ThreadEnvironment *		environment;

	MUTEX_LOCK(environmentCollectionMutex);

		environment	= environmentCollection;

		while(environment != NULL){
			if( THREAD_SAME(self, environment->self) ){
				found = e4c_true;
				break;
			}
			environment	= environment->next;
		}

	MUTEX_UNLOCK(environmentCollectionMutex);

	return(found ? environment : NULL);
}

struct e4c_ExceptionContext * e4c_getExceptionContext(){

	ThreadEnvironment * environment = e4c_getThreadEnvironment();

	return(environment == NULL ? NULL : &environment->context);
}

# endif

static void e4c_atUncaughtException(struct e4c_ExceptionContext * context){

	/* context cannot be NULL (checked by the caller) */
	e4c_UncaughtHandler			uncaughtHandler	= context->uncaughtHandler;
	struct e4c_ExceptionFrame *	frame			= context->currentFrame;

	if(uncaughtHandler != NULL){
		uncaughtHandler(frame->exception, frame->file, frame->line, frame->errorNumber);
	}

	e4c_endExceptionContext();

	STOP_EXECUTION;
}

static void e4c_propagate(struct e4c_ExceptionContext * context, const struct e4c_Exception exception, const char * file, int line, int errNumber){

	/* neither context nor frame is NULL (checked by the caller) */
	struct e4c_ExceptionFrame * frame = context->currentFrame;

	/* Update the frame with the exception information */
	frame->thrown		= e4c_true;
	frame->uncaught		= e4c_true;
	frame->exception	= exception;
	frame->file			= file;
	frame->line			= line;
	frame->errorNumber	= errNumber;

	/* if this is the upper frame, then this is an uncaught exception */
	if( IS_TOP_FRAME(frame) ) e4c_atUncaughtException(context);

	/* otherwise, we will jump to the upper frame */

	/* simple optimization */
	if(frame->stage == e4c_acquiring){
		/* if we are in the middle of an acquisition, we don't need to dispose the resource */
		frame->stage = e4c_disposing;
		/* (that actually jumps over the disposal stage) */
	}

	/* keep looping */
	E4C_LONGJMP(frame->address);
}

static void e4c_handleSignal(int signalNumber){

	struct e4c_ExceptionContext *	context		= E4C_CONTEXT;
	const struct e4c_Exception *	exception;
	int								index;

	/* check if 'handleSignal' was called before calling e4c_beginExceptionContext (very unlikely) */
	DEBUG_STOP_IF(context == NULL, ContextHasNotBegunYet, E4C_FILE_INFO, E4C_LINE_INFO);

	/* check if the current frame is NULL (very unlikely) */
	DEBUG_STOP_IF(context->currentFrame == NULL, ExceptionSystemFatalError, E4C_FILE_INFO, E4C_LINE_INFO);

	/* try to find a mapping for this signal */
	for(index = 0; index < context->signalMappings; index++){
		if(signalNumber == context->signalMapping[index].signalNumber){
			exception = context->signalMapping[index].exception;
			/* make ensure we have a proper exception to throw */
			if(exception == NULL) break;
			/* reset the handler for this signal */
			signal(signalNumber, e4c_handleSignal);
			/* throw the appropriate exception to the default context */
			e4c_propagate(context, *exception, E4C_FILE_SIGNAL, signalNumber, errno);
		}
	}
	/* this should never happen, but anyway... */
	/* we were unable to find the exception that represents the received signal number */
	e4c_propagate(context, ExceptionSystemFatalError, E4C_FILE_INFO, E4C_LINE_INFO, errno);
}

static void e4c_setHandlers(struct e4c_ExceptionContext * context, const struct e4c_SignalMapping * signalMapping, int signalMappings){

	/* context cannot be NULL (checked by the caller) */
	int					index;

	/* reset all the previously set signal handlers */
	for(index = 0; index < context->signalMappings; index++){
		signal(context->signalMapping[index].signalNumber, SIG_DFL);
	}

	/* update the current context (checked) */
	context->signalMapping		= signalMapping;
	context->signalMappings		= signalMappings;

	/* set up the handlers for the specified signals */
	for(index = 0; index < signalMappings; index++){
		signal(signalMapping[index].signalNumber, e4c_handleSignal);
	}
}

static e4c_bool e4c_extends(const struct e4c_Exception * child, const struct e4c_Exception * parent){

	const struct e4c_Exception * super = child->super;

	/* an exception with no supertype does not "extend" anything */
	if(super == NULL){
		return(e4c_false);
	}

	if( (child->name == parent->name) || (super->name == parent->name) ){
		return(e4c_true);
	}

	if(super == child){
		return(e4c_false);
	}

	return( e4c_extends(super, parent) );
}

E4C_JMP_BUF * e4c_first(enum e4c_Stage stage, const char * file, int line){

	struct e4c_ExceptionContext *	context			= E4C_CONTEXT;
	struct e4c_ExceptionFrame *		currentFrame;
	struct e4c_ExceptionFrame *		newFrame;

	/* check if 'try' was used before calling e4c_beginExceptionContext */
	STOP_IF(context == NULL, ContextHasNotBegunYet, file, line);

	currentFrame = context->currentFrame;

	/* check if the current frame is NULL (very unlikely) */
	DEBUG_STOP_IF(currentFrame == NULL, ExceptionSystemFatalError, E4C_FILE_INFO, E4C_LINE_INFO);

	/* create a new frame */
	newFrame = (struct e4c_ExceptionFrame *)malloc( sizeof(struct e4c_ExceptionFrame) );

	/* if there wasn't enough memory... */
	if(newFrame == NULL){
		e4c_propagate(context, NotEnoughMemoryException, E4C_FILE_INFO, E4C_LINE_INFO, errno);
	}

	newFrame->stage			= stage;
	newFrame->thrown		= e4c_false;
	newFrame->uncaught		= e4c_false;
	newFrame->exception		= E4C_NULL_EXCEPTION;
	newFrame->errorNumber	= 0;
	newFrame->file			= (const char *)NULL;
	newFrame->line			= 0;
	newFrame->previous		= currentFrame;

	/* make it the new current frame */
	context->currentFrame	= newFrame;

	return( &(newFrame->address) );
}

e4c_bool e4c_hook(enum e4c_Stage stage, const struct e4c_Exception exception){

	struct e4c_ExceptionContext *	context	= E4C_CONTEXT;
	struct e4c_ExceptionFrame *		frame;

	/* check if the current exception context is NULL (unlikely) */
	DEBUG_STOP_IF(context == NULL, ContextHasNotBegunYet, E4C_FILE_INFO, E4C_LINE_INFO);

	frame = context->currentFrame;

	if(frame->stage != stage) return(e4c_false);

	if(stage == e4c_catching){
		/* does this block catch current exception? */
		if( !e4c_extends(&frame->exception, &exception) ){
			/* nay, keep looking for an exception handler */
			return(e4c_false);
		}
		/* yay, catch current exception by executing the handler */
		frame->uncaught = e4c_false;
	}

	return(e4c_true);
}

e4c_bool e4c_next(){

	struct e4c_ExceptionContext *	context;
	struct e4c_ExceptionFrame *		currentFrame;
	struct e4c_ExceptionFrame		tmpFrame;

	/* get the current context */
	context = E4C_CONTEXT;

	/* check if the current exception context is NULL (unlikely) */
	DEBUG_STOP_IF(context == NULL, ContextHasNotBegunYet, E4C_FILE_INFO, E4C_LINE_INFO);

	currentFrame = context->currentFrame;

	/* check if the current frame is NULL (unlikely) */
	DEBUG_STOP_IF(currentFrame == NULL, ExceptionSystemFatalError, E4C_FILE_INFO, E4C_LINE_INFO);

	currentFrame->stage++;

	/* simple optimization */
	if(currentFrame->stage == e4c_catching && !currentFrame->uncaught){
		/* if no exception was thrown, we don't need to go through the catch stage */
		currentFrame->stage++;
	}

	/* keep looping until we reach the final stage */
	if(currentFrame->stage < e4c_end) return(e4c_true);

	/* the exception loop is finished; let's capture temporarily the information */
	/* of the current frame, so we can propagate an exception (if it was thrown) */
	tmpFrame = *currentFrame;

	/* delete the current frame */
	free(currentFrame);

	/* promote the previous frame to the current one */
	context->currentFrame	= currentFrame = tmpFrame.previous;

	/* check if the current frame is NULL (unlikely) */
	DEBUG_STOP_IF(currentFrame == NULL, ExceptionSystemFatalError, E4C_FILE_INFO, E4C_LINE_INFO);

	/* if the current frame has an uncaught exception, then we will propagate it */
	if(tmpFrame.uncaught){
		e4c_propagate(context, tmpFrame.exception, tmpFrame.file, tmpFrame.line, tmpFrame.errorNumber);
	}
	/* otherwise, we're free to go */

	/* get out of the exception loop */
	return(e4c_false);
}

void e4c_throw(const struct e4c_Exception exception, const char * file, int line){

	struct e4c_ExceptionContext *	context;
	struct e4c_ExceptionFrame *		frame;

	/* ignore unnamed exceptions */
	if(exception.name == NULL) return;

	/* get the current context */
	context = E4C_CONTEXT;

	/* check if 'throw' was used before calling e4c_beginExceptionContext */
	STOP_IF(context == NULL, ContextHasNotBegunYet, file, line);

	/* get the current frame */
	frame = context->currentFrame;

	/* check if the current frame is NULL (unlikely) */
	DEBUG_STOP_IF(frame == NULL, ExceptionSystemFatalError, E4C_FILE_INFO, E4C_LINE_INFO);

	e4c_propagate(context, exception, file, line, errno);
}

void e4c_dumpException(struct e4c_Exception exception, const char * file, int line, int errorNumber){
# ifndef NDEBUG
	fprintf(stderr, "\n\nUncaught %s: %s\n\n", exception.name, exception.description);

	if(file != NULL && *file == '<'){
		fprintf(stderr, "    received %s number %d\n\n", file, line);
	}else{
		fprintf(stderr, "    thrown at line %d of %s\n\n", line, file);
	}

	fprintf(stderr, "The value of errno was %d.\n\n", errorNumber);

	e4c_printExceptionHierarchy(exception);
# else
	fprintf(stderr, "\n\nFatal Error: %s (%s)\n\n", exception.name, exception.description);
	/* get rid of the "unused parameter" warnings */
	if(file != NULL) line = errorNumber = 0;
# endif
}

void e4c_printExceptionHierarchy(struct e4c_Exception exception){

	const char * separator = "________________________________________________________________";
	int deep = 0;
	fprintf(stderr, "Exception hierarchy\n%s\n\n   %s\n", separator, exception.name);
	while( exception.super != NULL && exception.name != exception.super->name){
		exception = *exception.super;
		fprintf(stderr, "    %*s |\n    %*s +-- %s\n", deep * 6, "", deep * 6, "", exception.name);
		deep++;
	}
	fprintf(stderr, "%s\n", separator);
}

void e4c_setSignalMappings(const struct e4c_SignalMapping * mapping, int mappings){

	struct e4c_ExceptionContext *	context	= E4C_CONTEXT;

	/* check if e4c_setSignalMappings was called before calling e4c_beginExceptionContext */
	STOP_IF(context == NULL, ContextHasNotBegunYet, E4C_FILE_SET_HANDLERS, E4C_LINE_INFO);

	/* sanity check */
	mapping		= (mappings == 0 ? NULL : mapping);
	mappings	= (mapping == NULL ? 0 : mappings);

	e4c_setHandlers(context, mapping, mappings);
}

const struct e4c_SignalMapping * e4c_getSignalMappings(int * mappings){

	struct e4c_ExceptionContext *	context	= E4C_CONTEXT;

	/* check if e4c_setSignalMappings was called before calling e4c_beginExceptionContext */
	STOP_IF(context == NULL, ContextHasNotBegunYet, E4C_FILE_GET_HANDLERS, E4C_LINE_INFO);

	/* sanity check */
	if(mappings == NULL){
		e4c_throw(NullPointerException, E4C_FILE_GET_HANDLERS, E4C_LINE_INFO);
	}

	if(context->signalMapping == NULL){
		*mappings = 0;
		return(NULL);
	}

	*mappings = context->signalMappings;

	return(context->signalMapping);
}

void e4c_beginExceptionContext(e4c_bool handleSignals, e4c_UncaughtHandler uncaughtHandler){

	struct e4c_ExceptionContext *	context;
	struct e4c_ExceptionFrame *		newFrame;

# ifdef E4C_THREADSAFE

	THREAD_TYPE			self		= THREAD_CURRENT;
	ThreadEnvironment *	environment;

	DEBUG_INITIALIZE_ONCE;

	/* get the current environment */
	environment = e4c_getThreadEnvironment();

	/* check if e4c_beginExceptionContext was called twice for this thread */
	if(environment != NULL){
		fatalErrorFlag = e4c_true;
		e4c_propagate(&environment->context, ContextAlreadyBegun, E4C_FILE_BEGIN, E4C_LINE_INFO, errno);
	}

	/* allocate memory for the new frame */
	environment	= malloc( sizeof(ThreadEnvironment) );

	/* if there wasn't enough memory... */
	STOP_IF(environment == NULL, NotEnoughMemoryException, E4C_FILE_INFO, E4C_LINE_INFO);

	/* allocate memory for the new frame */
	newFrame = malloc( sizeof(struct e4c_ExceptionFrame) );

	/* if there wasn't enough memory... */
	if(newFrame == NULL){
		free(environment);
		e4c_dumpException(NotEnoughMemoryException, E4C_FILE_INFO, E4C_LINE_INFO, errno);
		STOP_EXECUTION;
	}

	/* add the new environment to the collection */
	e4c_addThreadEnvironment(environment);

	/* bound the new environment to the current thread */
	environment->self = self;

	/* update local variable */
	context = &environment->context;

# else

	DEBUG_INITIALIZE_ONCE;

	/* get the current context */
	context = currentContext;

	/* check if e4c_beginExceptionContext was called twice for this program */
	/* this can also happen when the program uses threads but E4C_THREADSAFE is not defined */
	if(context != NULL){
		fatalErrorFlag = e4c_true;
		e4c_propagate(context, ContextAlreadyBegun, E4C_FILE_BEGIN, E4C_LINE_INFO, errno);
	}

	/* update local and global variable */
	context = currentContext = &mainContext;

	/* check if the current exception context is NULL (unlikely) */
	DEBUG_STOP_IF(context == NULL, ContextHasNotBegunYet, E4C_FILE_INFO, E4C_LINE_INFO);

	/* allocate memory for the new frame */
	newFrame = malloc( sizeof(struct e4c_ExceptionFrame) );

	/* if there wasn't enough memory... */
	STOP_IF(newFrame == NULL, NotEnoughMemoryException, E4C_FILE_INFO, E4C_LINE_INFO);

# endif

	context->uncaughtHandler	= uncaughtHandler;
	context->signalMapping		= NULL;
	context->signalMappings		= 0;
	context->currentFrame		= newFrame;

	newFrame->previous			= NULL;
	newFrame->stage				= e4c_end;
	newFrame->file				= NULL;
	newFrame->line				= 0;
	newFrame->errorNumber		= 0;
	newFrame->thrown			= e4c_false;
	newFrame->uncaught			= e4c_false;
	newFrame->exception			= E4C_NULL_EXCEPTION;

	if(handleSignals) e4c_setHandlers(context, e4c_defaultSignalMapping, e4c_defaultSignalMappings);
}

void e4c_endExceptionContext(){

	struct e4c_ExceptionContext *	context;
	struct e4c_ExceptionFrame *		frame;

# ifdef E4C_THREADSAFE

	ThreadEnvironment *	environment;

	/* remove (and get) the current context */
	environment = e4c_removeThreadEnvironment();

	/* check if e4c_endExceptionContext was called before calling e4c_beginExceptionContext */
	STOP_IF(environment == NULL, ContextHasNotBegunYet, E4C_FILE_END, E4C_LINE_INFO);

	/* update local variable */
	context = &environment->context;

	/* check if the current context is NULL (unlikely) */
	DEBUG_STOP_IF(context == NULL, ContextHasNotBegunYet, E4C_FILE_INFO, E4C_LINE_INFO);

# else

	/* get the current context */
	context = currentContext;

	/* check if e4c_endExceptionContext was called before calling e4c_beginExceptionContext */
	STOP_IF(context == NULL, ContextHasNotBegunYet, E4C_FILE_END, E4C_LINE_INFO);

# endif

	/* get the current frame */
	frame = context->currentFrame;

	/* check if there are no frames left (unlikely) */
	DEBUG_STOP_IF(frame == NULL, ExceptionSystemFatalError, E4C_FILE_INFO, E4C_LINE_INFO);

	/* check if there are too many frames left (unlikely) */
	DEBUG_STOP_IF(!IS_TOP_FRAME(frame), ExceptionSystemFatalError, E4C_FILE_INFO, E4C_LINE_INFO);

	/* deallocate the current, top frame */
	free(frame);

	/* reset all signal handlers */
	e4c_setHandlers(context, NULL, 0);

# ifdef E4C_THREADSAFE

	/* deallocate the thread environment */
	free(environment);

# else

	/* deactivate the current context */
	currentContext				= NULL;

	/* deactivate the top frame (for sanity) */
	mainContext.currentFrame	= NULL;

# endif

}
