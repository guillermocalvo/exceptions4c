/*
	----------------------------------------------------------------
	test.c

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


# define DEBUG
# define LINE(string)			printf("%s\n", string)
# define ECHO(string)			printf("    * %s\n", string)
# define SHOW(statement)		printf("             * %s;\n", #statement); statement
# define SVAR(svar)				printf("             * %s: \"%s\"\n", #svar, svar)


# include <stdio.h>
# include <stdlib.h>
# include "except4c.h"


void generateError(int whichOne);
void info(int test);
void runTest(int test);
void test1();
void test2();
void test3();
void aux2();
void aux3();
void pause();


DEFINE_EXCEPTION(ColorException,	"This is a colorful exception",		RuntimeException);
DEFINE_EXCEPTION(RedException,		"This exception is a red one",		ColorException);
DEFINE_EXCEPTION(BlueException,		"This exception is a blue one",		ColorException);
DEFINE_EXCEPTION(DarkRedException,	"This exception is dark red",		RedException);


int main(int argc, char *argv[]){

	int test;

	atexit(pause);
	atexit(atUncaughtException);

	LINE(">>");
	LINE(">> WE ARE INSIDE 'MAIN'");
	LINE(">>");

	for(test = 1; test < 4; test++){

		runTest(test);

		LINE("\n>>");
		LINE(">> WE ARE INSIDE 'MAIN' AGAIN");
		LINE(">>\n");

		pause();
	}

	ECHO("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");

	return(EXIT_SUCCESS);
}

void pause(){
	printf("\n **** Press <ENTER> to continue... ****\n\n");
	getchar();
}

void generateError(int whichOne){

	ECHO("We are inside 'generateError' now...");

	switch(whichOne){
		case 1: SHOW( throw(NullPointerException) ); break;
		case 2: SHOW( throw(BlueException) ); break;
		case 3: SHOW( throw(RedException) ); break;
	}

	ECHO("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
}

void runTest(int test){
	info(test);
	switch(test){
		case 1: test1(); break;
		case 2: test2(); break;
		case 3: test3(); break;
	}
}

void info(int test){

	const char * separator = "________________________________________________________________";

	printf("%s\n\nTEST #%d\n%s\n\n", separator, test, separator);

	LINE(" This is what's going to happen:");

	switch(test){

		case 1:
			LINE("  1. 'test1' will call 'generateError' inside a <try> block.");
			LINE("  2. 'NullPointerException' will be thrown from 'generateError'.");
			LINE("  3. 'test1' will catch it.");
			LINE("  4. Then 'test1's <finally> block will be executed.");
			LINE("  5. Return from 'test1' back to 'main'.");
			break;

		case 2:
			LINE("  1. 'test2' will call 'aux2' inside a <try> block.");
			LINE("  2. 'aux2' will call 'generateError' inside another <try> block.");
			LINE("  3. 'generateError' will throw 'BlueException'.");
			LINE("  4. 'aux2' will catch any 'ColorException'.");
			LINE("  5.' aux2' will throw 'RedException' inside its <catch> block.");
			LINE("  6. 'aux2's <finally> block will be executed.");
			LINE("  7. 'test2' will catch any 'RuntimeException'.");
			LINE("  8. 'test2's <finally> block will be executed.");
			LINE("  9. Return from 'test2' back to 'main'.");
			break;

		case 3:
			LINE("  1. OK. This test will *hurt*, so get ready.");
			LINE("  2. 'test3' will call 'aux3' inside a <try> block.");
			LINE("  3. 'aux3' will call 'generateError' inside another <try> block.");
			LINE("  4. 'generateError' will throw a 'BlueException'.");
			LINE("  5. 'aux3' will not be able to catch a blue exception.");
			LINE("  6. 'aux3's <finally> block will be executed.");
			LINE("  7. 'test3' will not be able to catch a blue exception.");
			LINE("  8. 'test3's <finally> block will be executed.");
			LINE("  9. There are no more outter <try> blocks, that means: fast fail.");
			LINE(" 10. The exception handling framework will call 'exit'.");
			LINE(" 11. 'exit' will call any 'atexit' functions.");
			LINE(" 12. That means 'atUncaughtException' will be called if you want.");
			LINE("If you #undefine DEBUG, 'atUncaughtException' will show less info.");
			LINE("This is the final test, try changing the code and see what happens.");
			break;
	}

	pause();
	LINE("  Let's go!");
}

void test1(){

	ECHO("We are inside 'test1'");

	try{
		ECHO("test1: We are inside our <try> block,");
		ECHO("       about to call function 'generateError'...");
		SHOW( generateError(1) );
		ECHO("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(NullPointerException){
		ECHO("test1: We are inside our <catch> block;");
		ECHO("       let's see what we got here:");
		SVAR(exception.name);
	}finally{
		ECHO("test1: We are inside our <finally> block.");
		ECHO("       We could clean up here.");
	}

	ECHO("test1: We are about to get back to 'main'...");
}

void aux2(){

	ECHO("We are inside 'aux2'");

	try{
		ECHO("aux2: We are inside our <try> block.");
		ECHO("      Let's call function 'generateError'...");
		SHOW( generateError(2) );
		ECHO("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(ColorException){
		ECHO("aux2: We are inside our <catch> block;");
		ECHO("      let's see what we got here:");
		SVAR(exception.name);
		ECHO("aux2: Blue exceptions are sad;");
		ECHO("      let's throw 'RedException' instead...");
		SHOW( throw(BlueException) );
	}finally{
		ECHO("aux2: We are inside our <finally> block.");
		ECHO("      Did you realize that?");
		ECHO("      We just threw 'RedException', but we can still clean up here.");
		ECHO("aux2: Next, we're jumping right to 'test2's <catch> block.");
	}

	ECHO("YOU ARE NOT GONNA SEE ANY OF THESE MESSAGES");
	ECHO("aux2: We are about to get back to 'test2'...");
}

void test2(){

	ECHO("We are inside 'test2'");

	try{
		ECHO("test2: We are inside our <try> block,");
		ECHO("       about to call function 'aux2'...");
		SHOW( aux2() );
		ECHO("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(RuntimeException){
		ECHO("test2: We are inside our <catch> block;");
		ECHO("       we've caught some kind of 'RuntimeException'.");
		SVAR(exception.name);
		SVAR(exception.description);
		LINE("");
		printExceptionHierarchy(exception);
		ECHO("test2: OK, <catch> block is done.");
	}finally{
		ECHO("test2: We are inside our <finally> block.");
		ECHO("       Again, we could clean up here.");
	}

	ECHO("test2: We are about to get back to 'main'...");
}

void aux3(){

	ECHO("We are inside 'aux3'");

	try{
		ECHO("aux3: We are inside our <try> block.");
		ECHO("      Let's call function 'generateError'...");
		SHOW( generateError(3) );
		ECHO("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(BlueException){
		ECHO("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}finally{
		ECHO("aux3: We are inside our <finally> block.");
		ECHO("      As you know, we didn't catch 'RedException',");
		ECHO("      because we are only able to catch 'BlueException'.");
		ECHO("aux3: Next, we're jumping right to 'test3's <finally> block.");
	}

	ECHO("YOU ARE NOT GONNA SEE ANY OF THESE MESSAGES");
	ECHO("aux3: We are about to get back to 'test3'...");
}

void test3(){

	ECHO("We are inside 'test3'");

	try{
		ECHO("test3: We are inside our <try> block, about to call function 'aux3'...");
		SHOW( aux3() );
		ECHO("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(DarkRedException){
		ECHO("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}finally{
		ECHO("test3: We are inside our <finally> block.");
		ECHO("       We didn't catch 'RedException',");
		ECHO("       we can only catch 'DarkRedException'.");
		ECHO("test3: This is last <try> block, so the application will exit now.");
	}

	ECHO("YOU ARE NOT GONNA SEE ANY OF THESE MESSAGES");
	ECHO("test3: We are about to get back to 'main'...");
}

