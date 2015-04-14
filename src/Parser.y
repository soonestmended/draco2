%{
#include "Draco2.hpp"
#include "Parser.hpp"
#include <iostream>
#include <vector>

extern int yylex();
extern FILE* yyin;
using namespace std;

%}

%union {
	char str[80];
	int int_token;
	float float_token;
	int csg_type;
	int light_type;
	int material_type;
	int shape_type;
	std::vector <float>* vector_type;
	ParamSet* paramset_type;
	Param* param_type;
}

%parse-param {Parser* parser}

%token <int_token> T_INT
%token <float_token> T_FLOAT
%token <str> T_STRING
%token <int_token> CSG
%token <int_token> LIGHT
%token <int_token> MATERIAL
%token <int_token> SHAPE
%token CAMERA LIGHT ENDFILE EOL TEXTURE LCB RCB

%type <vector_type> t_vector
%type <param_type> option
%type <paramset_type> option_list
%type <paramset_type> primitive
%type <paramset_type> primitive_list
%type <paramset_type> camera
%type <paramset_type> light
%type <paramset_type> material_statement

%start scene_description

%%
scene_description 
: scene_description statement
| statement
;

statement 
: primitive {parser->addPrimitive($1); $1->clear(); delete $1;}
| camera {parser->addCamera($1); $1->clear(); delete $1;}
| light {parser->addLight($1); $1->clear(); delete $1;}
;

camera
: CAMERA LCB option_list RCB {$$ = $3;}
;

light
: LIGHT LCB primitive option_list RCB {$$ = new ParamSet(); $$->addParam("primitive", $3); $$->joinParamSet($4); delete $4; $$->addParam("light_type", $1);}
;

primitive
: SHAPE LCB option_list RCB {cout << "SHAPE" << endl; $3->addParam("shape_type", $1); $$ = $3;}
| CSG LCB primitive_list RCB {cout << "CSG" << endl; $3->addParam("csg_type", $1); $$ = $3;}
;

primitive_list
: primitive_list primitive {$$ = new ParamSet(); $$->addParam("primitive", $2); $$->joinParamSet($1); delete $1;}
| primitive_list option_list {$$ = new ParamSet(); $$->joinParamSet($2); delete $2; $$->joinParamSet($1); delete $1;}
| primitive {$$ = new ParamSet(); $$->addParam("primitive", $1);}
| option_list {$$ = $1; }
;

sep
: ','
| EOL
| ',' EOL
;

option_list
: option_list sep option {$$ = new ParamSet(); $$->addParam($3); delete $3; $$->joinParamSet($1); delete $1;cout << "FOO2" << endl; $$->print(); cout << "FOOSCh2" << endl;}
| option {$$ = new ParamSet(); $$->addParam($1); delete $1; }
;

option
: material_statement {$$ = new Param("material", $1);}
| t_vector {$$ = new Param("anon_vector", $1);}
| T_FLOAT {$$ = new Param("anon_float", $1);}
| T_STRING T_FLOAT {$$ = new Param($1, $2);}
| T_STRING t_vector {$$ = new Param($1, $2);}
| T_STRING T_STRING {$$ = new Param($1, string($2));}
| T_STRING {$$ = new Param("anon_string", string($1));}
;

material_statement
: MATERIAL LCB option_list RCB {cout << "material_statement" << endl; $3->addParam("material_type", $1); $$ = new ParamSet(); $$->joinParamSet($3); delete $3; cout << "FOO1" << endl; $$->print(); cout << "FOOSCh1" << endl;}
;

t_vector
: '<' T_FLOAT ',' T_FLOAT '>' {$$ = new std::vector <float> (); $$->push_back($2); $$->push_back($4);}
| '<' T_FLOAT ',' T_FLOAT ',' T_FLOAT '>' {$$ = new std::vector <float> (); $$->push_back($2); $$->push_back($4); $$->push_back($6);}
| '<' T_FLOAT ',' T_FLOAT ',' T_FLOAT ',' T_FLOAT '>' {$$ = new std::vector <float> (); $$->push_back($2); $$->push_back($4); $$->push_back($6); $$->push_back($8);}
;


