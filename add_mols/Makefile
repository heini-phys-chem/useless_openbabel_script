CXX = g++

OBDIR = /home/heinen/anaconda3/envs/openbabel

INCLUDE = -I /home/heinen/anaconda3/envs/openbabel/include/openbabel3
LIBS = -L /home/heinen/anaconda3/envs/openbabel/lib

CXX_FLAGS = -std=c++17 -O3 -march=native -Wall
LINKER_FLAGS = -lopenbabel

all: add_mols

add_mols: src/add_mols.cpp src/utils.hpp
	$(CXX) $(INCLUDE) $(LIBS) $(CXX_FLAGS) src/add_mols.cpp -o add_mols $(LINKER_FLAGS)

clean:
	rm -f add_mols
	rm -f mols_combined.xyz

