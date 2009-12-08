/*
 *
 * @file except4c.c
 *
 * <code>exceptions4c</code> source code file
 *
 * @version 1.2
 * @author Copyright (c) 2009 Guillermo Calvo
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
# include "except4c.h"

typedef enum{beginning, trying, catching, finalizing, ending} Stage;
typedef struct Context Context;
struct Context{
	Context *			prev;
	Stage				stage;
	bool				uncaught;
	EXCEPT4C_JMP_BUF	address;
};

/* private global variables */

const static SignalMapping *	signalMapping	= NULL;
static int						signalMappings	= 0;
static Context *				currentContext	= NULL;
static const char *				srcFile			= NULL;
static int						srcLine			= 0;

/* public global variables */

Exception exception = EXCEPTION_LITERAL(
	(const char *)NULL, (const char *)NULL, (const Exception *)NULL
);

DEFINE_EXCEPTION(RuntimeException,				"Runtime exception",				RuntimeException);
DEFINE_EXCEPTION(NotEnoughMemoryException,		"Not enough memory",				RuntimeException);
DEFINE_EXCEPTION(NullPointerException,			"Null pointer",						RuntimeException);

DEFINE_EXCEPTION(SignalException,				"Signal received",					RuntimeException);
DEFINE_EXCEPTION(SignalAlarmException,			"Alarm clock signal received",		SignalException);
DEFINE_EXCEPTION(SignalChildException,			"Child process signal received",	SignalException);
DEFINE_EXCEPTION(SignalTrapException,			"Trace trap",						SignalException);
DEFINE_EXCEPTION(ErrorSignalException,			"Error signal received",			SignalException);
DEFINE_EXCEPTION(IllegalInstructionException,	"Illegal instruction",				ErrorSignalException);
DEFINE_EXCEPTION(ArithmeticException,			"Erroneous arithmetic operation",	ErrorSignalException);
DEFINE_EXCEPTION(BadPointerException,			"Segmentation violation",			ErrorSignalException);
DEFINE_EXCEPTION(BrokenPipeException,			"Broken pipe",						ErrorSignalException);
DEFINE_EXCEPTION(ControlSignalException,		"Control signal received",			SignalException);
DEFINE_EXCEPTION(StopException,					"Stop signal received",				ControlSignalException);
DEFINE_EXCEPTION(KillException,					"Kill signal received",				ControlSignalException);
DEFINE_EXCEPTION(HangUpException,				"Hang up signal received",			ControlSignalException);
DEFINE_EXCEPTION(TerminationException,			"Termination signal received",		ControlSignalException);
DEFINE_EXCEPTION(AbortException,				"Abort signal received",			ControlSignalException);
DEFINE_EXCEPTION(CPUTimeException,				"Exceeded CPU time",				ControlSignalException);
DEFINE_EXCEPTION(UserControlSignalException,	"User control signal received",		ControlSignalException);
DEFINE_EXCEPTION(UserQuitException,				"Quit signal received",				UserControlSignalException);
DEFINE_EXCEPTION(UserInterruptionException,		"Interrupt signal received",		UserControlSignalException);
DEFINE_EXCEPTION(UserBreakException,			"Break signal received",			UserControlSignalException);
DEFINE_EXCEPTION(ProgramSignalException,		"User-defined signal received",		SignalException);
DEFINE_EXCEPTION(ProgramSignal1Exception,		"User-defined signal 1 received",	ProgramSignalException);
DEFINE_EXCEPTION(ProgramSignal2Exception,		"User-defined signal 2 received",	ProgramSignalException);

const SignalMapping defaultSignalMapping[] = {
	SIGNAL_MAPPING(SIGABRT,		AbortException),
	SIGNAL_MAPPING(SIGFPE,		ArithmeticException),
	SIGNAL_MAPPING(SIGILL,		IllegalInstructionException),
	SIGNAL_MAPPING(SIGSEGV,		BadPointerException),
	SIGNAL_MAPPING(SIGTERM,		TerminationException),
	SIGNAL_MAPPING(SIGINT,		UserInterruptionException),

# ifdef SIGALRM
	SIGNAL_MAPPING(SIGALRM,		SignalAlarmException),
# endif

# ifdef SIGCHLD
	SIGNAL_MAPPING(SIGCHLD,		SignalChildException),
# endif

# ifdef SIGTRAP
	SIGNAL_MAPPING(SIGTRAP,		SignalTrapException),
# endif

# ifdef SIGPIPE
	SIGNAL_MAPPING(SIGPIPE,		BrokenPipeException),
# endif

# ifdef SIGSTOP
	SIGNAL_MAPPING(SIGSTOP,		StopException),
# endif

# ifdef SIGKILL
	SIGNAL_MAPPING(SIGKILL,		KillException),
# endif

# ifdef SIGHUP
	SIGNAL_MAPPING(SIGHUP,		HangUpException),
# endif

# ifdef SIGXCPU
	SIGNAL_MAPPING(SIGXCPU,		CPUTimeException),
# endif

# ifdef SIGQUIT
	SIGNAL_MAPPING(SIGQUIT,		UserQuitException),
# endif

# ifdef SIGBREAK
	SIGNAL_MAPPING(SIGBREAK,	UserBreakException),
# endif

# ifdef SIGUSR1
	SIGNAL_MAPPING(SIGUSR1,		ProgramSignal1Exception),
# endif

# ifdef SIGUSR2
	SIGNAL_MAPPING(SIGUSR2,		ProgramSignal2Exception),
# endif

};
const int defaultSignalMappings = ( sizeof(defaultSignalMapping) / sizeof(defaultSignalMapping[0]) );

static bool extends(const Exception * e1, const Exception * e2){

	const Exception * super = e1->super;

	if(super == NULL){
		return(false);
	}

	if( (e1->name == e2->name) || (super->name == e2->name) ){
		return(true);
	}

	if(super == e1){
		return(false);
	}

	return( extends(super, e2) );
}

static void handleSignal(int _signal_){

	const Exception *	exception = NULL;
	int					index;

	/* try to find a mapping for this signal */
	for(index = 0; index < signalMappings; index++){
		if(_signal_ == signalMapping[index].signal){
			const Exception * exception = signalMapping[index].exception;
			/* make ensure we have a proper exception to throw */
			if(exception == NULL) break;
			/* reset the handler for this signal */
			signal(_signal_, handleSignal);
			throw(*exception);
		}
	}
	/* we were not supposed to handle this signal */
	SIG_DFL(_signal_);
}

EXCEPT4C_JMP_BUF * except4c_firstStep(){

	/* Create a new context */
	Context * tmp = (Context *)malloc( sizeof(Context) );

	/* If there wasn't enough memory... */
	if(tmp == NULL){
		/* ...we will exit right away */
		currentContext = NULL;
		throw(NotEnoughMemoryException);
	}
	tmp->stage		= beginning;
	tmp->uncaught	= false;
	tmp->prev		= currentContext;
	currentContext	= tmp;

	return( &(tmp->address) );
}

bool e4c_isOkToBreak(){

	Context *	tmp;
	bool		uncaught;

	/* Is the stage Ok to break? */
	if(currentContext->stage < ending){
		return(false);
	}

	/* Delete last context */
	tmp				= currentContext;
	uncaught		= currentContext->uncaught;
	currentContext	= currentContext->prev;
	free(tmp);
	/* If current context has no uncaught exception... */
	if(uncaught == false){
		/* ...then clear it */
		exception.name			= (const char *)NULL;
		exception.description	= (const char *)NULL;
		exception.super			= (const Exception *)NULL;
	}else{
		/* ...otherwise, propagate it */
		e4c_throwException(exception, (const char *)NULL, 0);
	}

	/* Ok to break */
	return(true);
}

bool e4c_nextStep(){
	currentContext->stage++;
	return(true);
}

bool e4c_isOkToTry(){
	return(currentContext->stage == trying);
}

bool e4c_isOkToFinalize(){
	return(currentContext->stage == finalizing);
}

bool e4c_isOkToCatch(const Exception _exception_){

	const Exception * super;

	if( currentContext->stage != catching || !extends(&exception, &_exception_) ){
		return(false);
	}

	currentContext->uncaught = false;
	return(true);
}


extern void e4c_throwException(const Exception _exception_, const char * file, int line){

	/* Ignore unnamed exceptions */
	if(_exception_.name == NULL) return;

	if(file != NULL){
		/* Update debug info */
		srcFile = file;
		srcLine = line;
	}

	exception = _exception_;

	/* If we are not inside a try block... */
	if(currentContext == NULL){
		exit(EXIT_FAILURE);
	}

	/* ...otherwise, we'll jump to previous try block */
	currentContext->uncaught = true;

	EXCEPT4C_LONGJMP(currentContext->address);
}

const Exception newException(const char * name, const char * description, const Exception * super){

	Exception tmp = {
		name:			name,
		description:	description,
		super:			super
	};

	return(tmp);
}

void atUncaughtException(){

	if(exception.name != NULL){
		if(srcFile != NULL){
			fprintf(stderr, "\n\n%s:%u: %s (%s).\n\n", srcFile, srcLine, exception.name, exception.description);
			printExceptionHierarchy(exception);
		}else{
			fprintf(stderr, "\n\nFatal Error: %s (%s)\n\n", exception.name, exception.description);
		}
	}
}

void printExceptionHierarchy(Exception _exception_){

	const char * separator = "________________________________________________________________";
	int deep = 0;
	fprintf(stderr, "%s Exception hierarchy\n%s\n\n   %s\n", _exception_.name, separator, _exception_.name);
	while( _exception_.super != NULL && _exception_.name != _exception_.super->name){
		_exception_ = *_exception_.super;
		fprintf(stderr, "    %*s |\n    %*s +-- %s\n", deep * 6, "", deep * 6, "", _exception_.name);
		deep++;
	}
	fprintf(stderr, "%s\n\n", separator);
}

void setSignalHandlers(const SignalMapping * mapping, int mappings){

	int index;

	signalMapping	= mapping;
	signalMappings  = mappings;

	for(index = 0; index < signalMappings; index++){
        if(signalMapping[index].exception == NULL){
			signal(signalMapping[index].signal, SIG_DFL);
		}else{
			signal(signalMapping[index].signal, handleSignal);
		}
	}

}

void initializeExceptionHandling(bool handleSignals){

	atexit(atUncaughtException);

	if(handleSignals){
		setSignalHandlers(defaultSignalMapping, defaultSignalMappings);
	}
}
