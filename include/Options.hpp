#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

#include <cstring>

class RenderOptions {
public:
	RenderOptions() {}
	RenderOptions(int _iw, int _ih, int _spp, float _alpha) : imageWidth (_iw), imageHeight (_ih), samplesPerPixel (_spp), alpha (_alpha) {}
	int imageWidth;
	int imageHeight;
	int samplesPerPixel;
	float alpha;
	std::string outputFilename;
	std::string inputFilename;
};

#endif