#include "Draco2.hpp"
#include "Sampler.hpp"

using namespace std;

const int Distribution1D::binarySearch(const float val) const {
	int lo = 0;
	int hi = weights.size() - 1;
	int mid = (hi - lo) / 2;
	do {
		if (weights[mid] <= val) {
			if (weights[mid+1] >= val) return mid;
			else {
				lo = mid;
				mid = lo + (hi - lo) / 2;
			}
		}
		else {
			// else weights[mid] > val
			hi = mid;
			mid = lo + (hi - lo) / 2;
		}
	}	while (hi > lo);
	return -1;
}


bool StratifiedSampler2::makeSamples() {
	Vec2 delta(1.f / (float) sideLength, 1.f / (float) sideLength);
	for (int i = 0; i < sideLength; ++i) {
		for (int j = 0; j < sideLength; ++j) {
			Vec2 rv = Vec2(BasicSampler2::rand01(), BasicSampler2::rand01());
			samples[i*sideLength+j] = (Vec2((float) j, (float) i) + rv)* delta;
		}
	}
	
	return true;
}