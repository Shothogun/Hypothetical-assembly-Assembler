#include "../include_linker/module.hpp"
#include <fstream>
#include <string>
#include <regex>
#include <string.h>
#include <algorithm>
#include <iostream>

void Module::ReadObjectFile(std::string object_file_name){
  std::ifstream object_file(object_file_name);
  std::string file_line;
  std::string temp, temp2;

  std::smatch matches;
  std::regex usage_table_line_regex("(.*)(\\s\\s)(.*)");

  getline(object_file, file_line);
  
  // Read module name
  std::string::iterator c;
  temp = file_line.substr(3);
  for(c = temp.begin(); c != temp.end(); c++){
    this->_module_name.push_back(*c); 
  }
  getline(object_file, file_line);

  // Read module size

  temp = file_line.substr(3);
  for(c = temp.begin(); c != temp.end(); c++){
    temp2.push_back(*c);
  }
  this->_module_size = std::stoi(temp2);

  getline(object_file, file_line);
  // Read bit map
  temp = file_line.substr(3);
  for(c = temp.begin(); c != temp.end(); c++){
    this->_bit_map.push_back((int)(*c - 0x30));
  }

  getline(object_file, file_line);

  // Read object code

  std::regex object_code_regex("^(T:)(\\s)(.*)");
  std::regex split_regex("\\s+");
  std::regex_search(file_line, matches, object_code_regex);
  temp = matches[3];
  std::regex_token_iterator<std::string::iterator> itr(temp.begin(), temp.end(), split_regex, -1);
  std::regex_token_iterator<std::string::iterator> end;
  std::string aux;
  while(itr != end){
    aux = itr->str();
    if(aux.compare("") != 0){
      this->_object_code.push_back(stoi(aux));
    }
    itr++;
  }
  getline(object_file, file_line);
  // Read definition table
  if(file_line.compare("DEFINITION TABLE:") == 0){
    // Skips next line
    getline(object_file, file_line);
    while (getline(object_file, file_line) && (file_line.compare("USAGE TABLE:") != 0)){
      std::regex_search(file_line, matches, usage_table_line_regex);
      this->_definition_table.insert(std::pair<std::string, int>(matches[3] ,stoi(matches[1])));
    }  
  }
  // Read usage table
  if(file_line.compare("USAGE TABLE:") == 0){
    // Skips next line
    getline(object_file, file_line);
    while(getline(object_file, file_line)){
      std::regex_search(file_line, matches, usage_table_line_regex);
      this->_usage_table.insert(std::pair<int, std::string>(stoi(matches[1]), matches[3]));
    }
  }
  /*Debug

  */
  std::cout<<"name: "<<this->_module_name<<std::endl;
  std::cout<<"size: "<<this->_module_size<<std::endl;
  std::cout<<"bit map: ";
  for(auto bit: this->_bit_map){
    std::cout<<bit;
  }
  std::cout<<std::endl;
  std::cout<<"code: ";
  for(auto code: this->_object_code){
    std::cout<<code<<" ";
  }
  std::cout<<std::endl;
  std::cout<<"definition table:"<<std::endl;
  std::map<std::string, int>::iterator definition_table_line;
  for(definition_table_line = this->_definition_table.begin(); 
  definition_table_line != this->_definition_table.end(); definition_table_line++){
    std::cout<<"label: "<<definition_table_line->first<<" value: "<<definition_table_line->second<<std::endl;
  }
  std::cout<<"usage table:"<<std::endl;
  std::map<int, std::string>::iterator usage_table_line;
  for(usage_table_line = this->_usage_table.begin(); 
  usage_table_line != this->_usage_table.end(); usage_table_line++){
    std::cout<<"label: "<<usage_table_line->second<<" address: "<<usage_table_line->first<<std::endl;
  }
  std::cout<<std::endl;

  object_file.close();
}