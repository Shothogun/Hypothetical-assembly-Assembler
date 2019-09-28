#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <regex>
#include <algorithm>
#include <string>
#include <iostream>


/*!
 * Preprocessor class
 * 
 * Description: This class is a primary memory representation of the 
 * preprocessed file generated during the preprocessing
 */
class Preprocessor{
  public:
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
     *  Stores the source code's line in a vector format.
    */
    std::vector<std::string> _preprocessed_file;

    //! _preprocessed_file
    /*! 
     *  Stores all labels and its values declared at the EQU directives
    */
    std::map<std::string,int> _equ_values;

};


#endif