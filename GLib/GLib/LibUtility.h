#pragma once

template<typename T>
class LibUtility {
public:
	static T Square(T numb) {
		return numb * numb;
	};

	static void Save(std::ostream& out, const T& val, size_t size) {
		out.write(reinterpret_cast<const char*>(&val), size);
	}

	static void Load(std::istream& in, T& val, size_t size) {
		in.read(reinterpret_cast<char*>(&val), size);
	}
};
