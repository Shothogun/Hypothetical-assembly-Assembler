#include<iostream>
#include<string>
#include<regex>
#include "../include_linker/linker.hpp"

Linker::Linker(char* first_obj_file_name, char* second_obj_file_name){
  this->is_two_Module = true;
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
  if(is_two_Module){
    delete this->_module_B;
  }
}

void Linker::Linking(){
  return;
}

void Linker::MakeObjectFile(){
  return;
}