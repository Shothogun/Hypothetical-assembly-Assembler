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
    //! SECTION constants identifiers
    static const int NONE = 0;
    static const int TEXT = 1;
    static const int DATA = 2;

    //! GenerateObjCode(std::string instruction, std::string operand1);
    /*! 
     *  Produces a regular object code instruction
     */
    void GenerateObjCode(std::string instruction, std::string operand1);

    //! GenerateObjCode(std::string instruction, std::string operand1
    /*!                 std::string operand2);
     *
     *  Produces a COPY object code instruction
     */
    void GenerateObjCode(std::string instruction, std::string operand1,
                         std::string operand2);

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
    int _section_identifier = NONE;

    //! _current_line
    /*!
    *   Stores the current line from the preprocessed code
    */
    int _current_line = 0;
};

#endif