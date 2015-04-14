#include "Material.hpp"
#include "ONB.hpp"
#include "Ray.hpp"

bool DiffuseMaterial::sample(const IntersectRec& ir, Vec3& wi, const Vec3& wo, Color& brdf, float& pdf, const Vec2& uv) const {
	//cout << "ir.normal: " << ir.normal << endl;
	// ONB onb = ONB::makeFromW(ir.normal);

	// TODO CLUE: make the ONB at the time of intersection.
	// thereafter, wi and wo should be defined with respect to ir.onb

	//cout << "onb: " << onb.U << "\t" << onb.V << "\t" << onb.W << endl;
	wi = ir.onb.cosineSampleHemisphere(uv[0], uv[1]);
	//wi = onb.local2world(lwi);
	
	//cout << "lwi: " << lwi << endl;
	//cout << "nr.d: " << nextRay.d << endl;
	brdf = c * (float) M_1_PI;
	pdf = ONB::absCosTheta(wi) * M_1_PI;
	return true;
}

float DiffuseMaterial::PDF(const Vec3& wi, const Vec3& wo) const {
	// presumes wi and wo are normalized and transformed into a coordinate system with normal = <0, 0, 1>
	return ONB::sameHemisphere(wi, wo) ? ONB::absCosTheta(wi) * M_1_PI : 0.f;
}

/*
bool DiffuseMaterial::sample(const IntersectRec& ir, Ray& nextRay, Color& brdf, float& pdf, const Vec2& uv) const {
	ONB onb = ONB::makeFromW(ir.normal);
	Vec3 lwi = onb.uniformSampleHemisphere(uv[0], uv[1]);
	nextRay.o = ir.isectPoint;
	nextRay.d = onb.local2world(lwi);
	brdf = c;
	pdf = onb.uniformSampleHemispherePDF();
	return true;
}*/


bool EmissiveMaterial::sample(const IntersectRec& ir, Vec3& wi, const Vec3& wo, Color& brdf, float& pdf, const Vec2& uv) const {
	wi = ir.onb.cosineSampleHemisphere(uv[0], uv[1]);
	//wi = onb.local2world(lwi);
	
	//cout << "lwi: " << lwi << endl;
	//cout << "nr.d: " << nextRay.d << endl;
	brdf = Color::White();
	pdf = ONB::absCosTheta(wi) * M_1_PI;
	return true;
}

float EmissiveMaterial::PDF(const Vec3& wi, const Vec3& wo) const {
	return ONB::sameHemisphere(wi, wo) ? ONB::absCosTheta(wi) * M_1_PI : 0.f;
}
