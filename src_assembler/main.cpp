#include <iostream>
#include <string>
#include <cstring>
#include "../include_assembler/preprocessor.hpp"


using namespace std;

int main(int argc,char* argv[]) {
  char* source_code_file_name = argv[1];

  // Not deleted yet
  Preprocessor* preprocessed_file = new Preprocessor(source_code_file_name);

  return 0;
}
