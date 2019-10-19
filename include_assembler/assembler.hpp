#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <regex>
#include <algorithm>
#include <string>
#include <iostream>
#include "data_structure.hpp"

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

    //! ~Assembler();
    /*! 
     *  A class destructor
     */
    ~Assembler();


    //! Assembling();
    /*! 
     *  Executes the assembling, running the preprocess directives and 
     *  erasing the comments 
     */
    void Assembling();

    //! MakeObjectFile(char* source_code_name);
    /*! 
     * Produces the .obj file whose content is stored in _object_file
     * variable
     */
    void MakeObjectFile(char* source_code_name);

    //! Error log
    error_log* _assembling_errors;

  private:
    //! SECTION constants identifiers
    static const int NONE = 0;
    static const int TEXT = 1;
    static const int DATA = 2;

    // Instructions and directives types identifiers
    // Instructions
    static const int REGULAR_TYPE   = 10;
    static const int COPY_TYPE      = 11;
    static const int STOP_TYPE      = 12;

    // Directives
    static const int SPACE_TYPE     = 20;
    static const int CONST_TYPE     = 21;

    // SECTION
    static const int SECTION_TYPE     = 30;

    // ERROR Code
    static const int ERROR = -1;

    //! Label use context
    static const int LABEL_OPERAND = 100;
    static const int LABEL_DEFINITION = 101;

    //! Instruction table to object code generation
    instruction_table* _instruction_table;

    //! Symbol table 
    symbol_table* _symbol_table;

    //! _exists
    /*! 
     *  Indicates if the .asm file actually exists. If not,
     *  the object file is not produced
    */
    bool _exists = false;

    //! _SECTION_TEXT_exist
    /*!
     *
     * Indicates if a text section was found 
    */ 
    bool _SECTION_TEXT_exist = false;

    //! GenerateObjCode(std::string instruction);
    /*! 
     *  Produces a STOP object code instruction
     */
    void GenerateObjCode(std::string instruction);

    //! GenerateObjCode(std::string instruction, std::string operand1);
    /*! 
     *  Produces a regular object code instruction
     */
    void GenerateObjCode(std::string instruction, std::string operand1);

    //! GenerateObjCode(std::string instruction, std::string operand1
    /*!                 std::string operand2);
     *
     *  Produces a COPY object code instruction
     *  or a directive CONST or SPACE
     */
    void GenerateObjCode(std::string instruction, std::string operand1,
                         std::string operand2);
    //! Scanner(std::string source_code_line, int line_number)
    /*! 
      *
      * Identify and validate tokens 
    */
    void Scanner(); 
    
    //! Parser(std::string code_line)
    /*! 
      *
      * Identify and validate operators and 
      * operands according to language syntax
    */
    void Parser(std::string code_line);

    //! IdentifyCommandType();
    /*
     *  Associate the command with his type and creates
     *  his object machine code equivalent
     */
    void IdentifyCommandType();


    //! LabelIdentifier();
    /*
     *  Returns the value that will be inserted
     *  at object code:
     * 
     *  Identify defined labels, returning its value.
     *  At the case of undefined labels, 
     *  creates a list of undefined labels in object code,
     *  by consulting symbol tabel. Returns the label's last 
     *  reference address as operand at object code
     *  before the new reference, and 
     *  updates list_address at symbol table to the new reference address
     */
    int LabelIdentifier(std::string label, int use_type);

    //! StoreLabelOccurrence(std::string label)
    /*
     *  
     *  
     */

    void StoreLabelOperandOccurrence(std::string label_operand);

    //! ResolveLabelValue(std::string label);
    /*
     *  Once a label value mentioned earlier is defined,
     *  its values is substituted by the new defined one
     */
    void ResolveLabelValue(std::string label);

    //! AllocSizeManager()
    /*
     *  Indicates how many spaces are stored
     *  to the label defined. A regular ocupies 1.
     *  At SPACES directives, allocates N given.
     *  Argument label_reference: indicates
     *  which object code address occurs the reference.
     *  Returns the SPACE size allocated.
     */
    int AllocSizeManager(int label_reference);

    //! Error15Verify()
    /*! 
     *  A method that verifies error 15 type at 
     *  label value resolving process.
    */
    void Error15Verify(int label_reference);


    //! _pre_file
    /*! 
     *  Stores the preprocessed code's line in a vector format.
    */
    std::vector<std::string> _pre_file;

    //! _object_file
    /*! 
     *  Stores the object file's line in a vector format,
     *  containing the machine code instructions.
    */
    std::vector<std::string> _object_file;

    //! _section_data_preprocessed
    /*!
     * Stores code from date section for later evaluation
     * 
    */
    std::vector<std::string> _section_data_preprocessed;
    //! _section_data_commands
    /*! 
     *  Stores the preprocessed code's section data
     *  in machine code's version. This is appended
     *  to the object file at the its ending.
    */
    std::vector<std::string> _section_data_commands;

    //!
    /*!
     *
     * 
    */
    std::vector<label_occurrence> _JMP_label_occurrences;

    //!
    /*!
     *
     * 
    */ 
    std::vector<label_occurrence> _DIV_label_occurrences;

    //!
    /*!
     *
     * 
    */ 
    std::vector<label_occurrence> _label_occurrences;
   
    static const  int   OFFSET = 0;
    static const  int   LINE   = 1;

    //! _address_offset
    /*! 
     *  Stores all address offset.
     *  First value: offset
     *  Second value: line occurred it
    */
    map<int, int[2]> _address_offset;

    //! _current_line_string
    /*! 
     *  Stores preprocessed code's line to error report
    */
    std::string _current_line_string;

    //! _current_label
    /*!
      * Store last label found 
    */
    std::string _current_label;
    //! _instruction_operator
    /*! 
     *  Stores the command operator from preprocessed line instruction
    */

    std::string _instruction_operator;

    //! _instruction_operand_1
    /*! 
     *  Stores the command first operand from 
     *  preprocessed line instruction
    */
    std::string _instruction_operand_1;

    //! _instruction_operand_2
    /*! 
     *  Stores the command second operator 
     *  from preprocessed line instruction
    */
    std::string _instruction_operand_2;

    //! _operand_1_offset
    /*!
    *   Stores the label offset placed at as operand
    *   at operations
    */
    std::string _operand_1_offset;


    //! _operand_2_offset
    /*!
    *   Stores the current line from the preprocessed code
    */
    std::string _operand_2_offset;

    //! _section_identifier
    /*! 
     *  Stores the current section being analised 
    */
    int _section_identifier = NONE;

    //! _line_type_identifier
    /*! 
     *  Indicates if it's a instruction or directive
     *  and what kind of it is
    */
    int _line_type_identifier = REGULAR_TYPE;


    //! _current_line
    /*!
    *   Stores the current line from the preprocessed code
    */
    int _current_line_number = 0;

};

#endif