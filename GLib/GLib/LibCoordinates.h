#pragma once

template<typename T>
class LibCoordinates {
public:
	LibCoordinates() : m_x(0.0), m_y(0.0), m_z(0.0) {};
	LibCoordinates(T x, T y) : m_x(x), m_y(y), m_z(0.0) {};
	LibCoordinates(T x, T y, T z) : m_x(x), m_y(y), m_z(z) {};

	~LibCoordinates() = default;

	inline T X() const
	{
		return m_x;
	};

	inline T Y() const
	{
		return m_y;
	};

	inline T Z() const
	{
		return m_z;
	};

	inline LibCoordinates<T>& SetX(T x)
	{
		m_x = x;
		return *this;
	};

	inline LibCoordinates<T>& SetY(T y)
	{
		m_y = y;
		return *this;
	};

	inline LibCoordinates<T>& SetZ(T z)
	{
		m_z = z;
		return *this;
	};

	inline void SetXYZ(T x = 0.0, T y = 0.0, T z = 0.0)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	};

	void Save(std::ostream& out) const {
		LibUtility::SaveData(out, &m_x, 3);
	}

	void Load(std::istream& in) {
		LibUtility::LoadData(in, &m_x, 3);
	}

protected:
	T m_x;
	T m_y;
	T m_z;
};