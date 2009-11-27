/*
	----------------------------------------------------------------
	except4c.c

	exceptions4c
	version 1.0
	An exception handling framework for C.

	Copyright (c) 2009 Guillermo Calvo

	This is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This software is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this software.  If not, see <http://www.gnu.org/licenses/>.

	Credits:
	This little library has been inspired by these previous works:

	"Exceptions in C"
	By Laurent Deniau
	http://ldeniau.home.cern.ch/ldeniau/html/exception/exception.html

	"Implementing Exceptions in C Programming language"
	By Adomas Paltanavicius
	http://adomas.org/excc/

	----------------------------------------------------------------
*/


# include <stdlib.h>
# include "except4c.h"

typedef enum{beginning, trying, catching, finalizing, ending} Stage;
typedef struct ContextStruct Context;
struct ContextStruct{
	Context *			prev;
	Stage				stage;
	bool				uncaught;
	EXCEPT4C_JMP_BUF	address;
};

static Context *	currentContext	= NULL;
static const char *	srcFile			= NULL;
static int			srcLine			= 0;

Exception			exception		= {
	name:			(const char *)NULL,
	description:	(const char *)NULL,
	super:			(const Exception *)NULL
};

DEFINE_EXCEPTION(RuntimeException,			"Runtime exception",	RuntimeException);
DEFINE_EXCEPTION(NotEnoughMemoryException,	"Not enough memory",	RuntimeException);
DEFINE_EXCEPTION(NullPointerException,		"Null pointer",			RuntimeException);

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

bool except4c_isOkToBreak(){

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
		except4c_throwException(exception, (const char *)NULL, 0);
	}

	/* Ok to break */
	return(true);
}

bool except4c_nextStep(){
	currentContext->stage++;
	return(true);
}

bool except4c_isOkToTry(){
	return(currentContext->stage == trying);
}

bool except4c_isOkToFinalize(){
	return(currentContext->stage == finalizing);
}

bool except4c_isOkToCatch(const Exception exception_){

	const Exception * super;

	if( currentContext->stage != catching || !extends(&exception, &exception_) ){
		return(false);
	}

	currentContext->uncaught = false;
	return(true);
}


extern void except4c_throwException(const Exception exception_, const char * file, int line){
	if(exception_.name == NULL) return;

	if(file != NULL){
		/* Update debug info */
		srcFile = file;
		srcLine = line;
	}

	exception = exception_;

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
			printf("\n\n%s:%u: %s (%s).\n\n", srcFile, srcLine, exception.name, exception.description);
			printExceptionHierarchy(exception);
		}else{
			printf("\n\nFatal Error: %s (%s)\n\n", exception.name, exception.description);
		}
	}
}

void printExceptionHierarchy(Exception exception){
	const char * separator = "________________________________________________________________";
	int deep = 0;
	printf("%s Exception hierarchy\n%s\n\n   %s\n", exception.name, separator, exception.name);
	while( exception.super != NULL && exception.name != exception.super->name){
		exception = *exception.super;
		printf("    %*s |\n    %*s +-- %s\n", deep * 6, "", deep * 6, "", exception.name);
		deep++;
	}
	printf("%s\n\n", separator);
}
