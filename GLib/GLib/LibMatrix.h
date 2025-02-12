#pragma once

#include "LibEps.h"
#include "LibVector.h"
#include "LibPoint.h"

template<typename T>
class LibMatrix {
public:
	LibMatrix() {
		memset(m_data, 0, 16 * sizeof(T));
	};

	LibMatrix(const T* data) {
		std::memspy(m_data, data, 16 * sizeof(T));
	};

	~LibMatrix() = default;

	inline const T* Data() const {
		return m_data;
	};

	inline const T& GetNumb(uint32_t row, uint32_t col) const {
		return m_data[row * 4 + col];
	};

	inline const LibMatrix<T>& SetData(const T* data) {
		std::memspy(m_data, data, 16 * sizeof(T));
		return *this;
	};

	inline const LibMatrix<T>& SetNumb(T numb, uint32_t row, uint32_t col) {
		m_data[row * 4 + col] = numb;
		return *this;
	};

	static LibMatrix<T> ZeroMatrix() {
		static LibMatrix<T> mtrx_Zero;
		return mtrx_Zero;
	}

	static LibMatrix<T> IdentityMatrix() {
		static LibMatrix<T> matrix{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		return matrix;
	}

	bool IsEqual(const LibMatrix<T>& other, T eps = LibEps::eps) {
		LibMatrix<T> result;
		for (int i = 0; i < 16; ++i) {
			if (!LibEps::IsZero(Data()[i] - other.Data()[i], eps)) {
				return false;
			}
		}
		return true;
	}

	bool operator==(const LibMatrix<T>& other) const {
		return IsEqual(other);
	}

	const T* operator[](int row) const
	{
		return &m_data[row * 4];
	}

	const LibMatrix<T> operator*(const LibMatrix<T>& other) const {
		LibMatrix<T> result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m_data[i * 4 + j] =
					Data()[i * 4] * other.Data()[j] +
					Data()[i * 4 + 1] * other.Data()[j + 4] +
					Data()[i * 4 + 2] * other.Data()[j + 8] +
					Data()[i * 4 + 3] * other.Data()[j + 12];
			}
		}
		return result;
	}

	const LibVector<T> operator*(const LibVector<T>& vec) const {
		LibVector<T> result;
		result.SetX(GetNumb(0, 0) * vec.X() + GetNumb(1, 0) * vec.Y() + GetNumb(2, 0) * vec.Z());
		result.SetY(GetNumb(0, 1) * vec.X() + GetNumb(1, 1) * vec.Y() + GetNumb(2, 1) * vec.Z());
		result.SetZ(GetNumb(0, 2) * vec.X() + GetNumb(1, 2) * vec.Y() + GetNumb(2, 2) * vec.Z());
		return result;
	}

	const LibPoint<T> operator*(const LibPoint<T>& pt) const {
		LibPoint<T> result;
		result.SetX(GetNumb(0, 0) * pt.X() + GetNumb(1, 0) * pt.Y() + GetNumb(2, 0) * pt.Z() + GetNumb(3, 0));
		result.SetY(GetNumb(0, 1) * pt.X() + GetNumb(1, 1) * pt.Y() + GetNumb(2, 1) * pt.Z() + GetNumb(3, 1));
		result.SetZ(GetNumb(0, 2) * pt.X() + GetNumb(1, 2) * pt.Y() + GetNumb(2, 2) * pt.Z() + GetNumb(3, 2));
		return result;
	}

	T Determinant() const {
		T minor1[9] = { Data()[5], Data()[6], Data()[7],
				Data()[9], Data()[10], Data()[11],
				Data()[13], Data()[14], Data()[15] };

		T minor2[9] = { Data()[4], Data()[6], Data()[7],
			Data()[8], Data()[10], Data()[11],
			Data()[12], Data()[14], Data()[15] };

		T minor3[9] = { Data()[4], Data()[5], Data()[7],
			Data()[8], Data()[9], Data()[11],
			Data()[12], Data()[13], Data()[15] };

		T minor4[9] = { Data()[4], Data()[5], Data()[6],
			Data()[8], Data()[9], Data()[10],
			Data()[12], Data()[13], Data()[14] };

		return
			Data()[0] * Determinate33(minor1) -
			Data()[1] * Determinate33(minor2) +
			Data()[2] * Determinate33(minor3) -
			Data()[3] * Determinate33(minor4);
	}

	const LibMatrix<T> Transpose() const {
		LibMatrix<T> result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.SetNumb(GetNumb(i, j), j, i);
			}
		}
		return result;
	}

	const LibMatrix<T> InverseCopy() const {
		LibMatrix<T> result;
		T det = Determinant();

		LibMatrix<T> cofactors;
		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; ++col) {
				T minor[9];
				int minorIndex = 0;

				// made minor
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < 4; ++j) {
						if (i != row && j != col) {
							minor[minorIndex++] = m_data[i * 4 + j];
						}
					}
				}

				// get cofactor
				T cofactor = (row + col) % 2 == 0 ? 1 : (-1) * Determinate33(minor);
				cofactors.SetNumb(cofactor, row, col);
			}
		}

		// Transpose cafactors matrix
		result = cofactors.Transpose();

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.SetNumb(result.GetNumb(i, j) / det, i, j);
			}
		}
		return result;
	}

	void InverseThis() {
		memset(m_data, InverseCopy().Data(), 16 * sizeof(T));
	}

	static LibMatrix<T> TranslationInit(const LibVector<T>& move) const {
		LibMatrix<T> mtrx = IdentityMatrix();
		mtrx.SetNumb(move.X(), 3, 0);
		mtrx.SetNumb(move.Y(), 3, 1);
		mtrx.SetNumb(move.Z(), 3, 2);
		return mtrx;
	}

	static LibMatrix<T> ScalingInit(const LibVector<T>& scale) const {
		LibMatrix<T> mtrx = IdentityMatrix();
		mtrx.SetNumb(scale.X(), 0, 0);
		mtrx.SetNumb(scale.Y(), 1, 1);
		mtrx.SetNumb(scale.Z(), 2, 2);
		return mtrx;
	}

	static LibMatrix<T> RotationX(T theta) {
		LibMatrix<T> mtrx = IdentityMatrix();
		T cosTheta = std::cos(theta);
		T sinTheta = std::sin(theta);
		mtrx.SetNumb(cosTheta, 1, 1);
		mtrx.SetNumb(sinTheta, 1, 2);
		mtrx.SetNumb(-sinTheta, 2, 1);
		mtrx.SetNumb(cosTheta, 2, 2);
		return mtrx;
	}

	static LibMatrix<T> RotationY(T theta) {
		LibMatrix<T> mtrx = IdentityMatrix();
		T cosTheta = std::cos(theta);
		T sinTheta = std::sin(theta);
		mtrx.SetNumb(cosTheta, 0, 0);
		mtrx.SetNumb(-sinTheta, 0, 2);
		mtrx.SetNumb(sinTheta, 2, 0);
		mtrx.SetNumb(cosTheta, 2, 2);
		return mtrx;
	}

	static LibMatrix<T> RotationZ(T theta) {
		LibMatrix<T> mtrx = IdentityMatrix();
		T cosTheta = std::cos(theta);
		T sinTheta = std::sin(theta);
		mtrx.SetNumb(cosTheta, 0, 0);
		mtrx.SetNumb(sinTheta, 0, 1);
		mtrx.SetNumb(-sinTheta, 1, 0);
		mtrx.SetNumb(cosTheta, 1, 1);
		return mtrx;
	}

	static LibMatrix<T> Rotation(const LibVector<T> axis, T theta) {
		LibVector<T> normAxis = axis.GetNormalize();
		LibMatrix<T> res;

		T cos = std::cos(theta);
		T mCos = 1 - cos;
		T sin = std::sin(theta);
		T x = normAxis.X();
		T y = normAxis.Y();
		T z = normAxis.Z();
		res.SetNumb(cos + x * x * mCos, 0, 0);
		res.SetNumb(cos + y * y * mCos, 1, 1);
		res.SetNumb(cos + z * z * mCos, 2, 2);
		res.SetNumb(x * y * mCos - z * sin, 1, 0);
		res.SetNumb(x * y * mCos + z * sin, 0, 1);
		res.SetNumb(x * z * mCos + y * sin, 2, 0);
		res.SetNumb(x * z * mCos - y * sin, 0, 2);
		res.SetNumb(y * z * mCos + x * sin, 1, 2);
		res.SetNumb(y * z * mCos - x * sin, 2, 1);
	}

	static LibMatrix<T> ToCoordinatesInit(const LibVector<T>& a, const LibVector<T>& b, const LibVector<T>& c) {
		T new_mtrx[16] = { a.X(), a.Y(), a.Z(), 0, b.X(), b.Y(), b.Z(), 0, c.X(), c.Y(), c.Z(), 0, 0, 0, 0, 1 };
		LibMatrix<T> res(new_mtrx);

		res = InverseCopy(res);
		return res;
	}
protected:
	T Determinate33(const T* matrix) const {
		return matrix[0] * (matrix[4] * matrix[8] - matrix[5] * matrix[7]) -
			matrix[1] * (matrix[3] * matrix[8] - matrix[5] * matrix[6]) +
			matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
	}

private:
	T m_data[16];
};