#include "output_printer.h"

void print_line(char *line, Grep_args *grep_args, File_search_info *file_search_info) {
	char delimiter;
	if (file_search_info->cur_line_has_match) {
		delimiter = ":";
	}
	else {
		delimiter = "-";
	}

	if (grep_args->c_arg) {
		return;
	}

	if (file_search_info->cur_line_has_match) {
		if (grep_args->A_arg->arg) {
			if (file_search_info->number_of_lines_to_print_after_match == 0) {
				printf("--\n");
			}
			file_search_info->number_of_lines_to_print_after_match = grep_args->A_arg->number;
		}
	}

	if (grep_args->n_arg) {
	printf("%d%c", file_search_info->line_number, delimiter);
	}

	if (grep_args->b_arg) {
	printf("%d%c", file_search_info->byte_offset, delimiter);
	}

	printf("%s\n", line);
}