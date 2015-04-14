#ifndef __MONTE_CARLO_HPP__
#define __MONTE_CARLO_HPP__

inline float powerHeuristic(int nf, float fPDF, int ng, float gPDF) {
	float f = nf * fPDF, g = ng * gPDF;
	return (f*f) / (f*f + g*g);
}

inline float uniformSampleTriangle(const Vec2& uv, Vec2& bg) {
	float sqrtu = sqrtf(uv[0]);
	bg[0] = 1.f - sqrtu;
	bg[1] = uv[1] * sqrtu;
}

#endif