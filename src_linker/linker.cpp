#include<iostream>
#include<fstream>
#include<string>
#include<regex>
#include "../include_linker/linker.hpp"

Linker::Linker(char* first_obj_file_name, char* second_obj_file_name){
  this->is_two_Modules = true;
  this->_module_A = new Module();
  this->_module_B = new Module();
  std::string first_file_path = "../exec_assembler/obj_files/";
  std::string second_file_path = "../exec_assembler/obj_files/";
  first_file_path += first_obj_file_name;
  second_file_path += second_obj_file_name;
  this->_module_A->ReadObjectFile(first_file_path);
  this->_module_B->ReadObjectFile(second_file_path);
  this->_correction_factor = this->_module_A->get_module_size();
}

Linker::Linker(char* obj_file_name){
  this->_module_A = new Module();
  std::string file_path = "../exec_assembler/obj_files/";
  file_path += obj_file_name;
  this->_module_A->ReadObjectFile(file_path);
}

Linker::~Linker(){
  delete this->_module_A;
  if(is_two_Modules){
    delete this->_module_B;
  }
}

void Linker::Linking(){
  if(!is_two_Modules){
    this->_linked_object_code = this->_module_A->get_object_code();
  }
  else{

  }
  return;
}

void Linker::MakeObjectFile(char* linked_file_name){
  std::string file_name = "./linked_files/";
  std::string extension = ".obj"; 
  file_name += linked_file_name;
  file_name += extension;
  
  // Create linked obj file
  std::ofstream linked_file(file_name);

  for(auto code: this->_linked_object_code){
    linked_file<<code<<" ";
  }
  linked_file<<std::endl;

  linked_file.close();

  return;
}