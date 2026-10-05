#include <iostream>
#include <stack> 
#include <queue>
#include <deque>
using namespace std;

int main() {

   // FILO - first in - last out (первый пришел - последний ушел)

   stack <int> steck;  // создание стека

   steck.push(6); // добавление элемента

   steck.empty(); // проверяет пустой ли стек

   steck.top(); // выводит верхний элемент

   steck.pop();  // удаляем верхний элемент






   // FIFO - first in - first out (первый пришел - первый ушел)

   queue <int> q;

   q.push(1); // добавление в очередь

   cout << q.front() << endl;    // вывод ласт элемента

   q.pop();  // удаление элемента из очереди


   priority_queue <int> q1;   // та же очередь только отсортированная по неубыванию






   // Разница между Вектором и Деком состоит лишь в том, что в деках динамический массив открыт с двух сторон. 
   // Это и позволяет очень быстро добавлять новые элементы как в конец так и в начало контейнера. 
   // В векторах элементы можно добавлять лишь в конец массива.
   deque <int> d;

   d.push_back(1);
   d.push_front(2);

   d.pop_back();
   d.pop_front();

   return 0;
}