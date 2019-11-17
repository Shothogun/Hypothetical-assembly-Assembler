#include <iostream>
#include <string>
#include <cstring>
#include <string.h>
#include "../include_linker/linker.hpp"

int main(int argc,char* argv[]){

  if(argc == 3){
    char* first_obj_file_name = argv[1];
    char* linked_file_name = argv[2];
    Linker* linker = new Linker(first_obj_file_name);
    linker->Linking();
    linker->MakeObjectFile(linked_file_name);
    delete linker;
  }
  else if(argc == 4){
    char* first_obj_file_name = argv[1];
    char* second_obj_file_name = argv[2];
    char* linked_file_name = argv[3];
    Linker* linker = new Linker(first_obj_file_name, second_obj_file_name);
    linker->Linking();
    linker->MakeObjectFile(linked_file_name);
    delete linker;
  }  
  else{
    std::cout << "Invalid number of parameter " << std::endl;
  }

  return 0;
}