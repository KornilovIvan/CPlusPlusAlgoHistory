#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
		return 1;
	}
	const char *input_filename = argv[1];
	const char *output_filename = argv[2];
	FILE *input_file = fopen(input_filename, "r");
	if (input_file == NULL)
	{
		fprintf(stderr, "Error: Cannot open input file '%s'\n", input_filename);
		return 1;
	}
	int a, b;
	if (fscanf(input_file, "%d %d", &a, &b) != 2)
	{
		fprintf(stderr, "Error: Failed to read two integers from input file\n");
		fclose(input_file);
		return 1;
	}
	fclose(input_file);
	FILE *output_file = fopen(output_filename, "w");
	if (output_file == NULL)
	{
		fprintf(stderr, "Error: Cannot open output file '%s'\n", output_filename);
		return 1;
	}
	fprintf(output_file, "%d\n", a + b);
	fclose(output_file);
	return 0;
}
