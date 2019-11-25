#ifndef LINKER_HPP
#define LINKER_HPP
#include "../include_linker/module.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <map>

//! Linker class
/*
 * This class is a representation of an Linker, 
 * which role is get one or two object files and
 * generate a linked object file.
 * 
*/
class Linker{
  public:
    //! Linker(char* obj_file_name);
    /*
     * Generates an object code module from a single object file.
     *
    */
    Linker(char* obj_file_name);

    //! Linker(char* first_obj_file_name, char* second_obj_file_name);
    /*
     * Generates two object code module from two object files
     *
    */
    Linker(char* first_obj_file_name, char* second_obj_file_name);
    
    //! ~Linker();
    /*
     * Linker destructor.
     *
    */    
    ~Linker();

    //! Linking();
    /*
     * Generate linked object code from the provided modules.
     *
    */    
    void Linking();

    //! void MakeObjectFile(char* linked_file_name);      
    /*
     * Generates the linked object file with the specified name
     *
    */
    void MakeObjectFile(char* linked_file_name);      

  private:
    //! _module_A;
    /*
     * Store the first module provided.
     *
    */ 
    Module *_module_A;

    //! _module_B;
    /*
     * Store the second module provided.
     *
    */ 
    Module *_module_B;

    //! _correction_factor
    /*
     * Store de correction factor from the provided second module size.
     *
    */ 
    int _correction_factor;

    //! is_two_Modules
    /*
     * Indicates if is two modules.
     *
    */
    bool is_two_Modules = false;

    //! _module_B_start_point
    /*
     * Indicates Linked object code beginning
     * address
     *
    */
    int _module_B_start_point;

    //! _global_definition_table
    /*
     * Store all definition information from the provided modules.
     *
    */
    std::map<std::string, int> _global_definition_table;

    //! _global_usage_table
    /*
     * Store all usage information from the provided modules.
     *
    */
    std::map<int, std::string> _global_usage_table;

    //! _linked_object_code
    /*
     * Store the concatenation of the object codes from the provided modules.
     * 
    */
    std::vector<int> _linked_object_code;

    //! void MakeGlobalDefinitionTable();
    /*
     * Concatenates the definition tables from the provided modules with correction 
     * factor application in the definition table from the second module.
     *
    */ 
    void MakeGlobalDefinitionTable();
    
    //! void MakeGlobalUsageTable();
    /*
     * Concatenates the usage tables from the provided modules with correction
     * factor application in the usage table from the second module.
     *
    */
    void MakeGlobalUsageTable();
    
    //! void ResolveReferences();
    /*
     * Resolves pending references in concatenated object code,
     * with information in the global usage table and global definition table.
    */ 
    void ResolveReferences();
    
    //! void ConcatenatesObjectCodes();            
    /*
     * Concatenates the object codes from the provided modules, with correction 
     * factor application in the object code from the second module.
     *
    */
    void ConcatenatesObjectCodes();            
};

#endif