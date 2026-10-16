import std;

template <typename T>
class Stack {
private:
	T* _data = nullptr;
	size_t _capacity = 0;
	size_t _size = 0;

	void resize() {
		if (_capacity == 0) {
			_capacity = 1;
		}
		else {
			_capacity *= 2;
		}
		T* newData = new T[_capacity];
		for (int i = 0; i < _size; ++i) {
			newData[i] = _data[i];
		}
		delete[] _data;
		_data = newData;
	}

public:
	//конструктор по умолчанию - пустой стек;
	Stack() : _data(nullptr), _size(0), _capacity(0) {}

	//конструктор с параметром
	Stack(const Stack& other) : _capacity(other._capacity), _size(other._size) {
		if (_size > 0) {
			_data = new T[_size];
		}
		else {
			_data = new T[1];
		}
		for (int i = 0; i < other._size; ++i) {
			_data[i] = other._data[i];
		}
	}

	~Stack() {
		delete[] _data;
	}
};