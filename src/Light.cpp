#include "Draco2.hpp"
#include "IntersectRec.hpp"
#include "Light.hpp"

Color SurfaceLight::sample(const IntersectRec& ir, Vec3& wi, const Vec3& wo, float& lightPDF, float& lightDist, const Vec2& uv) const {
	// sample a direction to the light based on surface intersection in ir.

	// get a random point on the light
	Vec3 p, n;
	Color ans;
	surfacePtr->getRandomPointAndNormal(p, n, uv);

	wi = p - ir.isectPoint; // world coordinates
	float distSQ = wi.magnitudeSQ();
	lightDist = sqrt(distSQ);
	wi /= lightDist;

	// convert light sample weight to solid angle measure
	lightPDF = distSQ / (absDot(n, -wi) * surfacePtr->getSurfaceArea());
	ans = lightEmitted(p, n, -wi);

	// convert back to ir.onb coordinates
	wi = ir.onb.world2local(wi);
	return ans;
}

Color SurfaceLight::lightEmitted(const Vec3& p, const Vec3& n, const Vec3& dir) const {
	//cout << power << endl;
	return (dot(dir, n) > 0) ? (color * power)  : Color::Black();
}

float SurfaceLight::PDF(const IntersectRec& lir, const Ray& ray) const {
	float distSQ = (lir.isectPoint - ray.o).magnitudeSQ();
	return distSQ / (absDot(lir.normal, -ray.d / sqrtf(distSQ)) * surfacePtr->getSurfaceArea());
}
