#pragma once

template<typename T>
class LibPoint {
public:
	LibPoint();
	LibPoint(T x, T y);
	LibPoint(T x, T y, T z);

	~LibPoint() = default;

	T SquareDistanceTo(const LibPoint& other) const;
	T DistanceTo(const LibPoint& other) const;

	bool operator==(const LibPoint& other) const;
	bool operator!=(const LibPoint& other) const;
	LibPoint operator+(const LibPoint& other) const;
	LibPoint operator-(const LibPoint& other) const;

	bool IsEqual(const LibPoint& other, double eps = 1e-9) const;

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

	inline LibPoint& SetX(T x)
	{
		m_x = x;
		return *this;
	};
	inline LibPoint& SetY(T y)
	{
		m_y = y;
		return *this;
	};
	inline LibPoint& SetZ(T z)
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


private:
	T m_x;
	T m_y;
	T m_z;
};
