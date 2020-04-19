OBJS = my_grep.o file_search_handler.o output_printer.o test.o
EXEC = my_grep


.PHONY: all clean test

all: my_grep

my_grep: my_grep.o file_search_handler.o output_printer.o
	@gcc -Wall -Wextra -o my_grep my_grep.o output_printer.o file_search_handler.o

my_grep.o: my_grep.c file_search_handler.h my_grep.h
	@gcc -Wall -Wextra -c my_grep.c

file_search_handler.o: file_search_handler.c file_search_handler.h output_printer.h my_grep.h
	@gcc -Wall -Wextra -c file_search_handler.c

output_printer.o: output_printer.c output_printer.h my_grep.h
	@gcc -Wall -Wextra -c output_printer.c


test: 
	make all
	@/specific/a/home/cc/students/csguests/nimrodav/grep_tests/run_all.sh

clean:
	@-rm -f $(OBJS) $(EXEC)