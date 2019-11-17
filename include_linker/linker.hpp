#ifndef LINKER_HPP
#define LINKER_HPP
#include "../include_linker/module.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <map>

//!
/*
 *
 *
*/
class Linker{
  public:
    //!
    /*
     *
     *
    */
    Linker(char* obj_file_name);
    //!
    /*
     *
     *
    */
    Linker(char* first_obj_file_name, char* second_obj_file_name);
    
    //!
    /*
     *
     *
    */    
    ~Linker();
   
    //!
    /*
     *
     *
    */     
    void Linking();
    
    //!
    /*
     *
     *
    */
    void MakeObjectFile(char* linked_file_name);      

  private:
    //!
    /*
     *
     *
    */ 
    Module *_module_A;
    //!
    /*
     *
     *
    */ 
    Module *_module_B;
    //!
    /*
     *
     *
    */ 
    int _correction_factor;
    //!
    /*
     *
     *
    */
    bool is_two_Modules = false;
    //!
    /*
     *
     *
    */
    std::map<std::string, int> _global_definition_table;

    //!
    /*
     *
     *
    */
    std::map<int, std::string> _global_usage_table;

    //!
    /*
     *
     * 
    */
    std::vector<int> _linked_object_code;

    //!
    /*
     *
     *
    */ 
    void MakeGlobalDefinitionTable();
    
    //!
    /*
     *
     *
    */
    void MakeGlobalUsageTable();
    
    //!
    /*
     *
     *
    */ 
    void ResolveReferences();
    
    //!
    /*
     *
     *
    */
    void ConcatenatesObjectCodes();            
};

#endif