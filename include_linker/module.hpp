#ifndef MODULE_HPP
#define MODULE_HPP
#include <iostream>
#include <string>
#include <map>
#include <vector>

//! Module class
/*
 * This class is a representation of an object code module
 * that contains all the information required for the linker.
 * 
*/
class Module{
  private:
    //! _module_name
    /*
     * Stores the module name corresponding to the object file.
     *
    */
    std::string _module_name;

    //! _module_size
    /*
     * Stores object code size.
     *
    */
    uint _module_size;

    //! _bit_map
    /*
     * Stores the obj code bit map.
     *
    */
    std::vector<int> _bit_map;
           
    //! _definition_table
    /*
     * Stores the definition table corresponding to object code.
     *
    */
    std::map<std::string, int> _definition_table;

    //! _usage_table
    /*
     * Stores the usage table corresponding to object code.
     *
    */
    std::map<int, std::string> _usage_table;
    
    //! _object_code
    /*
     * Stores the object code.
     *
    */
    std::vector<int> _object_code;

  public:
    //! void ReadObjectFile(std::string object_file_name)
    /*
     * Read an object file and get header information 
     * and usage and definitions tabels.
     *
    */
    void ReadObjectFile(std::string object_file_name);

    //! std::string get_module_name()
    /*
     * Return the module name corresponding to the object file.
     *
    */
    std::string get_module_name(){
      return this->_module_name;
    }
    //! uint  get_module_size()
    /*
     * Return the object code size.
     *
    */
    uint  get_module_size(){
      return this->_module_size;
    }
    //! std::vector<int> get_bit_map()
    /*
     * Return the bit map corresponding to object code.
     *
    */
    std::vector<int> get_bit_map(){
      return this->_bit_map;
    }

    //! std::map<std::string, int> get_definition_table()
    /*
     * Return the definition table corresponding to object code.
     *
    */
    std::map<std::string, int> get_definition_table(){
      return this->_definition_table;
    }

    //! std::map<int, std::string> get_usage_table()
    /*
     * Return the usage table corresponding to object code.
     *
    */
    std::map<int, std::string> get_usage_table(){
      return this->_usage_table;
    }

    //! std::vector<int> get_object_code()
    /*
     * Return the object code.
     *
    */                  
    std::vector<int> get_object_code(){
      return this->_object_code;
    }
};

#endif