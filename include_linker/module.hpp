#ifndef MODULE_HPP
#define MODULE_HPP
#include <iostream>
#include <string>
#include <map>
#include <vector>

//!
/*
 *
 *
*/
class Module{
  private:
    //!
    /*
     *
     *
    */
    std::string _module_name;

    //!
    /*
     *
     *
    */
    uint _module_size;

    //!
    /*
     *
     *
    */
    std::vector<int> _bit_map;
           
    //!
    /*
     *
     *
    */
    std::map<std::string, int> _definition_table;

    //!
    /*
     *
     *
    */
    std::map<int, std::string> _usage_table;
    
    //!
    /*
     *
     *
    */
    std::vector<int> _object_code;

  public:
    //!
    /*
     *
     *
    */
    void ReadObjectFile(std::string object_file_name);

    //!
    /*
     *
     *
    */
    std::string get_module_name(){
      return this->_module_name;
    }
    //!
    /*
     *
     *
    */
    uint  get_module_size(){
      return this->_module_size;
    }
    //!
    /*
     *
     *
    */
    std::vector<int> get_bit_map(){
      return this->_bit_map;
    }

    //!
    /*
     *
     *
    */
    std::map<std::string, int> get_definition_table(){
      return this->_definition_table;
    }

    //!
    /*
     *
     *
    */
    std::map<int, std::string> get_usage_table(){
      return this->_usage_table;
    }

    //!
    /*
     *
     *
    */                  
    std::vector<int> get_object_code(){
      return this->_object_code;
    }
};

#endif