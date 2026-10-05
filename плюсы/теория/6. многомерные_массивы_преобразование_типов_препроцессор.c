/*
 * Файл: multidimensional_arrays_preprocessor.c
 * Многомерные массивы, преобразование типов и препроцессор в C
 */

#include <stdio.h>
#include <stdlib.h>

/* ==================== */
/* МНОГОМЕРНЫЕ МАССИВЫ И УКАЗАТЕЛИ */
/* ==================== */

/*
 * Разница между указателем и массивом:
 *
 * int *p;       - указатель на int
 * int arr[5];   - массив из 5 int
 *
 * &p = int**    - адрес указателя (указатель на указатель)
 * &arr = int(*)[5] - указатель на массив из 5 int
 *
 * sizeof(p) = размер указателя (4/8 байт)
 * sizeof(arr) = 5 * sizeof(int) (20 байт обычно)
 */

/* ==================== */
/* УКАЗАТЕЛИ НА МНОГОМЕРНЫЕ МАССИВЫ */
/* ==================== */

/*
 * int **p - указатель на указатель на int
 * Может использоваться для динамических 2D массивов
 * Данные НЕ обязательно расположены последовательно!
 */

void multidimensional_pointers(void)
{
	printf("\n=== МНОГОМЕРНЫЕ УКАЗАТЕЛИ ===\n");

	int rows = 3, cols = 4;

	// Выделение памяти для массива указателей
	int **matrix = malloc(rows * sizeof(int *));
	for (int i = 0; i < rows; i++)
	{
		matrix[i] = malloc(cols * sizeof(int));
		for (int j = 0; j < cols; j++)
		{
			matrix[i][j] = i * cols + j + 1;
		}
	}

	printf("Динамический массив %dx%d:\n", rows, cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			printf("%2d ", matrix[i][j]);
		}
		printf("\n");
	}

	// Преимущества:
	printf("\nПреимущества int** подхода:\n");
	printf("- Можно переставлять строки (менять указатели)\n");
	printf("- Гибкое управление памятью\n");
	printf("- Подходит для рваных массивов\n");

	// Освобождение памяти
	for (int i = 0; i < rows; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

/* ==================== */
/* УКАЗАТЕЛИ НА МАССИВЫ ФИКСИРОВАННОЙ ДЛИНЫ */
/* ==================== */

/*
 * int (*arr_ptr)[COLS] - указатель на массив из COLS элементов
 * Полезно для массивов с известными размерами на этапе компиляции
 */

void fixed_array_pointers(void)
{
	printf("\n=== УКАЗАТЕЛИ НА МАССИВЫ ФИКСИРОВАННОЙ ДЛИНЫ ===\n");

	const int COLS = 4;
	int matrix[3][COLS] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 } };

	// Указатель на массив из COLS int
	int (*ptr)[COLS] = matrix;

	printf("Доступ через указатель на массив:\n");
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			printf("%2d ", ptr[i][j]);
		}
		printf("\n");
	}
}

/* ==================== */
/* VLA - VARIABLE LENGTH ARRAYS */
/* ==================== */

/*
 * Массивы переменной длины (C99)
 * Размер определяется во время выполнения
 */

void vla_example(size_t rows, size_t cols)
{
	printf("\n=== МАССИВЫ ПЕРЕМЕННОЙ ДЛИНЫ (VLA) ===\n");

	// VLA - размер определяется параметрами
	int vla_matrix[rows][cols];

	// Инициализация
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			vla_matrix[i][j] = (i + 1) * 10 + (j + 1);
		}
	}

	printf("VLA массив %zux%zu:\n", rows, cols);
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			printf("%3d ", vla_matrix[i][j]);
		}
		printf("\n");
	}
}

// Функция, принимающая VLA
void process_vla(size_t rows, size_t cols, int matrix[rows][cols])
{
	// Эквивалентно: int (*matrix)[cols]
	printf("\nОбработка VLA в функции:\n");
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

/* ==================== */
/* УКАЗАТЕЛИ НА ФУНКЦИИ */
/* ==================== */

// Прототипы функций для демонстрации
int add(int a, int b)
{
	return a + b;
}
int sub(int a, int b)
{
	return a - b;
}
int mul(int a, int b)
{
	return a * b;
}
int divide(int a, int b)
{
	return b != 0 ? a / b : 0;
}

void function_pointers(void)
{
	printf("\n=== УКАЗАТЕЛИ НА ФУНКЦИИ ===\n");

	// Объявление указателя на функцию
	int (*func_ptr)(int, int);

	// Массив указателей на функции
	int (*operations[4])(int, int) = { add, sub, mul, divide };
	const char *op_names[] = { "Сложение", "Вычитание", "Умножение", "Деление" };

	int a = 10, b = 5;

	printf("Операции с числами %d и %d:\n", a, b);
	for (int i = 0; i < 4; i++)
	{
		int result = operations[i](a, b);
		printf("%s: %d %c %d = %d\n", op_names[i], a, "+-*/"[i], b, result);
	}

	// Константный массив указателей на функции
	int (*const const_ops[4])(int, int) = { add, sub, mul, divide };
	printf("\nКонстантный массив функций:\n");
	printf("3 + 2 = %d\n", const_ops[0](3, 2));
}

/* ==================== */
/* ПРЕОБРАЗОВАНИЕ ТИПОВ */
/* ==================== */

void type_conversion(void)
{
	// Неявное преобразование
	double d = 3.7;
	int x = d;	  // Неявное преобразование double -> int (отбрасывание дробной части)
	printf("Неявное преобразование: double %.1f -> int %d\n", d, x);

	// Явное преобразование (кастинг)
	x = (int)3.7;	 // Явное преобразование
	printf("Явное преобразование: (int)3.7 = %d\n", x);

	// Преобразование указателей (осторожно!)
	float f = 3.14f;
	float *fp = &f;
	// int *ip = (int*)fp;  // Опасное преобразование!
	// printf("Float как int: %f -> %d\n", f, *ip);

	// Константные преобразования
	const float cf = 2.71f;
	// float *non_const = &cf;  // Ошибка! Нужен const_cast в C++
	const float *const_ptr = &cf;	 // Правильно

	printf("Константная переменная: %.2f\n", cf);
}

/* ==================== */
/* ПРЕПРОЦЕССОР */
/* ==================== */
// Препроцессор - это программа, которая обрабатывает исходный код до компиляции. Он выполняет текстовые преобразования
// с директивами, начинающимися с #

// Макросы без параметров
#define PI 3.14159
#define MAX_SIZE 100
#define DEBUG_MODE 1

// Макросы с параметрами
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

// Многострочный макрос
#define PRINT_SUM(a, b)                                                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		printf("%d + %d = %d\n", (a), (b), (a) + (b));                                                                 \
	} while (0)

void preprocessor_demo(void)
{
	printf("\n=== ПРЕПРОЦЕССОР ===\n");

	// Использование макросов
	printf("PI = %.5f\n", PI);
	printf("SQUARE(5) = %d\n", SQUARE(5));
	printf("MAX(10, 20) = %d\n", MAX(10, 20));

	int arr[] = { 1, 2, 3, 4, 5 };
	printf("Размер массива: %zu элементов\n", ARRAY_SIZE(arr));

	PRINT_SUM(7, 3);

// Условная компиляция
#if DEBUG_MODE
	printf("Режим отладки включен\n");
#else
	printf("Режим отладки выключен\n");
#endif

#ifdef PI
	printf("Макрос PI определен\n");
#endif

#ifndef UNDEFINED_MACRO
	printf("Макрос UNDEFINED_MACRO не определен\n");
#endif
}
