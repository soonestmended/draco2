#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "Color.hpp"
#include "IntersectRec.hpp"
#include "Vector.hpp"

class IntersectRec;
class Ray;

class Material {
public:
	virtual Color getBRDF() const {return Color::Black();}
	virtual Color getBRDF(const IntersectRec& ir, const Vec3& wi, const Vec3& wo) const {return Color::Black();}

	virtual Color getAmbient() const {return Color::Black();}
	virtual Color getDiffuse() const {return Color::Black();}
	virtual Color getSpecular() const {return Color::Black();}

	virtual bool transmits() const {return false;}
	virtual Color getTransmission() const {return Color::Black();}
	virtual bool emits() const {return false;}
	virtual Color getEmission() const {return Color::Black();}

	virtual bool sample(const IntersectRec& ir, Vec3& wi, const Vec3& wo, Color& brdf, float& pdf, const Vec2& uv) const = 0;
	virtual float PDF(const Vec3& wi, const Vec3& wo) const = 0;
};

class DiffuseMaterial : public Material {
public:
	DiffuseMaterial(const Color &_c) 
		: c (_c) {}

	Color getBRDF(/* needs arguments */) const {return c;}
	Color getBRDF(const IntersectRec& ir, const Vec3& wi, const Vec3& wo) const {return c;}

	// virtual Color getAmbient() {return Color::Black();}
	Color getDiffuse() const {return c;}
	// virtual Color getSpecular() {return Color::Black();}

	// virtual bool transmits() {return false;}
	// virtual Color getTransmission() {return Color::Black();}
	// virtual bool emits() {return false;}
	// virtual Color getEmission() {return Color::Black();}
	bool sample(const IntersectRec& ir, Vec3& wi, const Vec3& wo, Color& brdf, float& pdf, const Vec2& uv) const;
	float PDF(const Vec3& wi, const Vec3& wo) const;

private:
	Color c;
};

class EmissiveMaterial : public Material {
public:
	EmissiveMaterial(const Color &_c) 
		: c (_c) {}

	virtual Color getBRDF(/* needs arguments */) const {return Color(Vec3(0.0, 1.0, 0.0));}

	// virtual Color getAmbient() {return Color::Black();}
	// virtual Color getDiffuse() const {return c;}
	// virtual Color getSpecular() {return Color::Black();}

	// virtual bool transmits() {return false;}
	// virtual Color getTransmission() {return Color::Black();}
	virtual bool emits() const {return true;}
	virtual Color getEmission() const {return c;}
	bool sample(const IntersectRec& ir, Vec3& wi, const Vec3& wo, Color& brdf, float& pdf, const Vec2& uv) const;
	float PDF(const Vec3& wi, const Vec3& wo) const;

private:
	Color c;
};

// todo: 

#endif