#include <string>
#include <iterator>
#include <map>
#include <list>
using namespace std;

// This class creates a generic table with a search method 
class table{
protected:
    static const int T_SIZE = 2;
    map<string, int[T_SIZE]> table;
    map<string, int[T_SIZE]>::iterator itr;
    
public:
    // Tells whether an identifier is present in the table or not
    bool search(string identifier);
};

// This class creates a instruction table with methods for acess number of operands and
// opcode for each instruction in table
class instruction_table : public table{
private:
    static const int OPCODE = 0;
    static const int N_OPERANDS = 1;
    static const int ERROR = -1;
public:
    //  Initializes the instruction table
    instruction_table();
    // Returns number of operands of instruction 
    int get_num_operands(string identifier);
    // Return instruction opcode
    int get_opcode(string identifier);
};

// This class creates a symbol table with methods for accessing the value, definition,
// and address of the undefined symbol list for each symbol in table
class symbol_table : public table{
private:
    static const int T_SIZE = 3;
    static const int VALUE = 0;
    static const int DEF = 1;
    static const int L_ADDRESS = 2;
    static const int ERROR = -1;
    map<string, int[T_SIZE]> table;
    map<string, int[T_SIZE]>::iterator itr;

public:

    void set_value(string identifier, int value){
        this->table[identifier][VALUE] = value;
    }

    void set_definition(string identifier, bool value){
        this->table[identifier][DEF] = (int)value;
    }

    void set_list_address(string identifier, int address){
        this->table[identifier][L_ADDRESS] = address;
    }

    int get_value(string identifier);
    bool get_definition(string identifier);
    int get_list_address(string identifier);

    void PrintTable();

    // Tells whether an identifier is present in the table or not
    bool search(string identifier);
};

//This class implements the error type and contains the line where the error occurred,
// a classification (lexical, syntactic and semantic), a error code and the description.
class error{
private:
    static const int ERROR_TYPE_MASK = 1000;
    string error_line;
    int error_line_number;
    int error_code;
    int error_type;
    static map<int, string> error_details;

public:

    static const int LEXICAL = 0;
    static const int SYNTACTIC = 1;
    static const int SEMANTIC = 2;
    static map<int, string> error_types;

    static const int error_00 = 1000; // syntactic
    static const int error_01 = 2000; // semantic
    static const int error_02 = 2001; // semantic
    static const int error_03 = 2002; // semantic
    static const int error_04 = 1001; // syntactic
    static const int error_05 = 1002; // syntactic
    static const int error_06 = 2003; // semantic
    static const int error_07 = 2004; // semantic
    static const int error_08 = 1003; // syntactic
    static const int error_09 = 1004; // syntactic
    static const int error_10 = 0000; // lexical
    static const int error_11 = 1005; // syntactic
    static const int error_12 = 2005; // semantic
    static const int error_13 = 1006; // syntactic
    static const int error_14 = 1007; // syntactic
    static const int error_15 = 2006; // semantic
    static const int error_16 = 2007; // semantic

    error(string error_line, int error_line_number, int error_code);
    int get_error_line_number(){
        return this->error_line_number;
    }
    int get_error_code(){
        return this->error_code;
    }
    string get_error_line(){
        return this->error_line;
    }
    int get_error_type(){
        return this->error_type;
    }
    string get_error_details(){
        return this->error_details[this->error_code];
    }
};

// This class implements an error log that stores a set of errors 
// and allows them to be displayed simply or in detail.

class error_log{
private:
    list<error> error_list;
    list<error>::iterator itr;
public:
    static const int DETAILED = 1;
    static const int SIMPLE = 0;
    void include_error(error error){
        this->error_list.push_back(error);
    }
    void display(int option);
};