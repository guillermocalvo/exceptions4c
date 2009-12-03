/*
	----------------------------------------------------------------
	test.c

	exceptions4c
	version 1.1
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


# define LINE(string)			printf("%s\n", string)

# define BLOCK_1(string)		printf("\n%s\n----------------------------------------------------------------\n", string)
# define BLOCK_2(string)		printf("\n        %s\n        --------------------------------------------------------\n", string)
# define BLOCK_3(string)		printf("\n                %s\n                ------------------------------------------------\n", string)

# define ECHO_1(string)			printf("    * %s\n", string)
# define ECHO_2(string)			printf("            * %s\n", string)
# define ECHO_3(string)			printf("                    * %s\n", string)

# define SHOW_1(statement)		printf("    * %s;\n", #statement); statement
# define SHOW_2(statement)		printf("            * %s;\n", #statement); statement
# define SHOW_3(statement)		printf("                    * %s;\n", #statement); statement

# define SVAR_1(svar)			printf("    * %s: \"%s\"\n", #svar, svar)
# define SVAR_2(svar)			printf("            * %s: \"%s\"\n", #svar, svar)
# define SVAR_3(svar)			printf("                    * %s: \"%s\"\n", #svar, svar)


# include <stdio.h>
# include <stdlib.h>
# include "except4c.h"


void generateError(int whichOne);
void info(int test);
void runTest(int test);
void test1();
void test2();
void test3();
void test4();
void test5();
void aux3();
void aux5();
void pause();


DEFINE_EXCEPTION(ColorException,	"This is a colorful exception",		RuntimeException);
DEFINE_EXCEPTION(RedException,		"This exception is a red one",		ColorException);
DEFINE_EXCEPTION(BlueException,		"This exception is a blue one",		ColorException);
DEFINE_EXCEPTION(DarkRedException,	"This exception is dark red",		RedException);


int main(int argc, char *argv[]){

	int test;

	atexit(pause);
	initializeExceptionHandling(true);

	/*

	try{
		int i = 10 / 0;
	}catch(ArithmeticException){
		printf("Aqui no ha pasado nada, señores... :-)\n");
	}

	try{
		int * p = NULL;
		*p = 123;
	}catch(BadPointerException){
		printf("Aqui no ha pasado nada, señores... :-)\n");
	}

	return(0);

	*/

	LINE(">>");
	LINE(">> WE ARE INSIDE 'MAIN'");
	LINE(">>");

	for(test = 1; test < 6; test++){

		runTest(test);

		LINE("\n>>");
		LINE(">> WE ARE INSIDE 'MAIN' AGAIN");
		LINE(">>\n");

		pause();
	}

	ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");

	return(EXIT_SUCCESS);
}

void pause(){
	printf("\n **** Press <ENTER> to continue... ****\n\n");
	getchar();
}

void generateError(int whichOne){

	if(whichOne == 3 || whichOne == 5){
        BLOCK_3("We are inside 'generateError' now...");
	}else{
        BLOCK_2("We are inside 'generateError' now...");
	}

	switch(whichOne){

		case 1:
			SHOW_2( throw(NotEnoughMemoryException) );
			break;

		case 2:
			{
				int zero, number;
				SHOW_2( zero = 0 );
				SHOW_2( number = 100 / zero );
			}
			break;

		case 3:
			SHOW_3( throw(BlueException) );
			break;

		case 4:
			{
				int * pointer, number;
				SHOW_2( pointer = NULL );
				SHOW_2( number = *pointer );
			}
			break;

		case 5:
			SHOW_3( throw(RedException) );
			break;
	}

	ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
}

void runTest(int test){
	info(test);
	switch(test){
		case 1: test1(); break;
		case 2: test2(); break;
		case 3: test3(); break;
		case 4: test4(); break;
		case 5: test5(); break;
	}
}

void info(int test){

	const char * separator = "________________________________________________________________";

	printf("%s\n\nTEST #%d\n%s\n\n", separator, test, separator);

	LINE(" This is what's going to happen:");

	switch(test){

		case 1:
			LINE("  1. 'test1' will call 'generateError' inside a <try> block.");
			LINE("  2. 'NotEnoughMemoryException' will be thrown from 'generateError'.");
			LINE("  3. 'test1' will catch it.");
			LINE("  4. Then 'test1's <finally> block will be executed.");
			LINE("  5. Return from 'test1' back to 'main'.");
			break;

		case 2:
			LINE("  1. 'test2' will call 'generateError' inside a <try> block.");
			LINE("  2. 'generateError' will attempt to divide 100 by 0.");
			LINE("  3. A signal SIGFPE will be raised.");
			LINE("  4. The signal will be converted to 'ArithmeticException'.");
			LINE("  5. 'test2' will catch it. There is no <finally> block.");
			LINE("  6. Return from 'test2' back to 'main'.");
			break;

		case 3:
			LINE("  1. 'test3' will call 'aux3' inside a <try> block.");
			LINE("  2. 'aux3' will call 'generateError' inside another <try> block.");
			LINE("  3. 'generateError' will throw 'BlueException'.");
			LINE("  4. 'aux3' will catch any 'ColorException'.");
			LINE("  5.' aux3' will throw 'RedException' inside its <catch> block.");
			LINE("  6. 'aux3's <finally> block will be executed.");
			LINE("  7. 'test3' will catch any 'RuntimeException'.");
			LINE("  8. 'test3's <finally> block will be executed.");
			LINE("  9. Return from 'test3' back to 'main'.");
			break;

		case 4:
			LINE("  1. 'test4' will call 'generateError' inside a <try> block.");
			LINE("  2. 'generateError' will attempt to dereference a NULL pointer.");
			LINE("  3. A signal SIGSEGV will be raised.");
			LINE("  4. The signal will be converted to 'BadPointerException'.");
			LINE("  5. 'test4' will catch it.");
			LINE("  6. Then 'test4's <finally> block will be executed.");
			LINE("  7. Return from 'test4' back to 'main'.");
			break;

		case 5:
			LINE("  1. OK. This test will *hurt*, so get ready.");
			LINE("  2. 'test5' will call 'aux5' inside a <try> block.");
			LINE("  3. 'aux5' will call 'generateError' inside another <try> block.");
			LINE("  4. 'generateError' will throw a 'BlueException'.");
			LINE("  5. 'aux5' will not be able to catch a blue exception.");
			LINE("  6. 'aux5's <finally> block will be executed.");
			LINE("  7. 'test5' will not be able to catch a blue exception.");
			LINE("  8. 'test5's <finally> block will be executed.");
			LINE("  9. There are no more outter <try> blocks, that means: fast fail.");
			LINE(" 10. The exception handling framework will call 'exit'.");
			LINE(" 11. 'exit' will call any 'atexit' functions.");
			LINE(" 12. That means 'atUncaughtException' will be called if you want.");
			LINE("If you #define DEBUG, 'atUncaughtException' will show more info.");
			LINE("This is the final test, try changing the code and see what happens.");
			break;
	}

	pause();
	LINE("Let's go!");
}

void test1(){

	try{
		BLOCK_1("test1/try");
		ECHO_1("Let's call function 'generateError'...");
		SHOW_1( generateError(1) );
		ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(NullPointerException){
		/* NullPointerException is not being thrown in test1 */
		ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(NotEnoughMemoryException){
		BLOCK_1("test1/catch(NotEnoughMemoryException)");
		ECHO_1("Exception information:");
		SVAR_1(exception.name);
	}catch(UserInterruptionException){
		/* UserInterruptionException is not being thrown in test1 */
		ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}finally{
		BLOCK_1("test1/finally");
		ECHO_1("We could clean up here.\n");
	}

}

void test2(){

	try{
		BLOCK_1("test2/try");
		ECHO_1("What about dividing by zero?");
		SHOW_1( generateError(2) );
		ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(ArithmeticException){
		BLOCK_1("test1/catch(ArithmeticException)");
		ECHO_1("Exception information:");
		SVAR_1(exception.name);
		SVAR_1(exception.description);
		ECHO_1("We have just divided by zero, but our process didn't crash!");
	}

}

void aux3(){

	try{
        BLOCK_2("aux3/try");
		ECHO_2("Let's call function 'generateError'...");
		SHOW_2( generateError(3) );
		ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(ColorException){
        BLOCK_2("aux3/catch(ColorException)");
		ECHO_2("Exception information:");
		SVAR_2(exception.name);
		ECHO_2("Blue exceptions are sad;");
		ECHO_2("let's throw 'RedException' instead...");
		SHOW_2( throw(RedException) );
	}finally{
        BLOCK_2("aux3/finally");
		ECHO_2("Did you realize that?");
		ECHO_2("We threw 'RedException' ourselves,");
		ECHO_2("but we can still clean up here.");
	}

	ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
}

void test3(){

	try{
        BLOCK_1("test3/try");
		ECHO_1("Let's call function 'aux3'...");
		SHOW_1( aux3() );
		ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(RuntimeException){
        BLOCK_1("test3/catch(RuntimeException)");
		ECHO_1("Exception information:");
		SVAR_1(exception.name);
		ECHO_1("We caught RedException because it \"is a\" RuntimeException:");
		LINE("");
		printExceptionHierarchy(exception);
	}finally{
        BLOCK_1("test3/finally");
		ECHO_1("Again, we could clean up here.");
	}

}

void test4(){

	try{
        BLOCK_1("test4/try");
		ECHO_1("We are about to perform a segment violation...");
		SHOW_1( generateError(4) );
		ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(SignalException){
        BLOCK_1("test4/catch(SignalException)");
		ECHO_1("Exception information:");
		SVAR_1(exception.name);
		SVAR_1(exception.description);
		ECHO_1("We tried to dereference a NULL pointer,");
		ECHO_1("but we didn't core dump!");
	}finally{
        BLOCK_1("test4/finally");
		ECHO_1("We have just avoided our program to crash.");
		ECHO_1("Pretty cool, isn't it?");
	}

}

void aux5(){

	try{
        BLOCK_2("aux5/try");
		ECHO_2("Let's call function 'generateError'...");
		SHOW_2( generateError(5) );
		ECHO_2("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(BlueException){
		ECHO_2("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}finally{
        BLOCK_2("aux5/finally");
		ECHO_2("As you know, we didn't catch 'RedException',");
		ECHO_2("because we are only able to catch 'BlueException'.");
	}

	ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
}

void test5(){

	try{
        BLOCK_1("test5/try");
		ECHO_1("Let's call function 'aux5'...");
		SHOW_1( aux5() );
		ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}catch(DarkRedException){
		ECHO_1("==== YOU ARE NOT GONNA SEE THIS MESSAGE ====");
	}finally{
        BLOCK_1("test5/finally");
		ECHO_1("We didn't catch 'RedException',");
		ECHO_1("we can only catch DarkRedException'.");
		ECHO_1("This is last <try> block, so the application will exit now.");
	}

	ECHO_1("YOU ARE NOT GONNA SEE ANY OF THESE MESSAGES");
}

