#include <stdio.h>                 
                                   
#include <iostream>                
#include <string.h>                
#include <fstream>                 
#include <vector>
#include <getopt.h>
                                   
#include <openbabel/babelconfig.h> 
#include <openbabel/base.h>        
#include <openbabel/mol.h>         
#include <openbabel/obconversion.h>

struct Option {
  std::string f_in;
  std::string format_in;
  std::string format_out;
};

Option get_options (int argc, char **argv) {
  int c;

  Option opts;

  while (1) {
    static struct option long_options[] = {
      {"f_in", required_argument, 0, 'a'},
      {"format_in", required_argument, 0, 'b'},
      {"format_out", required_argument, 0, 'c'}
    };

    int option_index = 0;

    c = getopt_long(argc, argv, "a:b:c:", long_options, &option_index);

    if (c == -1)
      break;

    switch (c) {

      case 0:
        if (long_options[option_index].flag != 0) break;
      break;

      case 'a':
        opts.f_in = optarg;
      case 'b':
        opts.format_in = optarg;
      case 'c':
        opts.format_out = optarg;
    }
  }

  if (optind < argc) {
    printf("non-option ARGV-elements: ");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    putchar ('\n');
  }

  if (opts.f_in.compare("") == 0) {
    printf("ERROR: no molecule specified.\n");
    exit(0);
  }
  if (opts.format_in.compare("") == 0) {
    printf("ERROR: no input format specified.\n");
    exit(0);
  }
  if (opts.format_out.compare("") == 0) {
    printf("ERROR: no output format specified.\n");
    exit(0);
  }

  return opts;

}


OpenBabel::OBMol readfile(std::string filename, std::string InFormat) {

    OpenBabel::OBMol mol;

    OpenBabel::OBConversion conv;
    conv.SetInFormat(InFormat.c_str());
    std::ifstream ifs;

    ifs.open(filename.c_str());
    conv.Read(&mol, &ifs);
    ifs.close();

    return mol;

}

void writefile(OpenBabel::OBMol mol, std::string filename, std::string OutFormat) {

  OpenBabel::OBConversion conv;         
  conv.SetOutFormat(OutFormat.c_str());

  std:: ofstream ofs(filename);
  conv.Write(&mol, &ofs);
  ofs.close();

}

std::string get_f_out(std::string f_in, std::string format_out) {
  std::string f_out;

  f_out = f_in.substr(0, f_in.size()-4);
  f_out += ".";
  f_out += format_out;

  return f_out;
}

int main(int argc, char *argv[]) {

  Option opts = get_options(argc, argv);

  std::string f_out = get_f_out(opts.f_in, opts.format_out);

  //std::cout << f_in << " " << format_in << " " << format_out << std::endl;
  OpenBabel::OBMol mol = readfile(opts.f_in, opts.format_in);
  writefile(mol, f_out, opts.format_out);

}
