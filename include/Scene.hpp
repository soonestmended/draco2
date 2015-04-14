#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <vector>

#include "IntersectRec.hpp"
#include "Ray.hpp"
#include "Vector.hpp"

class Accelerator;
class Light;
class Material;
class Primitive;

class Scene {
public:
	Scene() : acceleratorSP (nullptr), bbox() {}

	// Functions to add Lights
	bool addLight(const std::shared_ptr <Light>& _lightSP) {
		lights.push_back(_lightSP);
		return true;
	}

	// Functions to add Materials
	bool addMaterial(const std::shared_ptr <Material>& _materialSP) {
		materials.push_back(_materialSP);
		return true;
	}
	
	bool addPrimitive(const std::shared_ptr <Primitive>& _primitiveSP) {
		assert (_primitiveSP != nullptr);
		primitives.push_back(_primitiveSP);
		updateBBox(_primitiveSP.get());
		return true;
	}

	bool closestIntersection(const Ray& ray, IntersectRec& ir, const float tmin, const float tmax) const;
	bool intersectYN(const Ray& ray, const float tmin, const float tmax) const;

	const std::vector <std::shared_ptr <Light>> getLights() const {
		return lights;
	}

private:
	std::shared_ptr <Accelerator> acceleratorSP;
	std::vector <std::shared_ptr <Light>> lights;
	std::vector <std::shared_ptr <Material>> materials;
	std::vector <std::shared_ptr <Primitive>> primitives; 
	// note: transforms just take care of themselves. No list needed here because we don't have to loop over transforms
	void updateBBox(const Primitive *p) {
		bbox.enclose(p->getBBox());
	}
	BBox bbox;
};


#endif