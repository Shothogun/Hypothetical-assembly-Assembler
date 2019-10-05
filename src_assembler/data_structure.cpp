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

map<int, string> error::error_details = {
    {error::error_00, "declaração e rótulo ausente"},
    {error::error_01, "declaração e rótulo repetido"},
    {error::error_02, "pulo para rótulo inválido"},
    {error::error_03, "pulo para seção errada"},
    {error::error_04, "diretiva inválida"},
    {error::error_05, "instrução inválida"},
    {error::error_06, "diretiva ou instrução na seção errada"},
    {error::error_07, "divisão por zero"},
    {error::error_08, "instrução com a quantidade de operandos inválida"},
    {error::error_09, "instrução com o tipo de operando inválido"},
    {error::error_10, "token inválido"},
    {error::error_11, "dois rótulos na mesma linha"},
    {error::error_12, "seção TEXT faltante"},
    {error::error_13, "seção inválida"},
    {error::error_14, "tipo de argumento inválido"},
    {error::error_15, "modificação de um valor constante"},
    {error::error_16, "acesso a posição não reservada pelo SPACE"},
};

error::error(string error_line, int error_line_number, int error_code){
    this->error_line_number = error_line_number;
    this->error_line = error_line;
    this->error_code = error_code;
    this->error_type = (int)error_code/1000;
}

