#ifndef SRC_UTILS_HPP
#define SRC_UTILS_HPP
#define HAVE_EIGEN

namespace McDock {

OpenBabel::OBMol readsmile(std::string filename) {
    OpenBabel::OBMol mol;
    OpenBabel::OBConversion conv;

    conv.SetInFormat("smi");

    conv.ReadString(&mol, filename);

    return mol;
}

OpenBabel::OBMol readfile(std::string filename) {

    OpenBabel::OBMol mol;

    OpenBabel::OBConversion conv;
    conv.SetInAndOutFormats("xyz", "xyz");
    std::ifstream ifs;

    ifs.open(filename.c_str());
    conv.Read(&mol, &ifs);
    ifs.close();

    return mol;

}

OpenBabel::vector3 get_com(OpenBabel::OBMol mol) {

    OpenBabel::vector3 com;
    com.Set(0.0, 0.0, 0.0);
    OpenBabel::OBAtom *atom;

    OpenBabel::vector3 temp;
    for (unsigned int i = 1; i < mol.NumAtoms() + 1; i++) {

        atom = mol.GetAtom(i);
        temp = atom->GetVector();
        com += temp;
    }

    com /= mol.NumAtoms();

    return com;

}


void move_molecule(OpenBabel::OBMol &mol, OpenBabel::vector3 move,
        int startid = 1, int endid = -1) {

    if (endid == -1) endid = mol.NumAtoms() + 1;

    OpenBabel::vector3 temp;
    OpenBabel::OBAtom *atom;

    for (int i = startid; i < endid; i++) {
       atom = mol.GetAtom(i);
       temp = atom->GetVector();
       temp += move;
       atom->SetVector(temp);
    }

}

double minimize_molecule(OpenBabel::OBMol &mol, const std::string &ff) {

    OpenBabel::OBForceField* pFF = OpenBabel::OBForceField::FindForceField(ff);
    pFF->Setup(mol);

    double e = pFF->Energy();
    // printf("E_before = %10.4f kJ/mol   %i\n", e, mol.NumAtoms());

    const int steps = 200;
    const double crit = 5.0e-4;

    // pFF->SteepestDescentInitialize(steps, crit);
    pFF->ConjugateGradientsInitialize(steps, crit);

    bool done = true;

    while (done) {

        // done = pFF->SteepestDescentTakeNSteps(1);
        done = pFF->ConjugateGradientsTakeNSteps(1);

        if (pFF->DetectExplosion()) {

            std::cerr << "explosion has occured!" << std::endl;
            exit(1);

        } else {

            pFF->GetCoordinates(mol);
        }

    }
    e = pFF->Energy();
    mol.SetEnergy(e);

    // printf("E_after = %10.4f kJ/mol %i\n", e, mol.NumAtoms());
    return e;

}

void save_xyz(OpenBabel::OBMol mol, std::string filename) {

    OpenBabel::OBConversion conv;
    conv.SetInAndOutFormats("xyz", "xyz");

    std::ofstream ofs(filename);

    //std::string title = std::to_string(e_bind);
    //mol_ligand.SetTitle(title);

    conv.Write(&mol, &ofs);
    ofs.close();


}

// Prints help
void print_help() {

        printf("Usage: ./mcdock --target file1.xyz --ligand file2.xyz [--args]\n\n");

        printf("Optional arguments:\n");

        printf("--energy [string]       Potential energy function \"MMFF94\" (default),\n");
        printf("                        \"UFF\", \"PM6-D3H4\" (requires MOPAC). \n");
        printf("--temperature [float]   Temperature in units of [R T] (default = 1.0).\n");
        printf("--trajectories [int]    Number of independent trajectories (default = 10).\n");
        printf("--mc-steps [int]           Number of Monte Carlo steps in each trajectories (default = 1000).\n");
        printf("--no-rotor-search       Disable rotor search.  (default = perform rotor search).\n");

        printf("\n");
}

// Container for command-line options
struct Option {

    std::string ff = "MMFF94";
    std::string target;
    std::string ligand;
    bool use_mopac = false;

};

// Option parser
Option get_options (int argc, char **argv) {

    if (argc < 2) {
        print_help();
        exit(0);
    }

    int c;

    Option opts;

    while (1) {
        static struct option long_options[] = {
            {"target",        required_argument, 0, 'a'},
            {"ligand",        required_argument, 0, 'b'},
            {"energy",        required_argument, 0, 'd'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        c = getopt_long (argc, argv, "a:b:d:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {

            case 0:
                if (long_options[option_index].flag != 0) break;
            break;

            case 'a':
              opts.target = optarg;
              break;

            case 'b':
              opts.ligand = optarg;
              break;

            case 'd': {
                std::string ff = optarg;

                if (ff.compare("MMFF94") == 0) {
                    opts.ff = optarg;
                    opts.use_mopac = false;
                } else if (ff.compare("MMFF94s") == 0) {
                    opts.ff = optarg;
                    opts.use_mopac = false;
                } else if (ff.compare("GAFF") == 0) {
                    opts.ff = optarg;
                    opts.use_mopac = false;
                } else if (ff.compare("UFF") == 0) {
                    opts.ff = optarg;
                    opts.use_mopac = false;
                } else if (ff.compare("PM6-D3H4") == 0) {
                    opts.ff = "MMFF94";
                    opts.use_mopac = true;

                }  else {
                    printf ("ERROR: Unsupported force field `%s'\n", optarg);
                    exit(0);
                }
                break;
            }
            case '?':
              /* getopt_long already printed an error message. */
              break;

            default:
              abort ();
            }
    }

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (optind < argc)
        printf ("%s ", argv[optind++]);
      putchar ('\n');
    }

    if (opts.ligand.compare("") == 0) {
        printf("ERROR: No ligand xyz-file specified.\n");

        print_help();
        exit(0);
    }

    if (opts.target.compare("") == 0) {
        printf("ERROR: No target xyz-file specified.\n");

        print_help();
        exit(0);
    }

    return opts;

}


} // Namespace McDock

#endif

