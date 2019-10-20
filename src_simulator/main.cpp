#include <iostream>
#include <string>
#include <cstring>
#include "../include_simulator/memory.hpp"

using namespace std;

int main(int argc,char* argv[]) {
  char asm_line_code [5000];

  // Obj files are locates in the directory /src_simulator/obj_files/
  // outside current /exec_simulator/ directory(where the program is executed)
  string string_source_code_name = "../exec_assembler/obj_files/";

  // Source code name input at the terminal execution
  // concatenated with directory
  string_source_code_name += argv[1];

  // Convert source_code_name string variable to char type
  char char_source_code_name[string_source_code_name.size() + 1];
  strcpy(char_source_code_name, string_source_code_name.c_str());

  // Open the chosen obj file
  FILE* source_code = fopen(char_source_code_name, "r");
  if (source_code == NULL) perror ("Error opening file");
  else {
     // Until reach end of file
     while (!feof (source_code) )
     {
       // Gets 100 characters from the source code line
       if ( fgets (asm_line_code , 5000 , source_code) == NULL ) break;
     }
     fclose (source_code);
  }

  // Converts asm_line_code from char to string
  std::string asm_line_code_string = asm_line_code;

  // Loads object file into memory
  LoadProgram(asm_line_code_string);

  fetch_execute_cycle();
  
  
  return 0;
}
