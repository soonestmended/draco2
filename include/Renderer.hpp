#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "Options.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"

#include <memory>

class Camera;
class Color;
class Film;
class IntersectRec;
class Options;
class Scene;

class Shader {
public:
	Shader(const Scene *_scene, std::shared_ptr <Sampler <2>> _samplerSP = std::make_shared <BasicSampler2> ())
	: scene (_scene), samplerSP (_samplerSP) {}
	virtual Color lightAlongRay(const Ray &r) const = 0;
protected:
	const Scene* scene;
	std::shared_ptr <Sampler <2>> samplerSP;
};

class ColorOnlyShader : public Shader {
public:
	ColorOnlyShader(const Scene *_scene, std::shared_ptr <Sampler <2>> _samplerSP = std::make_shared <BasicSampler2> ()) : Shader(_scene, _samplerSP) {}
	Color lightAlongRay(const Ray &r) const;
};

class EDLShader : public Shader {
public:
	EDLShader(const Scene *_scene, std::shared_ptr <Sampler <2>> _samplerSP = std::make_shared <BasicSampler2> ()) : Shader(_scene, _samplerSP) {
		std::vector <std::shared_ptr <Light>> lightSPs = _scene->getLights();
		for (auto lSP : lightSPs) 
			lights.push_back(lSP.get());
		makeLightDistribution();
	}
	Color lightAlongRay(const Ray &r) const;

private:
	void makeLightDistribution();
	Color loopLAR(const Ray &r, int depth) const;
	Color EDL(const IntersectRec& ir, const Vec3& wo) const;
	const Light* pickLight(const float u) const {
		return lights[d1d.pick(u)];
	}
	Distribution1D d1d;
	std::vector <Light *> lights;
};


class PathTraceShader : public Shader {
public:
	PathTraceShader(const Scene *_scene, std::shared_ptr <Sampler <2>> _samplerSP = std::make_shared <BasicSampler2> ()) : Shader(_scene, _samplerSP) {}
	Color lightAlongRay(const Ray &r) const;
	
private:
	Color recursiveLAR(const Ray &r, int depth) const;
};

class Renderer {
public:
	Renderer(const Camera* _cameraPtr, Film* _filmPtr, const RenderOptions* _renderOptionsPtr, const Scene* _scenePtr) 
		: cameraPtr (_cameraPtr), filmPtr (_filmPtr), renderOptionsPtr (_renderOptionsPtr), scenePtr (_scenePtr) {}

	virtual bool render() const = 0;

protected:
	std::shared_ptr <Shader> shaderSP;

	const Camera* cameraPtr;
	Film* filmPtr;
	const RenderOptions* renderOptionsPtr;
	const Scene* scenePtr;
};

class BasicRenderer : public Renderer {
public:
	BasicRenderer(const Camera* _cameraPtr, Film* _filmPtr, const RenderOptions* _renderOptionsPtr, const Scene* _scenePtr) 
		: Renderer(_cameraPtr, _filmPtr, _renderOptionsPtr, _scenePtr) {
			// std::shared_ptr <Sampler <2>> ssp = std::make_shared <StratifiedSampler2> (_renderOptionsPtr->samplesPerPixel);
			std::shared_ptr <Sampler <2>> ssp = std::make_shared <BasicSampler2> (_renderOptionsPtr->samplesPerPixel);
			ssp->makeSamples();
			//shaderSP = std::make_shared <PathTraceShader> (_scenePtr, ssp);
			shaderSP = std::make_shared <EDLShader> (_scenePtr, ssp);
		}
	
	bool render() const;
};

#endif