#include <iostream>
#include <string>
#include <cstring>
#include "../include_assembler/preprocessor.hpp"
#include "../include_assembler/assembler.hpp"



using namespace std;

int main(int argc,char* argv[]) {
  char* source_code_file_name = argv[1];

  // Creates the Preprocessed file instance, not preprocessed yet
  Preprocessor* preprocessed_file = new Preprocessor(source_code_file_name);

  // Preprocessing of the preprocess file
  preprocessed_file->Preprocessing();

  // Produces the Preprocessed file "*.pre"
  preprocessed_file->MakePreFile(source_code_file_name);

  // Creates the Assembler instance, ready to compile the .pre file
  Assembler* assembler = new Assembler(source_code_file_name);
  assembler->Assembling();

  delete preprocessed_file;
  delete assembler;

  return 0;
}
