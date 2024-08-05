# Copyright [2023] <Burr Etienne>

CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra

TEXI2HTML=makeinfo

CALC_DIR=calc/
CALC_HEADERS_DIR=headers/
GUI_DIR=gui/
TESTS_DIR=../tests/
BUILD_DIR=../build/
GCOV_REPORT_DIR=gcov_report

LIB_NAME=calc.a
BIN_FILE=calculator
TEST_BIN_FILE=test_calculator
DVI_FILENAME=src_program
TAR_NAME=SmartCalc_v1.0.tar
GCOV_INFO_NAME=report.info
VALGRIND_LOG_FILE=valgrind.log

LEAKS=
LIBS=-lcheck `pkg-config --libs --cflags gtk+-3.0`
GTK_INSTALL=

UNAME=$(shell uname)
ifeq ($(UNAME), Linux)
# For testing full app use: valgrind --suppressions=/usr/share/glib-2.0/valgrind/glib.supp 
# --suppressions=/usr/share/gtk-3.0/valgrind/gtk.supp --log-file="valgrind_full.log" --quiet ../build/calculator
	LEAKS=valgrind --tool=memcheck --leak-check=yes --log-file="$(VALGRIND_LOG_FILE)" --quiet ./$(TEST_BIN_FILE)
	LIBS+= -lm -lsubunit
	GTK_INSTALL=sudo apt-get install libgtk-3-dev
else
	LEAKS=leaks -atExit -- ./$(TEST_BIN_FILE)
	GTK_INSTALL=brew install gtk+3
endif



.SILENT: all $(LIB_NAME) install uninstall clean dvi dist test gcov_report run check clang cppcheck cpplint format leaks gtk_install



all: clean uninstall install run

$(LIB_NAME): clean $(CALC_DIR)*.c
	$(CC) $(CFLAGS) -c $(CALC_DIR)calc_*.c
	ar rc $(LIB_NAME) calc_*.o
	rm -rf *.o 2> /dev/null

install: uninstall $(LIB_NAME) src*.c $(GUI_DIR)*.c 
	mkdir $(BUILD_DIR)
	cp -R $(GUI_DIR) $(BUILD_DIR)$(GUI_DIR)
	$(CC) $(CFLAGS) -Wno-unused-parameter src*.c $(GUI_DIR)*.c $(LIB_NAME) -o $(BUILD_DIR)$(BIN_FILE) $(LIBS);

uninstall:
	rm -rf $(BUILD_DIR) 2> /dev/null

clean:
	rm -rf *.gch *.o *.out *.a *.gcno *.gcda *.gcov *.info $(TEST_BIN_FILE) $(VALGRIND_LOG_FILE) $(GCOV_REPORT_DIR) *.dSYM *.html 2> /dev/null

dvi:
	$(TEXI2HTML) --no-split --html $(DVI_FILENAME).texi
	open $(DVI_FILENAME).html

dist:
	tar -cf $(TAR_NAME) *.c *.h Makefile $(DVI_FILENAME).texi $(GUI_DIR) $(CALC_DIR)

test: $(LIB_NAME) $(TESTS_DIR)*.c $(TESTS_DIR)*.h
	$(CC) -o $(TEST_BIN_FILE) $(TESTS_DIR)*.c $(LIBS) $(LIB_NAME)
	./$(TEST_BIN_FILE)
	rm -rf *.o *.gch 2> /dev/null

gcov_report: clean $(TESTS_DIR)*.c $(TESTS_DIR)*.h $(CALC_DIR)*.c
	$(CC) -o $(TEST_BIN_FILE) $(CFLAGS) $(TESTS_DIR)*.c $(CALC_DIR)*.c $(LIBS) --coverage
	./$(TEST_BIN_FILE)
	lcov -t "$(GCOV_REPORT_DIR)" -o $(GCOV_INFO_NAME) -c -d .
	genhtml -o $(GCOV_REPORT_DIR) $(GCOV_INFO_NAME)
	open $(GCOV_REPORT_DIR)/index.html
	rm -rf *.o *.gcda *.gcno *.info *.gch $(TEST_BIN_FILE) 2> /dev/null

run:
	$(BUILD_DIR)$(BIN_FILE)



check: clang cppcheck cpplint leaks gcov_report

clang:
	clang-format -style=Google -n *.c *.h $(CALC_DIR)*.c $(CALC_DIR)$(CALC_HEADERS_DIR)*.h \
	$(TESTS_DIR)*.c $(GUI_DIR)*.c $(TESTS_DIR)*.h $(GUI_DIR)*.h
cppcheck:
	cppcheck --enable=all --quiet --suppress=missingIncludeSystem *.c *.h $(CALC_DIR)*.c \
	$(CALC_DIR)$(CALC_HEADERS_DIR)*.h $(TESTS_DIR)*.c $(GUI_DIR)*.c $(TESTS_DIR)*.h $(GUI_DIR)*.h
cpplint:
	cpplint --quiet --filter=-readability/casting *.c *.h $(CALC_DIR)*.c $(CALC_DIR)$(CALC_HEADERS_DIR)*.h \
	$(TESTS_DIR)*.c $(GUI_DIR)*.c $(TESTS_DIR)*.h $(GUI_DIR)*.h
format:
	clang-format -style=Google -i *.c *.h $(CALC_DIR)*.c $(CALC_DIR)$(CALC_HEADERS_DIR)*.h $(TESTS_DIR)*.c $(GUI_DIR)*.c $(TESTS_DIR)*.h $(GUI_DIR)*.h



leaks: test
	$(LEAKS)
	rm -rf $(TEST_BIN_FILE) $(LIB_NAME) 2> /dev/null
	
gtk_install:
	$(GTK_INSTALL)
