/*
 * Файл: strings_encoding_complete.c
 * Строки и кодировки в языке C
 */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/* ==================== */
/* КВАЛИФИКАТОР RESTRICT */
/* ==================== */

/*
 * restrict - ключевое слово, которое сообщает компилятору,
 * что через данный указатель осуществляется исключительный доступ(Указатель с restrict гарантирует, что через него
 * происходит единственный доступ к данным в определенной области кода) к данным Это позволяет компилятору проводить
 * более агрессивные оптимизации
 */

void add_arrays(const int *restrict a, const int *restrict b, int *restrict c, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		c[i] = a[i] + b[i];
	}
}

void demonstrate_restrict(void)
{
	printf("=== КВАЛИФИКАТОР RESTRICT ===\n\n");

	int a[5] = { 1, 2, 3, 4, 5 };
	int b[5] = { 5, 4, 3, 2, 1 };
	int c[5];

	add_arrays(a, b, c, 5);

	printf("Результат сложения массивов: ");
	for (int i = 0; i < 5; i++)
	{
		printf("%d ", c[i]);
	}
	printf("\n");

	printf("restrict гарантирует, что массивы не пересекаются в памяти\n");
	printf("Это позволяет компилятору лучше оптимизировать код\n");
}

/* ==================== */
/* ОСНОВЫ СТРОК В C */
/* ==================== */

/*
 * Строки в C - это массивы символов char, заканчивающиеся нулевым байтом ('\0')
 * Тип char обычно занимает 1 байт и может быть signed или unsigned
 */

void string_basics(void)
{
	// Различные способы инициализации строк
	char str1[] = "Hello";				 // Автоматическое определение размера
	char str2[10] = "World";			 // Фиксированный размер
	char str3[] = { 'H', 'i', '\0' };	 // Явная инициализация
	const char *str4 = "Constant";		 // Указатель на строковый литерал

	printf("str1: %s\n", str1);

	// Символы и их коды
	char c = 'A';
	printf("Символ '%c' имеет код: %d (0x%02x)\n", c, c, c);
	printf("Нулевой байт: '\\0' = %d\n", '\0');
}

/* ==================== */
/* ФУНКЦИИ ДЛЯ РАБОТЫ СО СТРОКАМИ */
/* ==================== */

// Аналог strlen - вычисление длины строки
size_t my_strlen(const char *str)
{
	size_t len = 0;
	while (str[len] != '\0')
	{
		len++;
	}
	return len;
}

// Аналог strchr - поиск символа в строке
const char *my_strchr(const char *str, char ch)
{
	for (size_t i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == ch)
		{
			return &str[i];
		}
	}
	return NULL;
}

// Аналог strcmp - сравнение строк
int my_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

/* ==================== */
/* ВВОД-ВЫВОД СТРОК */
/* ==================== */

void string_io(void)
{
	printf("\n=== ВВОД-ВЫВОД СТРОК ===\n\n");

	char buffer[100];

	printf("Введите строку (до 99 символов): ");
	if (fgets(buffer, sizeof(buffer), stdin) != NULL)
	{
		// Убираем символ новой строки, если он есть
		size_t len = strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n')
		{
			buffer[len - 1] = '\0';
		}

		printf("Вы ввели: %s\n", buffer);
		printf("Длина введенной строки: %zu символов\n", strlen(buffer));
	}

	// Опасность gets (не использовать!)
	// char dangerous[10];
	// gets(dangerous); // Может привести к переполнению буфера!
}

/* ==================== */
/* КОДИРОВКИ */
/* ==================== */

void demonstrate_encodings(void)
{
	printf("\n=== КОДИРОВКИ ===\n\n");

	printf("ASCII (American Standard Code for Information Interchange):\n");
	printf("- 7-битная кодировка (0-127)\n");
	printf("- Символы: A-Z, a-z, 0-9, управляющие символы\n");
	printf("Пример: 'A' = %d, 'a' = %d, '0' = %d\n", 'A', 'a', '0');

	printf("\nРасширенные ASCII кодировки (128-255):\n");
	printf("- CP-866 (DOS), CP-1251 (Windows), KOI-8 (Unix)\n");
	printf("- Содержат символы национальных алфавитов\n");

	printf("\nUnicode:\n");
	printf("- Универсальная кодировка для всех языков\n");
	printf("- Изначально 16 бит (65,536 символов)\n");
	printf("- Сейчас до 1,114,112 символов\n");

	printf("\nUTF-8 (Unicode Transformation Format - 8 bit):\n");
	printf("- Переменная длина: 1-4 байта на символ\n");
	printf("- Совместим с ASCII (первые 128 символов одинаковы)\n");
	printf("- Наиболее популярная кодировка в вебе и Linux\n");
}

/* ==================== */
/* РАБОТА С UNICODE В C */
/* ==================== */

void unicode_example(void)
{
	printf("\n=== РАБОТА С UNICODE ===\n\n");

	// Установка локали для поддержки Unicode
	setlocale(LC_ALL, "en_US.UTF-8");

	// Строка в UTF-8 с русскими символами
	const char *utf8_str = "Привет, мир! 🌍";

	printf("UTF-8 строка: %s\n", utf8_str);
	printf("Длина в байтах: %zu\n", strlen(utf8_str));

	// Подсчет символов (не байтов!) в UTF-8 строке
	int char_count = 0;
	const char *p = utf8_str;
	while (*p)
	{
		if ((*p & 0xC0) != 0x80)
		{	 // Начало нового символа UTF-8
			char_count++;
		}
		p++;
	}
	printf("Количество символов: %d\n", char_count);

	// Широкие символы (wchar_t)
	wchar_t wide_str[] = L"Hello, 世界!";
	printf("Широкая строка: ");
	wprintf(L"%ls\n", wide_str);
}

/* ==================== */
/* БЕЗОПАСНАЯ РАБОТА СО СТРОКАМИ */
/* ==================== */

void safe_string_operations(void)
{
	printf("\n=== БЕЗОПАСНАЯ РАБОТА СО СТРОКАМИ ===\n\n");

	char dest[20];
	char src[] = "This is a long string that might not fit";

	// Безопасное копирование с ограничением длины
	strncpy(dest, src, sizeof(dest) - 1);
	dest[sizeof(dest) - 1] = '\0';	  // Гарантируем нулевое завершение

	printf("Безопасное копирование: %s\n", dest);

	// Безопасная конкатенация
	char base[30] = "Hello";
	char to_append[] = ", World! This is too long!";

	strncat(base, to_append, sizeof(base) - strlen(base) - 1);
	printf("Безопасная конкатенация: %s\n", base);

	printf("\nРекомендации по безопасности:\n");
	printf("- Используйте strncpy вместо strcpy\n");
	printf("- Используйте strncat вместо strcat\n");
	printf("- Используйте snprintf вместо sprintf\n");
	printf("- Всегда проверяйте размеры буферов\n");
}

/* ==================== */
/* КОМПИЛЯЦИЯ И НАСТРОЙКИ */
/* ==================== */

/*
 * Примеры флагов компиляции:
 * gcc -std=c23 -O2 -Wall -Wextra program.c -o program
 *
 * Ключевые флаги:
 * -std=c23    - стандарт C23
 * -O2         - уровень оптимизации
 * -Wall       - все предупреждения
 * -Wextra     - дополнительные предупреждения
 */

void compilation_tips(void)
{
	printf("\n=== КОМПИЛЯЦИЯ И НАСТРОЙКИ ===\n\n");

	printf("Рекомендуемые флаги компиляции:\n");
	printf("gcc -std=c23 -O2 -Wall -Wextra -Wpedantic program.c -o program\n\n");

	printf("Флаги для отладки:\n");
	printf("gcc -g -O0 -Wall program.c -o program_debug\n\n");

	printf("Для работы с Unicode:\n");
	printf("setlocale(LC_ALL, \"\"); // Установка локали по умолчанию\n");
}
