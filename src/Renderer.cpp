#include "Draco2.hpp"

#include "Camera.hpp"
#include "Color.hpp"
#include "Film.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "MonteCarlo.hpp"
#include "Primitive.hpp"
#include "Ray.hpp"
#include "Renderer.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"
#include "Vector.hpp"

using namespace std;

Color ColorOnlyShader::lightAlongRay(const Ray &r) const {
	IntersectRec ir;
	if (scene->closestIntersection(r, ir, EPSILON, HUGE_VALF)) {
		return ir.materialPtr->getBRDF();
	} else {
		return Color::Black();
	}
};

Color EDLShader::lightAlongRay(const Ray &r) const {
	return loopLAR(r, 5);
};

Color EDLShader::loopLAR(const Ray &r, int maxDepth) const {
	bool cel = true;
	Color ans = Color::Black(), throughput = Color::White();
	IntersectRec ir;
	Ray nextRay = r;
	for (int i = 0; i < maxDepth; ++i) {

		// intersect with scene; if ray leaves scene, accumulation is done.
		// TODO: account for non-geometric lights.
		if (!scene->closestIntersection(nextRay, ir, .001f, HUGE_VALF)) {
			break;
		}

		// finalize intersectRec (make ONB)
		ir.finish();
		const Primitive *p = ir.primitivePtr;

		// if we're counting emitted light and we've hit a light source, terminate the path with 
		// the light source emisison value
		if (p->isLight() && cel) {
			ans += throughput * ir.primitivePtr->getMaterial()->getEmission();
		} 

		Vec3 wo = ir.onb.world2local(-nextRay.d);

		// add EDL contribution
		ans += throughput * EDL(ir, wo);

		// get next ray to trace and multiply in the BRDF
		Vec3 wi;
		Color brdf;
		float pdf;
		const Material *m = ir.primitivePtr->getMaterial().get();
		m->sample(ir, wi, wo, brdf, pdf, Vec2(BasicSampler2::rand01(), BasicSampler2::rand01())); //samplerSP->next());
		
		if (pdf < EPSILON || brdf.isBlack()) {
			cout << "wi: " << wi << "\two: " << wo << endl;
			break;
		} 
		// convert wi to world coordinates for next tracing
		Vec3 nrd = ir.onb.local2world(wi);
		nextRay = Ray(ir.isectPoint + nrd * .0001f, nrd);
		
		throughput *= absDot(ir.normal, nextRay.d) * brdf;
		throughput *= 1.0f / pdf;
		cel = false;
	}
	return ans;
}

Color EDLShader::EDL(const IntersectRec& ir, const Vec3& wo) const {
	// estimates incoming radiance due to lights in scene.

	// Select random light
	const Light *light = this->pickLight(BasicSampler2::rand01());
	const Material *m = ir.primitivePtr->getMaterial().get();
	
	Vec3 wi;
	Ray rayToLight; // used for both kinds of light sampling

	Color ans = Color::Black(); // accumulate results

	Color 	lightRadianceSurfaceSample, // radiance from light for the surface-sampled ray
			lightRadianceLightSample; 	// and light-sampled ray, respectively
	
	Color 	surfaceSampleBRDF, 			// reflectance coefficients from surface-sampled direction
			lightSampleBRDF; 			// and light-sampled direction
	
	float lightPDF, surfacePDF; // PDFs for above

	// One:
	// Sample based on surface BRDF and see if ray hits light. For lights that can't be intersected, this doesn't make sense.
	// (non area lights aren't implemented yet)

	if (light->canIntersect()) {

		// sample direction to light based on surface
		Color surfaceBRDF;
		
		m->sample(ir, wi, wo, surfaceSampleBRDF, surfacePDF, Vec2(BasicSampler2::rand01(), BasicSampler2::rand01())); 

		// put wi in world coordinates here
		rayToLight = Ray(ir.isectPoint, ir.onb.local2world(wi));

		// if this ray sampled based on surface hits the light, get EDL for it
		IntersectRec lightIR;
		lightRadianceSurfaceSample = Color::Black();
		if (scene->closestIntersection(rayToLight, lightIR, EPSILON, HUGE_VALF) && light->getPrimitive() == lightIR.primitivePtr) {
			// rayToLight.d.normalize(); // ? necessary

			// get radiance leaving light in direction of sampled point
			lightRadianceSurfaceSample = light->lightEmitted(lightIR.isectPoint, lightIR.normal, -rayToLight.d);

			// PDF of ray had it been selected according to the light 
			// world coordinates
			lightPDF = light->PDF(lightIR, rayToLight);

			float weight = powerHeuristic(1., surfacePDF, 1., lightPDF);
			ans += surfaceSampleBRDF * lightRadianceSurfaceSample * absDot(rayToLight.d, ir.normal) * weight / surfacePDF;
		}
		// else no contribution from this sample
	}

	// Two:
	// second sample based on light surface distribution.
	float lightDist;

	lightRadianceLightSample = light->sample(ir, wi, wo, lightPDF, lightDist, Vec2(BasicSampler2::rand01(), BasicSampler2::rand01()));
	// wi in ir.onb coordinates

	if (!lightRadianceLightSample.isBlack() && lightPDF > 0.) {
		
		// back to world coordinates for intersections
		rayToLight = Ray(ir.isectPoint + .0001f*ir.normal, ir.onb.local2world(wi));

		//rayToLight.normalize();
		lightSampleBRDF = m->getBRDF(ir, wi, wo);

		// if surface reflects light from that direction && light is not occluded
		if (!lightSampleBRDF.isBlack() && !scene->intersectYN(rayToLight, .001f, lightDist - .001f)) {
			// TODO: *rayToLight.d and wo need to be transformed to ir.onb*
			surfacePDF = m->PDF(wi, wo); // probability of choosing direction rayToLight based on surface sampling 
			float weight = powerHeuristic(1., lightPDF, 1., surfacePDF);
			ans += lightSampleBRDF * lightRadianceLightSample * absDot(rayToLight.d, ir.normal) * weight / lightPDF;
			//cout << "foo";
		}
	}

	return ans;

}

void EDLShader::makeLightDistribution() {
	// Make 1 d distribution from lights in scene
	vector <float> lightPowers;
	for (auto lPtr : lights) {
		lightPowers.push_back(lPtr->getPower());
	}
	d1d = Distribution1D(lightPowers);
	d1d.print();
}

Color PathTraceShader::lightAlongRay(const Ray &r) const {
	return recursiveLAR(r, 4);
};

Color PathTraceShader::recursiveLAR(const Ray &r, int depth) const {
	if (depth == 0) return Color::Black();
	IntersectRec ir;
	if (!scene->closestIntersection(r, ir, EPSILON, HUGE_VALF)) {
		return Color::Black();
	}
	//return Color(ir.normal);
	//cout << "hit";
	ir.finish();
	const Primitive *p = ir.primitivePtr;
	Material *m = p->getMaterial().get();
	if (p->isLight()) {
		//cout << "hit light";
		return m->getEmission();
	} else {
		Vec3 wi;
		Color brdf;
		float pdf;
		m->sample(ir, wi, -r.d, brdf, pdf, Vec2(BasicSampler2::rand01(), BasicSampler2::rand01())); //samplerSP->next());
		Ray nextRay(ir.isectPoint + wi * EPSILON, wi);
		
		if (pdf < EPSILON) {
			cout << "wi: " << wi << "\two: " << -r.d << endl;
			return Color::Black();
		} 
		brdf /= pdf;
		//return brdf/pdf;
		//return ir.normal;
		//cout << nextRay.d << endl;
		return brdf * recursiveLAR(nextRay, depth-1) * dot(ir.normal, nextRay.d);
	}
}

bool BasicRenderer::render() const {
	StratifiedSampler2 sampler(renderOptionsPtr->samplesPerPixel);
	//BasicSampler2 bs(renderOptionsPtr->samplesPerPixel);
	IntersectRec ir;
	for (int j = 0; j < renderOptionsPtr->imageHeight; ++j) {
		for (int i = 0; i < renderOptionsPtr->imageWidth; ++i) {
			/*if (i < 280 || i > 320 || j <300 || j > 500) {
				filmPtr->addSample(i, j, Color(0.05, 0.05, 0.05) * (float)renderOptionsPtr->samplesPerPixel, Vec2(0.));
				continue;
			}*/
			cout << "\r(" << i << ", " << j << ")";
			cout.flush();
			sampler.makeSamples();
			auto samples = sampler.getSamples();
			Vec2 pixelCenter((float)i + 0.5, (float)(renderOptionsPtr->imageHeight-j-1) + 0.5);
			//cout << "Pixel: " << pixelCenter << endl;
			for (int k = 0; k < renderOptionsPtr->samplesPerPixel; ++k) {
				Vec2 dxy = samples[k] - Vec2(0.5);
				Ray r = cameraPtr->getRay((pixelCenter + dxy) / Vec2(renderOptionsPtr->imageWidth, renderOptionsPtr->imageHeight) - Vec2(0.5, 0.5));
				//cout << r << endl;
				filmPtr->addSample(i, j, shaderSP->lightAlongRay(r), dxy);
				//cout << "\t" << r << endl;
			}
		}
	}
	return true;
}
