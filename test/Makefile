
RUN					= ALL_SUITES
REPORT_FILE			= report.html
CC                  = gcc
CDEFINES			= -DE4C_NOKEYWORDS
CFLAGS              = -Wall -Wextra -ansi -pedantic $(CDEFINES)
SPLINT				= splint
SDEFINES			= -D_ISOC99_SOURCE
SFLAGS				= -strict -namechecks -whileblock -forblock -elseifcomplete -stringliteralsmaller $(SDEFINES)
BIN                 = e4c_test
RM                  = rm -f
WGET                = wget
URL_TRUNK           = http://exceptions4c.googlecode.com/svn/trunk
URL_SRC             = $(URL_TRUNK)/src
URL_TEST            = $(URL_TRUNK)/test
URL_ETC             = $(URL_TRUNK)/etc
LINKOBJ             = $(OBJ)

SRC                 = $(SRC_LIBRARY) $(SRC_TEST_FRAMEWORK) $(SRC_TEST_SUITES)
SRC_LIBRARY         = e4c.h e4c.c
SRC_TEST_FRAMEWORK  = main.c testing.h testing.c html.h html.c macros.h macros.c platform.h e4c_rsc.h e4c_rsc.c
SRC_TEST_SUITES     = run__all.c $(SRC_TEST_SUITE_A) $(SRC_TEST_SUITE_B) $(SRC_TEST_SUITE_C) $(SRC_TEST_SUITE_D) $(SRC_TEST_SUITE_E) $(SRC_TEST_SUITE_F) $(SRC_TEST_SUITE_G) $(SRC_TEST_SUITE_H) $(SRC_TEST_SUITE_Z)
SRC_TEST_SUITE_A    = run_a.c suite_a.c test_a01.c test_a02.c test_a03.c test_a04.c test_a05.c test_a06.c
SRC_TEST_SUITE_B    = run_b.c suite_b.c test_b01.c test_b02.c test_b03.c test_b04.c test_b05.c test_b06.c test_b07.c test_b08.c test_b09.c test_b10.c test_b11.c test_b12.c test_b13.c test_b14.c
SRC_TEST_SUITE_C    = run_c.c suite_c.c test_c01.c test_c02.c
SRC_TEST_SUITE_D    = run_d.c suite_d.c test_d01.c test_d02.c test_d03.c test_d04.c test_d05.c
SRC_TEST_SUITE_E    = run_e.c suite_e.c test_e01.c test_e02.c test_e03.c
SRC_TEST_SUITE_F    = run_f.c suite_f.c test_f01.c test_f02.c test_f03.c test_f04.c test_f05.c test_f06.c test_f07.c
SRC_TEST_SUITE_G    = run_g.c suite_g.c test_g01.c test_g02.c test_g03.c test_g04.c test_g05.c test_g06.c test_g07.c test_g08.c test_g09.c
SRC_TEST_SUITE_H    = run_h.c suite_h.c test_h01.c test_h02.c test_h03.c test_h04.c test_h05.c test_h06.c test_h07.c test_h08.c test_h09.c test_h10.c test_h11.c
SRC_TEST_SUITE_Z    = run_z.c suite_z.c test_z01.c test_z02.c test_z03.c test_z04.c test_z05.c test_z06.c test_z07.c test_z08.c test_z09.c test_z10.c test_z11.c test_z12.c

OBJ                 = $(OBJ_LIBRARY) $(OBJ_TEST_FRAMEWORK) $(OBJ_TEST_SUITES)
OBJ_LIBRARY         = e4c.o
OBJ_TEST_FRAMEWORK  = main.o testing.o html.o macros.o e4c_rsc.o
OBJ_TEST_SUITES     = run__all.o $(OBJ_TEST_SUITE_A) $(OBJ_TEST_SUITE_B) $(OBJ_TEST_SUITE_C) $(OBJ_TEST_SUITE_D) $(OBJ_TEST_SUITE_E) $(OBJ_TEST_SUITE_F) $(OBJ_TEST_SUITE_G) $(OBJ_TEST_SUITE_H) $(OBJ_TEST_SUITE_Z)
OBJ_TEST_SUITE_A    = run_a.o suite_a.o test_a01.o test_a02.o test_a03.o test_a04.o test_a05.o test_a06.o
OBJ_TEST_SUITE_B    = run_b.o suite_b.o test_b01.o test_b02.o test_b03.o test_b04.o test_b05.o test_b06.o test_b07.o test_b08.o test_b09.o test_b10.o test_b11.o test_b12.o test_b13.o test_b14.o
OBJ_TEST_SUITE_C    = run_c.o suite_c.o test_c01.o test_c02.o
OBJ_TEST_SUITE_D    = run_d.o suite_d.o test_d01.o test_d02.o test_d03.o test_d04.o test_d05.o
OBJ_TEST_SUITE_E    = run_e.o suite_e.o test_e01.o test_e02.o test_e03.o
OBJ_TEST_SUITE_F    = run_f.o suite_f.o test_f01.o test_f02.o test_f03.o test_f04.o test_f05.o test_f06.o test_f07.o
OBJ_TEST_SUITE_G    = run_g.o suite_g.o test_g01.o test_g02.o test_g03.o test_g04.o test_g05.o test_g06.o test_g07.o test_g08.o test_g09.o
OBJ_TEST_SUITE_H    = run_h.o suite_h.o test_h01.o test_h02.o test_h03.o test_h04.o test_h05.o test_h06.o test_h07.o test_h08.o test_h09.o test_h10.o test_h11.o
OBJ_TEST_SUITE_Z    = run_z.o suite_z.o test_z01.o test_z02.o test_z03.o test_z04.o test_z05.o test_z06.o test_z07.o test_z08.o test_z09.o test_z10.o test_z11.o test_z12.o

.PHONY: all run clean

all: $(SRC) $(BIN)

run: all
	./$(BIN)

clean:
	${RM} $(OBJ) $(BIN)

splint: $(SRC)
	$(SPLINT) $(SFLAGS) *.c

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN)


e4c.o: e4c.c
	$(CC) -c e4c.c -o e4c.o $(CFLAGS)


main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS) -DRUN=$(RUN) -DREPORT_FILE="\"$(REPORT_FILE)"\"

testing.o: testing.c
	$(CC) -c testing.c -o testing.o $(CFLAGS)

html.o: html.c
	$(CC) -c html.c -o html.o $(CFLAGS)

macros.o: macros.c
	$(CC) -c macros.c -o macros.o $(CFLAGS)

e4c_rsc.o: e4c_rsc.c
	$(CC) -c e4c_rsc.c -o e4c_rsc.o $(CFLAGS)


run__all.o: run__all.c
	$(CC) -c run__all.c -o run__all.o $(CFLAGS)

run_a.o: run_a.c
	$(CC) -c run_a.c -o run_a.o $(CFLAGS)

run_b.o: run_b.c
	$(CC) -c run_b.c -o run_b.o $(CFLAGS)

run_c.o: run_c.c
	$(CC) -c run_c.c -o run_c.o $(CFLAGS)

run_d.o: run_d.c
	$(CC) -c run_d.c -o run_d.o $(CFLAGS)

run_e.o: run_e.c
	$(CC) -c run_e.c -o run_e.o $(CFLAGS)

run_f.o: run_f.c
	$(CC) -c run_f.c -o run_f.o $(CFLAGS)

run_g.o: run_g.c
	$(CC) -c run_g.c -o run_g.o $(CFLAGS)

run_h.o: run_h.c
	$(CC) -c run_h.c -o run_h.o $(CFLAGS)

run_z.o: run_z.c
	$(CC) -c run_z.c -o run_z.o $(CFLAGS)


suite_a.o: suite_a.c
	$(CC) -c suite_a.c -o suite_a.o $(CFLAGS)

suite_b.o: suite_b.c
	$(CC) -c suite_b.c -o suite_b.o $(CFLAGS)

suite_c.o: suite_c.c
	$(CC) -c suite_c.c -o suite_c.o $(CFLAGS)

suite_d.o: suite_d.c
	$(CC) -c suite_d.c -o suite_d.o $(CFLAGS)

suite_e.o: suite_e.c
	$(CC) -c suite_e.c -o suite_e.o $(CFLAGS)

suite_f.o: suite_f.c
	$(CC) -c suite_f.c -o suite_f.o $(CFLAGS)

suite_g.o: suite_g.c
	$(CC) -c suite_g.c -o suite_g.o $(CFLAGS)

suite_h.o: suite_h.c
	$(CC) -c suite_h.c -o suite_h.o $(CFLAGS)

suite_z.o: suite_z.c
	$(CC) -c suite_z.c -o suite_z.o $(CFLAGS)


test_a01.o: test_a01.c
	$(CC) -c test_a01.c -o test_a01.o $(CFLAGS)

test_a02.o: test_a02.c
	$(CC) -c test_a02.c -o test_a02.o $(CFLAGS)

test_a03.o: test_a03.c
	$(CC) -c test_a03.c -o test_a03.o $(CFLAGS)

test_a04.o: test_a04.c
	$(CC) -c test_a04.c -o test_a04.o $(CFLAGS)

test_a05.o: test_a05.c
	$(CC) -c test_a05.c -o test_a05.o $(CFLAGS)

test_a06.o: test_a06.c
	$(CC) -c test_a06.c -o test_a06.o $(CFLAGS)

test_b01.o: test_b01.c
	$(CC) -c test_b01.c -o test_b01.o $(CFLAGS)

test_b02.o: test_b02.c
	$(CC) -c test_b02.c -o test_b02.o $(CFLAGS)

test_b03.o: test_b03.c
	$(CC) -c test_b03.c -o test_b03.o $(CFLAGS)

test_b04.o: test_b04.c
	$(CC) -c test_b04.c -o test_b04.o $(CFLAGS)

test_b05.o: test_b05.c
	$(CC) -c test_b05.c -o test_b05.o $(CFLAGS)

test_b06.o: test_b06.c
	$(CC) -c test_b06.c -o test_b06.o $(CFLAGS)

test_b07.o: test_b07.c
	$(CC) -c test_b07.c -o test_b07.o $(CFLAGS)

test_b08.o: test_b08.c
	$(CC) -c test_b08.c -o test_b08.o $(CFLAGS)

test_b09.o: test_b09.c
	$(CC) -c test_b09.c -o test_b09.o $(CFLAGS)

test_b10.o: test_b10.c
	$(CC) -c test_b10.c -o test_b10.o $(CFLAGS)

test_b11.o: test_b11.c
	$(CC) -c test_b11.c -o test_b11.o $(CFLAGS)

test_b12.o: test_b12.c
	$(CC) -c test_b12.c -o test_b12.o $(CFLAGS)

test_b13.o: test_b13.c
	$(CC) -c test_b13.c -o test_b13.o $(CFLAGS)

test_b14.o: test_b14.c
	$(CC) -c test_b14.c -o test_b14.o $(CFLAGS)

test_c01.o: test_c01.c
	$(CC) -c test_c01.c -o test_c01.o $(CFLAGS)

test_c02.o: test_c02.c
	$(CC) -c test_c02.c -o test_c02.o $(CFLAGS)

test_d01.o: test_d01.c
	$(CC) -c test_d01.c -o test_d01.o $(CFLAGS)

test_d02.o: test_d02.c
	$(CC) -c test_d02.c -o test_d02.o $(CFLAGS)

test_d03.o: test_d03.c
	$(CC) -c test_d03.c -o test_d03.o $(CFLAGS)

test_d04.o: test_d04.c
	$(CC) -c test_d04.c -o test_d04.o $(CFLAGS)

test_d05.o: test_d05.c
	$(CC) -c test_d05.c -o test_d05.o $(CFLAGS)

test_e01.o: test_e01.c
	$(CC) -c test_e01.c -o test_e01.o $(CFLAGS)

test_e02.o: test_e02.c
	$(CC) -c test_e02.c -o test_e02.o $(CFLAGS)

test_e03.o: test_e03.c
	$(CC) -c test_e03.c -o test_e03.o $(CFLAGS)

test_f01.o: test_f01.c
	$(CC) -c test_f01.c -o test_f01.o $(CFLAGS)

test_f02.o: test_f02.c
	$(CC) -c test_f02.c -o test_f02.o $(CFLAGS)

test_f03.o: test_f03.c
	$(CC) -c test_f03.c -o test_f03.o $(CFLAGS)

test_f04.o: test_f04.c
	$(CC) -c test_f04.c -o test_f04.o $(CFLAGS)

test_f05.o: test_f05.c
	$(CC) -c test_f05.c -o test_f05.o $(CFLAGS)

test_f06.o: test_f06.c
	$(CC) -c test_f06.c -o test_f06.o $(CFLAGS)

test_f07.o: test_f07.c
	$(CC) -c test_f07.c -o test_f07.o $(CFLAGS)

test_g01.o: test_g01.c
	$(CC) -c test_g01.c -o test_g01.o $(CFLAGS)

test_g02.o: test_g02.c
	$(CC) -c test_g02.c -o test_g02.o $(CFLAGS)

test_g03.o: test_g03.c
	$(CC) -c test_g03.c -o test_g03.o $(CFLAGS)

test_g04.o: test_g04.c
	$(CC) -c test_g04.c -o test_g04.o $(CFLAGS)

test_g05.o: test_g05.c
	$(CC) -c test_g05.c -o test_g05.o $(CFLAGS)

test_g06.o: test_g06.c
	$(CC) -c test_g06.c -o test_g06.o $(CFLAGS)

test_g07.o: test_g07.c
	$(CC) -c test_g07.c -o test_g07.o $(CFLAGS)

test_g08.o: test_g08.c
	$(CC) -c test_g08.c -o test_g08.o $(CFLAGS)

test_g09.o: test_g09.c
	$(CC) -c test_g09.c -o test_g09.o $(CFLAGS)

test_h01.o: test_h01.c
	$(CC) -c test_h01.c -o test_h01.o $(CFLAGS)

test_h02.o: test_h02.c
	$(CC) -c test_h02.c -o test_h02.o $(CFLAGS)

test_h03.o: test_h03.c
	$(CC) -c test_h03.c -o test_h03.o $(CFLAGS)

test_h04.o: test_h04.c
	$(CC) -c test_h04.c -o test_h04.o $(CFLAGS)

test_h05.o: test_h05.c
	$(CC) -c test_h05.c -o test_h05.o $(CFLAGS)

test_h06.o: test_h06.c
	$(CC) -c test_h06.c -o test_h06.o $(CFLAGS)

test_h07.o: test_h07.c
	$(CC) -c test_h07.c -o test_h07.o $(CFLAGS)

test_h08.o: test_h08.c
	$(CC) -c test_h08.c -o test_h08.o $(CFLAGS)

test_h09.o: test_h09.c
	$(CC) -c test_h09.c -o test_h09.o $(CFLAGS)

test_h10.o: test_h10.c
	$(CC) -c test_h10.c -o test_h10.o $(CFLAGS)

test_h11.o: test_h11.c
	$(CC) -c test_h11.c -o test_h11.o $(CFLAGS)


test_z01.o: test_z01.c
	$(CC) -c test_z01.c -o test_z01.o $(CFLAGS)

test_z02.o: test_z02.c
	$(CC) -c test_z02.c -o test_z02.o $(CFLAGS)

test_z03.o: test_z03.c
	$(CC) -c test_z03.c -o test_z03.o $(CFLAGS)

test_z04.o: test_z04.c
	$(CC) -c test_z04.c -o test_z04.o $(CFLAGS)

test_z05.o: test_z05.c
	$(CC) -c test_z05.c -o test_z05.o $(CFLAGS)

test_z06.o: test_z06.c
	$(CC) -c test_z06.c -o test_z06.o $(CFLAGS)

test_z07.o: test_z07.c
	$(CC) -c test_z07.c -o test_z07.o $(CFLAGS)

test_z08.o: test_z08.c
	$(CC) -c test_z08.c -o test_z08.o $(CFLAGS)

test_z09.o: test_z09.c
	$(CC) -c test_z09.c -o test_z09.o $(CFLAGS)

test_z10.o: test_z10.c
	$(CC) -c test_z10.c -o test_z10.o $(CFLAGS)

test_z11.o: test_z11.c
	$(CC) -c test_z11.c -o test_z11.o $(CFLAGS)

test_z12.o: test_z12.c
	$(CC) -c test_z12.c -o test_z12.o $(CFLAGS)

e4c.h:
	$(WGET) $(URL_SRC)/e4c.h

e4c.c:
	$(WGET) $(URL_SRC)/e4c.c

main.c:
	$(WGET) $(URL_TEST)/main.c

testing.h:
	$(WGET) $(URL_TEST)/testing.h

testing.c:
	$(WGET) $(URL_TEST)/testing.c

html.h:
	$(WGET) $(URL_TEST)/html.h

html.c:
	$(WGET) $(URL_TEST)/html.c

macros.h:
	$(WGET) $(URL_TEST)/macros.h

macros.c:
	$(WGET) $(URL_TEST)/macros.c

platform.h:
	$(WGET) $(URL_TEST)/platform.h

e4c_rsc.h:
	$(WGET) $(URL_ETC)/e4c_rsc.h

e4c_rsc.c:
	$(WGET) $(URL_ETC)/e4c_rsc.c

run__all.c:
	$(WGET) $(URL_TEST)/run__all.c

run_a.c:
	$(WGET) $(URL_TEST)/run_a.c

run_b.c:
	$(WGET) $(URL_TEST)/run_b.c

run_c.c:
	$(WGET) $(URL_TEST)/run_c.c

run_d.c:
	$(WGET) $(URL_TEST)/run_d.c

run_e.c:
	$(WGET) $(URL_TEST)/run_e.c

run_f.c:
	$(WGET) $(URL_TEST)/run_f.c

run_g.c:
	$(WGET) $(URL_TEST)/run_g.c

run_h.c:
	$(WGET) $(URL_TEST)/run_h.c

run_z.c:
	$(WGET) $(URL_TEST)/run_z.c


suite_a.c:
	$(WGET) $(URL_TEST)/suite_a.c

suite_b.c:
	$(WGET) $(URL_TEST)/suite_b.c

suite_c.c:
	$(WGET) $(URL_TEST)/suite_c.c

suite_d.c:
	$(WGET) $(URL_TEST)/suite_d.c

suite_e.c:
	$(WGET) $(URL_TEST)/suite_e.c

suite_f.c:
	$(WGET) $(URL_TEST)/suite_f.c

suite_g.c:
	$(WGET) $(URL_TEST)/suite_g.c

suite_h.c:
	$(WGET) $(URL_TEST)/suite_h.c

suite_z.c:
	$(WGET) $(URL_TEST)/suite_z.c


test_a01.c:
	$(WGET) $(URL_TEST)/test_a01.c

test_a02.c:
	$(WGET) $(URL_TEST)/test_a02.c

test_a03.c:
	$(WGET) $(URL_TEST)/test_a03.c

test_a04.c:
	$(WGET) $(URL_TEST)/test_a04.c

test_a05.c:
	$(WGET) $(URL_TEST)/test_a05.c

test_a06.c:
	$(WGET) $(URL_TEST)/test_a06.c

test_b01.c:
	$(WGET) $(URL_TEST)/test_b01.c

test_b02.c:
	$(WGET) $(URL_TEST)/test_b02.c

test_b03.c:
	$(WGET) $(URL_TEST)/test_b03.c

test_b04.c:
	$(WGET) $(URL_TEST)/test_b04.c

test_b05.c:
	$(WGET) $(URL_TEST)/test_b05.c

test_b06.c:
	$(WGET) $(URL_TEST)/test_b06.c

test_b07.c:
	$(WGET) $(URL_TEST)/test_b07.c

test_b08.c:
	$(WGET) $(URL_TEST)/test_b08.c

test_b09.c:
	$(WGET) $(URL_TEST)/test_b09.c

test_b10.c:
	$(WGET) $(URL_TEST)/test_b10.c

test_b11.c:
	$(WGET) $(URL_TEST)/test_b11.c

test_b12.c:
	$(WGET) $(URL_TEST)/test_b12.c

test_b13.c:
	$(WGET) $(URL_TEST)/test_b13.c

test_b14.c:
	$(WGET) $(URL_TEST)/test_b14.c

test_c01.c:
	$(WGET) $(URL_TEST)/test_c01.c

test_c02.c:
	$(WGET) $(URL_TEST)/test_c02.c

test_d01.c:
	$(WGET) $(URL_TEST)/test_d01.c

test_d02.c:
	$(WGET) $(URL_TEST)/test_d02.c

test_d03.c:
	$(WGET) $(URL_TEST)/test_d03.c

test_d04.c:
	$(WGET) $(URL_TEST)/test_d04.c

test_d05.c:
	$(WGET) $(URL_TEST)/test_d05.c

test_e01.c:
	$(WGET) $(URL_TEST)/test_e01.c

test_e02.c:
	$(WGET) $(URL_TEST)/test_e02.c

test_e03.c:
	$(WGET) $(URL_TEST)/test_e03.c

test_f01.c:
	$(WGET) $(URL_TEST)/test_f01.c

test_f02.c:
	$(WGET) $(URL_TEST)/test_f02.c

test_f03.c:
	$(WGET) $(URL_TEST)/test_f03.c

test_f04.c:
	$(WGET) $(URL_TEST)/test_f04.c

test_f05.c:
	$(WGET) $(URL_TEST)/test_f05.c

test_f06.c:
	$(WGET) $(URL_TEST)/test_f06.c

test_f07.c:
	$(WGET) $(URL_TEST)/test_f07.c

test_g01.c:
	$(WGET) $(URL_TEST)/test_g01.c

test_g02.c:
	$(WGET) $(URL_TEST)/test_g02.c

test_g03.c:
	$(WGET) $(URL_TEST)/test_g03.c

test_g04.c:
	$(WGET) $(URL_TEST)/test_g04.c

test_g05.c:
	$(WGET) $(URL_TEST)/test_g05.c

test_g06.c:
	$(WGET) $(URL_TEST)/test_g06.c

test_g07.c:
	$(WGET) $(URL_TEST)/test_g07.c

test_g08.c:
	$(WGET) $(URL_TEST)/test_g08.c

test_g09.c:
	$(WGET) $(URL_TEST)/test_g09.c

test_h01.c:
	$(WGET) $(URL_TEST)/test_h01.c

test_h02.c:
	$(WGET) $(URL_TEST)/test_h02.c

test_h03.c:
	$(WGET) $(URL_TEST)/test_h03.c

test_h04.c:
	$(WGET) $(URL_TEST)/test_h04.c

test_h05.c:
	$(WGET) $(URL_TEST)/test_h05.c

test_h06.c:
	$(WGET) $(URL_TEST)/test_h06.c

test_h07.c:
	$(WGET) $(URL_TEST)/test_h07.c

test_h08.c:
	$(WGET) $(URL_TEST)/test_h08.c

test_h09.c:
	$(WGET) $(URL_TEST)/test_h09.c

test_h10.c:
	$(WGET) $(URL_TEST)/test_h10.c

test_h11.c:
	$(WGET) $(URL_TEST)/test_h11.c


test_z01.c:
	$(WGET) $(URL_TEST)/test_z01.c

test_z02.c:
	$(WGET) $(URL_TEST)/test_z02.c

test_z03.c:
	$(WGET) $(URL_TEST)/test_z03.c

test_z04.c:
	$(WGET) $(URL_TEST)/test_z04.c

test_z05.c:
	$(WGET) $(URL_TEST)/test_z05.c

test_z06.c:
	$(WGET) $(URL_TEST)/test_z06.c

test_z07.c:
	$(WGET) $(URL_TEST)/test_z07.c

test_z08.c:
	$(WGET) $(URL_TEST)/test_z08.c

test_z09.c:
	$(WGET) $(URL_TEST)/test_z09.c

test_z10.c:
	$(WGET) $(URL_TEST)/test_z10.c

test_z11.c:
	$(WGET) $(URL_TEST)/test_z11.c

test_z12.c:
	$(WGET) $(URL_TEST)/test_z12.c
