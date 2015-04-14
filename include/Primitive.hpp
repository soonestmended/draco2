#ifndef __PRIMITIVE_HPP__
#define __PRIMITIVE_HPP__

#include <memory>
#include <vector>

#include "Draco2.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"
#include "Vector.hpp"

class IntersectRec;
class Material;

class Intersectable {
public:
	virtual const bool intersect(const Ray& ray, IntersectRec& ir, const float tmin, const float tmax) const = 0;
	virtual const bool intersectYN(const Ray& ray, const float tmin, const float tmax) const = 0;
};

class ReportsBounds {
	virtual const BBox& getBBox() const = 0;
};

class SurfaceInfo {
	virtual const Vec3 getRandomPoint(const Vec2& uv) const = 0;
	virtual void getRandomPointAndNormal(Vec3& p, Vec3& n, const Vec2& uv) const = 0;
	virtual const float getSurfaceArea() const = 0;
};

class Geometry : public Intersectable, public SurfaceInfo, public ReportsBounds {
public:
	virtual const bool intersect(const Ray& ray, IntersectRec& ir, const float tmin, const float tmax) const = 0;
	virtual const bool intersectYN(const Ray& ray, const float tmin, const float tmax) const = 0;
	
	virtual const Vec3 getRandomPoint(const Vec2& uv) const = 0;
	virtual void getRandomPointAndNormal(Vec3& p, Vec3& n, const Vec2& uv) const = 0;
	virtual const float getSurfaceArea() const = 0;
	
	virtual const BBox& getBBox() const = 0;
	virtual const std::string getType() const {return std::string("Geometry");}
};

class HasMaterial {
public:
	virtual const std::shared_ptr <Material>& getMaterial() const = 0;
};

class Primitive : public Intersectable, public HasMaterial, public ReportsBounds {

public:
	virtual ~Primitive() {}
	virtual const bool intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const = 0;
	virtual const bool intersectYN(const Ray& ray, float tmin, float tmax) const = 0;
	virtual const std::shared_ptr <Material>& getMaterial() const = 0;
	virtual const bool isLight() const {return false;}
	virtual const BBox& getBBox() const = 0;
	virtual const std::string getType() const = 0;
};

class CSG : public Primitive {
public:
	CSG (std::vector <std::shared_ptr <Primitive>>& _primitiveSPvector, const std::shared_ptr <Material>& _defaultMaterialSP, const std::shared_ptr <Transform>& _transformSP) 
	: primitiveSPvector (_primitiveSPvector), defaultMaterialSP (_defaultMaterialSP), transformSP (_transformSP), bbox () {}
	CSG() = delete;
	virtual ~CSG() {}
	
	virtual const bool intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const = 0;
	virtual const bool intersectYN(const Ray& ray, float tmin, float tmax) const = 0;

	const std::shared_ptr <Material>& getMaterial() const {return defaultMaterialSP;}
	const BBox& getBBox() const {return bbox;}

	virtual const std::string getType() const = 0;

protected:
	BBox bbox;
	virtual const void makeBBox() = 0;
	const std::shared_ptr <Transform> transformSP;
	std::vector <std::shared_ptr <Primitive>> primitiveSPvector;
	std::shared_ptr <Material> defaultMaterialSP;
};

class CSGUnion : public CSG {
public:
	CSGUnion (std::vector <std::shared_ptr <Primitive>>& _primitiveSPvector, const std::shared_ptr <Material>& _defaultMaterialSP, const std::shared_ptr <Transform>& _transformSP) 
	: CSG(_primitiveSPvector, _defaultMaterialSP, _transformSP) {makeBBox();}
	CSGUnion() = delete;
	virtual ~CSGUnion() {}
	
	const bool intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const;
	const bool intersectYN(const Ray& ray, float tmin, float tmax) const;

	const std::string getType() const {return "CSG union";}

protected:
	const void makeBBox();
};

class Surface : public Primitive, public SurfaceInfo {

/* Describes a Primitive with a Material. */

public:
	Surface (const std::shared_ptr <Geometry>& _geometrySP, const std::shared_ptr <Material>& _materialSP, const bool _lightYN = false) 
	: geometrySP (_geometrySP), materialSP (_materialSP), lightYN(_lightYN) {}
	Surface() = delete;
	virtual ~Surface() {}
	
	virtual const bool intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const;
	virtual const bool intersectYN(const Ray& ray, float tmin, float tmax) const;

	virtual const Vec3 getRandomPoint(const Vec2& uv) const;
	virtual void getRandomPointAndNormal(Vec3& p, Vec3& n, const Vec2& uv) const;
	virtual const float getSurfaceArea() const;

	virtual const std::shared_ptr <Material>& getMaterial() const {return materialSP;}
	virtual const BBox& getBBox() const {return geometrySP->getBBox();}

	virtual const std::string getType() const {return std::string("Surface: ") + geometrySP->getType();}

	virtual const bool isLight() const {return lightYN;}


//protected:
	const std::shared_ptr <Geometry> geometrySP;
	const std::shared_ptr <Material> materialSP;
	const bool lightYN;
};

class TransformedSurface : public Surface {
public:
	TransformedSurface(const std::shared_ptr <Geometry>& _geometrySP, const std::shared_ptr <Material>& _materialSP, const std::shared_ptr <Transform>& _transformSP) 
	 : Surface (_geometrySP, _materialSP), transformSP(_transformSP), bbox (transformSP->transformBBox(geometrySP->getBBox())) {}
	TransformedSurface() = delete;
	virtual ~TransformedSurface() {}

	const Vec3 getRandomPoint(const Vec2& uv) const;
	void getRandomPointAndNormal(Vec3& p, Vec3& n, const Vec2& uv) const;
	const float getSurfaceArea() const;

	const bool intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const;
	const bool intersectYN(const Ray& ray, float tmin, float tmax) const;
	const BBox& getBBox() const {return bbox;}

	const Transform& getTransform() {return *transformSP;}

	const std::string getType() {return "Transformed Surface";}

private:
	const std::shared_ptr <Transform> transformSP;
	const BBox bbox;
};

class Sphere : public Geometry {

// TODO: fix constructors.

public:
	Sphere() {}
	virtual ~Sphere() {}
	const bool intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const;
	const bool intersectYN(const Ray& ray, float tmin, float tmax) const;	
	const Vec3 getRandomPoint(const Vec2& uv) const;
	void getRandomPointAndNormal(Vec3& p, Vec3& n, const Vec2& uv) const;
	const float getSurfaceArea() const;
	const BBox& getBBox() const {return bbox;}

	const static BBox bbox;

	const std::string getType() const {return "Sphere";}
};

class Triangle : public Geometry {

// TODO: fix constructors.

public:
	Triangle() : v0 ({-2.0, 0.0, 0.0}), v1 ({2.0, 0.0, 0.0}), v2 ({0.0, 2.0, 0.0}), bbox () {
		init();
	}
	Triangle(Vec3 _v0, Vec3 _v1, Vec3 _v2) : v0 (_v0), v1 (_v1), v2 (_v2), bbox () {
		init();
	}
	virtual ~Triangle() {}
	const bool intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const;
	const bool intersectYN(const Ray& ray, float tmin, float tmax) const;	
	const Vec3 getRandomPoint(const Vec2& uv) const;
	void getRandomPointAndNormal(Vec3& p, Vec3& n, const Vec2& uv) const;
	const float getSurfaceArea() const;
	const BBox& getBBox() const {return bbox;}

	const std::string getType() const {return "Triangle";}

//private:
	void init();
	Vec3 v0, v1, v2;
	Vec3 e0, e1;
	Vec3 n;
	BBox bbox;
};

#endif