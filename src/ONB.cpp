#include "Draco2.hpp"
#include "ONB.hpp"

std::ostream& operator<<(std::ostream& os, const ONB& uvw) {
	os << "U:\t" << uvw.U << endl;
	os << "V:\t" << uvw.V << endl;
	os << "W:\t" << uvw.W << endl;
	return os;
}

ONB ONB::makeFromU(const Vec3& _U) {
	ONB uvw;
	/* From Shirley:
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0, 1.0f, 0.0f);
	uvw.U = Vector <float, 3>  ::normalize(_U);
	uvw.V = cross(uvw.U, n);
	if (uvw.V.magnitudeSQ() < EPSILON) {
		uvw.V = cross(uvw.U, m);
	}
	uvw.W = cross(uvw.U, uvw.V);
	*/

	// From me:
	uvw.U = Vec3::normalize(_U);
	if (_U[2] > 0.0 || _U[1] > 0.0)
		uvw.V = Vec3::normalize({0.0, -_U[2], _U[1]});
	else 
		uvw.V = Vec3::normalize({-_U[1], _U[0], 0.0});
	uvw.W = cross(uvw.U, uvw.V);

	return uvw;
}

ONB ONB::makeFromV(const Vec3& _V) {
	ONB uvw;
	/*
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0, 1.0f, 0.0f);
	uvw.V = Vec3::normalize(_V);
	uvw.U = cross(uvw.V, n);
	if (uvw.U.magnitudeSQ() < EPSILON) {
		uvw.U = cross(uvw.V, m);
	}
	uvw.W = cross(uvw.U, uvw.V);
	*/
	uvw.V = Vec3::normalize(_V);
	if (_V[2] > 0.0 || _V[1] > 0.0)
		uvw.W = Vec3::normalize({0.0, -_V[2], _V[1]});
	else 
		uvw.W = Vec3::normalize({-_V[2], 0.0, _V[0]});
	uvw.U = cross(uvw.V, uvw.W);
	return uvw;
}

ONB ONB::makeFromW(const Vec3& _W) {
	ONB uvw;
/*
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0, 1.0f, 0.0f);
	uvw.W = Vec3::normalize(_W);
	uvw.U = cross(uvw.W, n);
	if (uvw.U.magnitudeSQ() < EPSILON) {
		uvw.U = cross(uvw.W, m);
	}
	uvw.V = cross(uvw.W, uvw.U);
*/

	/*
	uvw.W = _W;
	if (_W[2] > 0.0 || _W[1] > 0.0)
		uvw.U = Vec3::normalize({0.0, -_W[2], _W[1]});
	else 
		uvw.U = Vec3::normalize({_W[2], 0.0, -_W[0]});
	uvw.V = cross(uvw.W, uvw.U);
*/
	uvw.W = _W;
	if (fabsf(_W[0]) > fabsf(_W[1])) {
		float invLen = 1.f / sqrtf(_W[0]*_W[0] + _W[2]*_W[2]);
		uvw.U = Vec3(-_W[2]*invLen, 0.f, _W[0]*invLen);
	} else {
		float invLen = 1.f / sqrtf(_W[1]*_W[1] + _W[2]*_W[2]);
		uvw.U = Vec3(0.f, _W[2]*invLen, -_W[1]*invLen);
	}
	uvw.V = cross(uvw.U, uvw.W);
	return uvw;

}

ONB ONB::makeFromUV(const Vec3& _U, const Vec3& _V) {
	ONB uvw;
	uvw.U = Vec3::normalize(_U);
	uvw.V = Vec3::normalize(_V - dot(_V, uvw.U) * uvw.U);
	uvw.W = cross(uvw.U, uvw.V);
	return uvw;
}

ONB ONB::makeFromVU(const Vec3& _V, const Vec3& _U) {
	ONB uvw;
	uvw.V = Vec3::normalize(_V);
	uvw.U = Vec3::normalize(_U - dot(_U, uvw.V) * uvw.V);
	uvw.W = cross(uvw.U, uvw.V);
	return uvw;
}

ONB ONB::makeFromUW(const Vec3& _U, const Vec3& _W) {
	ONB uvw;
	uvw.U = Vec3::normalize(_U);
	uvw.W = Vec3::normalize(_W - dot(_W, uvw.U) * uvw.U);
	uvw.V = cross(uvw.W, uvw.U);
	return uvw;
}

ONB ONB::makeFromWU(const Vec3& _W, const Vec3& _U) {
	ONB uvw;
	uvw.W = Vec3::normalize(_W);
	uvw.U = Vec3::normalize(_U - dot(_U, uvw.W) * uvw.W);
	uvw.V = cross(uvw.W, uvw.U);
	return uvw;
}

ONB ONB::makeFromVW(const Vec3& _V, const Vec3& _W) {
	ONB uvw;
	uvw.V = Vec3::normalize(_V);
	uvw.W = Vec3::normalize(_W - dot(_W, uvw.V) * uvw.V);
	uvw.U = cross(uvw.V, uvw.W);
	return uvw;
}

ONB ONB::makeFromWV(const Vec3& _W, const Vec3& _V) {
	ONB uvw;
	uvw.W = Vec3::normalize(_W);
	uvw.V = Vec3::normalize(_V - dot(_V, uvw.W) * uvw.W);
	uvw.U = cross(uvw.V, uvw.W);
	return uvw;
}

void ONB::uniformSampleDisk(float u1, float u2, float *x, float *y) {
    float r = sqrtf(u1);
    float theta = 2.0f * M_PI * u2;
    *x = r * cosf(theta);
    *y = r * sinf(theta);
}


void ONB::concentricSampleDisk(float u1, float u2, float *dx, float *dy) {
    float r, theta;
    // Map uniform random numbers to $[-1,1]^2$
    float sx = 2 * u1 - 1;
    float sy = 2 * u2 - 1;

    // Map square to $(r,\theta)$

    // Handle degeneracy at the origin
    if (sx == 0.0 && sy == 0.0) {
        *dx = 0.0;
        *dy = 0.0;
        return;
    }
    if (sx >= -sy) {
        if (sx > sy) {
            // Handle first region of disk
            r = sx;
            if (sy > 0.0) theta = sy/r;
            else          theta = 8.0f + sy/r;
        }
        else {
            // Handle second region of disk
            r = sy;
            theta = 2.0f - sx/r;
        }
    }
    else {
        if (sx <= sy) {
            // Handle third region of disk
            r = -sx;
            theta = 4.0f - sy/r;
        }
        else {
            // Handle fourth region of disk
            r = -sy;
            theta = 6.0f + sx/r;
        }
    }
    theta *= M_PI / 4.f;
    //cout << "r: " << r << "\ttheta: " << theta << "\n";

    *dx = r * cosf(theta);
    *dy = r * sinf(theta);
    //cout << "dx: " << *dx << "\tdy: " << *dy << endl;
}

Vec3 ONB::uniformSampleHemisphere(float u, float v) {
	float z = u;
	float r = sqrtf(max(0.f, 1.f-z*z));
	float phi = M_TWICE_PI*v;
	float x = r * cosf(phi);
	float y = r * sinf(phi);
	return Vec3(x, y, z);
}

float ONB::uniformSampleHemispherePDF() {
	return M_1_TWICE_PI;
}

Vec3 ONB::cosineSampleHemisphere(float u, float v) {
	Vec3 ret;
	concentricSampleDisk(u, v, &ret[0], &ret[1]);
	ret[2] = sqrtf(max(0.f, 1.f - ret[0]*ret[0] - ret[1]*ret[1]));
	return ret;
}

float ONB::cosineSampleHemispherePDF(float costheta, float phi) {
	return costheta * M_1_PI;
}
