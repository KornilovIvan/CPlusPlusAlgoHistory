/*
 * Лекция по исключениям и приведениям типов в C++
 *
 * Содержание:
 * 1. Исключения (exceptions)
 * 2. Спецификатор noexcept
 * 3. RAII (Resource Acquisition Is Initialization)
 * 4. Конструкторы преобразования
 * 5. Операторы присваивания и правило трех
 * 6. Приведения типов (C++ casts)
 * 7. RTTI (Run-Time Type Information)
 */

#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
using namespace std;

// ===== 1. ИСКЛЮЧЕНИЯ (EXCEPTIONS) =====

/*
 * ТЕОРИЯ: Исключения в C++
 *
 * Исключения - механизм обработки ошибок, который позволяет:
 * - Отделить код обработки ошибок от основной логики
 * - Автоматически раскручивать стек вызовов
 * - Гарантировать освобождение ресурсов через деструкторы
 *
 * Основные компоненты:
 * - throw: бросает исключение
 * - try: блок кода, в котором могут возникнуть исключения
 * - catch: блок обработки исключений
 *
 * Особенности:
 * - Исключения могут быть любого типа
 * - Можно иметь несколько catch блоков
 * - Исключения распространяются вверх по стеку вызовов
 * - Если исключение не перехвачено, программа завершается
 */

// Базовый класс для пользовательских исключений
class MathException : public exception
{
  private:
	string message;

  public:
	MathException(const string& msg) : message(msg) {}

	const char* what() const noexcept override { return message.c_str(); }
};

class DivisionByZeroException : public MathException
{
  public:
	DivisionByZeroException() : MathException("Division by zero") {}
};

class NegativeValueException : public MathException
{
  public:
	NegativeValueException() : MathException("Negative value not allowed") {}
};

// Функция, которая бросает исключения
double safe_divide(double a, double b)
{
	if (b == 0)
	{
		throw DivisionByZeroException();
	}
	return a / b;
}

double calculate_sqrt(double x)
{
	if (x < 0)
	{
		throw NegativeValueException();
	}
	// Упрощенная реализация
	return x;	 // В реальности здесь было бы вычисление корня
}

void demo_basic_exceptions()
{
	cout << "=== Демонстрация базовых исключений ===" << endl;

	// Пример 1: Обработка исключения
	try
	{
		double result = safe_divide(10, 0);
		cout << "Result: " << result << endl;
	} catch (const DivisionByZeroException& e)
	{
		cout << "Caught DivisionByZeroException: " << e.what() << endl;
	}

	// Пример 2: Обработка нескольких типов исключений
	try
	{
		calculate_sqrt(-5);
	} catch (const NegativeValueException& e)
	{
		cout << "Caught NegativeValueException: " << e.what() << endl;
	} catch (const exception& e)
	{
		cout << "Caught general exception: " << e.what() << endl;
	}

	// Пример 3: Исключения разных типов
	try
	{
		throw string("String exception");
	} catch (const string& e)
	{
		cout << "Caught string exception: " << e << endl;
	}

	// Пример 4: Переброс исключения
	try
	{
		try
		{
			throw runtime_error("Inner exception");
		} catch (const runtime_error& e)
		{
			cout << "Inner catch: " << e.what() << endl;
			throw;	  // Переброс того же исключения
		}
	} catch (const runtime_error& e)
	{
		cout << "Outer catch: " << e.what() << endl;
	}
}

// ===== 2. СПЕЦИФИКАТОР NOEXCEPT =====

/*
 * ТЕОРИЯ: Спецификатор noexcept
 *
 * noexcept указывает, что функция не бросает исключений:
 * - Позволяет компилятору лучше оптимизировать код
 * - Если функция с noexcept все же бросит исключение, программа завершится
 * - Важен для move-семантики и некоторых алгоритмов STL
 *
 * Формы:
 * - noexcept: функция никогда не бросает исключений
 * - noexcept(expression): функция не бросает, если expression истинно
 * - noexcept(true/false): явное указание
 */

class NoExceptDemo
{
  public:
	// Функция, которая гарантированно не бросает исключений
	void simple_operation() noexcept { cout << "This function never throws" << endl; }

	// Функция, которая может бросать исключения
	void potentially_dangerous() { throw runtime_error("This might throw"); }

	// Условный noexcept
	template< typename T >
	void conditional_noexcept(T&& value) noexcept(noexcept(T(std::forward< T >(value))))
	{
		// Реализация, которая зависит от того, бросает ли T исключения
	}
};

void demo_noexcept()
{
	cout << "\n=== Демонстрация noexcept ===" << endl;

	NoExceptDemo demo;
	demo.simple_operation();

	try
	{
		demo.potentially_dangerous();
	} catch (const exception& e)
	{
		cout << "Caught: " << e.what() << endl;
	}
}

// ===== 3. RAII (RESOURCE ACQUISITION IS INITIALIZATION) =====

/*
 * ТЕОРИЯ: RAII - Resource Acquisition Is Initialization
 *
 * Идиома RAII связывает время жизни ресурса с временем жизни объекта:
 * - Ресурс захватывается в конструкторе
 * - Ресурс освобождается в деструкторе
 * - Гарантирует освобождение ресурсов даже при исключениях
 *
 * Применяется для:
 * - Управления памятью (умные указатели)
 * - Работы с файлами
 * - Сетевых соединений
 * - Блокировок мьютексов
 */

// Плохая реализация - ресурс не управляется автоматически
class BadResourceManager
{
  private:
	int* data;

  public:
	BadResourceManager(int size)
	{
		data = new int[size];
		cout << "BadResourceManager: allocated " << size << " integers" << endl;
	}

	// Проблема: если между new и delete выбросится исключение - утечка памяти
	void problematic_operation()
	{
		int* temp = new int[100];
		// Если здесь исключение - утечка temp
		delete[] temp;
	}
};

// Хорошая реализация с RAII
class GoodResourceManager
{
  private:
	vector< int > data;

  public:
	GoodResourceManager(size_t size) : data(size)
	{
		cout << "GoodResourceManager: allocated " << size << " integers" << endl;
	}

	// Безопасно - vector сам управляет памятью
	void safe_operation()
	{
		vector< int > temp(100);
		// Даже при исключении память temp будет освобождена
	}

	// Деструктор не нужен - vector сам очистится
};

// RAII для файлового дескриптора (упрощенный пример)
class FileRAII
{
  private:
	FILE* file;

  public:
	FileRAII(const char* filename, const char* mode)
	{
		file = fopen(filename, mode);
		if (!file)
		{
			throw runtime_error("Cannot open file");
		}
		cout << "File opened: " << filename << endl;
	}

	~FileRAII()
	{
		if (file)
		{
			fclose(file);
			cout << "File closed" << endl;
		}
	}

	// Запрещаем копирование
	FileRAII(const FileRAII&) = delete;
	FileRAII& operator=(const FileRAII&) = delete;

	FILE* get() const { return file; }
};

void demo_raii()
{
	cout << "\n=== Демонстрация RAII ===" << endl;

	// Пример с исключением внутри RAII
	try
	{
		GoodResourceManager manager(100);
		throw runtime_error("Something went wrong");
		// Деструктор manager вызовется автоматически, даже при исключении
	} catch (const exception& e)
	{
		cout << "Exception caught, but resources were properly freed: " << e.what() << endl;
	}

	// Пример с файлом
	try
	{
		FileRAII file("test.txt", "w");
		// Работа с файлом...
		throw runtime_error("File operation failed");
		// Файл закроется автоматически в деструкторе
	} catch (const exception& e)
	{
		cout << "File exception: " << e.what() << endl;
	}
}

// ===== 4. КОНСТРУКТОРЫ ПРЕОБРАЗОВАНИЯ =====

/*
 * ТЕОРИЯ: Конструкторы преобразования и explicit
 *
 * Конструктор с одним параметром создает неявное преобразование:
 * - Позволяет автоматически конвертировать типы
 * - Может приводить к неожиданному поведению
 * - explicit запрещает неявные преобразования
 *
 * Оператор преобразования:
 * - Позволяет конвертировать объект в другой тип
 * - Может быть explicit (C++11)
 */

class StringWrapper
{
  private:
	string data;

  public:
	// Конструктор преобразования (неявный)
	StringWrapper(const char* str) : data(str) { cout << "StringWrapper from const char*: " << data << endl; }

	// explicit конструктор - запрещает неявное преобразование
	explicit StringWrapper(int number) : data(to_string(number)) { cout << "StringWrapper from int: " << data << endl; }

	// Оператор преобразования в string
	operator string() const
	{
		cout << "Converting to string: " << data << endl;
		return data;
	}

	// explicit оператор преобразования (C++11)
	explicit operator int() const
	{
		cout << "Converting to int: " << data << endl;
		return stoi(data);
	}

	void print() const { cout << "StringWrapper: " << data << endl; }
};

void demo_conversion()
{
	cout << "\n=== Демонстрация преобразований ===" << endl;

	// Неявное преобразование из const char*
	StringWrapper s1 = "Hello";	   // Вызывает StringWrapper(const char*)
	s1.print();

	// Явное преобразование из int
	StringWrapper s2 = StringWrapper(42);	 // Требуется explicit
	s2.print();

	// Неявное преобразование в string
	string str = s1;	// Вызывает operator string()
	cout << "As string: " << str << endl;

	// Явное преобразование в int
	int num = static_cast< int >(s2);	 // Требуется explicit
	cout << "As int: " << num << endl;
}

// ===== 5. ОПЕРАТОРЫ ПРИСВАИВАНИЯ И ПРАВИЛО ТРЕХ =====

/*
 * ТЕОРИЯ: Правило трех (Rule of Three)
 *
 * Если классу нужен один из следующих методов, то обычно нужны все три:
 * 1. Деструктор
 * 2. Конструктор копирования
 * 3. Оператор присваивания копированием
 *
 * Причина: если класс управляет ресурсами, все три метода должны быть
 * определены корректно для предотвращения утечек и двойного освобождения.
 */

class StringResource
{
  private:
	char* data;

  public:
	// Конструктор
	StringResource(const char* str = nullptr)
	{
		if (str)
		{
			data = new char[strlen(str) + 1];
			strcpy(data, str);
		}
		else
		{
			data = nullptr;
		}
		cout << "Constructed: " << (data ? data : "null") << endl;
	}

	// Деструктор (1 из правила трех)
	~StringResource()
	{
		cout << "Destructing: " << (data ? data : "null") << endl;
		delete[] data;
	}

	// Конструктор копирования (2 из правила трех)
	StringResource(const StringResource& other)
	{
		if (other.data)
		{
			data = new char[strlen(other.data) + 1];
			strcpy(data, other.data);
		}
		else
		{
			data = nullptr;
		}
		cout << "Copy constructed: " << (data ? data : "null") << endl;
	}

	// Оператор присваивания (3 из правила трех)
	StringResource& operator=(const StringResource& other)
	{
		if (this != &other)
		{					  // Защита от самоприсваивания
			delete[] data;	  // Освобождаем старые данные

			if (other.data)
			{
				data = new char[strlen(other.data) + 1];
				strcpy(data, other.data);
			}
			else
			{
				data = nullptr;
			}
		}
		cout << "Assigned: " << (data ? data : "null") << endl;
		return *this;
	}

	// Move конструктор (C++11) - часть правила пяти
	StringResource(StringResource&& other) noexcept
	{
		data = other.data;
		other.data = nullptr;
		cout << "Move constructed" << endl;
	}

	// Move оператор присваивания (C++11)
	StringResource& operator=(StringResource&& other) noexcept
	{
		if (this != &other)
		{
			delete[] data;
			data = other.data;
			other.data = nullptr;
		}
		cout << "Move assigned" << endl;
		return *this;
	}

	void print() const { cout << "StringResource: " << (data ? data : "null") << endl; }
};

void demo_rule_of_three()
{
	cout << "\n=== Демонстрация правила трех ===" << endl;

	StringResource s1("Hello");
	StringResource s2 = s1;	   // Конструктор копирования
	StringResource s3;
	s3 = s1;	// Оператор присваивания

	s1.print();
	s2.print();
	s3.print();

	// Move семантика
	StringResource s4 = move(s1);	 // Move конструктор
	s4.print();
	s1.print();	   // s1 теперь пустой
}

// ===== 6. ПРИВЕДЕНИЯ ТИПОВ (C++ CASTS) =====

/*
 * ТЕОРИЯ: Приведения типов в C++
 *
 * C++ предоставляет 4 типа кастов для замены C-style cast:
 *
 * 1. static_cast:
 *    - Стандартные преобразования (числа, указатели в иерархии)
 *    - Явные преобразования с проверкой на этапе компиляции
 *
 * 2. const_cast:
 *    - Удаление/добавление const и volatile
 *    - Опасен - может привести к неопределенному поведению
 *
 * 3. reinterpret_cast:
 *    - Низкоуровневые побитовые преобразования
 *    - Очень опасен - ломает систему типов
 *
 * 4. dynamic_cast:
 *    - Безопасное приведение в иерархии классов
 *    - Требует полиморфные типы (виртуальные функции)
 *    - Возвращает nullptr при неудаче для указателей
 */

class Base
{
  public:
	virtual ~Base() = default;
	virtual void speak() const { cout << "Base" << endl; }
};

class Derived : public Base
{
  public:
	void speak() const override { cout << "Derived" << endl; }
	void special_method() const { cout << "Special method" << endl; }
};

class Unrelated
{
};

void demo_casts()
{
	cout << "\n=== Демонстрация приведений типов ===" << endl;

	// === static_cast ===
	cout << "--- static_cast ---" << endl;
	double d = 3.14;
	int i = static_cast< int >(d);	  // Явное преобразование double -> int
	cout << "double " << d << " -> int " << i << endl;

	Base* base_ptr = new Derived();
	Derived* derived_ptr = static_cast< Derived* >(base_ptr);	 // Вниз по иерархии
	derived_ptr->speak();

	// === const_cast ===
	cout << "--- const_cast ---" << endl;
	const string const_str = "Hello";
	// string& ref = const_str;  // Ошибка компиляции
	string& ref = const_cast< string& >(const_str);	   // Убираем const
	ref = "Modified";
	cout << "After const_cast: " << const_str << endl;	  // Опасно!

	// === reinterpret_cast ===
	cout << "--- reinterpret_cast ---" << endl;
	int number = 42;
	void* void_ptr = reinterpret_cast< void* >(&number);
	int* int_ptr = reinterpret_cast< int* >(void_ptr);
	cout << "reinterpret_cast: " << *int_ptr << endl;

	// === dynamic_cast ===
	cout << "--- dynamic_cast ---" << endl;
	Base* base1 = new Derived();
	Base* base2 = new Base();

	// Успешное приведение
	Derived* derived1 = dynamic_cast< Derived* >(base1);
	if (derived1)
	{
		cout << "dynamic_cast successful: ";
		derived1->special_method();
	}

	// Неудачное приведение
	Derived* derived2 = dynamic_cast< Derived* >(base2);
	if (!derived2)
	{
		cout << "dynamic_cast failed - returned nullptr" << endl;
	}

	delete base1;
	delete base2;
}

// ===== 7. RTTI (RUN-TIME TYPE INFORMATION) =====

/*
 * ТЕОРИЯ: RTTI (Run-Time Type Information)
 *
 * RTTI позволяет получать информацию о типах во время выполнения:
 * - typeid: получение информации о типе
 * - dynamic_cast: безопасное приведение типов
 *
 * Требования:
 * - Классы должны быть полиморфными (иметь виртуальные функции)
 * - Включает некоторый оверхед по памяти и производительности
 * - Может быть отключено в некоторых компиляторах
 */

void demo_rtti()
{
	cout << "\n=== Демонстрация RTTI ===" << endl;

	Base* base_ptr = new Derived();

	// Использование typeid
	cout << "typeid of base_ptr: " << typeid(*base_ptr).name() << endl;
	cout << "typeid of Base: " << typeid(Base).name() << endl;
	cout << "typeid of Derived: " << typeid(Derived).name() << endl;

	// Проверка типов
	if (typeid(*base_ptr) == typeid(Derived))
	{
		cout << "base_ptr actually points to Derived object" << endl;
	}

	// typeid с ссылками
	try
	{
		Base& base_ref = *base_ptr;
		cout << "typeid of reference: " << typeid(base_ref).name() << endl;
	} catch (const bad_typeid& e)
	{
		cout << "bad_typeid exception: " << e.what() << endl;
	}

	delete base_ptr;
}

// ===== ГЛАВНАЯ ФУНКЦИЯ =====

int main()
{
	cout << "ЛЕКЦИЯ: ИСКЛЮЧЕНИЯ И ПРИВЕДЕНИЯ ТИПОВ В C++" << endl;
	cout << "============================================" << endl;

	demo_basic_exceptions();
	demo_noexcept();
	demo_raii();
	demo_conversion();
	demo_rule_of_three();
	demo_casts();
	demo_rtti();

	cout << "\n============================================" << endl;
	cout << "ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА" << endl;

	return 0;
}

/*
 * ДОПОЛНИТЕЛЬНАЯ ТЕОРИЯ:
 *
 * 1. Исключения и производительность:
 *    - В 32-битных системах исключения могут замедлять работу
 *    - В 64-битных системах оверхед минимален
 *    - Компиляторы хуже оптимизируют код с исключениями
 *
 * 2. Best practices для исключений:
 *    - Бросайте по значению, ловите по константной ссылке
 *    - Наследуйте пользовательские исключения от std::exception
 *    - Используйте RAII для управления ресурсами
 *    - Документируйте, какие исключения бросают функции
 *
 * 3. Правило пяти (C++11):
 *    - Добавляет move конструктор и move оператор присваивания
 *    - Если класс управляет ресурсами, нужно определить все пять методов
 *
 * 4. Smart pointers и RAII:
 *    - unique_ptr: исключительное владение
 *    - shared_ptr: разделяемое владение
 *    - weak_ptr: невладеющая ссылка
 *
 * 5. Exception safety гарантии:
 *    - No-throw guarantee: функция никогда не бросает исключения
 *    - Strong guarantee: при исключении состояние программы не меняется
 *    - Basic guarantee: при исключении сохраняется инвариант
 *    - No guarantee: нет никаких гарантий
 */
