CXX=g++
SRC=src/main.cpp
OUT=build/program
FLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf

all:
	$(CXX) $(SRC) -o $(OUT) $(FLAGS)