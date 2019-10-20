#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#include <vector>
#include <string>
#include <iterator>
#include <map>
#include <list>
using namespace std;

/*! Table class
 * 
 * This class creates a generic table with a search method 
*/
class table{
protected:
  /*! T_SIZE
   *
   * Constant that indicates the number of columns in the table
  */
  static const int T_SIZE = 2;
  /*! table
   *
   * Table structure
  */
  map<string, int[T_SIZE]> table;
  /*! itr
   *
   * Iterator of table structure
  */
  map<string, int[T_SIZE]>::iterator itr;

public:
/*! search(string identifier)
 *    
 * Tells whether an identifier is present in the table or not
*/ 
  bool search(string identifier);
};

/*!
 *
 * This class creates a instruction table with methods for acess number of operands and
 * opcode for each instruction in table
*/
class instruction_table : public table{
  private:
    /*! OPCODE
     *
     * Constant indicating opcode column number in table
    */
    static const int OPCODE = 0;
    /*! N_OPERANDS
     *
     * Constant indicating operands number column number in table
    */
    static const int N_OPERANDS = 1;
    /*! ERROR
     *
     * Constatant indicating a error
    */
    static const int ERROR = -1;
  public:
    /*! instruction_table()
     *
     *  Initializes the instruction table
    */
    instruction_table();
    /*! get_num_operands(string identifier)
     *
     * Returns number of operands of instruction 
    */
    int get_num_operands(string identifier);
    /*! get_opcode(string identifier)
     *
     * Return instruction opcode
    */
    int get_opcode(string identifier);
};
/*!
 *
 * This class creates a symbol table with methods for accessing the value, definition,
 * and address of the undefined symbol list for each symbol in table
*/
class symbol_table : public table{
  private:
    /*! T_SIZE
     *
     * Constant that indicates the number of columns in the table
    */
    static const int T_SIZE = 3;
    /*! VALUE
     *
     * Constant indicating value column number in table
    */
    static const int VALUE = 0;
    /*! DEF
     *
     * Constant indicating definition column number in table
    */
    static const int DEF = 1;
    /*! L_ADDRESS
     *
     * Constant indicating list address column number in table
    */
    static const int L_ADDRESS = 2;
    /*! ERROR
     *
     * Constatant indicating a error
    */
    static const int ERROR = -1;
    /*! table
     *
     * Table structure
    */
    map<string, int[T_SIZE]> table;
    /*! itr
     *
     * Iterator of table structure
    */
    map<string, int[T_SIZE]>::iterator itr;

  public:
    /*! set_value(string identifier, int value)
     *
     * Sets the value of a symbol
    */
    void set_value(string identifier, int value){
      this->table[identifier][VALUE] = value;
    }
    /*! set_definition(string identifier, bool value)
     * 
     * Sets the definition of a symbol
    */
    void set_definition(string identifier, bool value){
      this->table[identifier][DEF] = (int)value;
    }
    /*! set_list_address(string identifier, int address)
     *
     * Sets the list address of a symbol
    */
    void set_list_address(string identifier, int address){
      this->table[identifier][L_ADDRESS] = address;
    }
    /*! get_value(string identifier)
     *
     * Returns the value of a symbol.
    */
    int get_value(string identifier);
    /*! get_definition(string identifier)
     *
     * Returns the definition of a symbol.
    */
    bool get_definition(string identifier);
    /*! get_list_address(string identifier)
     *
     * Returns the list address of a symbol.
    */
    int get_list_address(string identifier);

    /*! PrintTable();
     * A debug function
     *
    */ 
    void PrintTable();

    /*! search(string identifier)
     *
     * Tells whether an identifier is present in the table or not
    */
    bool search(string identifier);

    vector<string> search_not_defined();
};

/*!
 *
 * This class stores information about an occurrence of a label
*/
class label_occurrence{
  private:
    /*! instruction_operator
     *
     * Stores the instruction operator using the label
    */
    string instruction_operator;
    /*! code_line
     *
     * Stores the line of code where the label is used
    */
    string code_line;
    /*! line_number
     *
     * Stores the line number where the label is used
    */
    int line_number;
    /*! label
     *
     * Stores the label;
    */
    string label;
  public:
    /*! label_occurrence(string label, string instruction_operator, string code_line, int line_number)
     *
     * label_occurence class constructor
    */
    label_occurrence(string label, string instruction_operator, string code_line, int line_number);
    /*! set_instruction_operator(string instruction_operator
     *
     * Sets the instruction operator using the label
    */
    void set_instruction_operator(string instruction_operator){
      this->instruction_operator = instruction_operator;
    }
    /*! set_label(string label)
     *
     * Sets the label;
    */
    void set_label(string label){
      this->label = label;
    } 
    /*! set_code_line(string code_line)
     *
     * Sets the line of code where the label is used
    */
    void set_code_line(string code_line){
      this->code_line = code_line;
    }
    /*! set_line_number(int line_number)
     *
     * Sets the line number where the label is used
    */
    void set_line_number(int line_number){
      this->line_number = line_number;
    }
    /*! get_instruction_operator()
     *
     * Return the instruction operator using the label
    */
    string get_instruction_operator(){
      return this->instruction_operator;
    }
    /*! get_label()
     *
     * Return the label
    */
    string get_label(){
      return this->label;
    }
    /*! get_code_line()
     *
     * Return the line of code where the label is used
    */
    string get_code_line(){
      return this->code_line;
    }
    /*! get_line_number()
     *
     * Return the line number where the label is used
    */
    int get_line_number(){
      return this->line_number;
    }
};

/*!
 *
 * This class implements the error type and contains the line where the error occurred,
 * a classification (lexical, syntactic and semantic), a error code and the description.
*/
class error{
  private:
    /*! ERROR_TYPE_MASK
     *
     * Mask that lets you determine the type of error from your error code
    */
    static const int ERROR_TYPE_MASK = 1000;
    /*! error_line
     *
     * Stores error line string
    */
    string error_line;
    /*! error_line_number
     *
     * Stores error line number
    */
    int error_line_number;
    /*! error_code
     *
     * Stores error code
    */
    int error_code;
    /*! error_type
     *
     * Stores error type 
    */
    int error_type;
    /*! error_details
     *
     * Stores strings matching error details
    */
    static map<int, string> error_details;

  public:
    /*! LEXICAL
     *
     * Constant indicating lexical type
    */
    static const int LEXICAL = 0;
    /*! SYNTACTIC
     *
     * Constant indicating syntactic type
    */
    static const int SYNTACTIC = 1;
    /*! SEMANTIC
     *
     * Constant indicating semantic type
    */
    static const int SEMANTIC = 2;
    /*! error_types
     *
     * Stores strings matching error type
    */
    static map<int, string> error_types;
    /*! error_00
     *
     * Code for the error "Declaração e rótulo ausente."
    */
    static const int error_00 = 1000; // syntactic
    /*! error_01
     *
     * Code for the error "Declaração e rótulo repetido."
    */
    static const int error_01 = 2000; // semantic
    /*! error_02
     *
     * Code for the error "Pulo para rótulo inválido."
    */
    static const int error_02 = 2001; // semantic
    /*! error_03
     *
     * Code for the error "Pulo para seção errada."
    */
    static const int error_03 = 2002; // semantic
    /*! error_04
     *
     * Code for the error "Diretiva inválida."
    */
    static const int error_04 = 1001; // syntactic
    /*! error_05
     *
     * Code for the error "Instrução inválida."
    */
    static const int error_05 = 1002; // syntactic
    /*! error_06
     *
     * Code for the error "Diretiva ou instrução na seção errada."
    */
    static const int error_06 = 2003; // semantic
    /*! error_07
     *
     * Code for the error "Divisão por zero."
    */
    static const int error_07 = 2004; // semantic
    /*! error_08
     *
     * Code for the error "Instrução com a quantidade de operandos inválida."
    */
    static const int error_08 = 1003; // syntactic
    /*! error_09
     *
     * Code for the error "Instrução com o tipo de operando inválido."
    */
    static const int error_09 = 1004; // syntactic
    /*! error_10
     *
     * Code for the error "Token inválido."
    */
    static const int error_10 = 0000; // lexical
    /*! error_11
     *
     * Code for the error "Dois rótulos na mesma linha."
    */
    static const int error_11 = 1005; // syntactic
    /*! error_12
     *
     * Code for the error "Seção TEXT faltante."
    */
    static const int error_12 = 2005; // semantic
    /*! error_13
     *
     * Code for the error "Seção inválida."
    */
    static const int error_13 = 1006; // syntactic
    /*! error_14
     *
     * Code for the error  "Tipo de argumento inválido."
    */
    static const int error_14 = 1007; // syntactic
    /*! error_15
     *
     * Code for the error "Modificação de um valor constante."
    */
    static const int error_15 = 2006; // semantic
    /*! error_16
     *
     * Code for the error "Acesso a posição não reservada pelo SPACE."
    */
    static const int error_16 = 2007; // semantic

    /*! error(string error_line, int error_line_number, int error_code)
     *
     * error class constructor
    */    
    error(string error_line, int error_line_number, int error_code);
    /*! get_error_line_number()
     *
     * Returns the number of the line on which the error occurred.
    */    
    int get_error_line_number(){
      return this->error_line_number;
    }
    /*! get_error_code()
     *
     * Returns the number of the line on which the error occurred.
    */    
    int get_error_code(){
      return this->error_code;
    }
    /*! get_error_line()
     *
     * Returns the line on which the error occurred.
    */    
    string get_error_line(){
      return this->error_line;
    }
    /*! get_error_type()
     *
     * Returns the type of error occurred
    */    
    int get_error_type(){
      return this->error_type;
    }
    /*! get_error_details()
     *
     * Returns details of error occurred
    */    
    string get_error_details(){
      return this->error_details[this->error_code];
    }
};

/*!
 *
 * This class implements an error log that stores a set of errors 
 * and allows them to be displayed simply or in detail.
*/
class error_log{
  private:
    /*! error_list
     *
     * error log structure
    */
    list<error> error_list;
    /*!  itr
     *
     * Iterator of error log structure
    */
    list<error>::iterator itr;
  public:
    /*! DETAILED
     *
     * Constant indicating detailed display option
    */    
    static const int DETAILED = 1;
    /*! SIMPLE
     *
     * Constant indicating simple display option
    */
    static const int SIMPLE = 0;
    /*! include_error(error error)
     *
     * Insert object of error class into error log
    */
    void include_error(error error){
      this->error_list.push_back(error);
    }
    /*! display(int option)
     *
     * Display all errors in error log detailed or simple form
    */    
    void display(int option);
};
#endif
