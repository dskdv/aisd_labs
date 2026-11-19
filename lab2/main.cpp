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
		if (other.head == nullptr) return;   // ← добавьте эту строку
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
