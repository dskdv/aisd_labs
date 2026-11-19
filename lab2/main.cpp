import std;

template <typename T>
struct Node {
	T data;
	Node* next;
	Node* prev;

	// Создание узла с заданным значением
	Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename T>
class LinkedList {
private:
	Node<T>* head = nullptr;
	size_t _size = 0;

	// Копирование всех узлов из другого списка (вспомогательный метод)
	void copy_from(const LinkedList& other) {
		if (other.head == nullptr) {
			head = nullptr;
			_size = 0;
			return;
		}
		head = new Node<T>(other.head->data);
		Node<T>* current = head;
		Node<T>* other_current = other.head->next;
		while (other_current != other.head) {
			Node<T>* new_node = new Node<T>(other_current->data);
			current->next = new_node;
			new_node->prev = current;
			current = new_node;
			other_current = other_current->next;
		}
		current->next = head;
		head->prev = current;
		_size = other._size;
	}
public:
	// Конструктор по умолчанию
	LinkedList() : head(nullptr), _size(0) {}

	// Конструктор копирования
	LinkedList(const LinkedList& other) : head(nullptr), _size(0) {
		copy_from(other);
	}

	// Конструктор с параметрами: заполнение случайными значениями
	LinkedList(size_t count, T min_val, T max_val) : head(nullptr), _size(0) {
		if (min_val > max_val) {
			throw std::invalid_argument("min_val must not be greater than max_val");
		}
		if (count == 0) {
			return;
		}

		std::random_device rd;
		std::mt19937 gen(rd());

		if constexpr (std::is_integral_v<T> && !std::is_same_v<T, char>) {
			std::uniform_int_distribution<T> dist(min_val, max_val);
			for (size_t i = 0; i < count; ++i) {
				push_tail(dist(gen));
			}
		}
		else if constexpr (std::is_same_v<T, char>) {
			std::uniform_int_distribution<int> dist((int)min_val, (int)max_val);
			for (size_t i = 0; i < count; ++i) {
				push_tail((char)dist(gen));
			}
		}
		else if constexpr (std::is_floating_point_v<T>) {
			std::uniform_real_distribution<T> dist(min_val, max_val);
			for (size_t i = 0; i < count; ++i) {
				push_tail(dist(gen));
			}
		}
		else {
			for (size_t i = 0; i < count; ++i) {
				push_tail(min_val);
			}
		}
	}
	// Деструктор
	~LinkedList() {
		if (head == nullptr) {
			return;
		}

		Node<T>* current = head->next;
		while (current != head) {
			Node<T>* temp = current;
			current = current->next;
			delete temp;
		}
		delete head;
	}

	// Оператор присваивания
	LinkedList& operator=(const LinkedList& other) {
		if (this == &other) {
			return *this;
		}
		while (_size > 0) {
			pop_head();
		}
		copy_from(other);
		return *this;
	}

	// Добавление элемента в конец
	void push_tail(const T& value) {
		Node<T>* new_node = new Node<T>(value);
		if (head == nullptr) {
			head = new_node;
			head->next = head;
			head->prev = head;
		}
		else {
			Node<T>* tail = head->prev;
			tail->next = new_node;
			new_node->prev = tail;
			new_node->next = head;
			head->prev = new_node;
		}
		_size++;
	}
	// Добавление другого списка в конец
	void push_tail(const LinkedList& other) {
		if (other.head == nullptr) return;
		size_t count = other._size;
		Node<T>* other_current = other.head;
		for (size_t i = 0; i < count; ++i) {
			push_tail(other_current->data);
			other_current = other_current->next;
		}
	}

	// Добавление элемента в начало
	void push_head(const T& value) {
		push_tail(value);
		head = head->prev; // Сдвигаем указатель head на новый элемент
	}

	// Добавление другого списка в начало
	void push_head(const LinkedList& other) {
		if (other.head == nullptr) return;
		size_t count = other._size;
		Node<T>* other_current = other.head->prev;
		for (size_t i = 0; i < count; ++i) {
			push_head(other_current->data);
			other_current = other_current->prev;
		}
	}

	// Удаление элемента из начала
	void pop_head() {
		if (head == nullptr) {
			throw std::out_of_range("List is empty");
		}
		if (_size == 1) {
			delete head;
			head = nullptr;
		}
		else {
			Node<T>* tail = head->prev;
			Node<T>* new_head = head->next;

			tail->next = new_head;
			new_head->prev = tail;

			delete head;
			head = new_head;
		}
		_size--;
	}

	// Удаление элемента из конца
	void pop_tail() {
		if (head == nullptr) {
			throw std::out_of_range("List is empty");
		}
		if (_size == 1) {
			delete head;
			head = nullptr;
		}
		else {
			Node<T>* tail = head->prev;
			Node<T>* new_tail = tail->prev;

			new_tail->next = head;
			head->prev = new_tail;

			delete tail;
		}
		_size--;
	}

	// Удаление всех элементов, равных переданному значению
	void delete_node(const T& value) {
		if (head == nullptr) {
			return;
		}

		Node<T>* current = head;
		size_t original_size = _size;

		for (size_t i = 0; i < original_size; ++i) {
			Node<T>* next_node = current->next;

			if (current->data == value) {
				if (_size == 1) {
					delete head;
					head = nullptr;
					_size = 0;
					return;
				}
				else {
					if (current == head) {
						head = current->next;
					}
					current->prev->next = current->next;
					current->next->prev = current->prev;
					delete current;
					_size--;
				}
			}
			current = next_node;
		}
	}

	// Для чтения и записи
	T& operator[](size_t index) {
		if (head == nullptr) {
			throw std::out_of_range("List is empty");
		}
		size_t actual_index = index % _size;
		Node<T>* current = head;
		for (size_t i = 0; i < actual_index; ++i) {
			current = current->next;
		}
		return current->data;
	}

	// Только для чтения (константная версия)
	const T& operator[](size_t index) const {
		if (head == nullptr) {
			throw std::out_of_range("List is empty");
		}
		size_t actual_index = index % _size;
		Node<T>* current = head;
		for (size_t i = 0; i < actual_index; ++i) {
			current = current->next;
		}
		return current->data;
	}

	// Вывод списка в поток
	friend std::ostream& operator<<(std::ostream& os, const LinkedList& list) {
		os << "[ ";
		if (list.head != nullptr) {
			Node<T>* current = list.head;
			do {
				os << current->data << " ";
				current = current->next;
			} while (current != list.head);
		}
		os << "]";
		return os;
	}

	// Количество элементов в списке
	size_t size() const {
		return _size;
	}

	// Проверка, пуст ли список
	bool empty() const {
		return _size == 0;
	}
};
// Задача. Элементы по одному переносятся в новый список с конца в начало
template <typename T>
void reverse_list(LinkedList<T>& list) {
	LinkedList<T> reversed;
	while (!list.empty()) {
		reversed.push_head(list[0]);
		list.pop_head();
	}
	list = reversed;
}

int main() {
	std::cout << std::boolalpha;

	// 1. Конструктор по умолчанию
	std::cout << "1. default constructor\n";
	LinkedList<int> l1;
	std::cout << "l1: " << l1 << "\n";
	std::cout << "l1.empty() = " << l1.empty() << "\n";
	std::cout << "l1.size()  = " << l1.size() << "\n\n";

	// 2. push_tail(value)
	std::cout << "2. push_tail(value)\n";
	l1.push_tail(10);
	l1.push_tail(20);
	l1.push_tail(30);
	std::cout << "after push_tail: " << l1 << "\n";
	std::cout << "l1.size() = " << l1.size() << "\n\n";

	// 3. push_head(value)
	std::cout << "3. push_head(value)\n";
	l1.push_head(5);
	l1.push_head(1);
	std::cout << "after push_head: " << l1 << "\n\n";

	// 4. operator[]
	std::cout << "4. operator[]\n";
	std::cout << "l1[0] = " << l1[0] << "\n";
	std::cout << "l1[2] = " << l1[2] << "\n";
	std::cout << "l1[5] (5 % 5 = 0) = " << l1[5] << "\n";
	std::cout << "l1[7] (7 % 5 = 2) = " << l1[7] << "\n";
	l1[1] = 99;
	std::cout << "after l1[1] = 99: " << l1 << "\n\n";

	// 5. pop_head()
	std::cout << "5. pop_head()\n";
	l1.pop_head();
	std::cout << "after pop_head: " << l1 << "\n";
	std::cout << "l1.size() = " << l1.size() << "\n\n";

	// 6. pop_tail()
	std::cout << "6. pop_tail()\n";
	l1.pop_tail();
	std::cout << "after pop_tail: " << l1 << "\n";
	std::cout << "l1.size() = " << l1.size() << "\n\n";

	// 7. delete_node(value)
	std::cout << "7. delete_node(value)\n";
	LinkedList<int> l_del(6, 7, 7);
	std::cout << "before delete_node(7): " << l_del << "\n";
	l_del.delete_node(7);
	std::cout << "after delete_node(7): " << l_del << "\n";
	std::cout << "l_del.size() = " << l_del.size() << "\n\n";

	// 8. Конструктор с параметрами (случайное заполнение)
	std::cout << "8. random constructor\n";
	LinkedList<int> l_int(5, 1, 100);
	std::cout << "LinkedList<int>(5, 1, 100): " << l_int << "\n";

	LinkedList<float> l_float(5, -1.0f, 1.0f);
	std::cout << "LinkedList<float>(5, -1.0, 1.0): " << l_float << "\n";

	LinkedList<double> l_double(4, 0.0, 1.0);
	std::cout << "LinkedList<double>(4, 0.0, 1.0): " << l_double << "\n";

	LinkedList<char> l_char(6, 'a', 'z');
	std::cout << "LinkedList<char>(6, 'a', 'z'): " << l_char << "\n";

	LinkedList<std::string> l_string(3, std::string("Apple"), std::string("Samara"));
	std::cout << "LinkedList<std::string>(3, \"Apple\", \"Samara\"): " << l_string << "\n\n";

	// 9. Конструктор копирования
	std::cout << "9. copy constructor\n";
	LinkedList<int> l_copy(l_int);
	std::cout << "l_copy(l_int): " << l_copy << "\n\n";

	// 10. Оператор присваивания
	std::cout << "10. operator=\n";
	LinkedList<int> l_as;
	l_as = l_int;
	std::cout << "l_as = l_int: " << l_as << "\n";
	l_as = l_as;
	std::cout << "after self-assignment: " << l_as << "\n\n";

	// 11. push_tail(LinkedList)
	std::cout << "11. push_tail(LinkedList)\n";
	LinkedList<int> l_a;
	l_a.push_tail(1);
	l_a.push_tail(2);
	LinkedList<int> l_b;
	l_b.push_tail(3);
	l_b.push_tail(4);
	std::cout << "l_a: " << l_a << "\n";
	std::cout << "l_b: " << l_b << "\n";
	l_a.push_tail(l_b);
	std::cout << "l_a.push_tail(l_b): " << l_a << "\n\n";

	// 12. push_head(LinkedList)
	std::cout << "12. push_head(LinkedList)\n";
	LinkedList<int> l_c;
	l_c.push_tail(1);
	l_c.push_tail(2);
	LinkedList<int> l_d;
	l_d.push_tail(3);
	l_d.push_tail(4);
	std::cout << "l_c: " << l_c << "\n";
	std::cout << "l_d: " << l_d << "\n";
	l_c.push_head(l_d);
	std::cout << "l_c.push_head(l_d): " << l_c << "\n\n";

	// 13. Самодобавление списка в себя
	std::cout << "13. self push_tail\n";
	LinkedList<int> l_self;
	l_self.push_tail(1);
	l_self.push_tail(2);
	l_self.push_tail(3);
	std::cout << "before: " << l_self << "\n";
	l_self.push_tail(l_self);
	std::cout << "after l_self.push_tail(l_self): " << l_self << "\n\n";

	// 14. Практическая задача: reverse_list
	std::cout << "14. reverse_list\n";
	LinkedList<int> l_rev;
	l_rev.push_tail(1);
	l_rev.push_tail(2);
	l_rev.push_tail(3);
	l_rev.push_tail(4);
	l_rev.push_tail(5);
	std::cout << "before reverse: " << l_rev << "\n";
	reverse_list(l_rev);
	std::cout << "after reverse:  " << l_rev << "\n\n";

	// 15. Исключения
	std::cout << "15. exceptions\n";
	LinkedList<int> empty_l;
	try {
		empty_l.pop_head();
	}
	catch (const std::out_of_range& e) {
		std::cout << "Caught on pop_head(): " << e.what() << "\n";
	}
	try {
		empty_l.pop_tail();
	}
	catch (const std::out_of_range& e) {
		std::cout << "Caught on pop_tail(): " << e.what() << "\n";
	}
	try {
		int val = empty_l[0];
		std::cout << val;
	}
	catch (const std::out_of_range& e) {
		std::cout << "Caught on operator[]: " << e.what() << "\n";
	}
	try {
		LinkedList<int> bad(5, 10, 1);
	}
	catch (const std::invalid_argument& e) {
		std::cout << "Caught on random constructor: " << e.what() << "\n";
	}
}