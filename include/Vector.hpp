#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <initializer_list>
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>

#include "Draco2.hpp"

class Ray;

template <typename T, int N>
class Vector {
public:
	Vector () {
		for (int i = 0; i < N; ++i) {
			data[i] = T();
		}
	}

	Vector (const T val) {
		for (int i = 0; i < N; ++i)
			data[i] = val;
	}

	Vector (const T _v1, const T _v2) {
		data[0] = _v1;
		data[1] = _v2;
	}

	Vector (const T _v1, const T _v2, const T _v3) {
		data[0] = _v1;
		data[1] = _v2;
		data[2] = _v3;
	}

	Vector (const T _v1, const T _v2, const T _v3, const T _v4) {
		data[0] = _v1;
		data[1] = _v2;
		data[2] = _v3;
		data[3] = _v4;
	}

	Vector (const std::initializer_list<T>& il) {
		int i = 0;
		for (auto val: il) {
			data[i++] = val;
		}
	}

	Vector (const std::vector <T> & inVec) {
		assert (N == inVec.size());
		for (int i = 0; i < N; ++i) {
			data[i] = inVec[i];
		}
	}

	Vector <T, N>& operator= (const std::vector <T> & inVec) {
		assert (N == inVec.size());
		for (int i = 0; i < N; ++i) {
			data[i] = inVec[i];
		}
	}

	T& operator[](const int idx) {
		return data[idx];
	}

	const T& operator[](const int idx) const {
		return data[idx];
	}

	const Vector <T, N> operator- (void) const {
		Vector <T, N> ans(*this);
		for (int i = 0; i < N; ++i) {
			ans.data[i] = -ans.data[i];
		}
		return ans;
	} 

	Vector <T, N>& operator+= (const Vector <T, N> &v) {
		for (int i = 0; i < N; ++i) {
			data[i] += v[i];
		}
		return *this;
	}

	Vector <T, N>& operator-= (const Vector <T, N> &v) {
		for (int i = 0; i < N; ++i) {
			data[i] -= v[i];
		}
		return *this;
	}

	Vector <T, N>& operator*= (const float& s) {
		for (int i = 0; i < N; ++i) {
			data[i] *= s;
		}
		return *this;
	}

	Vector <T, N>& operator*= (const Vector <T, N>& v) {
		for (int i = 0; i < N; ++i) {
			data[i] *= v.data[i];
		}
		return *this;
	}

	Vector <T, N>& operator/= (const float& s) {
		for (int i = 0; i < N; ++i) {
			data[i] /= s;
		}
		return *this;
	}

	T magnitude(void) const {
		return sqrt(magnitudeSQ());
	}

	T magnitudeSQ(void) const {
		T msq = 0.;
		for (int i = 0; i < N; ++i) {
			msq += data[i] * data[i];
		}
		return msq;
	}

	static Vector <T, N> clampv(const Vector <T, N>& v, T low, T high) {
		Vector <T, N> ans;
		for (int i = 0; i < N; ++i)
			ans[i] = clamps(v[i], low, high);
		return ans;
	}

	void normalize(void);

	static Vector <T, N> normalize(const Vector <T, N>& v) {
		Vector <T, N> ans =  v;
		T length = 0;
		for (int i = 0; i < N; ++i)
			length += ans.data[i] * ans.data[i];
		length = (T) sqrt(length);

		for (int i = 0; i < N; ++i)
			ans.data[i] /= length;

		return ans;
	}

	T data[N];
};

typedef Vector <float, 2> Vec2;
typedef Vector <float, 3> Vec3;
typedef Vector <float, 4> Vec4;

// template <typename T, int N>
// std::ostream& operator<<(std::ostream& os, const Vector <T, N> & v);

// /*** Component-wise operators ***/
// template <typename T, int N>
// Vector <T, N> operator+ (const Vector <T, N> &v1, const Vector <T, N> & v2);

// template <typename T, int N>
// Vector <T, N> operator- (const Vector <T, N> &v1, const Vector <T, N> & v2);

// template <typename T, int N>
// Vector <T, N> operator* (const Vector <T, N> &v1, const Vector <T, N> & v2);

// template <typename T, int N>
// Vector <T, N> operator* (const Vector <T, N> &v1, const T & s);

// template <typename T, int N>
// Vector <T, N> operator* (const T &s, const Vector <T, N> & v1);

// template <typename T, int N>
// Vector <T, N> operator/ (const Vector <T, N> &v1, const Vector <T, N> & v2);

// template <typename T, int N>
// Vector <T, N> operator/ (const Vector <T, N> &v1, const T & s);

// template <typename T, int N>
// Vector <T, N> operator/ (const T &s, const Vector <T, N> & v1);

// /*** Dot products, etc ***/

// template <typename T, int N>
// T dot (const Vector <T, N> &v1, const Vector <T, N> &v2);

// Vec3 cross (const Vec3 &v1, const Vec3 &v2);



template <typename T, int N>
std::ostream& operator<<(std::ostream& os, const Vector <T, N>& v)
{
	os << "(";
	for (int i = 0; i < N; ++i) {
		os << v.data[i];
		os << ", ";
	}
	os << "\b\b)";
	return os;
}

/*** Component-wise operators ***/

template <typename T, int N>
inline Vector <T, N> operator+ (const Vector <T, N> &v1, const Vector <T, N> & v2) {
	Vector <T, N> ans;
	for (int i = 0; i < N; ++i)
		ans.data[i] = v1.data[i] + v2.data[i];
	return ans;
}

template <typename T, int N>
inline Vector <T, N> operator- (const Vector <T, N> &v1, const Vector <T, N> & v2) {
	Vector <T, N> ans;
	for (int i = 0; i < N; ++i)
		ans.data[i] = v1.data[i] - v2.data[i];
	return ans;
}

template <typename T, int N>
inline Vector <T, N> operator* (const Vector <T, N> &v1, const Vector <T, N> & v2) {
	Vector <T, N> ans;
	for (int i = 0; i < N; ++i)
		ans.data[i] = v1.data[i] * v2.data[i];
	return ans;
}

template <typename T, int N>
inline Vector <T, N> operator* (const Vector <T, N> &v1, const T & s) {
	Vector <T, N> ans;
	for (int i = 0; i < N; ++i)
		ans.data[i] = v1.data[i] * s;
	return ans;
}

template <typename T, int N>
inline Vector <T, N> operator* (const T &s, const Vector <T, N> & v1) {
	Vector <T, N> ans;
	for (int i = 0; i < N; ++i)
		ans.data[i] = v1.data[i] * s;
	return ans;
}

template <typename T, int N>
inline Vector <T, N> operator/ (const Vector <T, N> &v1, const Vector <T, N> & v2) {
	Vector <T, N> ans;
	for (int i = 0; i < N; ++i)
		ans.data[i] = v1.data[i] / v2.data[i];
	return ans;
}

template <typename T, int N>
inline Vector <T, N> operator/ (const Vector <T, N> &v1, const T & s) {
	Vector <T, N> ans;
	for (int i = 0; i < N; ++i)
		ans.data[i] = v1.data[i] / s;
	return ans;
}

template <typename T, int N>
inline Vector <T, N> operator/ (const T &s, const Vector <T, N> & v1) {
	Vector <T, N> ans;
	for (int i = 0; i < N; ++i)
		ans.data[i] = s / v1.data[i];
	return ans;
}

template <typename T, int N>
inline bool operator== (const Vector <T, N> & v1, const Vector <T, N> & v2) {
	for (int i = 0; i < N; ++i)
		if (v1.data[i] != v2.data[i]) return false;
	return true;
}

/*** Dot products, etc ***/

template <typename T, int N>
inline T dot (const Vector <T, N> &v1, const Vector <T, N> &v2) {
	T ans = (T) 0.;
	for (int i = 0; i < N; ++i)
		ans += v1.data[i] * v2.data[i];
	return ans;
}

template <typename T, int N>
inline T absDot (const Vector <T, N> &v1, const Vector <T, N> &v2) {
	return fabsf(dot(v1, v2));
}


template <typename T, int N>
inline void Vector <T, N>::normalize() {
	T length = 0;
	for (int i = 0; i < N; ++i)
		length += data[i] * data[i];
	length = (T) sqrt(length);

	for (int i = 0; i < N; ++i)
		data[i] /= length;
	return;
}

inline Vec3 cross (const Vec3 &v1, const Vec3 &v2) {
	return Vec3 (	v1[1] * v2[2] - v2[1] * v1[2],
					v1[2] * v2[0] - v2[2] * v1[0],
					v1[0] * v2[1] - v2[0] * v1[1]);
}

inline Vec3 sphericalToCartesian(float theta, float phi) {
	float sinTheta = sin(theta);
	float sinPhi = sin(phi);
	float cosTheta = sqrt(1.0f - sinTheta*sinTheta);
	float cosPhi = sqrt(1.0f - sinPhi*sinPhi);
	return Vec3(cosTheta*sinPhi, sinTheta*sinPhi, cosPhi);
}
/*
template <typename T>
inline const T min (const T a, const T b) {
	return (a < b) ? a : b;
}

template <typename T>
inline const T max(const T a, const T b) {
	return (a > b) ? a : b;
}
*/
template <typename T>
inline const T& heavyMin (const T& a, const T& b) {
	return (a < b) ? a : b;
}

template <typename T>
inline const T& heavyMax(const T& a, const T& b) {
	return (a > b) ? a : b;
}


class BBox {
public:
	BBox(const Vec3& _v1, const Vec3& _v2) : vMin (_v1), vMax (_v2) {}
	BBox() : BBox(Vec3(MAXFLOAT), Vec3(-MAXFLOAT)) {}

	void enclose(const Vec3& v);
	void enclose(const BBox& bbox);
	
	bool intersect(const Ray &r, float &a, float &b) const;
	bool intersectYN(const Ray &r, float t0, float t1) const;

	Vec3 vMin, vMax;
};

#endif