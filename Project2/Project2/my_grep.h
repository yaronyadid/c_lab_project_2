#ifndef MY_GREP_H
#define MY_GREP_H


typedef struct grep_args {
	int num_arg;
	int b_arg;
	int c_arg;
	int i_arg;
	int n_arg;
	int v_arg;
	int x_arg;
	int E_arg;
	int a_arg;
	char* keyword;
	FILE * searched_file;
	
}grep_args;

#endif