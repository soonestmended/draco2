#ifndef __FILM_HPP__
#define __FILM_HPP__

#include <cstring>
#include <png++/png.hpp>
#include <vector>

#include "Color.hpp"
#include "Vector.hpp"

class PixelFilter {
public:
	virtual Color filter(const Color& c, const Vec2& dxdy) = 0;
	virtual ~PixelFilter() = 0;
};

inline PixelFilter::~PixelFilter() {}

class BoxPixelFilter : public PixelFilter {
public:
	Color filter(const Color& c, const Vec2& dxdy) {
		return c;
	}
};

class Film {
public:
	Film(int _w, int _h, int _N) 
		: width (_w), height (_h), N (_N), pngImage(png::image<png::rgb_pixel> (_w, _h)) {
			imageData = std::vector <Color> (_w * _h, Color::Black());
			pf = new BoxPixelFilter();
		}

	~Film() {
		delete pf;
	}

//	bool filterImage();
	bool toneMapImage(float alpha);
	bool makePNG();
	bool writeImage(std::string filename) {
		pngImage.write(filename);
		return true;
	}

	const int getHeight() const {return height;}
	const int getWidth() const {return width;}
	const int getN() const {return N;}

	const Color& getColor(int i, int j, int k) {
		return imageData[j*width + i];
	}

	void addSample(int i, int j, const Color& c, const Vec2& dxdy) {
		imageData[j*width+i] += pf->filter(c, dxdy) / (float) N;
	}

	Color& colorAt(int i, int j, int k) {
		return imageData[j*width + i];
	}

protected:
	static float luminance(const Color& c) {
		return 0.27 * c[0] + 0.67 * c[1] + 0.06 * c[2];
	}

	static Color rgb2Yxy(const Color& c);
	static Color Yxy2rgb(const Color& c);

	/* Variables:
	 * sampler: takes care of samples for each pixel, one at a time.   
	 */

	PixelFilter* pf;
	std::vector <Color> imageData; // filtered image data
	png::image<png::rgb_pixel> pngImage; // image to be written to PNG
	int width, height, N;
};

#endif