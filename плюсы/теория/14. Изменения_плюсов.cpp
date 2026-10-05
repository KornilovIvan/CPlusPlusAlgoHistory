/*
 * Лекция по нововведениям в современных C++
 *
 * Содержание:
 * 1. Глобальные и статические переменные
 * 2. constexpr, constinit, consteval
 * 3. Время жизни временных объектов
 * 4. Structured bindings (автоматическая распаковка)
 * 5. Range-based for loops
 * 6. Итераторы и их использование
 */

#include <array>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

// ===== 1. ГЛОБАЛЬНЫЕ И СТАТИЧЕСКИЕ ПЕРЕМЕННЫЕ =====

/*
 * ТЕОРИЯ: Глобальные и статические переменные в C++
 *
 * Глобальные переменные:
 * - Находятся в секции данных программы
 * - Всегда инициализированы (по умолчанию нулем)
 * - В C++ можно использовать сложные инициализаторы
 * - Порядок инициализации между разными файлами не определен
 * - В пределах одного файла инициализируются в порядке определения
 *
 * Статические переменные:
 * - Локальные static: инициализируются при первом вызове функции
 * - Глобальные static: ограничены областью видимости файла
 * - Статические члены класса: общие для всех объектов
 */

// Глобальные переменные
int global_var = 42;		 // Явная инициализация
int global_uninitialized;	 // Неявно инициализируется нулем

// В C++ можно использовать функции для инициализации глобальных переменных
int initialize_value()
{
	return 100;
}

int global_complex = initialize_value();	// В C это было бы невозможно

// Статическая глобальная переменная - видна только в этом файле
static int file_static = 50;

class StaticDemo
{
  public:
	// Статический метод для инициализации
	static int get_value() { return 200; }

	void demonstrate_local_static()
	{
		// Локальная статическая переменная - инициализируется один раз
		static int call_count = 0;
		call_count++;
		cout << "Function called " << call_count << " times" << endl;
	}
};

void demo_global_static()
{
	cout << "=== Демонстрация глобальных и статических переменных ===" << endl;

	cout << "global_var: " << global_var << endl;
	cout << "global_uninitialized: " << global_uninitialized << endl;
	cout << "global_complex: " << global_complex << endl;
	cout << "file_static: " << file_static << endl;

	StaticDemo obj;
	obj.demonstrate_local_static();
	obj.demonstrate_local_static();
	obj.demonstrate_local_static();
}

// ===== 2. CONSTEXPR, CONSTINIT, CONTEVAL =====

/*
 * ТЕОРИЯ: Современные спецификаторы инициализации
 *
 * constexpr:
 * - Переменная/функция должна быть вычислена на этапе компиляции
 * - Значение не может изменяться во время выполнения
 * - Может использоваться в константных выражениях
 *
 * constinit:
 * - Гарантирует инициализацию на этапе компиляции
 * - Значение может изменяться во время выполнения
 * - Решает проблему "static initialization order fiasco"
 * - Только для переменных со статической продолжительностью хранения
 *
 * consteval:
 * - Функция должна выполняться исключительно на этапе компиляции
 * - Не может вызываться во время выполнения
 * - Гарантирует вычисление во время компиляции
 */

// constexpr функция - может выполняться на этапе компиляции
constexpr int factorial(int n)
{
	return (n <= 1) ? 1 : n * factorial(n - 1);
}

// constexpr переменная - вычисляется на этапе компиляции
constexpr int fact_5 = factorial(5);

// constinit переменная - инициализируется на этапе компиляции, но может изменяться
constinit static int compile_time_init = 100;

// consteval функция - должна выполняться только на этапе компиляции
consteval int square(int x)
{
	return x * x;
}

// consteval переменная
constexpr int squared_value = square(5);	// Вычисляется на этапе компиляции

class ConstexprDemo
{
  private:
	int value;

  public:
	// constexpr конструктор
	constexpr ConstexprDemo(int v) : value(v) {}

	// constexpr метод
	constexpr int get_value() const { return value; }

	constexpr int double_value() const { return value * 2; }
};

void demo_const_specifiers()
{
	cout << "\n=== Демонстрация constexpr, constinit, consteval ===" << endl;

	// constexpr демонстрация
	cout << "constexpr factorial(5): " << fact_5 << endl;

	constexpr ConstexprDemo obj(10);
	cout << "constexpr object value: " << obj.get_value() << endl;
	cout << "constexpr object doubled: " << obj.double_value() << endl;

	// constinit демонстрация
	cout << "constinit variable: " << compile_time_init << endl;
	compile_time_init = 200;	// Можно изменять!
	cout << "modified constinit variable: " << compile_time_init << endl;

	// consteval демонстрация
	constexpr int result = square(8);	 // Вычисляется на этапе компиляции
	cout << "consteval square(8): " << result << endl;

	// Сравнение времени вычисления
	int runtime_value = 6;
	// int compile_time_result = square(runtime_value);  // ОШИБКА: runtime_value не константа

	// Массивы с constexpr размерами
	constexpr int array_size = 10;
	int compile_time_array[array_size];	   // Размер известен на этапе компиляции

	cout << "Array size known at compile time: " << array_size << endl;
}

// ===== 3. ВРЕМЯ ЖИЗНИ ВРЕМЕННЫХ ОБЪЕКТОВ =====

/*
 * ТЕОРИЯ: Время жизни временных объектов и rvalue ссылки
 *
 * Временные объекты:
 * - Создаются как результат выражений
 * - Обычно уничтожаются в конце полного выражения
 * - С помощью rvalue ссылок можно продлить время жизни
 *
 * Rvalue ссылки (&&):
 * - Могут связываться с временными объектами
 * - Продлевают время жизни временного объекта до времени жизни ссылки
 * - Основа move-семантики в C++11
 */

class TemporaryDemo
{
  private:
	int id;
	static int next_id;

  public:
	TemporaryDemo() : id(next_id++) { cout << "TemporaryDemo " << id << " constructed" << endl; }

	~TemporaryDemo() { cout << "TemporaryDemo " << id << " destroyed" << endl; }

	void speak() const { cout << "Hello from TemporaryDemo " << id << endl; }
};

int TemporaryDemo::next_id = 1;

// Функция, возвращающая временный объект
TemporaryDemo create_temporary()
{
	return TemporaryDemo();
}

void demo_temporary_lifetime()
{
	cout << "\n=== Демонстрация времени жизни временных объектов ===" << endl;

	cout << "--- Без продления времени жизни ---" << endl;
	create_temporary().speak();	   // Временный объект уничтожается сразу после вызова speak()

	cout << "--- С продлением времени жизни через const rvalue ссылку ---" << endl;
	const TemporaryDemo&& temp1 = create_temporary();
	temp1.speak();	  // Объект живет, пока жива ссылка temp1

	cout << "--- С продлением времени жизни через rvalue ссылку ---" << endl;
	TemporaryDemo&& temp2 = create_temporary();
	temp2.speak();	  // Объект живет, пока жива ссылка temp2

	cout << "--- Конец блока demo_temporary_lifetime ---" << endl;
	// Здесь уничтожаются temp1 и temp2
}

// ===== 4. STRUCTURED BINDINGS (АВТОМАТИЧЕСКАЯ РАСПАКОВКА) =====

/*
 * ТЕОРИЯ: Structured Bindings (C++17)
 *
 * Позволяет автоматически распаковывать:
 * - Массивы
 * - Tuple-like объекты (std::tuple, std::pair)
 * - Структуры и классы с публичными полями
 *
 * Синтаксис:
 * - auto [var1, var2, ...] = expression;  // Копирование
 * - auto& [var1, var2, ...] = expression; // Ссылки
 * - const auto& [var1, var2, ...] = expression; // Константные ссылки
 */

struct Point
{
	int x;
	double y;
	string name;
};

tuple< int, string, double > create_tuple()
{
	return { 42, "hello", 3.14 };
}

array< int, 3 > create_array()
{
	return { 1, 2, 3 };
}

void demo_structured_bindings()
{
	cout << "\n=== Демонстрация Structured Bindings ===" << endl;

	// Распаковка структуры
	Point p{ 10, 20.5, "origin" };
	auto [x_coord, y_coord, point_name] = p;	// Копирование
	cout << "Point: x=" << x_coord << ", y=" << y_coord << ", name=" << point_name << endl;

	// Распаковка по ссылке (изменяет оригинал)
	auto& [x_ref, y_ref, name_ref] = p;
	x_ref = 100;	// Изменяет оригинальную структуру
	cout << "Modified point: x=" << p.x << ", y=" << p.y << endl;

	// Распаковка tuple
	auto [number, text, value] = create_tuple();
	cout << "Tuple: " << number << ", " << text << ", " << value << endl;

	// Распаковка массива
	int arr[2] = { 1, 2 };
	auto [first, second] = arr;	   // Копирование элементов
	cout << "Array elements: " << first << ", " << second << endl;

	// Распаковка по ссылке на массив
	auto& [first_ref, second_ref] = arr;
	first_ref = 99;	   // Изменяет оригинальный массив
	cout << "Modified array: " << arr[0] << ", " << arr[1] << endl;

	// Распаковка из функции, возвращающей tuple (аналог Python)
	auto [a, b, c] = create_tuple();
	cout << "From function: " << a << ", " << b << ", " << c << endl;
}

// ===== 5. RANGE-BASED FOR LOOPS =====

/*
 * ТЕОРИЯ: Range-based for loops (C++11)
 *
 * Упрощает итерацию по контейнерам:
 * - Работает с массивами, STL контейнерами, пользовательскими типами
 * - Автоматически определяет начало и конец диапазона
 * - Поддерживает различные способы доступа к элементам
 *
 * Синтаксис:
 * - for (auto element : container) {}        // Копирование
 * - for (auto& element : container) {}       // Ссылка (можно изменять)
 * - for (const auto& element : container) {} // Константная ссылка
 */

void demo_range_based_for()
{
	cout << "\n=== Демонстрация Range-based for loops ===" << endl;

	// Итерация по массиву
	int numbers[] = { 1, 2, 3, 4, 5 };
	cout << "Array elements: ";
	for (auto num : numbers)
	{
		cout << num << " ";
	}
	cout << endl;

	// Итерация по вектору с изменением элементов
	vector< string > words = { "hello", "world", "c++" };
	cout << "Original vector: ";
	for (const auto& word : words)
	{
		cout << word << " ";
	}
	cout << endl;

	// Изменение элементов через ссылку
	for (auto& word : words)
	{
		word += "!";	// Добавляем восклицательный знак к каждому слову
	}

	cout << "Modified vector: ";
	for (const auto& word : words)
	{
		cout << word << " ";
	}
	cout << endl;

	// Итерация по map
	map< int, string > id_to_name = { { 1, "Alice" }, { 2, "Bob" }, { 3, "Charlie" } };

	cout << "Map contents: " << endl;
	for (const auto& [id, name] : id_to_name)
	{	 // Structured binding + range-based for
		cout << "  " << id << ": " << name << endl;
	}
}

// ===== 6. ИТЕРАТОРЫ И ИХ ИСПОЛЬЗОВАНИЕ =====

/*
 * ТЕОРИЯ: Итераторы в C++
 *
 * Итераторы - обобщение указателей:
 * - Input iterator: только чтение, однонаправленный
 * - Forward iterator: чтение/запись, однонаправленный
 * - Bidirectional iterator: чтение/запись, двунаправленный
 * - Random access iterator: чтение/запись, произвольный доступ
 *
 * Range-based for internally uses iterators:
 * - begin(): итератор на первый элемент
 * - end(): итератор за последний элемент
 */

class CustomContainer
{
  private:
	vector< int > data;

  public:
	CustomContainer(initializer_list< int > init) : data(init) {}

	// Методы для поддержки range-based for
	auto begin() const { return data.begin(); }
	auto end() const { return data.end(); }

	// Неконстантные версии для изменения
	auto begin() { return data.begin(); }
	auto end() { return data.end(); }
};

void demo_iterators()
{
	cout << "\n=== Демонстрация итераторов ===" << endl;

	vector< int > numbers = { 10, 20, 30, 40, 50 };

	// Классический подход с итераторами
	cout << "Classic iterator approach: ";
	for (auto it = numbers.begin(); it != numbers.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;

	// Range-based for (внутренне использует итераторы)
	cout << "Range-based for: ";
	for (int num : numbers)
	{
		cout << num << " ";
	}
	cout << endl;

	// Пользовательский контейнер с поддержкой range-based for
	CustomContainer custom{ 1, 3, 5, 7, 9 };
	cout << "Custom container: ";
	for (int value : custom)
	{
		cout << value << " ";
	}
	cout << endl;

	// Различные типы итераторов
	cout << "Reverse iteration: ";
	for (auto it = numbers.rbegin(); it != numbers.rend(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;

	// Использование итераторов для модификации
	for (auto it = numbers.begin(); it != numbers.end(); ++it)
	{
		*it *= 2;	 // Удваиваем каждый элемент
	}

	cout << "Doubled elements: ";
	for (int num : numbers)
	{
		cout << num << " ";
	}
	cout << endl;
}

// ===== 7. ПРАКТИЧЕСКИЕ ПРИМЕРЫ СОВРЕМЕННОГО C++ =====

void demo_modern_cpp_features()
{
	cout << "\n=== Демонстрация современных возможностей C++ ===" << endl;

	// Пример 1: Комбинирование нескольких фич
	constexpr array< tuple< int, string >, 3 > data = { make_tuple(1, "apple"), make_tuple(2, "banana"), make_tuple(3, "cherry") };

	cout << "Combined features example:" << endl;
	for (const auto& [id, name] : data)
	{
		cout << "  " << id << ": " << name << endl;
	}

	// Пример 2: constexpr + structured binding
	constexpr Point constexpr_point{ 5, 10.5, "constexpr_point" };
	const auto& [cx, cy, cn] = constexpr_point;
	cout << "Constexpr point: " << cx << ", " << cy << ", " << cn << endl;

	// Пример 3: Range-based for с пользовательской логикой
	map< string, vector< int > > student_grades = { { "Alice", { 85, 90, 78 } }, { "Bob", { 92, 88, 95 } }, { "Charlie", { 76, 82, 80 } } };

	cout << "Student grades analysis:" << endl;
	for (const auto& [student, grades] : student_grades)
	{
		int sum = 0;
		for (int grade : grades)
		{
			sum += grade;
		}
		double average = static_cast< double >(sum) / grades.size();
		cout << "  " << student << ": average = " << average << endl;
	}
}

/*
 * ДОПОЛНИТЕЛЬНАЯ ТЕОРИЯ:
 *
 * 1. Static Initialization Order Fiasco:
 *    - Проблема, когда глобальные объекты в разных файлах зависят друг от друга
 *    - constinit решает эту проблему для скалярных типов
 *    - Для сложных объектов используют "construct on first use"
 *
 * 2. Отличия constexpr от const:
 *    - const: значение не меняется после инициализации (может быть runtime)
 *    - constexpr: значение известно на этапе компиляции
 *
 * 3. Perfect forwarding с rvalue ссылками:
 *    - Позволяет передавать аргументы без потери категории значения
 *    - Используется в шаблонах для оптимальной передачи параметров
 *
 * 4. Concept-based programming (C++20):
 *    - Ограничения шаблонов для лучших сообщений об ошибках
 *    - Более читаемый и безопасный generic код
 *
 * 5. Метапрограммирование с constexpr:
 *    - Вычисления на этапе компиляции становятся проще и мощнее
 *    - Можно создавать сложные структуры данных на этапе компиляции
 *
 * 6. Structured bindings с пользовательскими типами:
 *    - Можно специализировать std::tuple_size и std::tuple_element
 *    - Или использовать get<N>() для пользовательских типов
 */
