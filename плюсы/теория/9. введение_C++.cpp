/*
 * ПОДРОБНОЕ ВВЕДЕНИЕ В C++ - ОСНОВНЫЕ ОТЛИЧИЯ ОТ C
 * Ссылки, перегрузка функций, значения по умолчанию
 */

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

/* ==================== ССЫЛКИ (REFERENCES) ==================== */

/*
 * ССЫЛКИ В C++ - это псевдонимы для переменных
 * Основные характеристики:
 * - Должны быть инициализированы при объявлении
 * - Не могут быть перенаправлены на другую переменную
 * - Не занимают дополнительной памяти
 * - Более безопасны и удобны, чем указатели
 */

// Пример на C (указатели) - оставлено для сравнения
void f_c_style(int y, int *q)
{
	y = 2;	   // Изменяет локальную копию
	*q = 3;	   // Изменяет значение по указателю
}

// Пример на C++ (ссылки)
void f_cpp_style(int &y, int *q)
{
	y = 2;	   // Изменяет оригинальную переменную (через ссылку)
	*q = 3;	   // Изменяет значение по указателю
}

void demonstrate_references()
{
	cout << "=== ССЫЛКИ В C++ ===" << endl;

	int x = 1, z = 0;
	int *p = &x;

	cout << "До вызова: x = " << x << ", z = " << z << endl;

	// C стиль
	f_c_style(z, p);
	cout << "После C стиля: x = " << x << ", z = " << z << " (z не изменился)" << endl;

	// C++ стиль
	x = 1;
	z = 0;	  // Сбрасываем значения
	f_cpp_style(z, p);
	cout << "После C++ стиля: x = " << x << ", z = " << z << " (z изменился через ссылку)" << endl;

	// Создание ссылки
	int &r = z;	   // r - ссылка на z (псевдоним)
	r = 10;
	cout << "После r = 10: z = " << z << endl;
}

/* ==================== КОНСТАНТНЫЕ ССЫЛКИ ==================== */

/*
 * КОНСТАНТНЫЕ ССЫЛКИ используются когда:
 * - Нужно избежать копирования больших объектов
 * - Не планируется изменять передаваемый объект
 * - Нужно передать временный объект или литерал
 */

void process_value(const int &y, int *q)
{
	// y = 2;  // ОШИБКА: y - константная ссылка
	*q = 3;	   // Можно изменять значение по указателю
	cout << "Обработано значение: " << y << endl;
}

void demonstrate_const_references()
{
	cout << "\n=== КОНСТАНТНЫЕ ССЫЛКИ ===" << endl;

	int x = 5;

	// Можно передавать литералы через const ссылку
	process_value(10, &x);	  // Создается временный объект для 10
	cout << "x после process_value: " << x << endl;

	// Константная ссылка на переменную
	const int &ref = x;
	// ref = 20;  // ОШИБКА: нельзя изменить через константную ссылку
	cout << "Константная ссылка ref = " << ref << endl;
}

/* ==================== ССЫЛКИ НА ОБЪЕКТЫ ==================== */

class Person
{
  public:
	string name;
	int age;

	Person(const string &n, int a) : name(n), age(a) {}

	void display() const { cout << "Имя: " << name << ", Возраст: " << age << endl; }
};

void process_person(const Person &person)
{
	cout << "Обработка человека: ";
	person.display();
	// person.age = 30; // ОШИБКА: константная ссылка
}

void demonstrate_object_references()
{
	cout << "\n=== ССЫЛКИ НА ОБЪЕКТЫ ===" << endl;

	Person alice("Алиса", 25);
	Person &alice_ref = alice;	  // Ссылка на объект

	alice_ref.age = 26;	   // Изменяем оригинальный объект
	alice.display();

	process_person(alice);	  // Передача по константной ссылке (без копирования)
}

/* ==================== ОПАСНОСТИ ССЫЛОК ==================== */

/*
 * ОПАСНАЯ СИТУАЦИЯ: возврат ссылки на локальную переменную
 * Локальная переменная уничтожается при выходе из функции
 * Ссылка становится висячей (dangling reference)
 */

// НЕПРАВИЛЬНО - возврат ссылки на локальную переменную
int &bad_function(const int &y, int *q)
{
	int k = y + *q;	   // k - локальная переменная
	return k;		   // ОПАСНО: возвращаем ссылку на локальную переменную
}

// ПРАВИЛЬНО - возврат по значению
int good_function(const int &y, int *q)
{
	int k = y + *q;
	return k;	 // Безопасно: возвращаем копию значения
}

// ПРАВИЛЬНО - возврат ссылки на статическую переменную
int &safe_function(const int &y, int *q)
{
	static int result = 0;	  // Статическая переменная (не уничтожается)
	result = y + *q;
	return result;
}

void demonstrate_dangerous_references()
{
	cout << "\n=== ОПАСНОСТИ ССЫЛОК ===" << endl;

	int x = 5;

	// Опасный вызов (может привести к неопределенному поведению)
	// int &dangerous_ref = bad_function(10, &x);
	// cout << "Опасная ссылка: " << dangerous_ref << endl; // НЕПРЕДСКАЗУЕМО

	// Безопасный вызов
	int safe_value = good_function(10, &x);
	cout << "Безопасное значение: " << safe_value << endl;

	// Безопасный вызов со статической переменной
	int &safe_ref = safe_function(10, &x);
	cout << "Безопасная ссылка: " << safe_ref << endl;
}

/* ==================== ЗНАЧЕНИЯ ПО УМОЛЧАНИЮ ==================== */

/*
 * ЗНАЧЕНИЯ ПО УМОЛЧАНИЮ позволяют:
 * - Упростить вызов функций с необязательными параметрами
 * - Создавать удобные интерфейсы
 * - Обеспечивать обратную совместимость
 *
 * ПРАВИЛА:
 * - Параметры с значениями по умолчанию должны быть в конце
 * - Нельзя пропускать параметры в середине списка
 */

int calculate(int x, int y = 3, float z = 4.5f)
{
	return x + y + static_cast< int >(z);
}

// Функция с параметрами разных типов и значениями по умолчанию
void print_message(const string &message, bool uppercase = false, int repeat = 1)
{
	string output = message;
	if (uppercase)
	{
		for (char &c : output)
		{
			c = toupper(c);
		}
	}

	for (int i = 0; i < repeat; ++i)
	{
		cout << output << endl;
	}
}

/* ==================== ПЕРЕГРУЗКА ФУНКЦИЙ ==================== */

/*
 * ПЕРЕГРУЗКА ФУНКЦИЙ позволяет иметь несколько функций с одним именем,
 * но разными параметрами.
 *
 * КОМПИЛЯЦИЯ:
 * - Компилятор кодирует типы параметров в имени функции (name mangling)
 * - Это позволяет линковщику различать перегруженные функции
 * - Имена становятся длинными и специфичными для компилятора
 */

// Перегруженные функции для разных типов
float process_value(float x)
{
	cout << "Вызвана float версия: ";
	return x + 2.25f;
}

double process_value(double x)
{
	cout << "Вызвана double версия: ";
	return x + 1.43453234234;
}

long double process_value(long double x)
{
	cout << "Вызвана long double версия: ";
	return x + 3.23322313213213123132123L;
}

// Перегрузка по количеству параметров
int process_value(int x, int y)
{
	cout << "Вызвана версия с двумя int: ";
	return x * y;
}

// Перегрузка по типу и количеству параметров
string process_value(const string &x, int repeat)
{
	cout << "Вызвана строковая версия: ";
	string result;
	for (int i = 0; i < repeat; ++i)
	{
		result += x + " ";
	}
	return result;
}

void demonstrate_function_overloading()
{
	cout << "\n=== ПЕРЕГРУЗКА ФУНКЦИЙ ===" << endl;

	float f_val = 1.5f;
	double d_val = 2.5;
	long double ld_val = 3.5L;
	string str_val = "Test";

	cout << "process_value(" << f_val << ") = " << process_value(f_val) << endl;
	cout << "process_value(" << d_val << ") = " << setprecision(10) << process_value(d_val) << endl;
	cout << "process_value(" << ld_val << ") = " << setprecision(15) << process_value(ld_val) << endl;
	cout << "process_value(5, 3) = " << process_value(5, 3) << endl;
	cout << "process_value(\"" << str_val << "\", 3) = " << process_value(str_val, 3) << endl;

	// ПРОБЛЕМА: неоднозначность вызова
	// float ambiguous = process_value(3/2); // ОШИБКА: 3/2 = 1 (int)
	// Нет функции process_value(int)

	// РЕШЕНИЕ: явное приведение типа
	float solved = process_value(static_cast< float >(3 / 2));
	cout << "Явное приведение: " << solved << endl;
}

/* ==================== EXTERN "C" ==================== */

/*
 * EXTERN "C" отключает name mangling для совместимости с C
 * Используется когда:
 * - Создаются библиотеки, которые должны работать и в C, и в C++
 * - Вызываются C функции из C++ кода
 * - Обеспечивается бинарная совместимость
 */

#ifdef __cplusplus
extern "C"
{
#endif

	// Эти функции будут иметь C-совместимые имена (без name mangling)
	int simple_function(int x);
	double another_function(double y);

#ifdef __cplusplus
}
#endif

// Реализации функций
int simple_function(int x)
{
	return x * 2;
}

double another_function(double y)
{
	return y / 2.0;
}

void demonstrate_extern_c()
{
	cout << "\n=== EXTERN \"C\" ДЛЯ СОВМЕСТИМОСТИ ===" << endl;

	cout << "simple_function(5) = " << simple_function(5) << endl;
	cout << "another_function(10.0) = " << another_function(10.0) << endl;

	cout << "Компилируется как C++ код" << endl;
}

/* ==================== СОВРЕМЕННЫЙ C++ СИНТАКСИС ==================== */

// Автоматическое определение типа
auto auto_function(auto x, auto y)
{
	return x + y;
}

// Шаблонная функция как альтернатива перегрузке
template< typename T >
T template_process_value(T x)
{
	cout << "Шаблонная функция с типом: " << typeid(T).name() << " - ";
	return x * 2;
}

// Специализация шаблона для string
template<>
string template_process_value< string >(string x)
{
	cout << "Специализация для string - ";
	return x + x;
}

void demonstrate_modern_cpp()
{
	cout << "\n=== СОВРЕМЕННЫЙ C++ ===" << endl;

	// auto и шаблоны
	auto result1 = auto_function(5, 3);
	auto result2 = auto_function(2.5, 3.7);
	auto result3 = auto_function(string("Hello"), string(" World"));

	cout << "auto_function(5, 3) = " << result1 << endl;
	cout << "auto_function(2.5, 3.7) = " << result2 << endl;
	cout << "auto_function(string, string) = " << result3 << endl;

	cout << "\n--- Шаблонные функции ---" << endl;
	cout << "template_process_value(5) = " << template_process_value(5) << endl;
	cout << "template_process_value(2.5) = " << template_process_value(2.5) << endl;
	cout << "template_process_value(\"Hi\") = " << template_process_value< string >("Hi") << endl;
}

/*
 * ИСТОРИЧЕСКАЯ СПРАВКА:
 * - Первые версии C++ были препроцессорами для C
 * - C++ добавляет много "синтаксического сахара" над C
 * - Стандарты развиваются независимо от C
 * - Современный C++ сильно отличается от классического C
 */
