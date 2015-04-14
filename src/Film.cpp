#include "Draco2.hpp"
#include "Film.hpp"

using namespace std;

static const float RGB2XYZ[3][3] = { 
	{ 0.41239083F, 0.35758433F, 0.18048081F },
	{ 0.21263903F, 0.71516865F, 0.072192319F },
	{ 0.019330820F, 0.11919473F, 0.95053220F }
};

static const float XYZ2RGB[3][3] = { 
	{ 3.2409699F, -1.5373832F, -0.49861079F },
	{ -0.96924376F, 1.8759676F, 0.041555084F },
	{ 0.055630036F, -0.20397687F, 1.0569715F }
};

Color Film::rgb2Yxy(const Color& c) {
	Vec3 result(0.0);
	Color ans;
	for (int i = 0; i < 3; ++i) {
		result[i] += RGB2XYZ[i][0] * c[0];
		result[i] += RGB2XYZ[i][1] * c[1];
		result[i] += RGB2XYZ[i][2] * c[2];
	}
	const float W = result[0] + result[1] + result[2];
	const float Y = result[1];
	if(W > 0) { 
		ans[0] = Y;			    // Y 
		ans[1] = result[0] / W;	// x 
		ans[2] = result[1] / W;	// y 	
	} else {
		ans = Color::Black();
	}
	return ans;
}

Color Film::Yxy2rgb(const Color& c) {
	Vec3 result(0.0);
	float X, Y, Z;
	Color ans;
	Y = c[0]; // Y 
	result[1] = c[1];	// x 
	result[2] = c[2];	// y 
	if ((Y > EPSILON) && (result[1] > EPSILON) && (result[2] > EPSILON)) {
		X = (result[1] * Y) / result[2];
		Z = (X / result[1]) - X - Y;
	} else {
		X = Z = EPSILON;
	}
	ans[0] = X;
	ans[1] = Y;
	ans[2] = Z;
	result[0] = result[1] = result[2] = 0.f;
	for (int i = 0; i < 3; i++) {
		result[i] += XYZ2RGB[i][0] * ans[0];
		result[i] += XYZ2RGB[i][1] * ans[1];
		result[i] += XYZ2RGB[i][2] * ans[2];
	}
	ans = result;
	return ans;
}

/*bool Film::filterImage() {
	// for each pixel in the output image, sum contributions from nearby samples
	imageData.resize(width*height);
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			Color c(0.0, 0.0, 0.0);
			for (int k = 0; k < N; ++k) {
				// c += getSample(i, j, k).color;
				c += getColor(i, j, k);
			}
			c /= (float) N;
			imageData[j*width+i] = c;
		}
	}
	return true;
}
*/
bool Film::toneMapImage(float alpha) {
	// convert to Yxy
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			imageData[j*width+i] = rgb2Yxy(imageData[j*width+i]);
		}
	}

	// find log average luminance
	float lal = 0.f;
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			lal += log(.0001 + imageData[j*width+i][0]); 
			if (j%100 == 0 && i%100 == 0) {
				cout << imageData[j*width+i] << "\t";
				cout << "log: " << log(.0001 + imageData[j*width+i][0]) << "\t";
				
			}

		}
	}
	cout << "cum. lal: " << lal << endl;
	lal = exp(lal / (float) (width*height));
	cout << "final lal: " << lal << endl;
	// scale luminances
	float key_lal = alpha / lal;
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			imageData[j*width+i][0] *= key_lal; 
			imageData[j*width+i][0] /= (1.0f + imageData[j*width+i][0]);
		}
	}

	// convert back to RGB
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			imageData[j*width+i] = Yxy2rgb(imageData[j*width+i]); 
			if (j%100 == 0 && i%100 == 0) {
				cout << imageData[j*width+i] << endl;
			}
		}
	}

	return true;
}

bool Film::makePNG() {
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			Color c = Vec3::clampv(imageData[j*width+i], 0.0, 1.0);
			pngImage[j][i] = png::rgb_pixel( (int) (c[0] * 255.0), (int) (c[1] * 255.0), (int) (c[2] * 255.0));
		}
	}
	return true;
}