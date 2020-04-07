#include "matching_pattern.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>





void tolower_string(char* string)
{
	int i = 0;
	while (string[i] != '\0') 
	{
		string[i] = tolower(string[i]);
		i++;
	}
}