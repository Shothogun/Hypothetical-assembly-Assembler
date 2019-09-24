#include <iostream>
#include <string>
#include <cstring>


using namespace std;

int main(int argc,char* argv[])
{
  char asm_line_code [100];

  // Source codes are locates in the directory /sources/asm_codes/
  // outside current /exec/ directory(where the program is executed)
  string string_source_code_name = "../src_assembler/asm_codes/";

  // Source code name input at the terminal execution
  // concatenated with directory
  string_source_code_name += argv[1];

  // Convert source_code_name stringvariable to char type
  char char_source_code_name[string_source_code_name.size() + 1];
  strcpy(char_source_code_name, string_source_code_name.c_str());

  // Open the chosen source code file
  FILE* source_code = fopen(char_source_code_name, "r");
  if (source_code == NULL) perror ("Error opening file");
  else
   {
     // Until reach end of file
     while (!feof (source_code) )
     {
       // Gets 100 characters from the source code line
       if ( fgets (asm_line_code , 100 , source_code) == NULL ) break;
     }
     fclose (source_code);
   }

  return 0;
}
