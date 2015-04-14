#include "Draco2.hpp"
#include "Primitive.hpp"
#include "Scene.hpp"

using namespace std;

bool Scene::closestIntersection(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const {
	if (!bbox.intersectYN(ray, tmin, tmax)) return false;
	float localtmax = tmax;
	bool intersected = false;
	IntersectRec tir;
	//cout << "before intersect loop\n";
	for (const auto& pr : primitives) {
		//cout << "intersecting: " << pr->getType() << endl;

		if (pr->getBBox().intersectYN(ray, tmin, tmax) && pr->intersect(ray, tir, tmin, localtmax)) {
			//cout << "start inner loop\n";
			tir.primitivePtr = pr.get();
			localtmax = tir.t;
			ir = tir;
			intersected = true;
			//cout << "end inner loop\n";
		}
	}
	return intersected;
}

bool Scene::intersectYN(const Ray& ray, const float tmin, const float tmax) const {
	if (!bbox.intersectYN(ray, tmin, tmax)) return false;
	for (const auto& pr : primitives) {
		if (pr->getBBox().intersectYN(ray, tmin, tmax) && pr->intersectYN(ray, tmin, tmax)) {
			return true;
		}
	}
	return false;
}