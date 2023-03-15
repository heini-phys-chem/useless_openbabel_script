# useless_openbabel_script

## Installation
OpenBabel is needed. Install it via conda:
```
conda install -c conda-forge openbabel
```
Then adapt the Makefile (example from add_mols)
```
XX = g++

OBDIR = <path/to/directory>/anaconda3/envs/openbabel

INCLUDE = -I <path/to/directory>/anaconda3/envs/openbabel/include/openbabel3
LIBS = -L <path/to/directory>/anaconda3/envs/openbabel/lib

CXX_FLAGS = -std=c++17 -O3 -march=native -Wall
LINKER_FLAGS = -lopenbabel

all: add_mols

add_mols: src/add_mols.cpp src/utils.hpp
	$(CXX) $(INCLUDE) $(LIBS) $(CXX_FLAGS) src/add_mols.cpp -o add_mols $(LINKER_FLAGS)

clean:
	rm -f add_mols
	rm -f mols_combined.xyz

```
If it compiles but after running the executable following error appears:
```
error while loading shared libraries: libopenbabel.so.7: cannot open shared object file: No such file or directory
```
Add following line to your .bashrc:
```
LD_LIBRARY_PATH=<path/to/directory>/anaconda3/envs/openbabel/lib/:$LD_LIBRARY_PATH'
```


## Use
*convert*: reads in a molecule using a specified file format and converts it to a desired format.
```
conv --f_in <full path to molecule> --format_in <input format> --format_out <output format>
```
*add_mols*: reading in 2 smiles codes, add hydrogens, generates 3D geometries, puts molecule in the same 'file' moves them apart and minimizes the energy using a forcefield and finally saves them as an xyz file.
```
add_mols --target 'O=Cl(=O)([O-])=O' --ligand 'C12=C(C=CC=C2)C=C3C=CC=CC3=[NH+]1'
```

TODO: a lot!
