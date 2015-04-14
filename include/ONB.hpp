#ifndef __ONB_HPP__
#define __ONB_HPP__

#include "Draco2.hpp"
#include "Vector.hpp"

using namespace std;

class ONB {
public:
	ONB()
		: U ({1.0, 0.0, 0.0}), V ({0.0, 1.0, 0.0}), W ({0.0, 0.0, 1.0}) {}
	
	ONB(const Vec3& _U, const Vec3& _V, const Vec3& _W)
		: U (_U), V (_V), W(_W) {}

	static ONB makeFromU(const Vec3& _U);
	static ONB makeFromV(const Vec3& _V);
	static ONB makeFromW(const Vec3& _W);

	static ONB makeFromUV(const Vec3& _U, const Vec3& _V);
	static ONB makeFromVU(const Vec3& _V, const Vec3& _U);
	static ONB makeFromUW(const Vec3& _U, const Vec3& _W);
	static ONB makeFromWU(const Vec3& _W, const Vec3& _U);
	static ONB makeFromVW(const Vec3& _V, const Vec3& _W);
	static ONB makeFromWV(const Vec3& _W, const Vec3& _V);

	static Vec3 uniformSampleHemisphere(float u, float v);
	static float uniformSampleHemispherePDF();

	static Vec3 cosineSampleHemisphere(float u, float v);
	static float cosineSampleHemispherePDF(float costheta, float phi);

	static void uniformSampleDisk(float u1, float u2, float *x, float *y);
	static void concentricSampleDisk(float u1, float u2, float *dx, float *dy);

	static bool sameHemisphere(const Vec3& wi, const Vec3& wo);
	Vec3 makeVec(const float u, const float v, const float w) const;
	Vec3 makeUnitVec(const float u, const float v, const float w) const;
	Vec3 local2world(const Vec3& v) const;
	Vec3 world2local(const Vec3& v) const;
	static float cosTheta(const Vec3& w);
	static float absCosTheta(const Vec3& w);
	static float sinTheta2(const Vec3& w);
	static float sinTheta(const Vec3& w);
	static float cosPhi(const Vec3& w);
	static float sinPhi(const Vec3& w);

	Vec3 U, V, W;
};


inline Vec3 ONB::makeVec(const float u, const float v, const float w) const {
	return u*U + v*V + w*W;
}

inline Vec3 ONB::makeUnitVec(const float u, const float v, const float w) const {
	return Vector <float, 3>::normalize(u*U + v*V + w*W);
}

inline Vec3 ONB::world2local(const Vec3& v) const {
	return Vec3(dot(v, U), dot(v, V), dot(v, W));
}

inline Vec3 ONB::local2world(const Vec3& v) const {
	return Vec3(U[0]*v[0] + V[0]*v[1] + W[0]*v[2],
				U[1]*v[0] + V[1]*v[1] + W[1]*v[2],
				U[2]*v[0] + V[2]*v[1] + W[2]*v[2]);				
}

inline float ONB::cosTheta(const Vec3& w) {return w[2];}

inline float ONB::absCosTheta(const Vec3& w) {return fabsf(w[2]);}

inline float ONB::sinTheta2(const Vec3& w) {return fmax(0.f, 1.f - cosTheta(w)*cosTheta(2));}

inline float ONB::sinTheta(const Vec3& w) {return sqrtf(sinTheta2(w));}

inline float ONB::cosPhi(const Vec3& w) {
	float sintheta = sinTheta(w);
	if (sintheta == 0.f) return 1.f;
	return clamps(w[1] / sintheta, -1.f, 1.f);
}

inline float ONB::sinPhi(const Vec3& w) {
	float sintheta = sinTheta(w);
	if (sintheta == 0.f) return 0.f;
	return clamps(w[1] / sintheta, -1.f, 1.f);
}

inline bool ONB::sameHemisphere(const Vec3& wi, const Vec3& wo) {
	return wi[2] * wo[2] > 0.f;
}

#endif