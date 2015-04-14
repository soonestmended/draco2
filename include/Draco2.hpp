#ifndef DRACO2_HPP__
#define DRACO2_HPP__

#define EPSILON .00001f
#define M_TWICE_PI 6.28318530718f
#define M_1_TWICE_PI 0.15915494309f

template <typename T>
inline T clamps(T f, T low, T high) {
	if (f < low) return low;
	else if (f > high) return high;
	return f;
}



#endif