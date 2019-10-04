#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <regex>
#include <algorithm>
#include <string>
#include <iostream>

/*!
 * Assembler class
 * 
 * Description: This class is a representation of the assembler,
 * which role is get a preprocessed file and generate a machine
 * code file.
 */
class Assembler{
  public:
    //! Assembler(std::string source_code_name);
    /*! 
     * Produces the vector(_object_file) containing each line from
     * the preprocessed file, before the compiling process
     */
    Assembler(char* preprocessed_code_name);

    //! Assembling();
    /*! 
     *  Executes the assembling, running the preprocess directives and 
     *  erasing the comments 
     */
    void Assembling();

  private:
    //! _pre_file
    /*! 
     *  Stores the preprocessed code's line in a vector format.
    */
    std::vector<std::string> _pre_file;

    //! _pre_file
    /*! 
     *  Stores the object file's line in a vector format,
     *  containing the machine code instructions.
    */
    std::vector<std::string> _object_file;

    //! _section_identifier
    /*! 
     *  Stores the current section being analised 
    */
    std::vector<std::string> _section_identifier;
};

#endif