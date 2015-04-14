#ifndef __ACCELERATOR_HPP__
#define __ACCELERATOR_HPP__

class Ray;
class IntersectRec;

#include "Primitive.hpp"

class Accelerator : public Intersectable {
	virtual const bool intersect(const Ray& ray, IntersectRec& ir, const float tmin, const float tmax) const = 0;
	virtual const bool intersectYN(const Ray& ray, const float tmin, const float tmax) const = 0;
};

class BBoxOnlyAccelerator : public Accelerator {
	const bool intersect(const Ray& ray, IntersectRec& ir, const float tmin, const float tmax) const;
	const bool intersectYN(const Ray& ray, const float tmin, const float tmax) const;
};

#endif