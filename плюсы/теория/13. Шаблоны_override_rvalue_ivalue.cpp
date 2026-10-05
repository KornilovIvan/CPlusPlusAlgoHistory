/*
 * Лекция по шаблонам, override, rvalue/lvalue в C++
 *
 * Содержание:
 * 1. Ключевое слово override
 * 2. Глобальные константы и extern
 * 3. Вывод типа auto и decltype(auto)
 * 4. Лямбда-функции и захваты
 * 5. Категории значений (lvalue, rvalue, xvalue, prvalue)
 * 6. Семантика перемещения (move semantics)
 * 7. Шаблоны функций и классов
 * 8. Специализация шаблонов
 */

#include <type_traits>

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// ===== 1. КЛЮЧЕВОЕ СЛОВО OVERRIDE =====

/*
 * ТЕОРИЯ: Ключевое слово override
 *
 * override явно указывает, что метод переопределяет виртуальный метод базового класса:
 * - Помогает обнаружить ошибки на этапе компиляции
 * - Улучшает читаемость кода
 * - Защищает от опечаток в сигнатурах методов
 *
 * Без override компилятор не предупредит, если сигнатура не совпадает
 */

class Base
{
  public:
	virtual void process() const { cout << "Base::process()" << endl; }

	virtual int calculate(int x) const { return x * 2; }

	virtual ~Base() = default;
};

class Derived : public Base
{
  public:
	// Правильно - явно указываем override
	void process() const override { cout << "Derived::process()" << endl; }

	int calculate(int x) const override { return x * 3; }

	// ОШИБКА компиляции - метод не существует в базовом классе
	// void some_method() override { }  // Error: marked 'override' but does not override
};

class BadDerived : public Base
{
  public:
	// Без override - компилятор не поймет ошибку (опечатка в имени)
	void proces() const
	{	 // Должно быть process()
		cout << "This won't be called polymorphically!" << endl;
	}
};

void demo_override()
{
	cout << "=== Демонстрация override ===" << endl;

	Derived derived;
	Base* base_ptr = &derived;

	base_ptr->process();	// Вызовется Derived::process()
	cout << "calculate(5): " << base_ptr->calculate(5) << endl;

	BadDerived bad;
	Base* bad_ptr = &bad;
	bad_ptr->process();	   // Вызовется Base::process(), а не BadDerived::proces()
}

// ===== 2. ГЛОБАЛЬНЫЕ КОНСТАНТЫ И EXTERN =====

/*
 * ТЕОРИЯ: Глобальные константы и линковка
 *
 * В C++ глобальные const переменные по умолчанию имеют internal linkage
 * (как будто объявлены static). Это отличается от C.
 *
 * extern const заставляет переменную иметь external linkage
 */

// По умолчанию - internal linkage (не видна в других файлах)
const int INTERNAL_CONST = 100;

// External linkage - видна в других файлах
extern const int EXTERNAL_CONST = 200;

// constexpr также имеет internal linkage по умолчанию
constexpr int INTERNAL_CONSTEXPR = 300;

// extern constexpr - external linkage
extern constexpr int EXTERNAL_CONSTEXPR = 400;

void demo_linkage()
{
	cout << "\n=== Демонстрация линковки констант ===" << endl;
	cout << "INTERNAL_CONST: " << INTERNAL_CONST << endl;
	cout << "EXTERNAL_CONST: " << EXTERNAL_CONST << endl;
	cout << "INTERNAL_CONSTEXPR: " << INTERNAL_CONSTEXPR << endl;
	cout << "EXTERNAL_CONSTEXPR: " << EXTERNAL_CONSTEXPR << endl;
}

// ===== 3. ВЫВОД ТИПА AUTO И DECLTYPE(AUTO) =====

/*
 * ТЕОРИЯ: auto и decltype(auto)
 *
 * auto:
 * - Выводит тип, отбрасывая ссылки и константность
 * - Удобно для длинных типов или итераторов
 *
 * decltype(auto):
 * - Сохраняет точный тип выражения (включая ссылки и константность)
 * - Полезно для perfect forwarding в шаблонах
 */

const int global_var = 42;
const int& get_ref()
{
	return global_var;
}

void demo_auto_decltype()
{
	cout << "\n=== Демонстрация auto и decltype(auto) ===" << endl;

	const int x = 10;
	const int& x_ref = x;

	// auto отбрасывает const и ссылки
	auto y1 = x;		// type: int (const отброшен)
	auto y2 = x_ref;	// type: int (const и & отброшены)

	y1 = 20;	// OK - y1 не const
	cout << "auto y1: " << y1 << endl;

	// decltype(auto) сохраняет точный тип
	decltype(auto) z1 = x;		  // type: const int
	decltype(auto) z2 = x_ref;	  // type: const int&

	// z1 = 30;  // ОШИБКА: z1 - const

	// Пример с функциями
	auto result1 = get_ref();			   // type: int (значение)
	decltype(auto) result2 = get_ref();	   // type: const int& (ссылка)

	cout << "auto: " << result1 << ", decltype(auto): " << result2 << endl;

	// Указатели с auto
	auto ptr1 = &x;		// type: const int*
	auto* ptr2 = &x;	// type: const int* (явно указатель)

	cout << "ptr1: " << *ptr1 << ", ptr2: " << *ptr2 << endl;
}

// Функции с auto возвращаемым типом
auto deduce_return_type() -> int
{
	return 42;
}

auto deduce_with_trailing() -> double
{
	return 3.14;
}

decltype(auto) perfect_forwarding_example()
{
	return get_ref();	 // Возвращает const int&
}

// ===== 4. ЛЯМБДА-ФУНКЦИИ И ЗАХВАТЫ =====

/*
 * ТЕОРИЯ: Лямбда-функции
 *
 * Лямбда - это анонимный функциональный объект:
 * - [capture] (params) -> return_type { body }
 * - Компилятор генерирует уникальный класс с operator()
 *
 * Захваты (capture):
 * - [] - ничего не захватывает
 * - [=] - захват по значению (все видимые переменные)
 * - [&] - захват по ссылке (все видимые переменные)
 * - [var] - захват конкретной переменной по значению
 * - [&var] - захват конкретной переменной по ссылке
 * - [this] - захват текущего объекта
 * - mutable - позволяет изменять захваченные по значению переменные
 */

class LambdaDemo
{
  private:
	int class_var = 100;

  public:
	void demonstrate_lambdas()
	{
		cout << "\n=== Демонстрация лямбда-функций ===" << endl;

		int x = 5;
		int y = 10;

		// 1. Простая лямбда без захватов
		auto simple = []()
		{
			return 42;
		};
		cout << "Simple lambda: " << simple() << endl;

		// 2. Захват по значению
		auto capture_by_value = [x](int a)
		{
			return a + x;
		};
		x = 0;	  // Изменение оригинала не влияет на захваченную копию
		cout << "Capture by value: " << capture_by_value(10) << " (x was captured as 5)" << endl;

		// 3. Захват по ссылке
		auto capture_by_reference = [&y](int a)
		{
			return a + y;
		};
		y = 20;	   // Изменение оригинала влияет на захват
		cout << "Capture by reference: " << capture_by_reference(10) << " (y is now 20)" << endl;

		// 4. Захват с mutable
		auto mutable_lambda = [x]() mutable
		{
			x++;	// Можно изменять захваченную копию
			return x;
		};
		cout << "Mutable lambda first call: " << mutable_lambda() << endl;
		cout << "Mutable lambda second call: " << mutable_lambda() << endl;

		// 5. Захват this
		auto capture_this = [this]()
		{
			return class_var;
		};
		cout << "Capture this: " << capture_this() << endl;

		// 6. Смешанный захват
		auto mixed_capture = [&, x](int a)
		{
			return a + x + y;
		};
		cout << "Mixed capture: " << mixed_capture(5) << endl;

		// 7. Переименование при захвате
		auto renamed_capture = [value = x, ref = y]()
		{
			return value + ref;
		};
		cout << "Renamed capture: " << renamed_capture() << endl;

		// 8. Явное указание возвращаемого типа
		auto explicit_return = [x](double a) -> long
		{
			return static_cast< long >(a + x);
		};
		cout << "Explicit return type: " << explicit_return(3.14) << endl;
	}
};

// Структура с mutable полем
struct MutableStruct
{
	mutable int mutable_field;
	int normal_field;

	MutableStruct(int a, int b) : mutable_field(a), normal_field(b) {}
};

void demo_mutable()
{
	cout << "\n=== Демонстрация mutable ===" << endl;

	const MutableStruct obj(1, 2);
	obj.mutable_field = 3;	  // OK - поле mutable
	// obj.normal_field = 4;  // ОШИБКА - обычное поле в const объекте

	cout << "Mutable field: " << obj.mutable_field << endl;
	cout << "Normal field: " << obj.normal_field << endl;
}

// ===== 5. КАТЕГОРИИ ЗНАЧЕНИЙ =====

/*
 * ТЕОРИЯ: Категории значений в C++
 *
 * lvalue (locator value):
 * - Имеет имя и адрес в памяти
 * - Может находиться слева от =
 * - Примеры: переменные, ссылки, вызов функций, возвращающих lvalue-ссылку
 *
 * xvalue (eXpiring value):
 * - Временный объект, ресурсы которого можно переместить
 * - Примеры: результат std::move, возврат локальной переменной по значению
 *
 * prvalue (pure rvalue):
 * - Временное значение без адреса
 * - Примеры: литералы, арифметические выражения, вызов функций, возвращающих по значению
 *
 * glvalue (generalized lvalue): lvalue + xvalue
 * rvalue: xvalue + prvalue
 */

string create_string()
{
	return "temporary string";
}

void process_value(string&& str)
{
	cout << "Processing rvalue: " << str << endl;
}

void process_value(const string& str)
{
	cout << "Processing lvalue: " << str << endl;
}

void demo_value_categories()
{
	cout << "\n=== Демонстрация категорий значений ===" << endl;

	string str = "hello";	  // str - lvalue
	string& str_ref = str;	  // str_ref - lvalue

	// Литералы - prvalue
	cout << "Literal (prvalue): " << "literal" << endl;

	// Результат функции - prvalue
	string temp = create_string();	  // create_string() - prvalue

	// lvalue передается в const& версию
	process_value(str);	   // lvalue -> const string&

	// prvalue передается в && версию
	process_value(create_string());	   // prvalue -> string&&

	// xvalue создается через std::move
	process_value(move(str));	 // xvalue -> string&&

	// Арифметическое выражение - prvalue
	int result = 2 + 3;	   // (2 + 3) - prvalue

	cout << "All value categories demonstrated" << endl;
}

// ===== 6. СЕМАНТИКА ПЕРЕМЕЩЕНИЯ =====

/*
 * ТЕОРИЯ: Семантика перемещения
 *
 * Позволяет "перемещать" ресурсы из временных объектов вместо копирования:
 * - Конструктор перемещения: T(T&& other)
 * - Оператор присваивания перемещением: T& operator=(T&& other)
 * - std::move: преобразует lvalue в rvalue
 *
 * Эффективно для тяжелых объектов (строки, векторы, умные указатели)
 */

class ResourceHolder
{
  private:
	string* data;
	size_t size;

  public:
	// Конструктор
	ResourceHolder(const string& str = "") : size(str.size())
	{
		data = new string(str);
		cout << "Constructor: " << *data << endl;
	}

	// Конструктор копирования (глубокое копирование)
	ResourceHolder(const ResourceHolder& other) : size(other.size)
	{
		data = new string(*other.data);
		cout << "Copy constructor: " << *data << endl;
	}

	// Конструктор перемещения (поверхностное копирование + обнуление)
	ResourceHolder(ResourceHolder&& other) noexcept : data(other.data), size(other.size)
	{
		other.data = nullptr;
		other.size = 0;
		cout << "Move constructor" << endl;
	}

	// Оператор присваивания копированием
	ResourceHolder& operator=(const ResourceHolder& other)
	{
		if (this != &other)
		{
			delete data;
			size = other.size;
			data = new string(*other.data);
			cout << "Copy assignment: " << *data << endl;
		}
		return *this;
	}

	// Оператор присваивания перемещением
	ResourceHolder& operator=(ResourceHolder&& other) noexcept
	{
		if (this != &other)
		{
			delete data;
			data = other.data;
			size = other.size;
			other.data = nullptr;
			other.size = 0;
			cout << "Move assignment" << endl;
		}
		return *this;
	}

	// Деструктор
	~ResourceHolder()
	{
		if (data)
		{
			cout << "Destructor: " << *data << endl;
			delete data;
		}
		else
		{
			cout << "Destructor: nullptr" << endl;
		}
	}

	void print() const
	{
		if (data)
		{
			cout << "Resource: " << *data << " (size: " << size << ")" << endl;
		}
		else
		{
			cout << "Resource: nullptr" << endl;
		}
	}
};

ResourceHolder create_resource()
{
	return ResourceHolder("Temporary Resource");
}

void demo_move_semantics()
{
	cout << "\n=== Демонстрация семантики перемещения ===" << endl;

	cout << "--- Создание объектов ---" << endl;
	ResourceHolder rh1("First Resource");
	ResourceHolder rh2("Second Resource");

	cout << "--- Копирование ---" << endl;
	ResourceHolder rh3 = rh1;	 // Конструктор копирования

	cout << "--- Перемещение ---" << endl;
	ResourceHolder rh4 = move(rh2);	   // Конструктор перемещения
	rh4.print();
	rh2.print();	// rh2 теперь пустой

	cout << "--- Присваивание перемещением ---" << endl;
	rh3 = create_resource();	// Оператор присваивания перемещением

	cout << "--- Возврат из функции ---" << endl;
	ResourceHolder rh5 = create_resource();	   // Может использоваться перемещение

	cout << "--- Конец блока ---" << endl;
}

// ===== 7. ШАБЛОНЫ ФУНКЦИЙ И КЛАССОВ =====

/*
 * ТЕОРИЯ: Шаблоны в C++
 *
 * Шаблоны позволяют писать обобщенный код:
 * - Инстанцируются на этапе компиляции для конкретных типов
 * - Поддерживают специализацию для конкретных типов
 * - Отличаются от дженериков Java/C# code generation подходом
 *
 * Преимущества:
 * - Type safety (проверка типов на этапе компиляции)
 * - Нет overhead runtime проверок
 * - Можно специализировать для конкретных типов
 */

// Шаблон функции
template< typename T >
T generic_abs(T x)
{
	return x < 0 ? -x : x;
}

// Шаблон с несколькими параметрами
template< typename T1, typename T2 >
auto generic_max(T1 a, T2 b) -> decltype(a > b ? a : b)
{
	return a > b ? a : b;
}

// Шаблон с нетиповыми параметрами
template< typename T, int SIZE >
class FixedArray
{
  private:
	T data[SIZE];

  public:
	T& operator[](int index) { return data[index]; }

	const T& operator[](int index) const { return data[index]; }

	int getSize() const { return SIZE; }
};

// Шаблон класса
template< typename T >
class Box
{
  private:
	T content;

  public:
	Box(const T& value) : content(value) {}

	T get() const { return content; }
	void set(const T& value) { content = value; }

	void print() const { cout << "Box contains: " << content << endl; }
};

void demo_templates()
{
	cout << "\n=== Демонстрация шаблонов ===" << endl;

	// Шаблоны функций
	cout << "abs(-5): " << generic_abs(-5) << endl;
	cout << "abs(-3.14): " << generic_abs(-3.14) << endl;

	cout << "max(3, 5.5): " << generic_max(3, 5.5) << endl;
	cout << "max(2.7, 1): " << generic_max(2.7, 1) << endl;

	// Шаблоны классов
	Box< int > int_box(42);
	int_box.print();

	Box< string > string_box("Hello Templates");
	string_box.print();

	// Шаблон с нетиповым параметром
	FixedArray< double, 5 > array;
	for (int i = 0; i < array.getSize(); ++i)
	{
		array[i] = i * 1.1;
	}

	cout << "FixedArray: ";
	for (int i = 0; i < array.getSize(); ++i)
	{
		cout << array[i] << " ";
	}
	cout << endl;
}

// ===== 8. СПЕЦИАЛИЗАЦИЯ ШАБЛОНОВ =====

/*
 * ТЕОРИЯ: Специализация шаблонов
 *
 * Позволяет предоставить особую реализацию для конкретных типов:
 * - Полная специализация: template<> class Name<SpecificType>
 * - Частичная специализация: template<typename T> class Name<T*>
 *
 * Используется для оптимизации или особого поведения
 */

// Основной шаблон
template< typename T >
class TypeInfo
{
  public:
	static string name() { return "unknown"; }
};

// Полная специализация для int
template<>
class TypeInfo< int >
{
  public:
	static string name() { return "int"; }
};

// Полная специализация для double
template<>
class TypeInfo< double >
{
  public:
	static string name() { return "double"; }
};

// Полная специализация для string
template<>
class TypeInfo< string >
{
  public:
	static string name() { return "string"; }
};

// Частичная специализация для указателей
template< typename T >
class TypeInfo< T* >
{
  public:
	static string name() { return TypeInfo< T >::name() + "*"; }
};

// Специализация функции
template< typename T >
bool is_positive(T value)
{
	return value > 0;
}

template<>
bool is_positive< string >(string value)
{
	return !value.empty() && value[0] != '-';
}

void demo_specialization()
{
	cout << "\n=== Демонстрация специализации шаблонов ===" << endl;

	cout << "TypeInfo<int>: " << TypeInfo< int >::name() << endl;
	cout << "TypeInfo<double>: " << TypeInfo< double >::name() << endl;
	cout << "TypeInfo<string>: " << TypeInfo< string >::name() << endl;
	cout << "TypeInfo<int*>: " << TypeInfo< int* >::name() << endl;
	cout << "TypeInfo<double*>: " << TypeInfo< double* >::name() << endl;

	cout << "is_positive(5): " << is_positive(5) << endl;
	cout << "is_positive(-3): " << is_positive(-3) << endl;
	cout << "is_positive(\"hello\"): " << is_positive(string("hello")) << endl;
	cout << "is_positive(\"\"): " << is_positive(string("")) << endl;
}

// ===== 9. AUTO В ПАРАМЕТРАХ ФУНКЦИЙ (C++20) =====

/*
 * ТЕОРИЯ: Auto в параметрах функций
 *
 * C++20 позволяет использовать auto в параметрах функций:
 * - Создает шаблон функции автоматически
 * - Упрощает написание generic кода
 */

// Эквивалентно template<typename T> void auto_param(T x)
void auto_param(auto x)
{
	cout << "auto_param: " << x << " (type: " << typeid(x).name() << ")" << endl;
}

// Несколько auto параметров
void multiple_auto_params(auto x, auto y)
{
	cout << "multiple_auto_params: " << x << ", " << y << endl;
}

void demo_auto_params()
{
	cout << "\n=== Демонстрация auto в параметрах ===" << endl;

	auto_param(42);
	auto_param(3.14);
	auto_param(string("hello"));

	multiple_auto_params(1, 2.5);
	multiple_auto_params("text", 100);
}

/*
 * ДОПОЛНИТЕЛЬНАЯ ТЕОРИЯ:
 *
 * 1. Perfect Forwarding:
 *    - Использование универсальных ссылок (T&&) и std::forward
 *    - Позволяет сохранять категорию значения при передаче через шаблоны
 *
 * 2. SFINAE (Substitution Failure Is Not An Error):
 *    - Механизм отбора перегрузок шаблонов на этапе компиляции
 *    - Основа для type traits и концептов
 *
 * 3. Концепты (C++20):
 *    - Ограничения для шаблонных параметров
 *    - Улучшают сообщения об ошибках и читаемость
 *
 * 4. Variable Templates (C++14):
 *    - Шаблоны переменных
 *    - Полезно для математических констант и метапрограммирования
 *
 * 5. Fold Expressions (C++17):
 *    - Упрощают работу с variadic templates
 *    - Позволяют применять операции к пакетам параметров
 *
 * 6. Template Argument Deduction Guides (C++17):
 *    - Помогают компилятору выводить аргументы шаблонов
 *    - Особенно полезно для шаблонов классов
 */
