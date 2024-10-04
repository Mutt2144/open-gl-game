CXX=g++
SRC=src/main.cpp
OUT=build/program
FLAGS=-lSDL2 -lSDL2_image

all:
	$(CXX) $(SRC) -o $(OUT) $(FLAGS)