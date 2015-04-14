#include <iostream>
#include <memory>

#include "Camera.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Parser.hpp"
#include "Primitive.hpp"
#include "Scene.hpp"
#include "Vector.hpp"

using namespace std;

const Vec3 Parser::makeVec3(const boost::any& a) const {
	if (a.type() == typeid(vector <float>*)) {
		cout << "vector <float>* type" << endl;
		vector <float>* v = boost::any_cast<vector <float>*> (a);
		if (v->size() != 3) yyerror(this, "Wrong vector length");
		return *v;
	}
	else if (a.type() == typeid(Vec3)) {
		cout << "Vec3 type" << endl;
		return boost::any_cast <Vec3> (a);
	} 
	yyerror(this, "Error making vector.");

}

template <class T>
void printVec(const vector <T> &v) {
	cout << "Vector (" << v.size() << "): <"; 
	for (auto it = v.begin(); it != v.end(); ++it)
		cout << *it << ", ";
	cout << "\b\b>";
}

void ParamSet::print() const {
	for (auto it = params.rbegin(); it != params.rend(); ++it) {
		cout << it->first << "\t";
		if (it->second.type() == typeid(float))
			cout << boost::any_cast<float> (it->second);
		
		else if (it->second.type() == typeid(int)) 
			cout << boost::any_cast<int> (it->second);

		else if (it->second.type() == typeid(float*)) {
			float *f = boost::any_cast <float*> (it->second);
			cout << "<" << f[0] << ", " << f[1] << ", " << f[2] << ">";
		}
		
		else if (it->second.type() == typeid(vector <float>*))
			printVec(*(boost::any_cast <vector <float>* > (it->second)));

		else if (it->second.type() == typeid(string))
			cout << boost::any_cast <string> (it->second);

		else if (it->second.type() == typeid(ParamSet*)) {
			cout << "(ParamSet):" << endl << "{{{{{{{{{{{{{{" << endl;
			(boost::any_cast <ParamSet*> (it->second))->print();
			cout << "}}}}}}}}}}}}}}" << endl;
		}

		cout << endl;
	}
}

void ParamSet::clear() {
	// clear parameter vector and free memory as needed
	for (auto it = params.begin(); it != params.end(); ++it) {
		//cout << "ParamSet::clear() : " << it->first << "\t" << (it->second.type() == typeid(ParamSet*)) << endl;
		if (it->second.type() == typeid(float *)) {
			//cout << "float*" << endl;
			delete[] (boost::any_cast<float*> (it->second));
		}
		else if (it->second.type() == typeid(vector <float>*)) {
			//cout << "vector <float>*" << endl;
			delete (boost::any_cast<vector<float>*> (it->second));
		}
		else if (it->second.type() == typeid(ParamSet*)) {
			//cout << "ParamSet*" << endl;
			ParamSet *ps = boost::any_cast <ParamSet*> (it->second);
			//if (ps) {
				ps->clear();
				delete ps;
			//}
		}
	}
	//cout << "before params.clear()" << endl;
	params.clear();
	//cout << "after params.clear()" << endl;
}

void ParamSet::joinParamSet(ParamSet *ps) {
	if (ps == nullptr) return;
	for (auto it = ps->params.begin(); it != ps->params.end(); ++it) {
		this->params.push_back(*it);
	}
}

shared_ptr <Material> Parser::parseMaterial(ParamSet *ps) const {
	cout << "Parse material:" << endl;
	ps->print();
	cout << "ps->size(): " << ps->params.size() << endl;
	int mt = -1;
	for (auto it = ps->params.rbegin(); it != ps->params.rend(); ++it) {
		if (!it->first.compare("material_type")) 
			mt = boost::any_cast <int> (it->second);
	}
	switch (mt) {
		case MATERIAL_DIFFUSE:
			return makeDiffuseMaterial(ps);
		case MATERIAL_EMISSIVE:
			return makeEmissiveMaterial(ps);
		case -1:
			yyerror(this, "Material type not specified.");
	}
	return nullptr;
}

shared_ptr <Material> Parser::makeDiffuseMaterial(const ParamSet *ps) const {
	cout << "Make diffuse material" << endl;
	Color c;
	for (auto it = ps->params.rbegin(); it != ps->params.rend(); ++it) {
		if (!it->first.compare("Kd")) {
			c = makeVec3(it->second);
		}
	}
	return make_shared <DiffuseMaterial> (c);
}

shared_ptr <Material> Parser::makeEmissiveMaterial(const ParamSet *ps) const {
	cout << "Make emissive material" << endl;
	Color c;
	for (auto it = ps->params.rbegin(); it != ps->params.rend(); ++it) {
		if (!it->first.compare("Ke")) {
			c = makeVec3(it->second);
		}
	}
	return make_shared <EmissiveMaterial> (c);
}

shared_ptr <Primitive> Parser::makeUnion(const ParamSet *ps) const {
	cout << endl << endl << "Make union:" << endl;
	ps->print();
	vector <shared_ptr <Primitive>> primitiveSPvector;
	shared_ptr <Material> materialSP = nullptr;
	for (auto it = ps->params.rbegin(); it != ps->params.rend(); ++it) {
		if (!it->first.compare("primitive")) {
			primitiveSPvector.push_back(makePrimitive(boost::any_cast <ParamSet *> (it->second)));
		}
		else if (!it->first.compare("material")) {
			materialSP = parseMaterial(boost::any_cast <ParamSet *> (it->second));
		}
	}
	return make_shared <CSGUnion> (primitiveSPvector, materialSP, nullptr);
}

bool Parser::addPrimitive(const ParamSet *ps) const {
	scene.addPrimitive(makePrimitive(ps));
}

bool Parser::addCamera(const ParamSet *ps) const {
	cout << "-----Adding camera with: " << endl;
	ps->print();
	cout << "-----" << endl;

	// Camera(const Vec3& _eye, const Vec3& _look, const Vec3& _up, const Vec3& _right, float _s) :
	Vec3 eye, look, look_at, up, right;
	eye = look = look_at = up = right = Vec3(0.0);
	float s = 1.0;

	for (auto it = ps->params.begin(); it != ps->params.end(); ++it) {
		
		if (!it->first.compare("location")) {
			cout << "Location: ";
			if (it->second.type() == typeid(vector <float>*)) {
				vector <float>* v = boost::any_cast<vector <float>*> (it->second);
				if (v->size() == 3) {
					eye = *v;
					cout << eye << endl;
				}
				else {
					yyerror(this, "wrong length for camera location vector.");
				}
			}
			else {
				yyerror(this, "location vector error.");
			}
		}

		else if (!it->first.compare("look_at")) {
			cout << "Look at: ";
			if (it->second.type() == typeid(vector <float>*)) {
				vector <float>* v = boost::any_cast<vector <float>*> (it->second);
				if (v->size() == 3) {
					look_at = *v;
					cout << look_at << endl;
				}
				else {
					yyerror(this, "wrong length for camera look_at vector.");
				}
			}
			else {
				yyerror(this, "look_at vector error.");
			}
		}

		else if (!it->first.compare("right")) {
			cout << "Right: ";
			if (it->second.type() == typeid(vector <float>*)) {
				vector <float>* v = boost::any_cast<vector <float>*> (it->second);
				if (v->size() == 3) {
					right = *v;
					cout << right << endl;
				}
				else {
					yyerror(this, "wrong length for camera right vector.");
				}
			}
			else {
				yyerror(this, "right vector error.");
			}
		}

		else if (!it->first.compare("distance")) {
			cout << "Distance: ";
			if (it->second.type() == typeid(float)) {
				s = boost::any_cast<float> (it->second);
				cout << s << endl;
			}
			else {
				yyerror(this, "wrong type for camera distance value.");
			}
		}
	}

	look = Vec3::normalize(look_at - eye);
	if (up == Vec3(0.0))
		up = Vec3::normalize(cross(right, look));
	camera = Camera(eye, look, up, right, s);
	return true;
}

shared_ptr <Light> Parser::makeLight(const ParamSet *ps, const int lt) const {
	switch (lt) {
		case LIGHT_SURFACE:
			return makeSurfaceLight(ps);
	}
	yyerror(this, "Light type error");
	return nullptr;
}

shared_ptr <Light> Parser::makeSurfaceLight(const ParamSet *ps) const {
	Color c = Color::Black();
	float p = 0.0;
	Surface* sPtr = nullptr;

	for (auto it = ps->params.begin(); it != ps->params.end(); ++it) {
		if (!it->first.compare("color")) {
			c = Color(makeVec3(it->second));
		}
		else if (!it->first.compare("power")) {
			p = boost::any_cast <float> (it->second);
		}
	}

	for (auto it = ps->params.begin(); it != ps->params.end(); ++it) {
		if (!it->first.compare("primitive")) {
			ParamSet* lps = boost::any_cast <ParamSet*> (it->second);
			// check primitive for existing material block
			bool hasMat = false;
			for (auto it2 = lps->params.begin(); it2 != lps->params.end(); ++it2) {
				if (!it2->first.compare("material")) {
					hasMat = true;
					ParamSet* lps2 = boost::any_cast <ParamSet*> (it2->second);

					lps2->clear();
					lps2->addParam("Ke", (Vec3) (c));
					lps2->addParam("material_type", (int) MATERIAL_EMISSIVE);
				}
			}
			if (!hasMat) {
				ParamSet* matPS = new ParamSet();
				matPS->addParam("Ke", (Vec3) (c));
				matPS->addParam("material_type", (int) MATERIAL_EMISSIVE);
				lps->addParam("material", matPS);
			}
			shared_ptr <Primitive> pp = makePrimitive(lps);
			sPtr = (Surface *) pp.get();
			scene.addPrimitive(pp);
		}
	
	}
	return make_shared <SurfaceLight> (sPtr, c, p);
}

bool Parser::addLight(const ParamSet *ps) const {
	cout << "-----Adding light with: " << endl;
	ps->print();
	cout << "-----" << endl;
	auto it = ps->params.rbegin();
	int lt = -1;
	if (!it->first.compare("light_type")) {
		lt = boost::any_cast <int> (it->second);
	}
	return scene.addLight(makeLight(ps, lt));
}

shared_ptr <Primitive> Parser::makePrimitive(const ParamSet *ps) const {
	auto it = ps->params.rbegin();
	if (!it->first.compare("shape_type")) {
		int st = boost::any_cast <int> (it->second);

		switch (st) {
			case ST_CONE:
				return makeCone(ps);
			case ST_CYLINDER:
				cout << "Make cyl stub" << endl;
				break;
			case ST_SPHERE:
				return makeSphere(ps);
			case ST_TRIANGLE:
				return makeTriangle(ps);
		}
	} else if (!it->first.compare("csg_type")) {
		int csgt = boost::any_cast <int> (it->second);
		switch (csgt) {
			case CSG_UNION:
				return makeUnion(ps);
				cout << "make CSG union" << endl;
				break;
		}
	}
	return nullptr;
}

shared_ptr <Primitive> Parser::makeCone(const ParamSet *ps) const {
	cout << "-----Making cone with: " << endl;
	ps->print();
	cout << "-----" << endl;
	return nullptr;
}

shared_ptr <Primitive> Parser::makeSphere(const ParamSet *ps) const {
	cout << "-----Making sphere with: " << endl;
	ps->print();
	cout << "-----" << endl;
	return nullptr;
}

shared_ptr <Primitive> Parser::makeTriangle(const ParamSet *ps) const {
	vector <Vec3> vertices;
	shared_ptr <Material> mat = nullptr;
	bool lyn = false;
	for (auto it = ps->params.rbegin(); it != ps->params.rend(); ++it) {
		cout << "addTriangle params: " << "\t" << it->first << endl;
		if (!it->first.compare("anon_vector")) {
			cout << "vertex: ";
			if (it->second.type() == typeid(vector <float>*)) {
				vector <float>& v = *(boost::any_cast<vector <float>*> (it->second));
				if (v.size() == 3) {
					vertices.push_back(Vec3(v[0], v[1], v[2]));
					cout << vertices.back() << endl;
				}
				else {
					yyerror(this, "wrong length for vertex vector.");
				}
			}
			else {
				yyerror(this, "vertex vector error.");
			}
		}

		if (!it->first.compare("material")) {
			cout << "addTriangle: matched it->first=material" << endl;
			if (it->second.type() == typeid(ParamSet*)) {
				cout << "addTriangle: matched it->second" << endl;
				mat = parseMaterial(boost::any_cast <ParamSet*> (it->second));
				lyn = mat->emits();
				cout << "after parseMaterial; lyn = " << lyn << endl;
			}
			else {
				yyerror(this, "material statement error.");
			}
		}

	}

	return make_shared <Surface> (make_shared <Triangle> (vertices[0], vertices[1], vertices[2]), mat, lyn);
}
