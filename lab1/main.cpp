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

	//оператор += - аналог метода push(value)
	Stack& operator+=(const T& value) {
		if (_size >= _capacity) {
			resize();
		}
		_data[_size] = value;
		_size++;
		return *this;
	}
	//оператор -=   -  аналог метода pop(),
	Stack& operator-=(const T&) {
		if (empty()) {
			throw std::out_of_range("Stack is empty");
		}
		_size--;
		return *this;
	}
	//методы top(), size(), empty();
	T& top() {
		if (empty()) {
			throw std::out_of_range("Stack is empty");
		}
		return _data[_size - 1];
	}
	const T& top() const{
		if (empty()) {
			throw std::out_of_range("Stack is empty");
		}
		return _data[_size - 1];
	}

	size_t size() const {
		return _size;
	}
	bool empty() const {
		return _size == 0;
	}

	bool operator==(const Stack& other) const {
		if (_size != other._size) {
			return false;
		}
		for (size_t i = 0; i < _size; ++i) {
			if (_data[i] != other._data[i]) {
				return false;
			}
		}
		return true;
	}
	bool operator!=(const Stack& other) const {
		return !(*this == other);
	}
	friend std::ostream& operator<<(std::ostream& os, const Stack& other) {
		os << "[ ";
		for (int i = 0; i < other._size; ++i) {
			os << other._data[i] << " ";
		}
		os << "]";
		return os;
	}
};

//Перевести неотрицательное целое число из десятичной системы в систему с основанием k (2 ≤ k ≤ 16).
std::string converter(unsigned int number, int base) {
	if (base < 2 || base > 16) {
		throw std::invalid_argument("Base must be beween 2 and 16");
	}
	if (number == 0) {
		return "0";
	}
	Stack<char> s;
	const char digits[] = "0123456789ABCDEF";

	while (number > 0) {
		int current = number % base;
		s += digits[current];
		number = number / base;
	}
	std::string result = "";
	while (!s.empty()) {
		result += s.top();
		s -= ' ';
	}
	return result;
}

int main() {
	//конструктор по умолчанию
	std::cout << std::boolalpha;
	std::cout << "1. default constructor\n";
	Stack<int> s1;
	std::cout << "s1:" << s1 << "\n";
	std::cout << "s1.empty() = " << s1.empty() << "\n";
	std::cout << "s1.size()  = " << s1.size() << "\n\n";

	//оператор += 
	std::cout << "2. operator +=\n";
	s1 += 10;
	s1 += 20;
	s1 += 30;
	std::cout << "after +=: " << s1<< "\n";
	std::cout << "s1.size() = " << s1.size() << "\n\n";

	//метод top()
	std::cout << "3. top()\n";
	std::cout << "s1.top() = " << s1.top() << "\n";
	s1.top() = 99;
	std::cout << "s1.top() = 99: " << s1.top() << "\n\n";

	const Stack<int>& cs1 = s1;
	std::cout << "cs1.top() (const) = " << cs1.top() << "\n\n";

	//оператор -=
	std::cout << "4. operator -=\n";
	s1 -= 0;
	std::cout << "after -=: " << s1 << "\n\n";

	//конструктор с параметрами (случайное заполнение)
	std::cout << "5. random constructor\n";

	Stack<int> sInt(5, 1, 100);
	std::cout << "Stack<int>(5, 1, 100): " << sInt << "\n";

	Stack<float> sFloat(5, -1.0f, 1.0f);
	std::cout << "Stack<float>(5, -1.0, 1.0): " << sFloat << "\n";

	Stack<double> sDouble(4, 0.0, 1.0);
	std::cout << "Stack<double>(4, 0.0, 1.0): " << sDouble << "\n";

	Stack<char> sChar(6, 'a', 'z');
	std::cout << "Stack<char>(6, 'a', 'z): " << sChar << "\n";

	Stack<std::string> sString(3, std::string("Apple"), std::string("Samara"));
	std::cout << "Stack<std::string(3, std::string(\"Apple\"), std::string(\"Samara\")): " << sString << "\n\n";

	//конструктор копирование
	std::cout << "6. copy constructor\n";
	Stack<int> sCopy(sInt);
	std::cout << "sCopy(sInt): " << sCopy << "\n\n";

	//оператор =
	std::cout << "7. operator =\n";
	Stack<int> sAs;
	sAs = sInt;
	std::cout << "sAssign = sInt: " << sAs << "\n";
	sAs = sAs;
	std::cout << "after self-assignment: " << sAs << "\n\n";

	//оператор == и !=
	std::cout << "8. operator == and !=\n";
	Stack<int> a(3, 5, 5);
	Stack<int> b(3, 5, 5);
	Stack<int> c(4, 5, 5);
	std::cout << "a = " << a << "\n";
	std::cout << "b = " << b << "\n";
	std::cout << "c = " << c << "\n";
	std::cout << "a == b ? " << (a == b) << "\n";
	std::cout << "a != b ? " << (a != b) << "\n";
	std::cout << "a == c ? " << (a == c) << "\n";
	std::cout << "a != c ? " << (a != c) << "\n\n";

	//исключения
	std::cout << "9. exceptions\n";
	Stack<int> emptyS;
	try {
		emptyS.top();
	}
	catch (const std::out_of_range& e) {
		std::cout << "Caught on top(): " << e.what() << "\n";
	}
	try {
		emptyS -= 0;
	}
	catch (const std::out_of_range& e) {
		std::cout << "Caught on pop(): " << e.what() << "\n\n";
	}

	//Перевод числа 
	std::cout << "10. Number base conversion\n";
	unsigned int num = 255;
	std::cout << "Number: " << num << "\n";
	std::cout << "Base 2: " << converter(num, 2) << "\n";
	std::cout << "Base 4: " << converter(num, 4) << "\n";
	std::cout << "Base 16: " << converter(num, 16) << "\n";
	std::cout << "Test for 0: " << converter(0, 2) << "\n\n";
	}