CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -march=native

OGLFLAGS = -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lGLU
SDLFLAGS = `sdl-config --cflags --libs` 

BINS = SDL_test
OBJ = SDL_test.o GL_setup.o distributions.o shape_functions.o test_pong.o
DEPS = include/SDL_test.h include/GL_setup.h include/distributions.h

%.o: ./src/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $(OGLFLAGS) $(SDLFLAGS) -c -o $@ $<

$(BINS): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OGLFLAGS) $(SDLFLAGS) -o $(BINS) $^

clean:
	rm -Rf $(BINS) $(OBJ)

