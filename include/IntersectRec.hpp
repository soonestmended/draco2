#ifndef __INTERSECTREC_HPP__
#define __INTERSECTREC_HPP__

#include <memory>

#include "ONB.hpp"
#include "Vector.hpp"

class Material;
class Primitive;

class IntersectRec {
public:
	Vec3 isectPoint;
	Vec3 normal;
	float t;
	ONB onb;
	const Primitive* primitivePtr;
	const Material* materialPtr;

	void finish() {
		onb = ONB::makeFromW(normal);
	}
};


#endif