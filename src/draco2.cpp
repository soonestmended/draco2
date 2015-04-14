#include <ctime>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include "Camera.hpp"
#include "Draco2.hpp"
#include "Film.hpp"
#include "Material.hpp"
#include "Matrix.hpp"
#include "ONB.hpp"
#include "Options.hpp"
#include "Parser.hpp"
#include "Primitive.hpp"
#include "Renderer.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"
#include "Vector.hpp"

extern void yyparse(Parser*);
extern FILE* yyin;
using namespace std;

int main33 (void) {
	vector <float> weights = {.1, .1, .1, 40, .1, .1, .1};
	Distribution1D d1d(weights);
	d1d.print();
	int stats[7] = {0, 0, 0, 0, 0, 0, 0};
	for (int i = 0; i < 1000; i++)
		stats[d1d.pick(BasicSampler2::rand01())]++;
	for (int i = 0; i < 7; i++)
		cout << i << ": " << stats[i] << endl;
	time_t tmt = time(0);
		tm *ltm = localtime(&tmt);
		cout << ltm->tm_year+1900 << "." << ltm->tm_mon+1 << "." << ltm->tm_mday << ".";
	return 0;
}

RenderOptions parseCommandLine(int argc, char** argv) {
	int width = 400, height = 400, spp = 9; // defaults
	float alpha = 0.45; // default tonemap alpha
	string inputFilename = "";
	stringstream outputFilenameSS;
	outputFilenameSS.str("");
	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];

		// width
		if (arg == "-w" || arg == "--width") {
			if (i+1 < argc) {
				width = atoi(argv[++i]);
			} else {
				cout << "width requires one argument." << endl;
				exit(1);
			}
		}

		// height
		else if (arg == "-h" || arg == "--height") {
			if (i+1 < argc) {
				height = atoi(argv[++i]);
			} else {
				cout << "height requires one argument." << endl;
				exit(1);
			}
		}

		// samples per pixel
		else if (arg == "-spp" || arg == "samplesperpixel") {
			if (i+1 < argc) {
				spp = atoi(argv[++i]);
			} else {
				cout << "Samples per pixel requires one argument." << endl;
				exit(1);
			}
		}

		// input file
		else if (arg == "-i" || arg == "--infile") {
			if (i+1 < argc) {
				// open a file handle to a particular file:
				FILE *myfile = fopen(argv[++i], "r");
				inputFilename = string(argv[i]);
				// make sure it's valid:
				if (!myfile) {
					cout << "can't open input file: " << arg << endl;
					exit(1);
				}
				// set lex to read from it instead of defaulting to STDIN:
				yyin = myfile;

			} else {
				cout << "infile requires one argument." << endl;
				exit(1);
			}
		}			
	}
	if (inputFilename == "") inputFilename = "stdin";
	if (outputFilenameSS.str() == "") {
		time_t tmt = time(0);
		tm *ltm = localtime(&tmt);
		outputFilenameSS << (int) ltm->tm_year +1900 << "." << (int) ltm->tm_mon + 1 << "." << (int) ltm->tm_mday << ".";
		outputFilenameSS << inputFilename << "-" << width << "x" << height << "-" << spp << "spp";
	}
	RenderOptions ans = RenderOptions(width, height, spp, alpha);
	ans.inputFilename = inputFilename;
	ans.outputFilename = outputFilenameSS.str();
	return ans;
}

int main (int argc, char **argv) {

	//PathTraceRenderer
	auto renderOptions = parseCommandLine(argc, argv);

	// Scene
	auto scene = make_shared <Scene> ();

	auto camera = make_shared <Camera> ();
	Parser p(*scene, *camera);
	yyparse (&p);


	// Make film
	auto film = make_shared <Film> (renderOptions.imageWidth, renderOptions.imageHeight, renderOptions.samplesPerPixel);

	// Make camera
	// Camera(const Vec3& _eye, const Vec3& _look, const Vec3& _up, const Vec3& _right, float _s) :
	
	BasicRenderer br(camera.get(), film.get(), &renderOptions, scene.get());

	br.render();

	/*** Structure.

	Load file specified on command line

	Construct objects from file:
		- Scene
		- Camera
		- Film
			- what does film do?
				- knows about the size of the image
				- has algorithms for tonemapping, etc
				- handles image output

		- Renderer
			- for each pixel in film:
				- make N samples using Sampler object 
				- trace samples and put colors into the Film object
					- camera->getRay operates on [-.5, .5] so we have to transform (e.g.)
					  [0, 800] --> [-.5, .5]. 
					  - get center of pixel as float
					  - add (.5, .5) - sample
					  - divide by width
					  - trace 


		- Options

	//  
	// figure out how to get a scene object out of yyparse.


	Initialize necessary data structures from Options object -- including 
		- Acceleration structures
		- Rendering object with any data structures it needs
		- Sampler(s)
		- Film, including output image
	// this is all on deck.


	Split rendering task into several subtasks (regions of image)

	For each subtask:

		For each pixel:
			camera.getRay(center, sampler);
			
			- trace ray through scene
			- add sample to pixel's list

	Convert samples into output image. */

	//film->filterImage();
	film->toneMapImage(renderOptions.alpha);
	film->makePNG();
	// write image to disk
	film->writeImage(renderOptions.outputFilename + ".png");

}