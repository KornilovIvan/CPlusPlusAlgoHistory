#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define get_index(n, i, j) ((n) * (i) + (j))
#define EPSILON 1e-12

static bool read_matrix_data(FILE *input_file, double *matrix, size_t n, size_t m)
{
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			if (!fscanf(input_file, "%lg", &matrix[get_index(m * 2, i, j)]))
			{
				fprintf(stderr, "Error: Invalid matrix data at row %zu, col %zu\n", i + 1, j + 1);
				fclose(input_file);
				free(matrix);
				return false;
			}
		}
		for (size_t j = m; j < 2 * m; j++)
		{
			matrix[get_index(m * 2, i, j)] = (i == j - m) ? 1.0 : 0.0;
		}
	}
	return true;
}

static double *read_matrix_from_file(const char **input_filename, size_t *n, size_t *m)
{
	FILE *input_file = fopen(*input_filename, "r");
	if (input_file == NULL)
	{
		fprintf(stderr, "Error: Cannot open input file '%s'\n", *input_filename);
		return NULL;
	}

	if (fscanf(input_file, "%zu %zu", &*n, &*m) != 2)
	{
		fprintf(stderr, "Error: incorrect number of arguments in %s", *input_filename);
		fclose(input_file);
		return NULL;
	}

	if (*n < 1 || *m < 1)
	{
		fprintf(stderr, "Error: incorrect matrix dimensions");
		fclose(input_file);
		return NULL;
	}

	double *matrix = malloc(sizeof(double) * *n * *m * 2);
	if (matrix == NULL)
	{
		fprintf(stderr, "Error: failed to allocate memory for the matrix\n");
		fclose(input_file);
		return NULL;
	}
	if (!read_matrix_data(input_file, matrix, *n, *m))
	{
		fclose(input_file);
		free(matrix);
		return NULL;
	}
	fclose(input_file);
	return matrix;
}

static void swap_row(double *matrix, int row1, int row2, int n)
{
	for (size_t i = 0; i < n * 2; i++)
	{
		double temp = matrix[get_index(n * 2, row1, i)];
		matrix[get_index(n * 2, row1, i)] = matrix[get_index(n * 2, row2, i)];
		matrix[get_index(n * 2, row2, i)] = temp;
	}
}

static bool gauss(double *matrix, int n)
{
	for (size_t i = 0; i < n; i++)
	{
		double maxi = fabs(matrix[get_index(n * 2, i, i)]);
		int row = i;
		for (size_t j = i; j < n; j++)
		{
			if (maxi < fabs(matrix[get_index(n * 2, j, i)]))
			{
				maxi = fabs(matrix[get_index(n * 2, j, i)]);
				row = j;
			}
		}
		if (fabs(maxi) < EPSILON)
		{
			return false;
		}
		if (row != i)
		{
			swap_row(matrix, row, i, n);
		}

		double d = matrix[get_index(n * 2, i, i)];
		if (fabs(d) < EPSILON)
		{
			return false;
		}
		for (size_t j = i; j < 2 * n; j++)
		{
			matrix[get_index(n * 2, i, j)] /= d;
		}

		for (size_t j = 0; j < n; j++)
		{
			if (j == i)
				continue;
			double c = -1 * (matrix[get_index(n * 2, j, i)] / matrix[get_index(n * 2, i, i)]);
			for (size_t k = i; k < 2 * n; k++)
			{
				matrix[get_index(n * 2, j, k)] += c * matrix[get_index(n * 2, i, k)];
			}
		}
	}
	return true;
}

static void write_solution_matrix(FILE *output_file, const double *matrix, size_t n)
{
	fprintf(output_file, "%zu %zu\n", n, n);
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = n; j < 2 * n; j++)
		{
			fprintf(output_file, "%g ", matrix[get_index(n * 2, i, j)]);
		}
		fprintf(output_file, "\n");
	}
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <input_file> <output_file>", argv[0]);
		return 1;
	}
	const char *input_filename = argv[1];
	const char *output_filename = argv[2];

	size_t n, m;
	double *matrix = read_matrix_from_file(&input_filename, &n, &m);
	if (matrix == NULL)
	{
		return 1;
	}

	bool has_solution = true;
	if (n != m)
	{
		has_solution = false;
	}
	else
	{
		has_solution = gauss(matrix, n);
	}

	FILE *output_file = fopen(output_filename, "w");
	if (output_file == NULL)
	{
		fprintf(stderr, "Error: Cannot open output file '%s'\n", output_filename);
		free(matrix);
		return 1;
	}

	if (!has_solution)
	{
		fprintf(output_file, "%s\n", "no_solution");
	}
	else
	{
		write_solution_matrix(output_file, matrix, n);
	}
	fclose(output_file);
	free(matrix);
	return 0;
}
