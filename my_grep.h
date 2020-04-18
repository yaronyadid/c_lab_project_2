#ifndef MY_GREP_H
#define MY_GREP_H

typedef struct a_arg {
	int arg;
	int number;
}A_arg;

typedef struct grep_args {
	int b_arg;
	int c_arg;
	int i_arg;
	int n_arg;
	int v_arg;
	int x_arg;
	int E_arg;
	A_arg *A_arg;
	char *expression_to_match;
	char *file_name;
}Grep_args;

#endif