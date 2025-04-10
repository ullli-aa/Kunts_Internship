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
		std::memcpy(m_data, data, 16 * sizeof(T));
	};

	~LibMatrix() = default;

	inline const T* Data() const {
		return m_data;
	};

	inline const T& GetNumb(size_t row, size_t col) const {
		return m_data[row * 4 + col];
	};

	inline const LibMatrix<T>& SetData(const T* data) {
		std::memcpy(m_data, data, 16 * sizeof(T));
		return *this;
	};

	inline const LibMatrix<T>& SetNumb(T numb, size_t row, size_t col) {
		m_data[row * 4 + col] = numb;
		return *this;
	};

	static LibMatrix<T> ZeroMatrix() {
		static LibMatrix<T> mtrx_Zero;
		return mtrx_Zero;
	}

	static LibMatrix<T> IdentityMatrix() {
		T data[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
		static LibMatrix<T> matrix(data);
		return matrix;
	}

	bool IsEqual(const LibMatrix<T>& other, T eps = LibEps::eps) const {
		LibMatrix<T> result;
		for (int i = 0; i < 16; ++i) {
			if (!LibEps::IsZero(Data()[i] - other.Data()[i], eps)) {
				return false;
			}
		}
		return true;
	}

	bool operator==(const LibMatrix<T>& other) const {
		return this->IsEqual(other);
	}

	const T* operator[](size_t row) const
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

	void operator*=(const LibMatrix<T>& other) {
		*this = *this * other;
	}

	static LibVector<T> MultVec(const LibVector<T>& vec, const LibMatrix<T>& mtrx) {
		LibVector<T> result;
		result.SetX(mtrx[0][0] * vec.X() + mtrx[1][0] * vec.Y() + mtrx[2][0] * vec.Z());
		result.SetY(mtrx[0][1] * vec.X() + mtrx[1][1] * vec.Y() + mtrx[2][1] * vec.Z());
		result.SetZ(mtrx[0][2] * vec.X() + mtrx[1][2] * vec.Y() + mtrx[2][2] * vec.Z());
		return result;
	}

	static LibPoint<T> MultPt(const LibPoint<T>& pt, const LibMatrix<T>& mtrx) {
		LibVector<T> res = MultVec(LibVector<T>(pt.X(), pt.Y(), pt.Z()), mtrx);
		return LibPoint<T>(res.X() + mtrx[3][0], res.Y() + mtrx[3][1], res.Z() + mtrx[3][2]);
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

	LibMatrix<T> InverseCopy() const {
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
				T cofactor = ((row + col) % 2 == 0 ? 1 : (-1)) * Determinate33(minor);
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
		*this = InverseCopy();
	}

	static LibMatrix<T> TranslationInit(const LibVector<T>& move) {
		LibMatrix<T> mtrx = IdentityMatrix();
		mtrx.SetNumb(move.X(), 3, 0);
		mtrx.SetNumb(move.Y(), 3, 1);
		mtrx.SetNumb(move.Z(), 3, 2);
		return mtrx;
	}

	static LibMatrix<T> ScalingInit(const LibVector<T>& scale)  {
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

	static LibMatrix<T> Rotation(const LibVector<T>& axis, T theta) {
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
		res.SetNumb(1, 3, 3);
		return res;
	}

	static LibMatrix<T> ToCoordinatesInit(const LibVector<T>& a, const LibVector<T>& b, const LibVector<T>& c, const LibPoint<T>& pt) {
		T new_mtrx[16] = { a.X(), a.Y(), a.Z(), 0,
						b.X(), b.Y(), b.Z(), 0,
						c.X(), c.Y(), c.Z(), 0,
						pt.X(), pt.Y(), pt.Z(), 1 };
		LibMatrix<T> res(new_mtrx);

		res = res.InverseCopy();
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