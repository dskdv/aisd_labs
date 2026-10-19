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

	//конструктор копирования
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

	//конструктор с параметром: заполнение стека случайными значениями
	Stack(size_t count, T minVal, T maxVal) : _size(count), _capacity(count) {
		_data = new T[_size];
		std::random_device rd;
		std::mt19937 gen(rd());

		if constexpr (std::is_integral_v<T> && !std::is_same_v<T, char>) {
			std::uniform_int_distribution<T> dist(minVal, maxVal);
			for (size_t i = 0;i < _size; ++i) {
				_data[i] = dist(gen);
			}
		}

		else if constexpr (std::is_same_v<T, char>) {
			std::uniform_int_distribution<int> dist((int)minVal, (int)maxVal);
			for (size_t i = 0; i < _size; ++i) {
				_data[i] = (char)dist(gen);
			}
		}

		else if constexpr (std::is_floating_point_v<T>) {
			std::uniform_real_distribution<T> dist(minVal, maxVal);
			for (size_t i = 0; i < _size; ++i) {
				_data[i] = dist(gen);
			}
		}

		else {
			for (size_t i = 0; i < _size; ++i) {
				_data[i] = minVal;
			}
		}
	}

	~Stack() {
		delete[] _data;
	}

	Stack& operator=(const Stack& other) {
		if (this == &other) {
			return *this;
		}
		delete[] _data;
		_size = other._size;
		_capacity = other._capacity;
		if (_size > 0) {
			_data = new T[_size];
		}
		else {
			_data = new T[1];
		}
		for (size_t i = 0; i < _size; ++i) {
			_data[i] = other._data[i];
		}
		return *this;
	}
public:
	//оператор += - аналог метода push(value)
	Stack& operator+=(const T& value) {
		if (_size >= _capacity) {
			resize();
		}
		_data[_size] = value;
		_size++;
		return *this;
	}
};

int main(){}