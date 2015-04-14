#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include <memory>

#include "Color.hpp"
#include "Primitive.hpp"

class Light {
public:
	Light() = delete;
	Light(const Color& _color, float _power) : color (_color), power (_power) {}
	virtual const Vec3 getRandomPoint(const Vec2& uv) const = 0;
	virtual void getRandomPointAndNormal(Vec3& p, Vec3& n, const Vec2& uv) const = 0;
 	virtual const float getPower() const {return power;}
	virtual Color getColor() const {return color;}
	virtual bool canIntersect() const {return false;}

	virtual Color sample(const IntersectRec& ir, Vec3& wi, const Vec3& wo, float& lightPDF, float& lightDist, const Vec2& uv) const = 0;
	virtual Color lightEmitted(const Vec3& p, const Vec3& n, const Vec3& dir) const = 0;
	virtual float PDF(const IntersectRec& lir, const Ray& ray) const = 0;

	virtual const Primitive* getPrimitive() const {return nullptr;}

protected:
	Color color;
	float power;
};

class SurfaceLight : public Light {
public:
	SurfaceLight() = delete;
	SurfaceLight(const Surface* _surfacePtr, const Color& _color, float _power) : Light(_color, _power), surfacePtr (_surfacePtr) {
		//power *= surfacePtr->getSurfaceArea();
	}

	const Vec3 getRandomPoint(const Vec2 &uv) const {
		return surfacePtr->getRandomPoint(uv);
	}

	void getRandomPointAndNormal(Vec3& p, Vec3& n, const Vec2& uv) const {
		surfacePtr->getRandomPointAndNormal(p, n, uv);
	}

 	const float getPower() const {
 		// need to account for surface area change if the transform does scaling.
 		return power;
 	}
	
	const Primitive* getPrimitive() const {return (Primitive *) surfacePtr;}
	bool canIntersect() const {return true;}

	Color sample(const IntersectRec& ir, Vec3& wi, const Vec3& wo, float& lightPDF, float& lightDist, const Vec2& uv) const;
	Color lightEmitted(const Vec3& p, const Vec3& n, const Vec3& dir) const;
	float PDF(const IntersectRec& lir, const Ray& ray) const;


private:
	const Surface* surfacePtr;
};

#endif