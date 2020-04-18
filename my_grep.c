#include <stdio.h>
#include <stdlib.h>
#include "my_grep.h"
#include "file_search_handler.h"

void parse_command_args(Grep_args *grep_args, char **command_args, int command_args_num) {
	int i = 1;

	for (i; i < command_args_num; i++) {
		if (!strcmp(command_args[i], "-b")) {
		grep_args->b_arg = 1;
		}
		else if (!strcmp(command_args[i], "-c")) {
		grep_args->c_arg = 1;
		}
		else if (!strcmp(command_args[i], "-i")) {
			grep_args->i_arg = 1;
		}
		else if (!strcmp(command_args[i], "-n")) {
			grep_args->n_arg = 1;
		}
		else if (!strcmp(command_args[i], "-v")) {
			grep_args->v_arg = 1;
		}
		else if (!strcmp(command_args[i], "-x")) {
			grep_args->x_arg = 1;
		}
		else if (!strcmp(command_args[i], "-A")) {
			grep_args->A_arg->arg = 1;
			i++;
			grep_args->A_arg->number = atoi(command_args[i]);
		}
		else if (!strcmp(command_args[i], "-E")) {
			grep_args->E_arg = 1;
			i++;
			grep_args->expression_to_match = command_args[i];
		}
		else if (grep_args->expression_to_match == NULL) {
			grep_args->expression_to_match = command_args[i];
		}
		else {
			grep_args->file_name = command_args[i];
		}
	}
}

Grep_args *initialize_grep_args() {
	A_arg *A_arg = malloc(sizeof(A_arg));
	if (A_arg == NULL) {
		exit(1);
	}

	Grep_args *grep_args = calloc(1, sizeof(grep_args));
	if (grep_args == NULL) {
		exit(1);
	}

	grep_args->A_arg = A_arg;
	grep_args->file_name = NULL;
	grep_args->expression_to_match = NULL;
	return grep_args;
}

void free_memory(Grep_args* grep_args, File_search_info *file_search_info, FILE *file) {
	free(file_search_info);
	free(grep_args->A_arg);
	free(grep_args);
	if (file != stdin) {
		fclose(file);
	}
}

int main(int argc, char **argv) {
	FILE *input_source;

	if (argc < 2) {
		exit(1);
	}

	Grep_args *grep_args = initialize_grep_args();
	parse_command_args(grep_args, argv, argc);
	File_search_info *file_search_info = initialize_file_search_info();
	input_source = initialize_input_source(grep_args->file_name);

	search_file(input_source, file_search_info, grep_args);
	if (grep_args->c_arg) {
		printf("%d\n", file_search_info->matched_line_number);
	}

	free_memory(grep_args, file_search_info, input_source);

	return 0;
}