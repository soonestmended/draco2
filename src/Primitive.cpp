#include "IntersectRec.hpp"
#include "Matrix.hpp"
#include "MonteCarlo.hpp"
#include "Primitive.hpp"

using namespace std;

const bool Surface::intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const {
	bool intersected = geometrySP->intersect(ray, ir, tmin, tmax);
	ir.materialPtr = materialSP.get();
	ir.primitivePtr = this;
	ir.isectPoint = ray.o + ray.d * ir.t;
	return intersected;
}

const bool Surface::intersectYN(const Ray& ray, float tmin, float tmax) const {
	return geometrySP->intersectYN(ray, tmin, tmax);
}

const Vec3 Surface::getRandomPoint(const Vec2& uv) const {
	return geometrySP->getRandomPoint(uv);
}

void Surface::getRandomPointAndNormal(Vec3& p, Vec3& n, const Vec2& uv) const {
	geometrySP->getRandomPointAndNormal(p, n, uv);
}

const float Surface::getSurfaceArea() const {
	return geometrySP->getSurfaceArea();
}


const bool TransformedSurface::intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const {
	Ray objectRay = transformSP->inverseTransformRay(ray);
	if (geometrySP->intersect(objectRay, ir, tmin, tmax)) {
		// any scale in the transformation was reflected in length of objectRay.d vector, which was
		// not renormalized. So the "t" returned by intersection works for the world space unit vector as well.
		// ir.isectPoint = ray.o + ray.d * ir.t;
		ir.normal = transformSP->transformNormal(ir.normal);
		ir.materialPtr = materialSP.get();
		ir.primitivePtr = this;
		//ir.normal = ir.isectPoint;
		return true;
	}
	return false;
}

const bool TransformedSurface::intersectYN(const Ray& ray, float tmin, float tmax) const {
	Ray objectRay = transformSP->inverseTransformRay(ray);
	return geometrySP->intersectYN(objectRay, tmin, tmax);
}

const Vec3 TransformedSurface::getRandomPoint(const Vec2& uv) const {
	return transformSP->inverseTransformPoint(geometrySP->getRandomPoint(uv));
}

void TransformedSurface::getRandomPointAndNormal(Vec3& p, Vec3& n, const Vec2& uv) const {
	Vec3 localP, localN;
	geometrySP->getRandomPointAndNormal(localP, localN, uv);
	p = transformSP->inverseTransformPoint(localP);
	n = transformSP->inverseTransformNormal(localN);
}

const float TransformedSurface::getSurfaceArea() const {
	// this is not straightforward to compute. maybe better to give some sort of estimate?
	return geometrySP->getSurfaceArea();
}

const bool Sphere::intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const {
	// NOTE that ray.d is not normalized for this calculation. Thus t can be compared against tmin, tmax wrt world coords.
	float a = dot(ray.d, ray.d);
	float b = 2.0 * dot(ray.d, ray.o);
	float c = dot(ray.o, ray.o) - 1.0;
	float discriminant = b*b - 4.0*a*c;
	if (discriminant >= 0.0) {
		// hit
		ir.t = (-b - sqrt(discriminant))/ (2.0*a);
		//float part2 = sqrt(discriminant) / (2.0*a);
		//ir.t = part1 - part2;
		if (ir.t >= tmin && ir.t <= tmax) {			
			ir.isectPoint = ir.normal = ray.o + ray.d*ir.t;
			return true;
		}
		/*else {
			ir.t = part1 + part2;
			if (ir.t >= tmin && ir.t <= tmax) {
				ir.normal = ray.o + ray.d*ir.t;
				return true;
			}
		}*/
	}
	return false;
}

const bool Sphere::intersectYN(const Ray& ray, float tmin, float tmax) const {
	// NOTE that ray.d is not normalized for this calculation. Thus t can be compared against tmin, tmax wrt world coords.
	float a = dot(ray.d, ray.d);
	float b = 2.0 * dot(ray.d, ray.o);
	float c = dot(ray.o, ray.o) - 1.0;
	return (b*b - 4.0*a*c >= 0.0);
}

const Vec3 Sphere::getRandomPoint(const Vec2& uv) const {
	float theta = M_TWICE_PI * uv[0];
	float phi = acos(2.f*uv[1]-1.f);
	return sphericalToCartesian(theta, phi);
}

void Sphere::getRandomPointAndNormal(Vec3& p, Vec3& n, const Vec2& uv) const {
	n = p = getRandomPoint(uv);
}

const float Sphere::getSurfaceArea() const {
	return 4.f * M_PI;
}

const BBox Sphere::bbox = BBox(Vec3(-1.0f), Vec3(1.0f));

const void CSGUnion::makeBBox() {
	for (auto it = primitiveSPvector.begin(); it != primitiveSPvector.end(); ++it) {
		bbox.enclose((*it)->getBBox());
	}
	if (transformSP) bbox = transformSP->transformBBox(bbox);
}

const bool CSGUnion::intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const {
	// intersects union if it intersects any primitive
	Ray tRay = ray;
	if (transformSP) {
		transformSP->inverseTransformRay(tRay);
	}


	float localtmax = tmax;
	bool intersected = false;
	IntersectRec tir;
	for (const auto& pr : primitiveSPvector) {
		if (pr->intersect(ray, tir, tmin, localtmax)) {
			localtmax = tir.t;
			ir = tir;
			ir.primitivePtr = this;
			ir.materialPtr = pr->getMaterial().get();
			if (!ir.materialPtr) ir.materialPtr = this->defaultMaterialSP.get();
			intersected = true;
		}
	}
	if (transformSP) {
		transformSP->inverseTransformNormal(ir.normal);
	}
	return intersected;
}

const bool CSGUnion::intersectYN(const Ray& ray, float tmin, float tmax) const {
	for (const auto& pr : primitiveSPvector) {
		if (pr->intersectYN(ray, tmin, tmax)) {
			return true;
		}
	}
	return false;
}


void Triangle::init() {
	bbox.enclose(v0);
	bbox.enclose(v1);
	bbox.enclose(v2);
	e0 = v1 - v0;
	e1 = v2 - v0;
	n = Vec3::normalize(cross(e0, e1));
}

const bool Triangle::intersect(const Ray& ray, IntersectRec& ir, float tmin, float tmax) const {

  Vec3 P, Q, T;
  float det, inv_det, u, v;
  float t;

  //Begin calculating determinant - also used to calculate u parameter
  P = cross(ray.d, e1);
 
  //if determinant is near zero, ray lies in plane of triangle
  det = dot(e0, P);

  //NOT CULLING
  if(det > -EPSILON && det < EPSILON) return false;
  inv_det = 1.f / det;
 
  //calculate distance from V0 to ray origin
  T = ray.o - v0;
 
  //Calculate u parameter and test bound
  u = dot(T, P) * inv_det;
  
  //The intersection lies outside of the triangle
  if(u < 0.f || u > 1.f) return false;
 
  //Prepare to test v parameter
  Q = cross(T, e0);
 
  //Calculate V parameter and test bound
  v = dot(ray.d, Q) * inv_det;

  //The intersection lies outside of the triangle
  if(v < 0.f || u + v  > 1.f) return false;
 
  t = dot(e1, Q) * inv_det;
 
  if(t >= tmin && t <= tmax) { //ray intersection
    ir.t = t;
    ir.normal = n;
    //std::cout << ir.normal << std::endl;
    return true;
  }
 
  // No hit, no win
  return false;
}

const bool Triangle::intersectYN(const Ray& ray, float tmin, float tmax) const {

	Vec3 P, Q, T;
	float det, inv_det, u, v;
	float t;

	//Begin calculating determinant - also used to calculate u parameter
	P = cross(ray.d, e1);

	//if determinant is near zero, ray lies in plane of triangle
	det = dot(e0, P);

	//NOT CULLING
	if(det > -EPSILON && det < EPSILON) return false;
	inv_det = 1.f / det;

	//calculate distance from V0 to ray origin
	T = ray.o - v0;

	//Calculate u parameter and test bound
	u = dot(T, P) * inv_det;

	//The intersection lies outside of the triangle
	if(u < 0.f || u > 1.f) return false;

	//Prepare to test v parameter
	Q = cross(T, e0);

	//Calculate V parameter and test bound
	v = dot(ray.d, Q) * inv_det;

	//The intersection lies outside of the triangle
	if(v < 0.f || u + v  > 1.f) return false;

	t = dot(e1, Q) * inv_det;

	if(t >= tmin && t <= tmax) { //ray intersection
		return true;
	}

	// No hit, no win
	return false;
}

const Vec3 Triangle::getRandomPoint(const Vec2& uv) const {
	Vec2 bg;
	uniformSampleTriangle(uv, bg);
	Vec3 p = bg[0] * v0 + bg[1] * v1 + (1.f - bg[0] - bg[1]) * v2;
	return p;
}

void Triangle::getRandomPointAndNormal(Vec3& p, Vec3& normal, const Vec2& uv) const {
	// TODO
	p = getRandomPoint(uv);
	normal = this->n;
}

const float Triangle::getSurfaceArea() const {
	// Heron's method
	float a = e0.magnitude();
	float b = e1.magnitude();
	float c = (v2-v1).magnitude();
	float s = 0.5 * (a+b+c);
	return sqrtf(s*(s-a)*(s-b)*(s-c));
}