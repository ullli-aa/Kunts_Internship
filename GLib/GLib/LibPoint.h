#pragma once

//template<class T>
class LibPoint {
public:
	LibPoint();
	LibPoint(double x, double y);
	LibPoint(double x, double y, double z);

	~LibPoint();

	double SquareDistanceTo(const LibPoint& other) const;
	double DistanceTo(const LibPoint& other) const;

	bool operator==(const LibPoint& other) const;
	bool operator!=(const LibPoint& other) const;

	bool IsEqual(const LibPoint& other, double eps = 1e-9) const;

	inline double X() const
	{
		return m_x;
	};
	inline double Y() const
	{
		return m_y;
	};
	inline double Z() const
	{
		return m_z;
	};

	inline LibPoint& SetX(double x)
	{
		m_x = x;
		return *this;
	};
	inline LibPoint& SetY(double y)
	{
		m_y = y;
		return *this;
	};
	inline LibPoint& SetZ(double z)
	{
		m_z = z;
		return *this;
	};
	inline void SetXYZ(double x = 0.0, double y = 0.0, double z = 0.0)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	};


private:
	double m_x;
	double m_y;
	double m_z;
};
