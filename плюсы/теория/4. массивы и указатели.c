/*
 * Файл: arrays_pointers_complete.c
 * Описание: Полное руководство по массивам и указателям в языке C
 */

#include <stdio.h>
#include <stdlib.h>

// Прототипы функций это объявления функций без их реализации. Они сообщают компилятору о существовании функции, ее
// имени, возвращаемом типе и параметрах
void process_array(int *arr, int size);
void print_array(int arr[], int size);
void demonstrate_pointers(void);
void demonstrate_dynamic_memory(void);

int main(void)
{
	/* ==================== */
	/* МАССИВЫ */
	/* ==================== */

	// 1. Объявление и инициализация массивов
	int arr1[10];	 // Объявление массива из 10 элементов (неинициализированного)

	// Доступ к элементам массива
	arr1[0] = 5;	 // Первый элемент (индекс 0)
	arr1[9] = 15;	 // Последний элемент (индекс 9)
	// arr1[10] = 20; // ОПАСНО: выход за границы массива!

	// 2. Инициализация при объявлении
	int arr2[5] = { 1, 2, 3, 4, 5 };	// Полная инициализация
	int arr3[5] = { 1, 2 };				// Частичная инициализация (остальные = 0)
	int arr4[] = { 1, 2, 3, 4, 5 };		// Размер определяется автоматически

	// 3. Определение размера массива
	int size = sizeof(arr2) / sizeof(arr2[0]);
	printf("Размер arr2: %d элементов\n", size);

	// 4. Многомерные массивы
	int matrix[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };

	printf("\nМногомерный массив:\n");
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("matrix[%d][%d] = %d\n", i, j, matrix[i][j]);
		}
	}

	// 5. Специфичные инициализаторы (C99)
	int arr5[10] = { [2] = 3, [5] = 4 };	// arr5[2] = 3, arr5[5] = 4, остальные 0
	printf("\nМассив с designators: ");
	print_array(arr5, 10);

	/* ==================== */
	/* УКАЗАТЕЛИ - ОСНОВЫ */
	/* ==================== */

	int a = 10, b = 20;

	// Объявление указателей
	int *p1;		   // Указатель на int
	int *p2 = NULL;	   // Инициализация нулевым указателем

	// Получение адреса переменной
	p1 = &a;	// p1 теперь содержит адрес переменной a
	printf("Переменная a = %d\n", a);
	printf("Адрес переменной a: %p\n", (void *)p1);
	printf("Значение через указатель: %d\n", *p1);

	// Разыменование указателя
	*p1 = 30;	 // Изменяем значение a через указатель
	printf("После *p1 = 30: a = %d\n", a);

	// Арифметика указателей
	p1 = &a;
	printf("p1 = %p\n", (void *)p1);
	printf("p1 + 1 = %p\n", (void *)(p1 + 1));	  // Сдвиг на размер int

	/* ==================== */
	/* УКАЗАТЕЛИ И МАССИВЫ */
	/* ==================== */

	int arr[] = { 10, 20, 30, 40, 50 };
	int *ptr = arr;	   // ptr указывает на первый элемент массива

	// Эквивалентные способы доступа
	printf("arr[0] = %d\n", arr[0]);
	printf("*ptr = %d\n", *ptr);
	printf("ptr[0] = %d\n", ptr[0]);

	// Доступ через арифметику указателей
	printf("*(ptr + 2) = %d\n", *(ptr + 2));	// Эквивалентно arr[2]
	printf("*(arr + 2) = %d\n", *(arr + 2));	// Эквивалентно arr[2]

	// Разность указателей
	int *ptr1 = &arr[1];
	int *ptr2 = &arr[4];
	printf("Разность указателей: %td\n", ptr2 - ptr1);	  // Результат: 3

	/* ==================== */
	/* КОНСТАНТНЫЕ УКАЗАТЕЛИ */
	/* ==================== */

	int x = 5, y = 10;
	const int c = 100;	  // Константная переменная

	// 1. Указатель на константу
	const int *p_const = &c;
	// *p_const = 50;  // ОШИБКА: нельзя изменить значение через указатель
	p_const = &x;	 // МОЖНО: изменить сам указатель

	// 2. Константный указатель
	int *const const_p = &x;
	*const_p = 15;	  // МОЖНО: изменить значение
	// const_p = &y;   // ОШИБКА: нельзя изменить указатель

	// 3. Константный указатель на константу
	const int *const const_p_const = &c;
	// *const_p_const = 50;  // ОШИБКА
	// const_p_const = &x;   // ОШИБКА

	/* ==================== */
	/* УКАЗАТЕЛИ НА УКАЗАТЕЛИ */
	/* ==================== */

	int value = 42;
	int *ptr_single = &value;
	int **ptr_double = &ptr_single;	   // Указатель на указатель

	printf("value = %d\n", value);
	printf("*ptr_single = %d\n", *ptr_single);
	printf("**ptr_double = %d\n", **ptr_double);

	// Изменение через двойной указатель
	**ptr_double = 100;
	printf("После **ptr_double = 100: value = %d\n", value);

	/* ==================== */
	/* УНИВЕРСАЛЬНЫЕ УКАЗАТЕЛИ (void*) */
	/* ==================== */

	int int_val = 255;
	float float_val = 3.14f;
	void *void_ptr;	   // Универсальный указатель

	void_ptr = &int_val;	  // МОЖНО: указатель на int
	void_ptr = &float_val;	  // МОЖНО: указатель на float

	// Для использования нужно явное приведение типа
	int *int_ptr = (int *)void_ptr;
	printf("Значение через void*: %d\n", *int_ptr);
	return 0;
}

/* ==================== */
/* ДИНАМИЧЕСКАЯ ПАМЯТЬ */
/* ==================== */

void demonstrate_dynamic_memory(void)
{
	// Выделение памяти для одного int
	int *single_ptr = (int *)malloc(sizeof(int));
	if (single_ptr == NULL)
	{
		printf("Ошибка выделения памяти!\n");
		return;
	}
	*single_ptr = 42;
	printf("Динамическая переменная: %d\n", *single_ptr);
	free(single_ptr);	  // ОБЯЗАТЕЛЬНО освободить память!
	single_ptr = NULL;	  // Хорошая практика

	// Выделение памяти для массива
	int array_size = 5;
	int *array_ptr = (int *)malloc(sizeof(int) * array_size);
	if (array_ptr == NULL)
	{
		printf("Ошибка выделения памяти для массива!\n");
		return;
	}

	// Инициализация динамического массива
	for (int i = 0; i < array_size; i++)
	{
		array_ptr[i] = i * 10;
	}

	printf("Динамический массив: ");
	print_array(array_ptr, array_size);

	// Освобождение памяти
	free(array_ptr);
	array_ptr = NULL;

	// Использование calloc (инициализирует нулями)
	int *zero_array = (int *)calloc(3, sizeof(int));
	printf("Массив через calloc: ");
	print_array(zero_array, 3);
	free(zero_array);
}
