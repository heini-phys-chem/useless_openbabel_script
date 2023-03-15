#define HAVE_EIGEN


#include <stdio.h>

#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <random>
#include <getopt.h>

#include <openbabel/babelconfig.h>
#include <openbabel/base.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/forcefield.h>
#include <openbabel/obutil.h>
#include <openbabel/builder.h>

#include "utils.hpp"



int main(int argc, char *argv[]) {


    McDock::Option opts = McDock::get_options(argc, argv);

    // Get OBMols from smiles
    OpenBabel::OBMol mol = McDock::readsmile(opts.target);
    OpenBabel::OBMol ligand = McDock::readsmile(opts.ligand);

    // generate 3D geometry
    OpenBabel::OBBuilder builder;
    builder.Build(mol);
    builder.Build(ligand);

    // add hydrogens
    mol.AddHydrogens();
    ligand.AddHydrogens();

    // define vector for center of mass
    OpenBabel::vector3 com;

    // Center ligand molecule
    com = McDock::get_com(ligand);
    McDock::move_molecule(ligand, -com);

    // Center target molecule
    com = McDock::get_com(mol);
    McDock::move_molecule(mol, -com);

    // Initialize random number generators
    std::default_random_engine generator;
    std::uniform_real_distribution<double> random_length(0.0, 0.25);
    std::uniform_real_distribution<double> random_angle(0.0, 90.0/180.0*M_PI);
    std::uniform_real_distribution<double> uniform1(0.0, 1.0);

    // move ligand molecule in random direction
    OpenBabel::vector3 temp;
    OpenBabel::vector3 dir;

    dir.randomUnitVector();
    com = McDock::get_com(ligand);
    temp = dir * 3.0 - com;
    McDock::move_molecule(ligand, temp);

    // Gent energies of target molecule (after FF optimization)
    double ea;
    ea = McDock::minimize_molecule(mol, opts.ff);
    printf("Target (minimized) E = %10.4f kcal/mol     file: mol \n", ea);

    // Initialize force field
    OpenBabel::OBForceField* pFF = OpenBabel::OBForceField::FindForceField(opts.ff);

    double eb;
    eb = McDock::minimize_molecule(ligand, opts.ff);
    printf("Target (minimized) E = %10.4f kcal/mol     file: ligand \n", eb);

    // Make object to roll-back rejected MC moves
    OpenBabel::OBMol mol_ligand = mol;
    mol_ligand += ligand;

    pFF->Setup(mol_ligand);

    double e;
    e = McDock::minimize_molecule(mol_ligand, opts.ff);
    printf("Target (minimized) E = %10.4f kcal/mol     file: mol_ligand \n", e);

    McDock::save_xyz(mol_ligand, "mols_combined.xyz");

}
