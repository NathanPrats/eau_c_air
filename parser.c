#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

char *Read_file(char *filename)  // Reads the content of a file
{
    FILE *f = fopen(filename, "rt");
    assert(f);
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = (char *) malloc(length + 1);
    buffer[length] = '\0';
    fread(buffer, 1, length, f);
    fclose(f);
    return buffer;
}


double Read_string(char* s, int in, int out) // Returns the double in position [in][out] of the string
{
	size_t i = 0;
	size_t save;
	size_t scale = 0;
	while (out != 1)
	{
		if (s[i] == ")"[0])
		{
			out--;
		}	
		i++;
	}
	i++;

	while (in > 0)
	{
		if (s[i] == "|"[0])
		{
			in--;
		}
		i++;
	}
	save = i;
	while (s[save] != "|"[0])
	{
		scale += 1;
		save++;
	}
	char c[scale + 1];
	save = 0;
	while (s[i] != "|"[0])
	{
		c[save] = s[i];
		i++;
		save++;
	}
	return atof(c);
}
