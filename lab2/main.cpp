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
};