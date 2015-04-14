#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <boost/any.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Vector.hpp"

enum CSGType {CSG_UNION, CSG_DIFFERENCE, CSG_INTERSECTION, CSG_MERGE};
enum LightType {LIGHT_SURFACE};
enum MaterialType {MATERIAL_DIFFUSE, MATERIAL_EMISSIVE};
enum ShapeType {ST_CONE, ST_CYLINDER, ST_PLANE, ST_SPHERE, ST_TORUS, ST_TRIANGLE};

class Camera;
class Light;
class Material;
class Primitive;
class Scene;

typedef std::pair <std::string, boost::any> Param;

class ParamSet {
public:

	ParamSet() : params () {}
	~ParamSet() {}

	// add a parameter. copies are made of passed arguments.
	void addParam(std::string name, boost::any a) {
		params.push_back(Param(name, a));
	}

	void addParam(Param *p) {
		params.push_back(*p);
	}

	void joinParamSet(ParamSet *ps);

	void clear();

	void print() const;

	std::vector <Param> params;
};

class Parser {
public:
	Parser() = delete;
	Parser(Scene& _scene, Camera& _camera) : scene (_scene), camera (_camera) {}

	bool addPrimitive(const ParamSet *params) const;
	bool addCamera(const ParamSet *params) const;
	bool addCSG(int csgt, const ParamSet *params) const;
	bool addLight(const ParamSet *params) const;

	Scene& scene;
	Camera& camera;

private:
	std::shared_ptr <Material> parseMaterial(ParamSet *ps) const;
	std::shared_ptr <Material> makeDiffuseMaterial(const ParamSet *params) const;
	std::shared_ptr <Material> makeEmissiveMaterial(const ParamSet *params) const;

	std::shared_ptr <Light> makeLight(const ParamSet *params, const int lt) const;
	std::shared_ptr <Light> makeSurfaceLight(const ParamSet *params) const;

	std::shared_ptr <Primitive> makePrimitive(const ParamSet *ps) const;
	std::shared_ptr <Primitive> makeCone(const ParamSet *params) const;
	std::shared_ptr <Primitive> makeSphere(const ParamSet *params) const;
	std::shared_ptr <Primitive> makeTriangle(const ParamSet *params) const;

	std::shared_ptr <Primitive> makeUnion(const ParamSet *params) const;

	


	const Vec3 makeVec3(const boost::any& a) const;

};

extern void yyerror (const Parser*, char*);


#endif