#include <stdio.h>                 
                                   
#include <iostream>                
#include <string.h>                
#include <fstream>                 
                                   
#include <openbabel/babelconfig.h> 
#include <openbabel/base.h>        
#include <openbabel/mol.h>         
#include <openbabel/obconversion.h>


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

  char *cmd1 = argv[1];
  char *cmd2 = argv[2];
  char *cmd3 = argv[3];

  std::string f_in;
  std::string format_in;
  std::string format_out;

  f_in       += cmd1;
  format_in  += cmd2;
  format_out += cmd3;

  std::string f_out = get_f_out(f_in, format_out);

  //std::cout << f_in << " " << format_in << " " << format_out << std::endl;
  OpenBabel::OBMol mol = readfile(f_in, format_in);
  writefile(mol, f_out, format_out);

}
