#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include <cmath>
#include <cstdlib>
#include <vector>

#include "Vector.hpp"

class Distribution1D {
public:
	Distribution1D() {}
	Distribution1D(const std::vector <float>& _values) : weights () {
		// normalize values
		float sum = 0.0;
		for (auto v : _values)
			sum += v;
		//weights.reserve(_values.size() + 1);
		weights.push_back(0.0);
		for (int i = 1; i < _values.size() + 1; ++i) {
			std::cout << "i: " << i;
			weights.push_back(weights[i-1] + _values[i-1] / sum);
			std::cout << "\tweights[i]: " << weights[i];
			std::cout << std::endl;
		}		
	}

	const int pick(const float val) const {
		return binarySearch(val);
	}

	const void print() {
		std::cout << "(";
		for (int i = 0; i < weights.size(); ++i)
			std::cout << weights[i] << ", ";
		std::cout << "\b\b)" << std::endl;
	}
private:
	const int binarySearch(const float val) const;
	std::vector <float> weights;
};

template <int DIM>
class Sampler {
public:
	Sampler() : samples(0), current(0) {}
	Sampler(int _N) : samples(_N), current(0) {}
	virtual ~Sampler() {}
	virtual const std::vector <Vector <float, DIM>>& getSamples() {return samples;}
	virtual bool makeSamples() = 0;
	virtual Vector <float, DIM> next() {
		return samples[current++];
	}
	static float rand01() {return (float) rand() / (float) RAND_MAX;}

protected:
	std::vector <Vector <float, DIM>> samples;
	int current;
};

template <int DIM>
class BasicSampler : public Sampler <DIM> {
public:
	BasicSampler() : Sampler<DIM> () {}
	BasicSampler(int _N) : Sampler<DIM> (_N) {
		makeSamples();
	}

	bool makeSamples() {
		for (auto& s : this->samples) {
			for (int i = 0; i < DIM; ++i) {
				s.data[i] = Sampler<2>::rand01();
			}
		}
		return true;
	}
};

class StratifiedSampler2 : public Sampler <2> {
public:
	StratifiedSampler2() : Sampler<2> (), sideLength (0) {}
	StratifiedSampler2(int _N) : Sampler<2> () {
		sideLength = (int) sqrtf(float (_N));
		int sampleSize = sideLength * sideLength;
		if (sampleSize != _N) std::cout << "Rounded sample size to: " << sampleSize << std::endl;
		this->samples = std::vector <Vec2> (sampleSize);
		makeSamples();
	}
	bool makeSamples();

private:
	int sideLength;
};


typedef BasicSampler <2> BasicSampler2;
#endif