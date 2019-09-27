#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <regex>


/*!
 * Preprocessor class
 * 
 * Description: This class is a primary memory representation of the 
 * preprocessed file generated during the preprocessing
 */
class Preprocessor{
  public:
    //! Preprocessor(std::string source_code_name);
    /*! Produces the vector containing each line from
     * the source file, before the preprocess
     */
    Preprocessor(char* source_code_name);
  private:
    std::vector<std::string> _preprocessed_file;
};

#endif