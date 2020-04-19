#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>  
#include "file_search_handler.h"
#include "output_printer.h"


int is_match(char*, char*, Grep_args*);


File_search_info *initialize_file_search_info() {
	File_search_info *file_search_info = calloc(1, sizeof(File_search_info));
	if (file_search_info == NULL) {
		exit(1);
	}
	file_search_info->line_number = 1;
	file_search_info->number_of_lines_to_print_after_match = -1;
	return file_search_info;
}



void to_lower_string(char *string) {
	int i = 0;
	while (string[i] != '\0') {
		string[i] = tolower(string[i]);
		i++;
	}
}


FILE *initialize_input_source(char *file_name) {
	FILE *input_source;

	if (file_name != NULL) {
		input_source = fopen(file_name, "r");
		if (input_source == NULL) {
			exit(1);
		}
	}
	else {
		input_source = stdin;
	}

	return input_source;
}



int is_match_with_square_brackets(char *expression_to_search, char *line, Grep_args *grep_args) {
	char top_range;
	char bottom_range;

	sscanf(&expression_to_search[1], " %c - %c", &bottom_range, &top_range);
	expression_to_search = strstr(expression_to_search, "]");

	if (line[0] >= bottom_range && line[0] <= top_range) {
		return is_match(&line[1], &expression_to_search[1], grep_args);
	}

	return 0;
}


int is_match_with_round_brackets(char *expression_to_search, char *line, Grep_args *grep_args) {
	int match_found = 0;
	int i;
	char *expression1 = malloc(strlen(expression_to_search));
	if (expression1 == NULL) {
	exit(1);
	}

	char *expression2 = malloc(strlen(expression_to_search));
	if (expression2 == NULL) {
	exit(1);
	}
	for(i = 0; i < (int) strlen(expression_to_search); i++){
		expression1[i] = 0;
		expression2[i] = 0;
	}
	sscanf(&expression_to_search[1], "%[^|]|%[^)]", expression1, expression2);
	expression_to_search = strstr(expression_to_search, ")");

	// temporarily assign x_arg = 0 to check if the bracket expression is a prefix of the given line:
	int original_x_arg = grep_args->x_arg; 
	grep_args->x_arg = 0;

	if (is_match(&line[0], expression1, grep_args)) {
	match_found = is_match(&line[strlen(expression1)], &expression_to_search[1], grep_args);
	} 
	else if (is_match(&line[0], expression2, grep_args)) {
	match_found = is_match(&line[strlen(expression2)], &expression_to_search[1], grep_args);
	}

	grep_args->x_arg = original_x_arg; // assign the original x_arg value back
	free(expression1);
	free(expression2);

	return match_found;
}

int is_match_with_period(char *expression_to_search, char *line, Grep_args *grep_args) {
	return is_match(&line[1], &expression_to_search[1], grep_args);
}


int is_match_for_regular_expression(char *expression_to_search, char *line, Grep_args *grep_args) {
	if (expression_to_search[0] == '.') {
		return is_match_with_period(expression_to_search, line, grep_args);
	}

	else if (expression_to_search[0] == '(') {
		return is_match_with_round_brackets(expression_to_search, line, grep_args);
	}

	else if (expression_to_search[0] == '[') {
		return is_match_with_square_brackets(expression_to_search, line, grep_args);
	}

	return 0;
}


int is_match(char *line, char *expression_to_search, Grep_args *grep_args) {
	if (expression_to_search[0] == '\0') {
		if (grep_args->x_arg) {
			return line[0] == '\0';
		}
		return 1;
	}

	if (line[0] == '\0') {
		return 0;
	}

	// escape "\"
	if (expression_to_search[0] == '\\') {
		if (expression_to_search[1] == line[0]) { // first char is identical, check the rest of the string
			return is_match(&line[1], &expression_to_search[2], grep_args);
		}
		else {
			return 0;
		}
	}

	if (grep_args->E_arg) {
		if (expression_to_search[0] == '.' || expression_to_search[0] == '[' || expression_to_search[0] == '(') {
			return is_match_for_regular_expression(expression_to_search, line, grep_args);
		}
	}

	if (expression_to_search[0] == line[0]) { // first char is identical, check the rest of the string
		return is_match(&line[1], &expression_to_search[1], grep_args);
	}

	return 0;
}




int is_expression_in_line(char *line, Grep_args *grep_args)
{
	unsigned int i = 0;
	int match_found = 0;

	char *modified_line = malloc(strlen(line) + 1); 
	if (modified_line == NULL) {
		exit(1);
	}
	strcpy(modified_line, line);

	if (grep_args->i_arg) {
		to_lower_string(modified_line);
		to_lower_string(grep_args->expression_to_match);
	}

	if (grep_args->x_arg) {
	// look for an exact match just :
		match_found = is_match(&modified_line[0], grep_args->expression_to_match, grep_args); 
	}
	else {
		for (i = 0; i < strlen(modified_line); i++) {
			// look for a match in all possible positions:
			if (is_match(&modified_line[i], grep_args->expression_to_match, grep_args) == 1) { 
				match_found = 1;
				break;
			}
		}
	}

	if (grep_args->v_arg) {
		match_found = 1 - match_found;
	}

	free(modified_line);
	return match_found;
}


void search_line(File_search_info* file_search_info, char* line, Grep_args* grep_args) {
	if (is_expression_in_line(line, grep_args)) {
		file_search_info->cur_line_has_match = 1;
		file_search_info->matched_line_number++;
		print_line(line, grep_args, file_search_info);
	}
	else {
		file_search_info->cur_line_has_match = 0;
		if (file_search_info->number_of_lines_to_print_after_match > 0) {
			print_line(line, grep_args, file_search_info);
			file_search_info->number_of_lines_to_print_after_match--;
		}
	}
}

void search_file(FILE *file, File_search_info *file_search_info, Grep_args* grep_args) {
	char *line = NULL;
	int bytes_read = 0;
	size_t line_len = 0;

	while ((bytes_read = (getline(&line, &line_len, file))) != EOF) {
		sscanf(line, "%[^\n]", line);
		search_line(file_search_info, line, grep_args);
		file_search_info->byte_offset += bytes_read;
		file_search_info->line_number++;
	}

	free(line);
}

