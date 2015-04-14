#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include "Vector.hpp"

class Color : public Vector <float, 3 > {
public:
	Color () : Vec3(0) {}
	Color (const Vec3& v) : Vec3(v) {}
	Color (float _r, float _g, float _b) : Vector <float, 3> (_r, _g, _b) {}

	bool isBlack() const {return this->data[0] < EPSILON && this->data[1] < EPSILON && this->data[2] < EPSILON;}

	static Color Black() {return Color(0.0, 0.0, 0.0);}
	static Color Blue() {return Color(0.0, 0.0, 1.0);}
	static Color Green() {return Color(0.0, 1.0, 0.0);}
	static Color Red() {return Color(1.0, 0.0, 0.0);}
	static Color White() {return Color(1.0, 1.0, 1.0);}
	static Color clamp(const Color& c) {
		Color ans(c);
		for (int i = 0; i < 3; ++i) {
			if (ans[i] < 0.0) ans[i] = 0.0;
			else if (ans[i] > 1.0) ans[i] = 1.0;
		}
		return ans;
	}
};


#endif