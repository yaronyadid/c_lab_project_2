#include <stdio.h>
#include <stdlib.h>
#include "my_grep.h"



void get_grep_args(grep_args *grep_arguments, char** program_argv, int program_argc)
{
	int i = 1;
	for (i; i < program_argc; i++) 
	{
		if (!strcmp(program_argv[i], "-i")) 
		{
			grep_arguments->i_arg = 1;
		}
		else if (!strcmp(program_argv[i], "-n")) 
		{
			grep_arguments->n_arg = 1;
		}
		else if (!strcmp(program_argv[i], "-A")) 
		{
			grep_arguments->num_arg = atoi(program_argv[i + 1]);
			grep_arguments->a_arg = 1;
			i++;
		}
		else if (!strcmp(program_argv[i], "-b")) 
		{
			grep_arguments->b_arg = 1;
		}
		else if (!strcmp(program_argv[i], "-c")) 
		{
			grep_arguments->c_arg = 1;
		}
		else if (!strcmp(program_argv[i], "-x")) 
		{
			grep_arguments->x_arg = 1;
		}
		else if (!strcmp(program_argv[i], "-v")) 
		{
			grep_arguments->v_arg = 1;
		}
		else if (!strcmp(program_argv[i], "-E")) 
		{
			grep_arguments->E_arg = 1;
			i++;
			grep_arguments->keyword = program_argv[i];
		}
		else {
			grep_arguments->searched_file = program_argv[i];
		}
	}
}


FILE* get_file_stream(char* file_path)
{
	FILE* file_stream;
	if (file_path == NULL)
	{
		file_stream = stdin;
	}
	else 
	{
		file_stream = fopen(file_path, "r");
		if (file_stream == NULL)
		{
			exit(1);
		}
	}
	return file_stream;
}






int main(int argc, char** argv)
{

	if (argc < 2)
	{
		exit(1);
	}

	FILE* file_to_be_searched;
	grep_args* args = calloc(1, sizeof(grep_args));
	if (args == NULL)
	{
		exit(1);
	}
	get_grep_args(args, argv, argc);
	file_to_be_searched = get_file_stream(args->searched_file);





}