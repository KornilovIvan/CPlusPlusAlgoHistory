/*
 * Лекция по концептам C++20 и тестированию
 *
 * Содержание:
 * 1. Концепты (Concepts) - теория и практика
 * 2. Требования к контейнерам C++
 * 3. Реализация кастомного контейнера с концептами
 * 4. Google Test framework
 */

#include <type_traits>

#include <gtest/gtest.h>

#include <algorithm>
#include <concepts>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <vector>

using namespace std;

// ===== 1. КОНЦЕПТЫ (CONCEPTS) =====

/*
 * ТЕОРИЯ: Концепты в C++20
 *
 * Концепты - это механизм для указания требований к шаблонным параметрам:
 * - Обеспечивают проверку типов на этапе компиляции
 * - Улучшают сообщения об ошибках
 * - Делают код более читаемым и безопасным
 *
 * Основные компоненты:
 * - concept: объявление концепта
 * - requires: выражение требований
 * - requires clause: ограничение шаблона
 */

// Базовые концепты из стандартной библиотеки
void demo_std_concepts()
{
	cout << "=== Демонстрация стандартных концептов ===" << endl;

	// std::integral - проверяет целочисленный тип
	static_assert(std::integral< int >);
	static_assert(std::integral< long >);
	static_assert(!std::integral< double >);

	// std::floating_point - проверяет тип с плавающей точкой
	static_assert(std::floating_point< double >);
	static_assert(std::floating_point< float >);
	static_assert(!std::floating_point< int >);

	// std::same_as - проверяет точное совпадение типов
	static_assert(std::same_as< int, int >);
	static_assert(!std::same_as< int, double >);

	// std::convertible_to - проверяет возможность преобразования
	static_assert(std::convertible_to< int, double >);
	static_assert(std::convertible_to< double, int >);	  // С потерей точности

	cout << "Все стандартные концепты проверены успешно!" << endl;
}

// Пользовательские концепты
template< typename T >
concept HasSize = requires(T obj) {
	{ obj.size() } -> std::convertible_to< size_t >;
};

template< typename T >
concept HasValueType = requires { typename T::value_type; };

template< typename T >
concept HasIterator = requires(T obj) {
	typename T::iterator;
	typename T::const_iterator;
	{ obj.begin() } -> std::same_as< typename T::iterator >;
	{ obj.end() } -> std::same_as< typename T::iterator >;
};

template< typename T >
concept Container = HasSize< T > && HasValueType< T > && HasIterator< T >;

template< typename T >
concept Printable = requires(T obj, ostream& os) { os << obj; };

// Концепт для четных целых чисел
template< typename T >
concept EvenIntegral = std::integral< T > && requires(T a) {
	requires(a % 2 == 0);	 // Проверка на четность
};

// Более практичный концепт для работы с числами
template< typename T >
concept Arithmetic = std::integral< T > || std::floating_point< T >;

// Функции с концептами
template< HasSize T >
void print_size(const T& container)
{
	cout << "Size: " << container.size() << endl;
}

template< Container T >
void print_container_info(const T& container)
{
	cout << "Container with value_type: " << typeid(typename T::value_type).name() << ", size: " << container.size() << endl;
}

template< Printable T >
void print_value(const T& value)
{
	cout << "Value: " << value << endl;
}

template< Arithmetic T >
T square(T x)
{
	return x * x;
}

void demo_custom_concepts()
{
	cout << "\n=== Демонстрация пользовательских концептов ===" << endl;

	vector< int > vec = { 1, 2, 3 };
	list< string > lst = { "hello", "world" };

	// Проверка концептов
	static_assert(HasSize< vector< int > >);
	static_assert(Container< vector< int > >);
	static_assert(Printable< string >);
	static_assert(Arithmetic< int >);
	static_assert(Arithmetic< double >);

	// Использование функций с концептами
	print_size(vec);
	print_size(lst);

	print_container_info(vec);
	print_container_info(lst);

	print_value("Hello Concepts!");
	print_value(42);

	cout << "square(5): " << square(5) << endl;
	cout << "square(2.5): " << square(2.5) << endl;

	// Ошибка компиляции - не удовлетворяет концепту
	// struct NotPrintable {};
	// NotPrintable np;
	// print_value(np); // Ошибка: не удовлетворяет концепту Printable
}

// ===== 2. ТРЕБОВАНИЯ К КОНТЕЙНЕРАМ C++ =====

/*
 * ТЕОРИЯ: Требования к контейнерам C++
 *
 * Контейнер должен предоставлять следующие типы и методы:
 *
 * Обязательные типы:
 * - value_type, reference, const_reference
 * - iterator, const_iterator
 * - difference_type, size_type
 *
 * Обязательные методы:
 * - begin(), end(), cbegin(), cend()
 * - size(), empty()
 * - Конструкторы: по умолчанию, копирования, перемещения
 * - Операторы присваивания: копирования, перемещения
 *
 * Дополнительные требования для разных категорий контейнеров:
 * - SequenceContainer (vector, list, deque): push_back, pop_back, etc.
 * - AssociativeContainer (map, set): find, count, etc.
 */

// Концепт для проверки требований контейнера
template< typename C >
concept StandardContainer = requires {
	typename C::value_type;
	typename C::reference;
	typename C::const_reference;
	typename C::iterator;
	typename C::const_iterator;
	typename C::difference_type;
	typename C::size_type;
} && requires(C c, const C cc, typename C::size_type n) {
	{ c.begin() } -> std::same_as< typename C::iterator >;
	{ c.end() } -> std::same_as< typename C::iterator >;
	{ cc.begin() } -> std::same_as< typename C::const_iterator >;
	{ cc.end() } -> std::same_as< typename C::const_iterator >;
	{ c.size() } -> std::same_as< typename C::size_type >;
	{ c.empty() } -> std::same_as< bool >;
};

void demo_container_requirements()
{
	cout << "\n=== Демонстрация требований к контейнерам ===" << endl;

	// Проверка стандартных контейнеров
	static_assert(StandardContainer< vector< int > >);
	static_assert(StandardContainer< list< double > >);
	static_assert(StandardContainer< map< int, string > >);

	vector< int > vec = { 1, 2, 3, 4, 5 };

	cout << "vector<int> удовлетворяет StandardContainer: " << StandardContainer< decltype(vec) > << endl;

	cout << "Типы vector<int>:" << endl;
	cout << "  value_type: " << typeid(vector< int >::value_type).name() << endl;
	cout << "  iterator: " << typeid(vector< int >::iterator).name() << endl;
	cout << "  size_type: " << typeid(vector< int >::size_type).name() << endl;
	cout << "  difference_type: " << typeid(vector< int >::difference_type).name() << endl;

	cout << "Методы:" << endl;
	cout << "  size(): " << vec.size() << endl;
	cout << "  empty(): " << vec.empty() << endl;
}

// ===== 3. РЕАЛИЗАЦИЯ КАСТОМНОГО КОНТЕЙНЕРА С КОНЦЕПТАМИ =====

/*
 * ТЕОРИЯ: Кастомный контейнер с концептами
 *
 * Реализуем контейнер для четных чисел с использованием концептов:
 * - Проверка типов на этапе компиляции
 * - Гарантии безопасности типов
 * - Явное указание требований
 */

template< typename T >
	requires std::integral< T >
class EvenNumberContainer
{
  private:
	T* m_data = nullptr;
	size_t m_size = 0;
	size_t m_capacity = 0;

  public:
	// Типы, требуемые для контейнера
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using iterator = T*;
	using const_iterator = const T*;
	using difference_type = ptrdiff_t;
	using size_type = size_t;

	// Конструкторы
	EvenNumberContainer() = default;

	// Конструктор копирования
	EvenNumberContainer(const EvenNumberContainer& other) :
		m_data(nullptr), m_size(other.m_size), m_capacity(other.m_capacity)
	{
		if (m_capacity > 0)
		{
			m_data = new T[m_capacity];
			memcpy(m_data, other.m_data, sizeof(T) * m_size);
		}
	}

	// Оператор копирующего присваивания (copy-and-swap идиома)
	EvenNumberContainer& operator=(const EvenNumberContainer& other)
	{
		if (this != &other)
		{
			EvenNumberContainer tmp(other);
			tmp.swap(*this);
		}
		return *this;
	}

	// Явно запрещаем перемещение (для демонстрации)
	EvenNumberContainer(EvenNumberContainer&&) = delete;
	EvenNumberContainer& operator=(EvenNumberContainer&&) = delete;

	// Деструктор
	~EvenNumberContainer() noexcept
	{
		if (m_data != nullptr)
		{
			delete[] m_data;
		}
	}

	// Метод для обмена
	void swap(EvenNumberContainer& other) noexcept
	{
		std::swap(m_data, other.m_data);
		std::swap(m_size, other.m_size);
		std::swap(m_capacity, other.m_capacity);
	}

	// Добавление элемента с проверкой на четность
	void push_back(T value)
		requires(value % 2 == 0)
	{
		if (m_size >= m_capacity)
		{
			resize(m_capacity == 0 ? 4 : m_capacity * 2);
		}
		m_data[m_size++] = value;
	}

	// Доступ к элементам
	reference operator[](size_type index) { return m_data[index]; }

	const_reference operator[](size_type index) const { return m_data[index]; }

	// Итераторы
	iterator begin() { return m_data; }
	iterator end() { return m_data + m_size; }
	const_iterator begin() const { return m_data; }
	const_iterator end() const { return m_data + m_size; }
	const_iterator cbegin() const { return m_data; }
	const_iterator cend() const { return m_data + m_size; }

	// Информация о размере
	size_type size() const { return m_size; }
	size_type capacity() const { return m_capacity; }
	bool empty() const { return m_size == 0; }

	// Вспомогательные методы
	void clear() { m_size = 0; }

  private:
	void resize(size_type new_capacity)
	{
		T* new_data = new T[new_capacity];
		if (m_data)
		{
			memcpy(new_data, m_data, sizeof(T) * m_size);
			delete[] m_data;
		}
		m_data = new_data;
		m_capacity = new_capacity;
	}
};

// Специализация для концепта StandardContainer
template< typename T >
struct StandardContainer< EvenNumberContainer< T > > : std::true_type
{
};

void demo_custom_container()
{
	cout << "\n=== Демонстрация кастомного контейнера ===" << endl;

	EvenNumberContainer< int > container;

	// Добавление четных чисел
	container.push_back(2);
	container.push_back(4);
	container.push_back(6);
	container.push_back(8);

	// Ошибка компиляции - нечетное число
	// container.push_back(3); // Error: constraint not satisfied

	cout << "EvenNumberContainer contents: ";
	for (auto it = container.begin(); it != container.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;

	cout << "Size: " << container.size() << endl;
	cout << "Capacity: " << container.capacity() << endl;
	cout << "Empty: " << container.empty() << endl;

	// Проверка концепта
	static_assert(StandardContainer< EvenNumberContainer< int > >);
	cout << "EvenNumberContainer удовлетворяет StandardContainer" << endl;
}

// ===== 4. GOOGLE TEST FRAMEWORK =====

/*
 * ТЕОРИЯ: Google Test Framework
 *
 * Google Test - популярный фреймворк для модульного тестирования:
 * - Макросы для assertions: EXPECT_, ASSERT_
 * - Фикстуры для настройки тестового окружения
 * - Параметризованные тесты
 * - Mock объекты
 */

// Простые тесты
TEST(EvenNumberContainerTest, BasicFunctionality)
{
	EvenNumberContainer< int > container;

	EXPECT_TRUE(container.empty());
	EXPECT_EQ(container.size(), 0);

	container.push_back(2);
	container.push_back(4);

	EXPECT_FALSE(container.empty());
	EXPECT_EQ(container.size(), 2);
	EXPECT_EQ(container[0], 2);
	EXPECT_EQ(container[1], 4);
}

TEST(EvenNumberContainerTest, Iteration)
{
	EvenNumberContainer< int > container;
	container.push_back(10);
	container.push_back(20);
	container.push_back(30);

	int sum = 0;
	for (auto num : container)
	{
		sum += num;
	}

	EXPECT_EQ(sum, 60);
}

TEST(EvenNumberContainerTest, CopySemantics)
{
	EvenNumberContainer< int > container1;
	container1.push_back(2);
	container1.push_back(4);

	// Конструктор копирования
	EvenNumberContainer< int > container2(container1);
	EXPECT_EQ(container2.size(), 2);
	EXPECT_EQ(container2[0], 2);
	EXPECT_EQ(container2[1], 4);

	// Оператор присваивания
	EvenNumberContainer< int > container3;
	container3 = container1;
	EXPECT_EQ(container3.size(), 2);
}

// Тестовый класс для демонстрации концептов
template< Arithmetic T >
class Calculator
{
  public:
	static T add(T a, T b) { return a + b; }
	static T multiply(T a, T b) { return a * b; }
};

TEST(ConceptsTest, ArithmeticOperations)
{
	EXPECT_EQ(Calculator< int >::add(2, 3), 5);
	EXPECT_EQ(Calculator< double >::multiply(2.5, 2.0), 5.0);
	EXPECT_EQ(Calculator< float >::add(1.5f, 2.5f), 4.0f);
}

// Фикстура для тестов
class ContainerFixture : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		container.push_back(2);
		container.push_back(4);
		container.push_back(6);
	}

	void TearDown() override { container.clear(); }

	EvenNumberContainer< int > container;
};

TEST_F(ContainerFixture, FixtureTest)
{
	EXPECT_EQ(container.size(), 3);
	EXPECT_FALSE(container.empty());

	container.push_back(8);
	EXPECT_EQ(container.size(), 4);
}

// ===== 5. ПРАКТИЧЕСКИЕ ПРИМЕРЫ С КОНЦЕПТАМИ =====

// Концепт для сортировки
template< typename Container >
concept SortableContainer = Container< Container > && requires(Container c) {
	requires std::is_swappable_v< typename Container::value_type >;
	requires std::less< typename Container::value_type > {};
};

// Обобщенная функция сортировки с концептами
template< SortableContainer Container >
void custom_sort(Container& container)
{
	std::sort(container.begin(), container.end());
}

// Концепт для математических операций
template< typename T >
concept MathOperations = requires(T a, T b) {
	{ a + b } -> std::same_as< T >;
	{ a - b } -> std::same_as< T >;
	{ a * b } -> std::same_as< T >;
	{ a / b } -> std::same_as< T >;
};

// Функция с математическими операциями
template< MathOperations T >
T compute_expression(T a, T b, T c)
{
	return (a + b) * c - (b / a);
}

void demo_advanced_concepts()
{
	cout << "\n=== Демонстрация продвинутых концептов ===" << endl;

	vector< int > numbers = { 5, 2, 8, 1, 9 };
	custom_sort(numbers);

	cout << "Sorted numbers: ";
	for (auto num : numbers)
	{
		cout << num << " ";
	}
	cout << endl;

	// Математические операции
	cout << "compute_expression(10, 5, 2): " << compute_expression(10, 5, 2) << endl;
	cout << "compute_expression(5.5, 2.2, 3.0): " << compute_expression(5.5, 2.2, 3.0) << endl;
}

/*
 * ДОПОЛНИТЕЛЬНАЯ ТЕОРИЯ:
 *
 * 1. Requires Expressions:
 *    - Простые требования: requires { expression; }
 *    - Составные требования: requires { {expression} -> concept; }
 *    - Требования типов: requires { typename T::type; }
 *
 * 2. Constraints и SFINAE:
 *    - Концепты заменяют сложные SFINAE конструкции
 *    - Более читаемые сообщения об ошибках
 *    - Лучшая производительность компиляции
 *
 * 3. Стандартные концепты:
 *    - <concepts> header содержит множество готовых концептов
 *    - ranges концепты для работы с диапазонами
 *    - iterator концепты для работы с итераторами
 *
 * 4. Best Practices:
 *    - Используйте стандартные концепты когда возможно
 *    - Создавайте конкретные и понятные концепты
 *    - Комбинируйте концепты с помощью && и ||
 *    - Документируйте требования концептов
 *
 * 5. Testing Best Practices:
 *    - Пишите маленькие и focused тесты
 *    - Используйте фикстуры для общей настройки
 *    - Тестируйте как позитивные, так и негативные сценарии
 *    - Используйте параметризованные тесты для разных входных данных
 */
