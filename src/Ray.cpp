#include "Ray.hpp"

std::ostream& operator<<(std::ostream& os, const Ray& ray) {
	os << ray.o << " --> " << ray.d;
	return os;
}