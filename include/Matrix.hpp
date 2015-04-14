#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <cassert>
#include <initializer_list>
#include <iostream>

#include "Ray.hpp"
#include "Vector.hpp"

// M rows, N columns
template <typename T, int M, int N>
class Matrix {
public:
	Matrix () {
		for (int i = 0; i < M; ++i) {
			for (int j = 0; j < N; ++j) {
				data[i][j] = T();
			}
		}
	}

	Matrix (T val) {
		for (int i = 0; i < M; ++i) {
			for (int j = 0; j < N; ++j) {
				data[i][j] = val;
			}
		}
	}

/*
	Vector (const std::initializer_list<T>& il) {
		int i = 0;
		for (auto val: il) {
			data[i++] = val;
		}
	}
*/

	T& operator()(const int x, const int y) {
		assert(x < M && y < N);
		return data[x][y];
	}

	const T& operator()(const int x, const int y) const {
		assert(x > -1 && x < M && y > -1 && y < N);
		return data[x][y];
	}

	Matrix <T, M, N> &operator += (const Matrix <T, M, N>& in);
	Matrix <T, M, N> &operator -= (const Matrix <T, M, N>& in);
	Matrix <T, M, N> &operator *= (const float s);
	Matrix <T, M, N> &operator *= (const Matrix <T, M, N>& in);
	Matrix <T, M, N> &operator /= (const float s);
	Matrix <T, M, N> operator -  (void); // this is plain old negation
	
	const Matrix <T, M, N> operator+ (const Matrix <T, M, N> &b) const;
	const Matrix <T, M, N> operator- (const Matrix <T, M, N> &b) const;
	const Matrix <T, M, N> operator* (const float s) const;
	const Matrix <T, M, N> operator/ (const float s) const;
	bool operator== (const Matrix <T, M, N> &b) const;
	bool operator!= (const Matrix <T, M, N> &b) const;
/*	
	real determinant();
	real det3(real a, real b, real c, real d, real e, real f, real g, real h, real i) {
		return a*e*i + d*h*c + g*b*f - g*e*c - d*b*i - a*h*f;
	}
	
	void setIdentity();
	void setRotation(real a, real b, real c, real u, real v, real w, real theta);
		
	void inverse(Matrix &ans);
	void transpose(Matrix &ans);
*/	


	static Matrix <T, M, N> Identity() {
		Matrix <T, M, N> ans;
		for (int i = 0; i < M; ++i) {
			for (int j = 0; j < N; ++j) {
				if (i == j) ans.data[i][j] = 1.;
				else ans.data[i][j] = 0.;
			}
		}
		return ans;
	} 



	T data[M][N];
};

typedef Matrix <float, 2, 2> Mat22;
typedef Matrix <float, 3, 3> Mat33;
typedef Matrix <float, 4, 4> Mat44;

template <typename T, int M, int N>
std::ostream& operator<<(std::ostream& os, const Matrix <T, M, N> & mat) {
	for (int i = 0; i < M; ++i) {
		os << "\t(";
		for (int j = 0; j < N; ++j) {
			os << mat.data[i][j];
			os << ", ";
		}
		os << "\b\b)\n";
	}
	return os;
}

/* Compound assignment operators */

template <typename T, int M, int N>
Matrix <T, M, N>& Matrix <T, M, N>::operator+= (const Matrix <T, M, N>& in) {
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			this->data[i][j] += in.data[i][j];
		}
	}
	return *this;
}

template <typename T, int M, int N>
Matrix <T, M, N>& Matrix <T, M, N>::operator-= (const Matrix <T, M, N>& in) {
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			this->data[i][j] -= in.data[i][j];
		}
	}
	return *this;
}

template <typename T, int M, int N>
Matrix <T, M, N>& Matrix <T, M, N>::operator*= (const float s) {
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			this->data[i][j] *= s;
		}
	}
	return *this;
}

/* *= only works for square matrices. */
template <typename T, int M, int N>
Matrix <T, M, N>& Matrix <T, M, N>::operator*= (const Matrix <T, M, N>& in) {
	assert (M == N);
	Matrix <T, M, M> ans(0.0);
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < M; ++j) {
			for (int k = 0; k < N; ++k) {
				ans->data[i][j] += this->data[i][k] * in.data[k][j];
			}
		}
	}
	*this = ans;
	return *this;
}

template <typename T, int M, int N>
Matrix <T, M, N>& Matrix <T, M, N>::operator /= (const float s) {
	float invS = 1.0/s;
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			this->data[i][j] *= invS;
		}
	}
	return *this;
}

template <typename T, int M, int N>
Matrix <T, M, N> Matrix <T, M, N>::operator- (void) {
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			this->data[i][j] = -this->data[i][j];
		}
	}
	return *this;
} 

/* Binary operators */

template <typename T, int M, int N>
const Vector <T, M> operator* (const Matrix <T, M, N> &m, const Vector <T, N> &v) {
	/* Matrix * vector. Template takes care of size matching. */
	Vector <T, M> ans (0.0);
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			ans[i] += m.data[i][j] * v.data[j];
		}
	}
	return ans;
}

template <typename T, int M, int N>
const Matrix <T, M, N> Matrix <T, M, N>::operator+ (const Matrix <T, M, N> &b) const {
	return Matrix <T, M, N> (*this) += b;
}
	
template <typename T, int M, int N>
const Matrix <T, M, N> Matrix <T, M, N>::operator- (const Matrix <T, M, N> &b) const {
	return Matrix <T, M, N> (*this) -= b;
}

template <typename T, int M, int N, int P>	
const Matrix <T, M, P> operator* (const Matrix <T, M, N> &a, const Matrix <T, N, P> &b) {
	Matrix <T, M, P> ans(0.0);
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < P; ++j) {
			for (int k = 0; k < N; ++k) {
				ans.data[i][j] += a.data[i][k] * b.data[k][j];
			}
		}
	}
	return ans;
}

template <typename T, int M, int N>
const Matrix <T, M, N> Matrix <T, M, N>::operator* (const float s) const {
	return Matrix <T, M, N> (*this) *= s;
}

template <typename T, int M, int N>
const Matrix <T, M, N> operator* (const float &s, const Matrix <T, M, N> &a) { 
	return a * s;
}

template <typename T, int M, int N>	
const Matrix <T, M, N> Matrix <T, M, N>::operator/ (const float s) const {
	return Matrix <T, M, N> (*this) /= s;
}

template <typename T, int M, int N>
bool Matrix <T, M, N>::operator== (const Matrix <T, M, N> &b) const {
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			if (this->data[i][j] - b.data[i][j] > .0000001) return false;
		}
	}
	return true;
}

template <typename T, int M, int N>
bool Matrix <T, M, N>::operator!= (const Matrix <T, M, N> &b) const {
	return !(*this == b);
}

class Transform {
public:
	Transform() : trans (Mat44::Identity()), invTrans (Mat44::Identity()) {}

	Transform& operator *= (const Transform& in) {
		this->trans = this->trans * in.trans; // AB = A*B
		this->invTrans = in.invTrans * this->invTrans; // (AB)^-1 = B^-1 * A^-1
		return *this;
	}

	const Transform operator* (const Transform& in) const {
		return Transform(*this) *= in;
	}
	
	bool operator== (const Transform &b) const {
		return (this->trans == b.trans && this->invTrans == b.invTrans);
	}
	
	bool operator!= (const Transform &b) const {
		return (this->trans != b.trans || this->invTrans != b.invTrans);
	}

	const Ray transformRay(const Ray& r) const {
		return Ray(this->transformPoint(r.o), this->transformVector(r.d));
	}
	
	const Ray inverseTransformRay(const Ray& r) const {
		return Ray(this->inverseTransformPoint(r.o), this->inverseTransformVector(r.d));
	}

	const Vec3 transformPoint(const Vec3& p) const {
		return Vec3(
			trans(0,0)*p[0] + trans(0,1)*p[1] + trans(0,2)*p[2] + trans(0,3),
			trans(1,0)*p[0] + trans(1,1)*p[1] + trans(1,2)*p[2] + trans(1,3),
			trans(2,0)*p[0] + trans(2,1)*p[1] + trans(2,2)*p[2] + trans(2,3));
	}
	const Vec3 inverseTransformPoint(const Vec3& p) const {
		return Vec3(
			invTrans(0,0)*p[0] + invTrans(0,1)*p[1] + invTrans(0,2)*p[2] + invTrans(0,3),
			invTrans(1,0)*p[0] + invTrans(1,1)*p[1] + invTrans(1,2)*p[2] + invTrans(1,3),
			invTrans(2,0)*p[0] + invTrans(2,1)*p[1] + invTrans(2,2)*p[2] + invTrans(2,3));
	}

	const Vec3 transformVector(const Vec3& v) const {
		return Vec3(
			trans(0,0)*v[0] + trans(0,1)*v[1] + trans(0,2)*v[2],
			trans(1,0)*v[0] + trans(1,1)*v[1] + trans(1,2)*v[2],
			trans(2,0)*v[0] + trans(2,1)*v[1] + trans(2,2)*v[2]);
	}

	const Vec3 inverseTransformVector(const Vec3& v) const {
		return Vec3(
			invTrans(0,0)*v[0] + invTrans(0,1)*v[1] + invTrans(0,2)*v[2],
			invTrans(1,0)*v[0] + invTrans(1,1)*v[1] + invTrans(1,2)*v[2],
			invTrans(2,0)*v[0] + invTrans(2,1)*v[1] + invTrans(2,2)*v[2]);
	}

	const Vec3 transformNormal(const Vec3& n) const {
		// (M^-1)^T
		return Vec3(
			invTrans(0,0)*n[0] + invTrans(1,0)*n[1] + invTrans(2,0)*n[2],
			invTrans(0,1)*n[0] + invTrans(1,1)*n[1] + invTrans(2,1)*n[2],
			invTrans(0,2)*n[0] + invTrans(1,2)*n[1] + invTrans(2,2)*n[2]);
	}
	
	const Vec3 inverseTransformNormal(const Vec3& n) const {
		// (M^-1^-1)^T = M^T
		return Vec3(
			trans(0,0)*n[0] + trans(1,0)*n[1] + trans(2,0)*n[2],
			trans(0,1)*n[0] + trans(1,1)*n[1] + trans(2,1)*n[2],
			trans(0,2)*n[0] + trans(1,2)*n[1] + trans(2,2)*n[2]);
	}

	const BBox transformBBox(const BBox& bbox) const {
		// from Graphics Gems, Jim Arvo, 1990
		// start at translation point
		BBox ans(Vec3(trans(0,3), trans(1,3), trans(2,3)), Vec3(trans(0,3), trans(1,3), trans(2,3)));

		float a, b;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				a = trans(i, j) * bbox.vMin[j];
				b = trans(i, j) * bbox.vMax[j];
				if (a < b) {
					ans.vMin += a;
					ans.vMax += b;
				} else {
					ans.vMin += b;
					ans.vMax += a;
				}
			}
		}
		return ans;

	}

	const BBox inverseTransformBBox(const BBox& bbox) const {
		// from Graphics Gems, Jim Arvo, 1990
		// start at translation point
		BBox ans(Vec3(invTrans(0,3), invTrans(1,3), invTrans(2,3)), Vec3(invTrans(0,3), invTrans(1,3), invTrans(2,3)));

		float a, b;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				a = invTrans(i, j) * bbox.vMin[j];
				b = invTrans(i, j) * bbox.vMax[j];
				if (a < b) {
					ans.vMin += a;
					ans.vMax += b;
				} else {
					ans.vMin += b;
					ans.vMax += a;
				}
			}
		}
		return ans;
	}

	static Transform Scale(const Vec3& sv) {
		return Transform(ScaleMatrix(sv), ScaleMatrix({1.0f/sv[0], 1.0f/sv[1], 1.0f/sv[2]}));
	}

	static Transform Translate(const Vec3& trans) {
		return Transform(TranslationMatrix(trans), TranslationMatrix(-trans));
	}

	static Transform Rotate(const Vec3& axis, float theta) {
		return Transform(RotationMatrix(axis, theta), RotationMatrix(axis, -theta));
	}

	static Mat44 ScaleMatrix(const Vec3 &sv) {
		Mat44 ans = Mat44::Identity();
		ans(0,0) = sv[0];
		ans(1,1) = sv[1];
		ans(2,2) = sv[2];
		return ans;
	}

	static Mat44 TranslationMatrix(const Vec3& trans) {
		Mat44 ans = Mat44::Identity();
		ans(0, 3) = trans[0];
		ans(1, 3) = trans[1];
		ans(2, 3) = trans[2];
		return ans;
	}

	static Mat44 RotationMatrix(const Vec3& axis, float theta) {
		Mat44 ans = Mat44::Identity();
		Vec3 unitAxis = Vec3::normalize(axis);
		float c = cos(theta);
		float s = sin(theta);
		float t = 1 - c;
		float tXY = t * unitAxis[0] * unitAxis[1];
		float tXZ = t * unitAxis[0] * unitAxis[2];
		float tYZ = t * unitAxis[1] * unitAxis[2];
		float sX = s * unitAxis[0];
		float sY = s * unitAxis[1];
		float sZ = s * unitAxis[2];
		ans(0, 0) = t * unitAxis[0] * unitAxis[0] + c;
		ans(0, 1) = tXY + sZ;
		ans(0, 2) = tXZ - sY;
		ans(1, 0) = tXY - sZ;
		ans(1, 1) = t * unitAxis[1] * unitAxis[1] + c;
		ans(1, 2) = tYZ + sX;
		ans(2, 0) = tXZ + sY;
		ans(2, 1) = tYZ - sX;
		ans(2, 2) = t * unitAxis[2] * unitAxis[2] + c;
		return ans;
	}

	const Mat44& inv() const {return invTrans;}
	friend std::ostream& operator<<(std::ostream& os, const Transform& t);

protected:
	Transform(const Mat44 &_trans, const Mat44 &invTrans) : trans(_trans), invTrans(invTrans) {}
	Mat44 trans;
	Mat44 invTrans;
};

inline std::ostream& operator<<(std::ostream& os, const Transform& t) {
	os << t.trans << std::endl << t.invTrans;
	return os;
}

#endif