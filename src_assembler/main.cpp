#include <iostream>
#include <string>
#include <cstring>
#include "../include_assembler/preprocessor.hpp"
#include "../include_assembler/assembler.hpp"



using namespace std;

int main(int argc,char* argv[]) {

  
  char* source_code_file_name = argv[1];
  
  if((argc < 2) || (argc > 3)){
    cout << "Invalid number of parameter " << endl;
    return 0;
  }

  // Creates the Preprocessed file instance, not preprocessed yet
  Preprocessor* preprocessed_file = new Preprocessor(source_code_file_name);

  // Preprocessing of the preprocess file
  preprocessed_file->Preprocessing();

  // Produces the Preprocessed file "*.pre"
  preprocessed_file->MakePreFile(source_code_file_name);

  // Creates the Assembler instance, ready to compile the .pre file
  Assembler* assembler = new Assembler(source_code_file_name);

  // If two source code, this is a module
  if(argc == 3){
    assembler->_is_MODULE = true;
  }
  
  assembler->Assembling();

  // If two source code, mounts the second file
  if(argc == 3){
    char* second_source_code_file_name = argv[2];
    // Creates the Preprocessed file instance, not preprocessed yet
    Preprocessor* second_preprocessed_file = new Preprocessor(second_source_code_file_name);

    // Preprocessing of the preprocess file
    second_preprocessed_file->Preprocessing();

    // Produces the Preprocessed file "*.pre"
    second_preprocessed_file->MakePreFile(second_source_code_file_name);

    // Creates the Assembler instance, ready to compile the .pre file
    Assembler* second_assembler = new Assembler(second_source_code_file_name);

    second_assembler->_is_MODULE = true;

    second_assembler->Assembling();

    second_assembler->MakeObjectFile(second_source_code_file_name);

    // Set offset values at extern labels
    assembler->LinkOffsetLabels();
    second_assembler->LinkOffsetLabels();
    
    second_assembler->_assembling_errors->display(1);

    delete second_preprocessed_file;
    delete second_assembler;
  }

  assembler->MakeObjectFile(source_code_file_name);

  assembler->_assembling_errors->display(1);

  delete preprocessed_file;
  delete assembler;

  return 0;
}
