import std;

template <typename T>
struct Node {
	T data;
	Node* next;
	Node* prev;

	Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename T>
class LinkedList {
private:
	Node<T>* head = nullptr;
	size_t _size = 0;

	void copyFrom(const LinkedList& other) {
		if (other.head == nullptr) {
			head = nullptr;
			_size = 0;
			return;
		}
		head = new Node<T>(other.head->data);
		Node<T>* current = head;
		Node<T>* otherCurrent = other.head->next;
		while (otherCurrent != other.head) {
			Node<T>* newNode = new Node<T>(otherCurrent->data);
			current->next = newNode;
			newNode->prev = current;
			current = newNode;
			otherCurrent = otherCurrent->next;
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
		copyFrom(other);
	}

	// Конструктор с параметрами: заполнение случайными значениями
	LinkedList(size_t count, T minVal, T maxVal) : head(nullptr), _size(0) {
		if (count == 0) {
			return;
		}

		std::random_device rd;
		std::mt19937 gen(rd());

		if constexpr (std::is_integral_v<T> && !std::is_same_v<T, char>) {
			std::uniform_int_distribution<T> dist(minVal, maxVal);
			for (size_t i = 0; i < count; ++i) {
				push_tail(dist(gen));
			}
		}
		else if constexpr (std::is_same_v<T, char>) {
			std::uniform_int_distribution<int> dist((int)minVal, (int)maxVal);
			for (size_t i = 0; i < count; ++i) {
				push_tail((char)dist(gen));
			}
		}
		else if constexpr (std::is_floating_point_v<T>) {
			std::uniform_real_distribution<T> dist(minVal, maxVal);
			for (size_t i = 0; i < count; ++i) {
				push_tail(dist(gen));
			}
		}
		else {
			for (size_t i = 0; i < count; ++i) {
				push_tail(minVal);
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
		copyFrom(other);
		return *this;
	}

	// Добавление элемента в конец
	void push_tail(const T& value) {
		Node<T>* newNode = new Node<T>(value);
		if (head == nullptr) {
			head = newNode;
			head->next = head;
			head->prev = head;
		}
		else {
			Node<T>* tail = head->prev;
			tail->next = newNode;
			newNode->prev = tail;
			newNode->next = head;
			head->prev = newNode;
		}
		_size++;
	}
	// Добавление другого списка в конец
	void push_tail(const LinkedList& other) {
		if (other.head == nullptr) return;
		Node<T>* otherCurrent = other.head;
		do {
			push_tail(otherCurrent->data);
			otherCurrent = otherCurrent->next;
		} while (otherCurrent != other.head);
	}

	// Добавление элемента в начало
	void push_head(const T& value) {
		push_tail(value);
		head = head->prev; // Сдвигаем указатель head на новый элемент
	}

	// Добавление другого списка в начало
	void push_head(const LinkedList& other) {
		if (other.head == nullptr) return;
		Node<T>* otherCurrent = other.head->prev;
		do {
			push_head(otherCurrent->data);
			otherCurrent = otherCurrent->prev;
		} while (otherCurrent != other.head->prev);
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
			Node<T>* newHead = head->next;

			tail->next = newHead;
			newHead->prev = tail;

			delete head;
			head = newHead;
		}
		_size--;
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
			Node<T>* newHead = head->next;

			tail->next = newHead;
			newHead->prev = tail;

			delete head;
			head = newHead;
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
			Node<T>* newTail = tail->prev;

			newTail->next = head;
			head->prev = newTail;

			delete tail;
		}
		_size--;
	}

};