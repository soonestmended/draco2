#include "Accelerator.hpp"

const bool BBoxOnlyAccelerator::intersect(const Ray& ray, IntersectRec& ir, const float tmin, const float tmax) const {
	return false;
}

const bool BBoxOnlyAccelerator::intersectYN(const Ray& ray, const float tmin, const float tmax) const {
	return false;
}
