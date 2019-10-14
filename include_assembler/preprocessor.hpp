#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <regex>
#include <algorithm>
#include <string>
#include <iostream>
#include "../include_assembler/data_structure.hpp"


/*!
 * Preprocessor class
 * 
 * Description: This class is a primary memory representation of the 
 * preprocessed file generated during the preprocessing
 */
class Preprocessor{
  public:
    //! MakePreFile(char* source_code_name);
    /*! 
     * Produces the .pre file whose content is stored in _preprocessed_file
     * variable
     */
    void MakePreFile(char* source_code_name);

    //! Preprocessor(std::string source_code_name);
    /*! 
     * Produces the vector(_preprocessed_file) containing each line from
     * the source file, before the preprocess
     */
    Preprocessor(char* source_code_name);

    //! Preprocessing();
    /*! 
     *  Executes the preprocessing, running the preprocess directives and 
     *  erasing the comments 
     */
    void Preprocessing();
  private:
    //! _preprocessed_file
    /*! 
     *  Stores all labels and its values declared at the EQU directives
    */
    std::map<std::string,int> _equ_values;

    //! _exists
    /*! 
     *  Indicates if the .asm file actually exists. If not,
     *  the preprocessed file is not produced
    */
    bool _exists = false;
    //! _preprocessing_errors 
    /*!
     * Store all errors from EQU and IF directives
     * 
    */
    error_log _preprocessing_errors;

    //! _preprocessed_file
    /*! 
     *  Stores the source code's line in a vector format.
     *  After, turns into the preprocessed file
    */
    std::vector<std::string> _preprocessed_file;
};


#endif