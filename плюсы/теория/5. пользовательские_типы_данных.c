/*
 * Файл: custom_types_switch_complete.c
 * Описание: Пользовательские типы данных и оператор switch в языке C
 */

#include <stdio.h>
#include <stdlib.h>

/* ==================== */
/* ПЕРЕЧИСЛЕНИЯ (ENUM) */
/* ==================== */

// 1. Базовое перечисление
enum Color
{
	RED,
	GREEN,
	BLUE
};	  // RED=0, GREEN=1, BLUE=2

// 2. Перечисление с явными значениями
enum Status
{
	OK = 0,
	ERROR = 1,
	PENDING = 2
};

// 3. Перечисление с частичными значениями
enum Numbers
{
	ONE,			// 0
	TWO,			// 1
	TEN = 10,		// 10
	ELEVEN,			// 11
	TWENTY = 20,	// 20
	TWENTY_ONE		// 21
};

// 4. Использование typedef с enum
typedef enum Color color_t;

// 5. Сложное перечисление (пример из конспекта)
enum For
{
	A,			 // 0
	B,			 // 1
	C = 10,		 // 10
	D,			 // 11
	E = 1,		 // 1
	F,			 // 2
	G,			 // 3
	H = F + C	 // 2 + 10 = 12
};

/* ==================== */
/* СТРУКТУРЫ (STRUCT) */
/* ==================== */

// 1. Базовая структура
struct Point1
{
	double x;
	double y;
	double z;
};

// 2. Использование typedef со структурами
typedef struct Point1 Point;

// 3. Структура с разными типами полей
struct DateTime
{
	int sec, min, hour, day, month, year;
};

// 4. Взаимно ссылающиеся структуры (forward declaration)
struct Y;	 // Предварительное объявление

struct X
{
	struct Y *p;
	int data;
};

struct Y
{
	struct X *q;
	int value;
};

// 5. Структура с перечислением
typedef struct
{
	double x;
	double y;
	double z;
	color_t color;	  // поле типа перечисления
} ColoredPoint;

/* ==================== */
/* ОБЪЕДИНЕНИЯ (UNION) */
/* ==================== */
// все члены разделяют одну и ту же область памяти
// Union для представления float как int и наоборот
union FloatInt
{
	float f;
	int i;
};

// Union для разных представлений данных
union Data
{
	int i;
	float f;
	char str[20];
};

/* ==================== */
/* БИТОВЫЕ ПОЛЯ */
/* ==================== */

struct BitFields
{
	unsigned int flag1 : 1;	   // 1 бит
	unsigned int flag2 : 2;	   // 2 бита
	unsigned int flag3 : 4;	   // 4 бита
	unsigned int	   : 3;	   // безымянное поле (заполнитель)
	unsigned int flag4 : 6;	   // 6 бит
};

/* ==================== */
/* ФУНКЦИИ ДЛЯ ДЕМОНСТРАЦИИ */
/* ==================== */

void demonstrate_enum(void)
{
	printf("=== ПЕРЕЧИСЛЕНИЯ (ENUM) ===\n\n");

	// Использование перечислений
	enum Color c = RED;
	color_t ct = GREEN;

	printf("Цвета: RED=%d, GREEN=%d, BLUE=%d\n", RED, GREEN, BLUE);
	printf("Текущий цвет: %d\n", c);

	// Перечисление с явными значениями
	printf("\nСложное перечисление:\n");
	printf("A=%d, B=%d, C=%d, D=%d, E=%d, F=%d, G=%d, H=%d\n", A, B, C, D, E, F, G, H);
}

void demonstrate_switch(void)
{
	printf("\n=== ОПЕРАТОР SWITCH ===\n\n");

	int number = 2;

	switch (number)
	{
	case 1:
		printf("Число 1\n");
		number = 0;
		break;	  // Важно: break прерывает выполнение
	case 2:
		printf("Число 2\n");
		number--;	 // Нет break - выполнение перейдет к case 3!
					 // fallthrough - специальный комментарий для компилятора
	case 3:
		printf("Число 3 или fallthrough из case 2\n");
		number++;
		break;
	default:
		printf("Другое число\n");
		break;
	}
	printf("Результат: number = %d\n", number);

	// Switch с do-while (необычный пример из конспекта)
	printf("\nНеобычный switch с do-while:\n");
	number = 11;
	int count = (number + 3) / 4;
	int i = 0;

	switch (number % 4)
	{
	case 0:
		do
		{
			++i;
		case 3:
			++i;
		case 2:
			++i;
		case 1:
			++i;
		} while (--count > 0);
	}
	printf("Результат итераций: i = %d\n", i);
}

void demonstrate_switch_vs_if(void)
{
	printf("\n=== SWITCH vs IF-ELSE ===\n\n");

	int value = 2;

	// Эквивалентность switch и if-else
	printf("С помощью switch:\n");
	switch (value)
	{
	case 1:
		printf("Один\n");
		break;
	case 2:
		printf("Два\n");
		break;
	case 3:
		printf("Три\n");
		break;
	default:
		printf("Другое\n");
		break;
	}

	printf("С помощью if-else:\n");
	if (value == 1)
	{
		printf("Один\n");
	}
	else if (value == 2)
	{
		printf("Два\n");
	}
	else if (value == 3)
	{
		printf("Три\n");
	}
	else
	{
		printf("Другое\n");
	}
}

void demonstrate_union(void)
{
	printf("\n=== ОБЪЕДИНЕНИЯ (UNION) ===\n\n");

	union FloatInt u;
	u.f = 10.5f;

	printf("Float представление: %.2f\n", u.f);
	printf("Int представление: %d (0x%08x)\n", u.i, u.i);

	// Изменяем через одно поле, читаем через другое
	u.i = 0x40490fdb;	 // Пример битового представления float
	printf("После установки int: float = %.6f\n", u.f);
}

void demonstrate_bit_fields(void)
{
	printf("\n=== БИТОВЫЕ ПОЛЯ ===\n\n");

	struct BitFields bf = { 0 };

	bf.flag1 = 1;	  // 1 бит (0 или 1)
	bf.flag2 = 2;	  // 2 бита (0-3)
	bf.flag3 = 7;	  // 4 бита (0-15)
	bf.flag4 = 25;	  // 6 бит (0-63)

	printf("Битовые поля: flag1=%u, flag2=%u, flag3=%u, flag4=%u\n", bf.flag1, bf.flag2, bf.flag3, bf.flag4);
	printf("Размер структуры с битовыми полями: %zu байт\n", sizeof(bf));
}

/*
 * Файл: array_methods_comparison.c
 * Сравнение различных способов создания массивов в C
 * Плюсы и минусы каждого подхода
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 3
#define COLS 4

/* ==================== */
/* СПОСОБ 1: Один большой 1D массив как 2D */
/* ==================== */

/*
 * ПРЕИМУЩЕСТВА:
 * + Единое выделение/освобождение памяти
 * + Гарантированно последовательное расположение данных в памяти
 * + Максимальная производительность доступа к данным
 * + Эффективное использование кэша процессора (пространственная локальность)
 * + Быстрое копирование всего массива (один вызов memcpy)
 *
 * НЕДОСТАТКИ:
 * - Сложная индексация элементов: array[y * width + x]
 * - Менее интуитивный доступ по сравнению с matrix[i][j]
 * - Сложность отладки и визуализации данных
 * - Риск невозможности выделить непрерывный блок памяти для больших массивов
 * - Жестко фиксированная прямоугольная структура (нельзя создать рваный массив)
 */
void single_1d_array_as_2d(void)
{
	printf("=== СПОСОБ 1: Один 1D массив как 2D ===\n");

	int *array = malloc(ROWS * COLS * sizeof(int));
	if (!array)
	{
		printf("Ошибка выделения памяти!\n");
		return;
	}

	// Инициализация
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			array[i * COLS + j] = i * COLS + j + 1;
		}
	}

	printf("Массив %dx%d (один блок памяти):\n", ROWS, COLS);
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			printf("%2d ", array[i * COLS + j]);
		}
		printf("\n");
	}

	// Преимущество: быстрое копирование
	int *array_copy = malloc(ROWS * COLS * sizeof(int));
	memcpy(array_copy, array, ROWS * COLS * sizeof(int));

	free(array);
	free(array_copy);
}

/* ==================== */
/* СПОСОБ 2: Массив указателей на массивы */
/* ==================== */

/*
 * ПЛЮСЫ:
 * + Гибкость - можно создавать "рваные" массивы
 * + Более интуитивный доступ (matrix[i][j])
 * + Можно освобождать строки по отдельности
 * + Легче работать с подмассивами разного размера
 * + Нет проблемы непрерывного блока памяти
 *
 * МИНУСЫ:
 * - Множественные выделения памяти
 * - Медленнее из-за двойного разыменования
 * - Сложнее управление памятью
 * - Данные могут быть разбросаны в памяти
 * - Меньшая эффективность кэша
 */
void array_of_pointers(void)
{
	printf("\n=== СПОСОБ 2: Массив указателей на массивы ===\n");

	// Выделяем память для массива указателей
	int **matrix = malloc(ROWS * sizeof(int *));
	if (!matrix)
	{
		printf("Ошибка выделения памяти для указателей!\n");
		return;
	}

	// Выделяем память для каждой строки
	for (int i = 0; i < ROWS; i++)
	{
		matrix[i] = malloc(COLS * sizeof(int));
		if (!matrix[i])
		{
			printf("Ошибка выделения памяти для строки %d!\n", i);
			// Освобождаем уже выделенную память
			for (int j = 0; j < i; j++)
			{
				free(matrix[j]);
			}
			free(matrix);
			return;
		}

		// Инициализация
		for (int j = 0; j < COLS; j++)
		{
			matrix[i][j] = (i + 1) * 10 + (j + 1);
		}
	}

	printf("Массив %dx%d (массив указателей):\n", ROWS, COLS);
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			printf("%2d ", matrix[i][j]);
		}
		printf("\n");
	}

	// Демонстрация расположения в памяти
	printf("\nАдреса строк в памяти:\n");
	for (int i = 0; i < ROWS; i++)
	{
		printf("Строка %d: %p\n", i, (void *)matrix[i]);
	}

	// Правильное освобождение памяти
	for (int i = 0; i < ROWS; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

/* ==================== */
/* ВЫБОР МЕТОДА ПО СИТУАЦИИ */
/* ==================== */

/*
=== РУКОВОДСТВО ПО ВЫБОРУ МЕТОДА РАБОТЫ С 2D МАССИВАМИ ===

Используйте ОДИН 1D МАССИВ когда:
• Размеры массива фиксированы и известны на этапе компиляции
• Требуется максимальная производительность обработки данных
• Работаете с большими массивами, где критически важно эффективное использование кэша процессора
• Планируете часто выполнять операции копирования всего массива
• Гарантировано наличие непрерывного блока памяти достаточного размера
• Приоритет - скорость доступа и минимальные накладные расходы

Используйте МАССИВ УКАЗАТЕЛЕЙ когда:
• Требуются рваные массивы (строки разной длины)
• Размеры массива определяются динамически во время выполнения программы
• Работаете с очень большими массивами, где проблематично выделить непрерывную память
• Нужна гибкость в управлении памятью (возможность освобождать строки по отдельности)
• Важен интуитивный доступ к элементам через синтаксис matrix[i][j]
• Приоритет - удобство разработки и гибкость архитектуры
*/
