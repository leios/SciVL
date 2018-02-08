CXX = g++
CXXFLAGS = -std=c++11 -Wall -march=native -Wextra -pedantic -I./include -g

ifeq ($(shell uname), Darwin)
OGLFLAGS = -lGLEW -framework OpenGL -lpthread -framework GLUT -lfreetype -I/usr/local/include/freetype2 -lfftw3
else
OGLFLAGS = -lX11 -lpthread -lXrandr -lXi -lfreetype -I/usr/include/freetype2 -lfftw3 -lGLEW -lGL -lGLU 
endif

SDLFLAGS = `sdl2-config --cflags --libs` 

BINS = SciVL
OBJ = SDL_test.o GL_setup.o distributions.o shape_functions.o aux_functions.o shaders.o operations.o parser.o parg.o platformer.o pong.o fourier.o test_animation.o euclid.o bits.o fft.o verlet.o traverse.o thanks.o convolution.o plot.o stable_marriage.o
#DEPS = include/SDL_test.h include/GL_setup.h include/distributions.h include/operations.h include/parser.h include/parg.h include/aux_functions 

%.o: ./src/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $(SDLFLAGS) -c -o $@ $< $(OGLFLAGS)

$(BINS): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ $(SDLFLAGS) -o $(BINS) $(OGLFLAGS)

clean:
	rm -Rf $(BINS) $(OBJ)

