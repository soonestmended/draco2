#include "Draco2.hpp"
#include "Ray.hpp"
#include "Vector.hpp"



void BBox::enclose(const Vec3& v) {
	for (int i = 0; i < 3; ++i) {
		vMin[i] = fmin(vMin[i], v[i]);
		vMax[i] = fmax(vMax[i], v[i]);
	}
}

void BBox::enclose(const BBox& bbox) {
	enclose(bbox.vMin);
	enclose(bbox.vMax);
}

bool BBox::intersect(const Ray &r, float &a, float &b) const {
	float tymin, tymax, tzmin, tzmax;
	
	const Vec3 *bounds[2] = {&this->vMin, &this->vMax};

	a = ((*bounds[r.sign[0]])[0] - r.o[0]) * r.inv_d[0];
	b = ((*bounds[1-r.sign[0]])[0] - r.o[0]) * r.inv_d[0];

	tymin = ((*bounds[r.sign[1]])[1] - r.o[1]) * r.inv_d[1];
	tymax = ((*bounds[1-r.sign[1]])[1] - r.o[1]) * r.inv_d[1];

	if ( (a > tymax) || (tymin > b) )
		return false;

	if (tymin > a)
		a = tymin;

	if (tymax < b) 
		b = tymax;

	tzmin = ((*bounds[r.sign[2]])[2] - r.o[2]) * r.inv_d[2];
	tzmax = ((*bounds[1-r.sign[2]])[2] - r.o[2]) * r.inv_d[2];

	if ( (a > tzmax) || (tzmin > b) )
		return false;

	if (tzmin > a)
		a = tzmin;

	if (tzmax < b)
		b = tzmax;
	
	return true;
}

bool BBox::intersectYN(const Ray &r, float t0, float t1) const {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	
	const Vec3 *bounds[2] = {&this->vMin, &this->vMax};

	tmin = ((*bounds[r.sign[0]])[0] - r.o[0]) * r.inv_d[0];
	tmax = ((*bounds[1-r.sign[0]])[0] - r.o[0]) * r.inv_d[0];

	tymin = ((*bounds[r.sign[1]])[1] - r.o[1]) * r.inv_d[1];
	tymax = ((*bounds[1-r.sign[1]])[1] - r.o[1]) * r.inv_d[1];

	if ( (tmin > tymax) || (tymin > tmax) )
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax) 
		tmax = tymax;

	tzmin = ((*bounds[r.sign[2]])[2] - r.o[2]) * r.inv_d[2];
	tzmax = ((*bounds[1-r.sign[2]])[2] - r.o[2]) * r.inv_d[2];

	if ( (tmin > tzmax) || (tzmin > tmax) )
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return ( (tmin < t1) && (tmax > t0) );
}