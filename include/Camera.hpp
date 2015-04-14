#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Ray.hpp"
#include "Vector.hpp"

class Camera {
public:
	Camera() {}

	Camera(const Vec3& _eye, const Vec3& _look, const Vec3& _up, const Vec3& _right, float _s) :
		eye (_eye), look (_look), up (_up), right (_right), s (_s) {}

	// Generate ray through the image plane, which goes from -cam.right/2 --> +cam.right/2
	// width of image plane is cam.right.magnitude; height is cam.up.magnitude
	// whole image plane is covered by sampling [-0.5, 0.5] x [-0.5, 0.5]
	virtual Ray getRay(const Vec2& center) const {
		return Ray(eye, Vec3::normalize(center[0] * right + center[1] * up + s * look));
	}

protected:
	Vec3 eye, look, up, right;
	float s;	// distance to image plane
								
};

#endif