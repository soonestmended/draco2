#ifndef __RAY_HPP__
#define __RAY_HPP__

#include "Vector.hpp"

class Ray {
public:
	Ray() {}

	Ray(const Vec3& _o, const Vec3& _d) : o(_o), d(_d) {
		inv_d = Vec3(1/d[0], 1/d[1], 1/d[2]);
		sign[0] = (inv_d[0] < 0);
		sign[1] = (inv_d[1] < 0);
		sign[2] = (inv_d[2] < 0);
	}

	const Ray operator- (void) const {
		return Ray(this->o, -this->d);
	} 
	
	void normalize() {
		*this = Ray(o, Vec3::normalize(d));
	}

	Vec3 o, d, inv_d;
	unsigned char sign[3];
};

std::ostream& operator<<(std::ostream& os, const Ray& ray);	

#endif