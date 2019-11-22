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
    MakeGlobalDefinitionTable();
    MakeGlobalUsageTable();
    ConcatenatesObjectCodes();
    ResolveReferences();
    
    /*Debug*/
    std::map<std::string, int>::iterator itr;
    std::map<int, std::string>::iterator itr2;
    std::vector<int>::iterator itr3;
    std::cout<<"Global definition table"<<std::endl;
    for(itr = this->_global_definition_table.begin(); itr != this->_global_definition_table.end();
    itr++){
      std::cout<<"label: "<<itr->first<<" value: "<<itr->second<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<"Global usage table"<<std::endl;
    for(itr2 = this->_global_usage_table.begin(); itr2 != this->_global_usage_table.end();
    itr2++){
      std::cout<<"address: "<<itr2->first<<" label: "<<itr2->second<<std::
      endl;
    }
    std::cout<<std::endl;
    std::cout<<"Object code: "<<std::endl;
    for(itr3 = this->_linked_object_code.begin(); itr3 != this->_linked_object_code.end();
    itr3++){
      std::cout<<*itr3<<" ";
    }
    std::cout<<std::endl;
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

void Linker::MakeGlobalDefinitionTable(){
  std::map<std::string, int>::iterator definition_table_line;
  // Insert definition table of first module in global definition table
  this->_global_definition_table = this->_module_A->get_definition_table();

  // Insert definition table of second module in global definition table with correction factor
  std::map<std::string, int> definition_table_B = this->_module_B->get_definition_table();
  for(definition_table_line = definition_table_B.begin(); definition_table_line != definition_table_B.end();
  definition_table_line++){
    this->_global_definition_table.insert(std::pair<std::string, int>(definition_table_line->first,
     definition_table_line->second + this->_correction_factor));
  }
}

void Linker::MakeGlobalUsageTable(){
  std::map<int, std::string>::iterator usage_table_line;
  // Insert usage table of first module in global usage table
  this->_global_usage_table = this->_module_A->get_usage_table();

  // Insert usage table of second module in global usage table with correction factor
  std::map<int, std::string> usage_table_B = this->_module_B->get_usage_table();
  for(usage_table_line = usage_table_B.begin(); usage_table_line != usage_table_B.end();
  usage_table_line++){
    this->_global_usage_table.insert(std::pair<int, std::string>(usage_table_line->first 
    + this->_correction_factor, usage_table_line->second));
  }
}

void Linker::ConcatenatesObjectCodes(){
  // Insert object code of first module in linked object code
  this->_linked_object_code = this->_module_A->get_object_code();
  std::vector<int>::iterator code;

  // Inser object code of second module in linked object code with correction factor
  // in relative values
  std::vector<int> object_code_B = this->_module_B->get_object_code();
  std::vector<int>::iterator bit;
  std::vector<int> bit_map = this->_module_B->get_bit_map();
  for(code = object_code_B.begin(), bit = bit_map.begin(); code != object_code_B.end(); code++, bit++){
    // If bit 1 then the value is relative and must be corrected with the correction factor
    if(*bit){
      this->_linked_object_code.push_back(*code + this->_correction_factor);
    }
    else{
      this->_linked_object_code.push_back(*code);
    }
  }
}

void Linker::ResolveReferences(){
  std::map<int, std::string>::iterator usage_table_line;
  for(usage_table_line = this->_global_usage_table.begin(); usage_table_line != this->_global_usage_table.end();
  usage_table_line++){
    this->_linked_object_code[usage_table_line->first] = this->_global_definition_table[usage_table_line->second];
  }
}