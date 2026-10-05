/*
 * Лекция по классам и наследованию в C++
 *
 * Содержание:
 * 1. Константы и constexpr
 * 2. Ключевое слово extern
 * 3. Структуры в C vs C++
 * 4. Методы классов
 * 5. Модификаторы доступа
 * 6. Наследование
 * 7. Виртуальные функции
 * 8. Конструкторы и деструкторы
 * 9. Множественное наследование
 * 10. Friend функции и классы
 */

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

// ===== 1. КОНСТАНТЫ И CONSTEXPR =====

/*
 * ТЕОРИЯ: Константы в C++
 *
 * const (константа времени выполнения):
 * - Значение не может быть изменено после инициализации
 * - Может инициализироваться значением, известным только во время выполнения
 * - Гарантирует неизменяемость объекта
 * - Может использоваться с методами: const методы не могут изменять поля объекта
 *
 * constexpr (константа времени компиляции):
 * - Значение должно быть известно на этапе компиляции
 * - Позволяет производить вычисления на этапе компиляции
 * - Может использоваться в контекстах, требующих константных выражений
 * - Улучшает производительность за счет вычислений во время компиляции
 *
 * Отличия:
 * - constexpr более строгий - требует известности на этапе компиляции
 * - constexpr функции могут выполняться как во время компиляции, так и во время выполнения
 * - constexpr переменные гарантированно инициализируются на этапе компиляции
 */

const int runtime_const = time(nullptr);	// OK - вычисляется во время выполнения
constexpr int compile_const = 42;			// OK - известно при компиляции
// constexpr int bad_const = time(nullptr); // ОШИБКА - time() не constexpr

// constexpr функция - может вычисляться на этапе компиляции
constexpr int factorial(int n)
{
	return (n <= 1) ? 1 : n * factorial(n - 1);
}

constexpr int fact_5 = factorial(5);	// Вычисляется на этапе компиляции

// ===== 2. КЛЮЧЕВОЕ СЛОВО EXTERN =====

/*
 * ТЕОРИЯ: Ключевое слово extern
 *
 * extern используется для:
 * 1. Объявления переменной без ее определения (определение в другом файле)
 * 2. Указания внешнего связывания (external linkage)
 * 3. Объявления функций, определенных в других файлах
 *
 * Особенности:
 * - Переменная с extern не выделяет память, только объявляет существование
 * - Может использоваться для доступа к глобальным переменным между файлами
 * - Функции по умолчанию имеют внешнее связывание, поэтому extern для них часто избыточен
 *
 * Правила использования:
 * - Определение переменной должно быть ровно одно во всей программе
 * - Объявлений extern может быть много
 * - Обычно помещается в заголовочные файлы (.h)
 */

extern int external_function(int, int);	   // Объявление внешней функции
extern int external_var;				   // Объявление внешней переменной

// В другом файле должно быть:
// int external_function(int a, int b) { return a + b; }
// int external_var = 100;

// ===== 3. СТРУКТУРЫ В C VS C++ =====

/*
 * ТЕОРИЯ: Структуры в C и C++
 *
 * В C:
 * - struct создает новый тип, но для использования нужно писать struct Имя
 * - Нет методов, модификаторов доступа, наследования
 * - Обычно используют typedef для удобства
 *
 * В C++:
 * - struct почти идентичен class
 * - Разница: по умолчанию в struct все члены public, в class - private
 * - Может иметь методы, конструкторы, деструкторы, наследование
 * - Поддерживает все возможности ООП
 *
 * Синтаксические различия:
 * C:  struct Point { int x, y; }; struct Point p1;
 * C++: struct Point { int x, y; }; Point p1;
 */

// C++ структура с методами
struct Point
{
	int x, y;

	// Конструктор
	Point(int x = 0, int y = 0) : x(x), y(y) {}

	// Методы
	void print() const { cout << "Point(" << x << ", " << y << ")" << endl; }

	// Операторы
	Point operator+(const Point& other) const { return Point(x + other.x, y + other.y); }
};

// Эмуляция C стиля (для сравнения)
struct C_Point
{
	int x, y;
};

void demo_structures()
{
	cout << "\n=== Демонстрация структур ===" << endl;

	// C++ стиль
	Point p1(1, 2), p2(3, 4);
	p1.print();
	Point p3 = p1 + p2;
	p3.print();

	// C стиль
	struct C_Point cp1 = { 1, 2 };
	cout << "C Point: " << cp1.x << ", " << cp1.y << endl;
}

// ===== 4. МЕТОДЫ КЛАССОВ =====

/*
 * ТЕОРИЯ: Методы классов в C++
 *
 * Типы методов:
 * 1. Обычные методы - работают с конкретным объектом (имеют this)
 * 2. Const методы - гарантируют неизменность объекта
 * 3. Static методы - не имеют this, работают на уровне класса
 * 4. Virtual методы - поддерживают полиморфизм
 * 5. Pure virtual методы - делают класс абстрактным
 *
 * Особенности:
 * - Методы могут быть определены внутри класса (inline) или вне (используя ::)
 * - Const методы могут вызываться на const объектах
 * - Static методы могут вызываться без создания объекта
 * - Виртуальные методы определяют поведение во время выполнения
 */

class Rectangle
{
  private:
	int width, height;

  public:
	// Конструкторы
	Rectangle() : width(0), height(0) {}
	Rectangle(int w, int h) : width(w), height(h) {}

	// Обычный метод
	int area() const { return width * height; }

	// Const метод - гарантирует, что не изменит объект
	void print() const { cout << "Rectangle " << width << "x" << height << endl; }

	// Статический метод - не имеет доступа к полям объекта
	static string className() { return "Rectangle"; }

	// Статическая переменная - общая для всех объектов
	static int object_count;

	// Геттеры
	int getWidth() const { return width; }
	int getHeight() const { return height; }

	// Сеттеры
	void setWidth(int w) { width = w; }
	void setHeight(int h) { height = h; }
};

// Инициализация статической переменной
int Rectangle::object_count = 0;

void demo_methods()
{
	cout << "\n=== Демонстрация методов ===" << endl;

	Rectangle rect(5, 3);
	rect.print();
	cout << "Area: " << rect.area() << endl;
	cout << "Class name: " << Rectangle::className() << endl;

	// Вызов статического метода без объекта
	cout << "Static method call: " << Rectangle::className() << endl;
}

// ===== 5. МОДИФИКАТОРЫ ДОСТУПА =====

/*
 * ТЕОРИЯ: Модификаторы доступа в C++
 *
 * public:
 * - Доступ отовсюду
 * - Интерфейс класса
 *
 * protected:
 * - Доступ из класса и производных классов
 * - Для реализации наследования
 *
 * private:
 * - Доступ только из самого класса
 * - Сокрытие реализации
 *
 * Друзья (friend):
 * - Дают доступ к private/protected членам
 * - Могут быть функции, другие классы
 *
 * Правила:
 * - По умолчанию в class: private, в struct: public
 * - Инкапсуляция - один из столпов ООП
 * - Следует минимизировать public интерфейс
 */

class BankAccount
{
  private:
	double balance;	   // Сокрытие данных - только класс управляет балансом

  protected:
	string accountNumber;	 // Наследники могут использовать номер счета

  public:
	BankAccount(string accNum, double initialBalance) : accountNumber(accNum), balance(initialBalance) {}

	// Публичный интерфейс - контролируемый доступ к приватным данным
	void deposit(double amount)
	{
		if (amount > 0)
			balance += amount;
	}

	bool withdraw(double amount)
	{
		if (amount > 0 && balance >= amount)
		{
			balance -= amount;
			return true;
		}
		return false;
	}

	double getBalance() const { return balance; }

	// protected метод - для наследников

  protected:
	void setBalance(double newBalance) { balance = newBalance; }
};

// Наследник с расширенной функциональностью
class SavingsAccount : public BankAccount
{
  private:
	double interestRate;

  public:
	SavingsAccount(string accNum, double balance, double rate) : BankAccount(accNum, balance), interestRate(rate) {}

	void applyInterest()
	{
		// Имеет доступ к protected методу
		setBalance(getBalance() * (1 + interestRate));
	}
};

void demo_encapsulation()
{
	cout << "\n=== Демонстрация инкапсуляции ===" << endl;

	BankAccount account("12345", 1000.0);
	account.deposit(500.0);
	cout << "Balance after deposit: " << account.getBalance() << endl;

	if (account.withdraw(200.0))
	{
		cout << "Withdrawal successful. New balance: " << account.getBalance() << endl;
	}

	SavingsAccount savings("67890", 1000.0, 0.05);
	savings.applyInterest();
	cout << "Savings after interest: " << savings.getBalance() << endl;
}

// ===== 6. НАСЛЕДОВАНИЕ =====

/*
 * ТЕОРИЯ: Наследование в C++
 *
 * Типы наследования:
 * - public:    public→public, protected→protected
 * - protected: public→protected, protected→protected
 * - private:   public→private, protected→private
 *
 * Принципы ООП:
 * 1. Инкапсуляция - сокрытие реализации
 * 2. Наследование - повторное использование кода
 * 3. Полиморфизм - одинаковый интерфейс, разная реализация
 *
 * Важные понятия:
 * - Базовый класс (родитель)
 * - Производный класс (наследник)
 * - Переопределение методов (override)
 * - Виртуальные функции
 * - Абстрактные классы
 */

class Shape
{
  protected:
	string color;

  public:
	Shape(string c) : color(c) {}

	// Виртуальный метод - может быть переопределен наследниками
	virtual double area() const { return 0.0; }

	virtual void draw() const { cout << "Drawing a " << color << " shape" << endl; }

	// Чисто виртуальный метод - делает класс абстрактным
	virtual string type() const = 0;

	// Виртуальный деструктор - ОБЯЗАТЕЛЕН при наследовании
	virtual ~Shape() { cout << "Shape destructor" << endl; }
};

// Public наследование - публичные методы базового класса остаются публичными
class Circle : public Shape
{
  private:
	double radius;

  public:
	Circle(string c, double r) : Shape(c), radius(r) {}

	// Переопределение виртуального метода
	double area() const override { return 3.14159 * radius * radius; }

	void draw() const override { cout << "Drawing a " << color << " circle with radius " << radius << endl; }

	string type() const override { return "Circle"; }

	~Circle() override { cout << "Circle destructor" << endl; }
};

class Square : public Shape
{
  private:
	double side;

  public:
	Square(string c, double s) : Shape(c), side(s) {}

	double area() const override { return side * side; }

	void draw() const override { cout << "Drawing a " << color << " square with side " << side << endl; }

	string type() const override { return "Square"; }

	~Square() override { cout << "Square destructor" << endl; }
};

void demo_inheritance()
{
	cout << "\n=== Демонстрация наследования ===" << endl;

	Circle circle("red", 5.0);
	Square square("blue", 4.0);

	circle.draw();
	cout << "Circle area: " << circle.area() << endl;
	cout << "Circle type: " << circle.type() << endl;

	square.draw();
	cout << "Square area: " << square.area() << endl;
	cout << "Square type: " << square.type() << endl;

	// Полиморфизм через указатели на базовый класс
	vector< Shape* > shapes = { &circle, &square };

	cout << "\nПолиморфизм через указатели:" << endl;
	for (Shape* shape : shapes)
	{
		shape->draw();
		cout << "Area: " << shape->area() << endl;
		cout << "Type: " << shape->type() << endl;
	}
}

// ===== 7. ВИРТУАЛЬНЫЕ ФУНКЦИИ =====

/*
 * ТЕОРИЯ: Виртуальные функции и полиморфизм
 *
 * Виртуальные функции (Функция, которая может быть переопределена в производных классах, и решение о том, какую
 * реализацию вызывать, принимается во время выполнения.):
 * - Определяют поведение во время выполнения (dynamic binding)
 * - Требуют наличия таблицы виртуальных функций (vtable)
 * - Добавляют небольшой оверхед по памяти и времени
 *
 * Чисто виртуальные функции:
 * - virtual void func() = 0;
 * - Делают класс абстрактным (нельзя создать объект)
 * - Должны быть переопределены в производных классах
 *
 * override и final:
 * - override: явно указывает переопределение виртуальной функции
 * - final: запрещает дальнейшее переопределение
 *
 * Правила:
 * - Деструктор должен быть виртуальным в базовом классе
 * - Конструктор не может быть виртуальным
 * - Виртуальные функции работают только через указатели/ссылки
 */

class Animal
{
  public:
	// Виртуальная функция
	virtual void speak() const { cout << "Animal sound" << endl; }

	// Чисто виртуальная функция - делает класс абстрактным
	virtual void move() const = 0;

	// Виртуальный деструктор
	virtual ~Animal() { cout << "Animal destructor" << endl; }
};

class Dog : public Animal
{
  public:
	void speak() const override { cout << "Woof!" << endl; }

	void move() const override { cout << "Dog is running" << endl; }

	~Dog() override { cout << "Dog destructor" << endl; }
};

class Cat : public Animal
{
  public:
	void speak() const override { cout << "Meow!" << endl; }

	void move() const override { cout << "Cat is walking" << endl; }

	// final - запрещает дальнейшее переопределение
	void speak() const final { cout << "Meow! (final)" << endl; }

	~Cat() override { cout << "Cat destructor" << endl; }
};

void demo_virtual()
{
	cout << "\n=== Демонстрация виртуальных функций ===" << endl;

	Dog dog;
	Cat cat;

	// Полиморфизм через указатели на базовый класс
	Animal* animals[] = { &dog, &cat };

	for (Animal* animal : animals)
	{
		animal->speak();	// Вызовется соответствующая версия метода
		animal->move();
	}

	// Полиморфизм через ссылки
	Animal& animal_ref1 = dog;
	Animal& animal_ref2 = cat;

	cout << "\nThrough references:" << endl;
	animal_ref1.speak();
	animal_ref2.speak();
}

// ===== 8. КОНСТРУКТОРЫ И ДЕСТРУКТОРЫ =====

/*
 * ТЕОРИЯ: Конструкторы и деструкторы
 *
 * Конструкторы:
 * - Имя совпадает с именем класса
 * - Нет возвращаемого типа
 * - Могут быть перегружены
 * - Могут иметь список инициализации
 * - Не наследуются
 *
 * Деструкторы:
 * - ~ИмяКласса()
 * - Вызываются автоматически при уничтожении объекта
 * - Должны быть виртуальными в базовых классах
 * - Не могут иметь параметров
 *
 * Особые методы (Rule of Five):
 * - Конструктор по умолчанию
 * - Конструктор копирования
 * - Оператор присваивания копированием
 * - Конструктор перемещения
 * - Оператор присваивания перемещением
 * - Деструктор
 */

class ResourceHolder
{
  private:
	int* data;
	const int id;
	static int next_id;
	size_t size;

  public:
	// Конструктор по умолчанию
	ResourceHolder() : id(next_id++), size(100)
	{
		data = new int[size];
		cout << "ResourceHolder " << id << " created (default)" << endl;
	}

	// Конструктор с параметром
	ResourceHolder(size_t s) : id(next_id++), size(s)
	{
		data = new int[size];
		cout << "ResourceHolder " << id << " created with size " << size << endl;
	}

	// Конструктор копирования
	ResourceHolder(const ResourceHolder& other) : id(next_id++), size(other.size)
	{
		data = new int[size];
		for (size_t i = 0; i < size; ++i)
		{
			data[i] = other.data[i];
		}
		cout << "ResourceHolder " << id << " copied from " << other.id << endl;
	}

	// Оператор присваивания копированием
	ResourceHolder& operator=(const ResourceHolder& other)
	{
		if (this != &other)
		{
			delete[] data;
			size = other.size;
			data = new int[size];
			for (size_t i = 0; i < size; ++i)
			{
				data[i] = other.data[i];
			}
		}
		cout << "ResourceHolder " << id << " assigned from " << other.id << endl;
		return *this;
	}

	// Деструктор
	~ResourceHolder()
	{
		delete[] data;
		cout << "ResourceHolder " << id << " destroyed" << endl;
	}
};

int ResourceHolder::next_id = 1;

void demo_constructors()
{
	cout << "\n=== Демонстрация конструкторов и деструкторов ===" << endl;

	ResourceHolder rh1;			 // Конструктор по умолчанию
	ResourceHolder rh2(200);	 // Конструктор с параметром
	ResourceHolder rh3 = rh1;	 // Конструктор копирования
	ResourceHolder rh4;
	rh4 = rh2;	  // Оператор присваивания

	cout << "Конец функции demo_constructors" << endl;
	// При выходе из функции автоматически вызываются деструкторы
}

// ===== 9. МНОЖЕСТВЕННОЕ НАСЛЕДОВАНИЕ =====

/*
 * ТЕОРИЯ: Множественное наследование
 *
 * Позволяет наследовать от нескольких базовых классов
 *
 * Проблемы:
 * - Неоднозначность (одинаковые имена в разных базовых классах)
 * - Ромбовидное наследование (дублирование базового класса)
 *
 * Решения:
 * - Виртуальное наследование (решает проблему ромба)
 * - Использование scope resolution operator (::)
 * - Переименование методов
 *
 * Рекомендации:
 * - Избегать множественного наследования реализации
 * - Использовать для наследования интерфейсов
 * - Предпочитать композицию наследованию
 */

class Printable
{
  public:
	virtual void print() const = 0;
	virtual ~Printable() = default;
};

class Serializable
{
  public:
	virtual string serialize() const = 0;
	virtual ~Serializable() = default;
};

class Identifiable
{
  public:
	virtual int getId() const = 0;
	virtual ~Identifiable() = default;
};

// Множественное наследование
class Document : public Printable, public Serializable, public Identifiable
{
  private:
	string content;
	int id;
	static int next_id;

  public:
	Document(string c) : content(c), id(next_id++) {}

	void print() const override { cout << "Document[" << id << "]: " << content << endl; }

	string serialize() const override { return "Document[" + to_string(id) + "]: " + content; }

	int getId() const override { return id; }
};

int Document::next_id = 1;

void demo_multiple_inheritance()
{
	cout << "\n=== Демонстрация множественного наследования ===" << endl;

	Document doc("Hello World");
	doc.print();
	cout << "Serialized: " << doc.serialize() << endl;
	cout << "ID: " << doc.getId() << endl;

	// Работа через указатели на базовые классы
	Printable* p = &doc;
	Serializable* s = &doc;
	Identifiable* i = &doc;

	cout << "\nThrough base pointers:" << endl;
	p->print();
	cout << "Serialized: " << s->serialize() << endl;
	cout << "ID: " << i->getId() << endl;
}

// ===== 10. FRIEND ФУНКЦИИ И КЛАССЫ =====

/*
 * ТЕОРИЯ: Дружественные функции и классы
 *
 * friend дает доступ к private и protected членам:
 * - Дружественные функции
 * - Дружественные классы
 * - Дружественные методы
 *
 * Особенности:
 * - Дружба не наследуется
 * - Дружба не транзитивна
 * - Дружба не взаимна (если A friend B, то B не автоматически friend A)
 *
 * Использование:
 * - Перегрузка операторов
 * - Функции-утилиты
 * - Тесное взаимодействие классов
 */

class SecretKeeper
{
  private:
	int secret;
	string password;

  public:
	SecretKeeper(int s, string pwd) : secret(s), password(pwd) {}

	// Дружественная функция
	friend void revealSecret(const SecretKeeper& sk);

	// Дружественный класс
	friend class SecretInspector;

	// Дружественная функция другого класса
	friend void ExternalAuditor::audit(const SecretKeeper&);
};

// Дружественная функция имеет доступ к приватным полям
void revealSecret(const SecretKeeper& sk)
{
	cout << "The secret is: " << sk.secret << endl;
	// Может обращаться к приватным полям
}

// Дружественный класс
class SecretInspector
{
  public:
	void inspect(const SecretKeeper& sk)
	{
		cout << "Inspector: secret = " << sk.secret << ", password = " << sk.password << endl;
	}

	void changeSecret(SecretKeeper& sk, int newSecret)
	{
		sk.secret = newSecret;	  // Может изменять приватные поля
	}
};

// Эмуляция внешнего аудитора
class ExternalAuditor
{
  public:
	static void audit(const SecretKeeper& sk) { cout << "Auditor: verifying secret " << sk.secret << endl; }
};

void demo_friend()
{
	cout << "\n=== Демонстрация friend функций и классов ===" << endl;

	SecretKeeper keeper(42, "qwerty");
	revealSecret(keeper);

	SecretInspector inspector;
	inspector.inspect(keeper);
	inspector.changeSecret(keeper, 100);
	inspector.inspect(keeper);
}
