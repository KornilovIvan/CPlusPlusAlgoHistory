/*
 * Лекция по namespace и перегрузке операторов в C++
 *
 * Содержание:
 * 1. Namespace (пространства имен) - теория и практика
 * 2. Выделение памяти: new/delete vs malloc/free
 * 3. Placement new и явный вызов деструкторов
 * 4. Перегрузка операторов - полная теория
 * 5. Основы Qt: сигналы и слоты
 */

#include <cstdlib>	  // для malloc/free
#include <iostream>
#include <new>	  // для placement new
#include <string>
#include <vector>

using namespace std;

// ===== 1. NAMESPACE (ПРОСТРАНСТВА ИМЕН) =====

/*
 * ТЕОРИЯ: Namespace (пространство имен)
 *
 * Namespace - это область видимости, которая содержит объявления идентификаторов
 * (переменных, функций, классов и т.д.). Основные цели:
 * - Предотвращение конфликтов имен
 * - Организация кода в логические группы
 * - Сокрытие реализации
 *
 * Свойства:
 * - Могут быть вложенными
 * - Могут расширяться в разных частях кода
 * - Могут быть анонимными (аналог static)
 * - Поддерживают using-директивы и using-объявления
 */

namespace ABC
{
	// Функция в namespace
	int f()
	{
		return 5;
	}

	// Переменная в namespace
	float x = 3.14f;

	// Дополнительные объявления можно добавлять позже
	int z = 100;

	// Вложенный namespace
	namespace Internal
	{
		float internal_var = 2.71f;

		void internal_func()
		{
			cout << "Внутренняя функция" << endl;
		}
	}	 // namespace Internal

	// Класс в namespace
	class Calculator
	{
	  public:
		static int add(int a, int b) { return a + b; }
	};
}	 // namespace ABC

// Глобальная переменная
int global_var = 10;

void demo_namespace_basic()
{
	cout << "=== Демонстрация основных возможностей namespace ===" << endl;

	// Доступ к элементам namespace через оператор ::
	cout << "ABC::f() = " << ABC::f() << endl;
	cout << "ABC::x = " << ABC::x << endl;
	cout << "ABC::z = " << ABC::z << endl;

	// Доступ к вложенному namespace
	cout << "ABC::Internal::internal_var = " << ABC::Internal::internal_var << endl;
	ABC::Internal::internal_func();

	// Использование класса из namespace
	cout << "ABC::Calculator::add(5, 3) = " << ABC::Calculator::add(5, 3) << endl;
}

void demo_namespace_advanced()
{
	cout << "\n=== Демонстрация продвинутых возможностей namespace ===" << endl;

	// Using-объявление (только для конкретного идентификатора)
	using ABC::f;
	cout << "После using ABC::f: f() = " << f() << endl;

	// Using-директива (все идентификаторы из namespace)
	using namespace ABC;
	cout << "После using namespace ABC: x = " << x << endl;

	// Разрешение конфликта имен с глобальной областью видимости
	int global_var = 20;	// локальная переменная
	cout << "Локальная global_var = " << global_var << endl;
	cout << "Глобальная global_var = " << ::global_var << endl;

	// Доступ к глобальному namespace через ::
	::global_var = 30;
	cout << "Измененная глобальная global_var = " << ::global_var << endl;
}

// Анонимный namespace - аналогичен static в C
namespace
{
	int anonymous_counter = 0;

	void increment_counter()
	{
		anonymous_counter++;
	}
}	 // namespace

void demo_anonymous_namespace()
{
	cout << "\n=== Демонстрация анонимного namespace ===" << endl;

	cout << "Начальное значение anonymous_counter = " << anonymous_counter << endl;
	increment_counter();
	cout << "После increment_counter() = " << anonymous_counter << endl;

	// Анонимный namespace обеспечивает внутреннюю линковку
	// Эквивалентно static в C, но более современно в C++
}

// ===== 2. ВЫДЕЛЕНИЕ ПАМЯТИ: NEW/DELETE VS MALLOC/FREE =====

/*
 * ТЕОРИЯ: new/delete vs malloc/free
 *
 * Основные различия:
 *
 * malloc/free (C стиль):
 * - Выделяет/освобождает сырую память
 * - Не вызывает конструкторы/деструкторы
 * - Возвращает void*, требуется приведение типов
 * - Может быть переопределен через realloc
 *
 * new/delete (C++ стиль):
 * - Выделяет память И вызывает конструктор
 * - Освобождает память И вызывает деструктор
 * - Возвращает указатель правильного типа
 * - Поддерживает обработку исключений
 * - Имеет формы для массивов: new[]/delete[]
 *
 * Критические отличия:
 * - new бросает std::bad_alloc при ошибке (или возвращает nullptr с nothrow)
 * - malloc возвращает nullptr при ошибке
 * - delete знает размер удаляемого объекта (не нужно передавать размер)
 */

class Resource
{
  private:
	int id;
	string name;

  public:
	Resource(int i, const string& n) : id(i), name(n)
	{
		cout << "Конструктор Resource(" << id << ", " << name << ")" << endl;
	}

	~Resource() { cout << "Деструктор Resource(" << id << ", " << name << ")" << endl; }

	void display() const { cout << "Resource: id=" << id << ", name=" << name << endl; }
};

void demo_memory_management()
{
	cout << "\n=== Демонстрация управления памятью ===" << endl;

	// ===== C++ STYLE: new/delete =====
	cout << "\n--- C++ style: new/delete ---" << endl;

	// Одиночный объект
	Resource* res1 = new Resource(1, "Одиночный");
	res1->display();
	delete res1;

	// Массив объектов
	Resource* res_array = new Resource[3]{ Resource(2, "Первый"), Resource(3, "Второй"), Resource(4, "Третий") };
	delete[] res_array;

	// new с nothrow (не бросает исключение, возвращает nullptr при ошибке)
	Resource* res_nothrow = new (nothrow) Resource(5, "Nothrow");
	if (res_nothrow != nullptr)
	{
		res_nothrow->display();
		delete res_nothrow;
	}

	// ===== C STYLE: malloc/free =====
	cout << "\n--- C style: malloc/free ---" << endl;

	// malloc не вызывает конструктор!
	Resource* res_malloc = static_cast< Resource* >(malloc(sizeof(Resource)));
	cout << "После malloc (конструктор не вызван)" << endl;

	// Явный вызов конструктора через placement new
	new (res_malloc) Resource(6, "Malloc+Placement");
	res_malloc->display();

	// Явный вызов деструктора
	res_malloc->~Resource();
	free(res_malloc);
}

// ===== 3. PLACEMENT NEW И ЯВНЫЙ ВЫЗОВ ДЕСТРУКТОРОВ =====

/*
 * ТЕОРИЯ: Placement new
 *
 * Placement new - это особая форма оператора new, которая:
 * - Не выделяет новую память
 * - Конструирует объект в уже выделенной памяти
 * - Полезен для:
 *   - Пулов объектов
 *   - Разделяемой памяти
 *   - Кастомных аллокаторов
 *   - Оптимизации производительности
 *
 * Синтаксис: new(ptr) Type(args...)
 * Деструктор нужно вызывать явно: ptr->~Type()
 */

void demo_placement_new()
{
	cout << "\n=== Демонстрация Placement new ===" << endl;

	// Способ 1: Выделение памяти через malloc
	cout << "\n--- Способ 1: malloc + placement new ---" << endl;
	void* raw_memory = malloc(sizeof(Resource));
	Resource* res1 = new (raw_memory) Resource(10, "Placement1");
	res1->display();
	res1->~Resource();
	free(raw_memory);

	// Способ 2: Использование стека (массив char)
	cout << "\n--- Способ 2: стек + placement new ---" << endl;
	char buffer[sizeof(Resource)];
	Resource* res2 = new (buffer) Resource(11, "Placement2");
	res2->display();
	res2->~Resource();

	// Способ 3: Массив объектов
	cout << "\n--- Способ 3: массив объектов ---" << endl;
	const int COUNT = 3;
	char array_buffer[sizeof(Resource) * COUNT];
	Resource* resources[COUNT];

	// Конструирование объектов в массиве
	for (int i = 0; i < COUNT; ++i)
	{
		resources[i] = new (array_buffer + i * sizeof(Resource)) Resource(20 + i, "ArrayObj_" + to_string(i));
	}

	// Использование объектов
	for (int i = 0; i < COUNT; ++i)
	{
		resources[i]->display();
	}

	// Уничтожение объектов (в обратном порядке - хорошая практика)
	for (int i = COUNT - 1; i >= 0; --i)
	{
		resources[i]->~Resource();
	}
}

// ===== 4. ПЕРЕГРУЗКА ОПЕРАТОРОВ =====

/*
 * ТЕОРИЯ: Перегрузка операторов
 *
 * Перегрузка операторов позволяет определить поведение операторов
 * для пользовательских типов. Основные правила:
 *
 * 1. Нельзя перегружать:
 *    - :: (разрешение области видимости)
 *    - . (доступ к члену)
 *    - .* (доступ к члену по указателю)
 *    - ?: (тернарный оператор)
 *    - sizeof, typeid
 *    - alignof
 *    - throw
 *
 * 2. Можно перегружать большинство операторов
 *
 * 3. Нельзя создавать новые операторы
 *
 * 4. Нельзя изменить приоритет и ассоциативность
 *
 * 5. Хотя бы один параметр должен быть пользовательского типа
 *
 * Формы перегрузки:
 * - Член класса (неявный this как первый параметр)
 * - Внешняя функция (все параметры явные)
 */

class Vector
{
  private:
	double x, y;

  public:
	Vector(double x_val = 0, double y_val = 0) : x(x_val), y(y_val) {}

	// ===== ГЕТТЕРЫ =====
	double getX() const { return x; }
	double getY() const { return y; }

	// ===== УНАРНЫЕ ОПЕРАТОРЫ (члены класса) =====

	// Унарный минус (префиксный)
	Vector operator-() const { return Vector(-x, -y); }

	// Префиксный инкремент
	Vector& operator++()
	{
		++x;
		++y;
		return *this;
	}

	// Постфиксный инкремент (фиктивный параметр int)
	Vector operator++(int)
	{
		Vector temp = *this;
		++(*this);
		return temp;
	}

	// ===== БИНАРНЫЕ ОПЕРАТОРЫ (члены класса) =====

	// Сложение векторов
	Vector operator+(const Vector& other) const { return Vector(x + other.x, y + other.y); }

	// Вычитание векторов
	Vector operator-(const Vector& other) const { return Vector(x - other.x, y - other.y); }

	// Скалярное произведение
	double operator*(const Vector& other) const { return x * other.x + y * other.y; }

	// Умножение на скаляр
	Vector operator*(double scalar) const { return Vector(x * scalar, y * scalar); }

	// Составное присваивание +=
	Vector& operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	// Составное присваивание -=
	Vector& operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	// Оператор вывода в поток (должен быть дружественной функцией)
	friend ostream& operator<<(ostream& os, const Vector& v);

	// Оператор ввода из потока (должен быть дружественной функцией)
	friend istream& operator>>(istream& is, Vector& v);
};

// ===== ВНЕШНИЕ ОПЕРАТОРЫ =====

// Умножение скаляра на вектор (скаляр слева)
Vector operator*(double scalar, const Vector& v)
{
	return v * scalar;	  // используем член-функцию
}

// Оператор вывода в поток
ostream& operator<<(ostream& os, const Vector& v)
{
	os << "Vector(" << v.x << ", " << v.y << ")";
	return os;
}

// Оператор ввода из потока
istream& operator>>(istream& is, Vector& v)
{
	cout << "Введите x и y: ";
	is >> v.x >> v.y;
	return is;
}

void demo_operator_overloading()
{
	cout << "\n=== Демонстрация перегрузки операторов ===" << endl;

	Vector v1(2, 3);
	Vector v2(1, 4);

	cout << "v1 = " << v1 << endl;
	cout << "v2 = " << v2 << endl;

	// Унарные операторы
	cout << "-v1 = " << -v1 << endl;
	cout << "++v1 = " << ++v1 << endl;
	cout << "v1++ = " << v1++ << endl;
	cout << "После v1++: v1 = " << v1 << endl;

	// Бинарные операторы
	cout << "v1 + v2 = " << (v1 + v2) << endl;
	cout << "v1 - v2 = " << (v1 - v2) << endl;
	cout << "v1 * v2 = " << (v1 * v2) << endl;
	cout << "v1 * 2.5 = " << (v1 * 2.5) << endl;
	cout << "2.5 * v1 = " << (2.5 * v1) << endl;

	// Составные присваивания
	v1 += v2;
	cout << "После v1 += v2: v1 = " << v1 << endl;

	v1 -= v2;
	cout << "После v1 -= v2: v1 = " << v1 << endl;
}

// ===== 5. ОСНОВЫ QT: СИГНАЛЫ И СЛОТЫ =====

/*
 * ТЕОРИЯ: Сигналы и слоты (Qt framework)
 *
 * Сигналы и слоты - это механизм связи между объектами в Qt:
 *
 * Сигналы (signals):
 * - Объявляются в секции signals
 * - Не реализуются разработчиком (генерируются MOC)
 * - Испускаются с помощью ключевого слова emit
 * - Могут иметь параметры
 *
 * Слоты (slots):
 * - Обычные функции-члены
 * - Могут быть публичными, защищенными или приватными
 * - Вызываются при получении сигнала
 * - Могут принимать параметры из сигнала
 *
 * Соединения (connections):
 * - Устанавливаются с помощью QObject::connect()
 * - Могут быть автоматическими (auto) или прямыми (direct)
 * - Поддерживают лямбда-выражения (новый синтаксис)
 *
 * Требования:
 * - Класс должен наследоваться от QObject
 * - В начале класса должен быть макрос Q_OBJECT
 * - Класс должен быть обработан MOC (Meta-Object Compiler)
 */

// Имитация Qt классов для демонстрации
class QObject
{
  public:
	virtual ~QObject() = default;
};

// Макрос для совместимости с демонстрацией
#define Q_OBJECT
#define signals public
#define slots public
#define emit

class Number : public QObject
{
	Q_OBJECT

  private:
	int value;

  public:
	Number(int v = 0) : value(v) {}

	// Сигналы (в реальном Qt объявляются в секции signals)
	void valueChanged(int newValue) { cout << "Сигнал: значение изменено на " << newValue << endl; }

	// Слоты
	void setValue(int newValue)
	{
		if (value != newValue)
		{
			value = newValue;
			cout << "Слот setValue: установлено значение " << value << endl;
			emit valueChanged(value);	 // Испускаем сигнал
		}
	}

	void printValue() { cout << "Слот printValue: текущее значение = " << value << endl; }

	int getValue() const { return value; }
};

// Имитация QObject::connect для демонстрации
template< typename Func1, typename Func2 >
void connect(Number* sender, Func1 signal, Number* receiver, Func2 slot)
{
	// В реальном Qt здесь сложная логика соединения
	cout << "Установлено соединение между объектами" << endl;
}

void demo_qt_signals_slots()
{
	cout << "\n=== Демонстрация сигналов и слотов (упрощенная) ===" << endl;

	Number num1, num2;

	cout << "Созданы два объекта Number" << endl;

	// Установка значений (вызовет сигналы)
	cout << "\n--- Установка значения num1 ---" << endl;
	num1.setValue(10);

	cout << "\n--- Установка значения num2 ---" << endl;
	num2.setValue(20);

	// Вызов слотов напрямую
	cout << "\n--- Прямой вызов слотов ---" << endl;
	num1.printValue();
	num2.printValue();
}
