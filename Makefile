CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -march=native -Wextra -pedantic

ifeq ($(shell uname), Darwin)
OGLFLAGS = -lGLEW -framework OpenGL -lpthread -framework GLUT -lfreetype -I/usr/local/include/freetype2 -lfftw3
else
OGLFLAGS = -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi -lGLU -lfreetype -I/usr/include/freetype2 -lfftw3
endif

SDLFLAGS = `sdl2-config --cflags --libs` 

BINS = SDL_test
OBJ = SDL_test.o GL_setup.o distributions.o shape_functions.o test_pong.o shaders.o operations.o
DEPS = include/SDL_test.h include/GL_setup.h include/distributions.h include/operations.h

%.o: ./src/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $(OGLFLAGS) $(SDLFLAGS) -c -o $@ $<

$(BINS): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OGLFLAGS) $^ $(SDLFLAGS) -o $(BINS)

clean:
	rm -Rf $(BINS) $(OBJ)

