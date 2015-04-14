INC_DIR_LOCAL =../include
INC_DIR = /usr/local/opt/boost/include
CC=g++
CFLAGS=-g -static -I$(INC_DIR_LOCAL) -I$(INC_DIR) -std=c++11 `libpng-config --cflags` 
LDFLAGS=-std=c++11 `libpng-config --ldflags` -g

OBJ_DIR=../obj
LIB_DIR =../lib

LIBS=-lm

#_DEPS = Camera.hpp Color.hpp Draco2.hpp IntersectRec.hpp Material.hpp Parser.hpp Primitive.hpp Ray.hpp Vector.hpp
_DEPS = Accelerator.hpp Camera.hpp Draco2.hpp Film.hpp Light.hpp Material.hpp Matrix.hpp ONB.hpp Options.hpp Parser.hpp Primitive.hpp Ray.hpp Renderer.hpp Sampler.hpp Scene.hpp Vector.hpp
DEPS = $(patsubst %,$(INC_DIR_LOCAL)/%,$(_DEPS))

_OBJ = Accelerator.o draco2.o Film.o Light.o Material.o ONB.o Parser.o Primitive.o Ray.o Renderer.o Sampler.o Scene.o Vector.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))
OBJ += Parser.tab.o lex.yy.o

all:	draco2

Parser.tab.c:	Parser.l Parser.y
			/usr/local/Cellar/bison/3.0.2/bin/bison -d -v Parser.y
			/usr/local/Cellar/flex/2.5.37/bin/flex -d Parser.l

Parser.tab.o:	Parser.tab.c
	$(CC) -c Parser.tab.c $(CFLAGS)

lex.yy.o:	Parser.tab.c
	$(CC) -c lex.yy.c $(CFLAGS)

$(OBJ_DIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

draco2: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o *~ core $(INC_DIR_LOCAL)/*~ 
	rm Parser.tab*
	rm lex.yy*
	rm ./draco2