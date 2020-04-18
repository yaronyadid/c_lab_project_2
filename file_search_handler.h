#ifndef FILE_SEARCH_HANDLER_H
#define FILE_SEARCH_HANDLER_H

#include <stdio.h> 
#include "my_grep.h"

typedef struct file_search_info {
	int matched_line_number;
	int cur_line_has_match;
	int line_number;
	int byte_offset;
	int number_of_lines_to_print_after_match;
} File_search_info;

File_search_info *initialize_file_search_info();
FILE *initialize_input_source(char *file_name);
void search_file(FILE *file, File_search_info *c, Grep_args *grep_args);

#endif