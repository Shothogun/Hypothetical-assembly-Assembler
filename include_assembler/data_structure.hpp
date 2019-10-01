#include <string>
#include <iterator>
#include <map>
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
};
