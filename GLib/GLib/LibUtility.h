#pragma once

#include <vector>

class LibUtility {
public:
	template<typename T>
	static T Square(T numb) {
		return numb * numb;
	};

	template<typename T>
	static void Save(std::ostream& out, const T& val) {
		out.write(reinterpret_cast<const char*>(&val), sizeof(T));
	}

	template<typename T>
	static void Load(std::istream& in, T& val) {
		in.read(reinterpret_cast<char*>(&val), sizeof(T));
	}

	template<typename T>
	static void SaveData(std::ostream& out, const T* val, size_t amount) {
		out.write(reinterpret_cast<const char*>(val), amount * sizeof(T));
	}
	template<typename T>
	static void LoadData(std::istream& in, T* val, size_t amount) {
		in.read(reinterpret_cast<char*>(val), amount * sizeof(T));
	}

	template<typename T>
	static void SaveVec(std::ostream& out, const std::vector<T>& vec) {
		size_t size = vec.size();
		LibUtility::Save(out, size);
		for (const auto& val : vec) {
			val.Save(out);
		}
	}

	template<typename T>
	static void LoadVec(std::istream& in, std::vector<T>& vec) {
		size_t size = 0;
		LibUtility::Load(in, size);
		for (size_t i = 0; i < size; i++)
		{
			T val;
			val.Load(in);
			vec.push_back(val);
		}
	}

};
