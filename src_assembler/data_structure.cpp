#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
#include <map>
#include "../include_assembler/data_structure.hpp"

using namespace std;

// CLASS TABLE

bool table::search(string identifier){
    itr = this->table.find(identifier);
    if(itr != this->table.end()){
        return true;
    }
    else{
        return false;
    }
}


// CLASS INSTRUCTION_TABLE

instruction_table::instruction_table(){
    this->table["ADD"][OPCODE]  = 1;
    this->table["ADD"][N_OPERANDS]  = 1;
    this->table["SUB"][OPCODE]  = 2;
    this->table["SUB"][N_OPERANDS]  = 1;
    this->table["MULT"][OPCODE]  = 3;
    this->table["MULT"][N_OPERANDS]  = 1;
    this->table["DIV"][OPCODE]  = 4;
    this->table["DIV"][N_OPERANDS]  = 1;
    this->table["JMP"][OPCODE]  = 5;
    this->table["JMP"][N_OPERANDS]  = 1;
    this->table["JMPN"][OPCODE]  = 6;
    this->table["JMPN"][N_OPERANDS]  = 1;
    this->table["JMPP"][OPCODE]  = 7;
    this->table["JMPP"][N_OPERANDS]  = 1;
    this->table["JMPZ"][OPCODE]  = 8;
    this->table["JMPZ"][N_OPERANDS]  = 1;
    this->table["COPY"][OPCODE]  = 9;
    this->table["COPY"][N_OPERANDS]  = 2;
    this->table["LOAD"][OPCODE]  = 10;
    this->table["LOAD"][N_OPERANDS]  = 1;
    this->table["STORE"][OPCODE]  = 11;
    this->table["STORE"][N_OPERANDS]  = 1;
    this->table["INPUT"][OPCODE]  = 12;
    this->table["INPUT"][N_OPERANDS]  = 1;
    this->table["OUTPUT"][OPCODE]  = 13;
    this->table["OUTPUT"][N_OPERANDS]  = 1;
    this->table["STOP"][OPCODE]  = 14;
    this->table["STOP"][N_OPERANDS]  = 0;

}


int instruction_table::get_num_operands(string identifier){
    // Check if instruction is in table
    itr = this->table.find(identifier);
    if(itr != this->table.end()){
        return table[identifier][N_OPERANDS];
    }
    else{
        return ERROR;
    }
}

int instruction_table::get_opcode(string identifier){
    // Check if instruction is in table
    itr = this->table.find(identifier);
    if(itr != this->table.end()){
        return table[identifier][OPCODE];
    }
    else{
        return ERROR;
    }
}

// CLASS SYMBOL_TABLE
bool symbol_table::search(string identifier){
    itr = this->table.find(identifier);
    if(itr != this->table.end()){
        return true;
    }
    else{
        return false;
    }
}

int symbol_table::get_value(string identifier){
    // Check if symbol is in table
    itr = this->table.find(identifier);
    if(itr != this->table.end()){
        return table[identifier][VALUE];
    }
    else{
        return ERROR;
    }
}

bool symbol_table::get_definition(string identifier){
    // Check if symbol is in table
    itr = this->table.find(identifier);
    if(itr != this->table.end()){
        return (bool)table[identifier][DEF];
    }
    else{
        return false;
    }
}

int symbol_table::get_list_address(string identifier){
    // Check if symbol is in table
    itr = this->table.find(identifier);
    if(itr != this->table.end()){
        return table[identifier][L_ADDRESS];
    }
    else{
        return ERROR;
    }
}

void symbol_table::PrintTable(){
    map<string, int[T_SIZE]> table;
    
    for (map<string, int[T_SIZE]>::iterator it= this->table.begin(); it!=this->table.end(); ++it)
        std::cout << it->first << " => " << it->second[0] << " " <<it->second[1]<< " "
         << it->second[2] << '\n';
}

// CLASS ERROR

map<int, string> error::error_details = {
    {1000, "Declaração e rótulo ausente."},
    {2000, "Declaração e rótulo repetido."},
    {2001, "Pulo para rótulo inválido."},
    {2002, "Pulo para seção errada."},
    {1001, "Diretiva inválida."},
    {1002, "Instrução inválida."},
    {2003, "Diretiva ou instrução na seção errada."},
    {2004, "Divisão por zero."},
    {1003, "Instrução com a quantidade de operandos inválida."},
    {1004, "Instrução com o tipo de operando inválido."},
    {0000, "Token inválido."},
    {1005, "Dois rótulos na mesma linha."},
    {2005, "Seção TEXT faltante."},
    {1006, "Seção inválida."},
    {1007, "Tipo de argumento inválido."},
    {2006, "Modificação de um valor constante."},
    {2007, "Acesso a posição não reservada pelo SPACE."}
};

map<int, string> error::error_types = {
    {0, "Sintático"},
    {1, "Semantico"},
    {2, "Léxico"}
};


error::error(string error_line, int error_line_number, int error_code){
    this->error_line_number = error_line_number;
    this->error_line = error_line;
    this->error_code = error_code;
    this->error_type = (int)error_code/ERROR_TYPE_MASK;
}

// CLASS ERROR_LOG

void error_log::display(int option){
    cout << "Relatório de Erros:" << endl;
    for(itr=error_list.begin(); itr!=error_list.end(); itr++){
        cout << "Número da linha: " << itr->get_error_line_number() << " Tipo: " << 
        itr->error_types[itr->get_error_type()] << endl; 
        if(option == DETAILED){
            cout << "Código: " << itr->get_error_line() << endl;
            cout << "Detalhes: " << itr->get_error_details() << endl;
        }
    }
}
