#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
#include <map>
#include "../include_assembler/data_structure.hpp"

using namespace std;

bool table::search(string identifier){
    itr = this->table.find(identifier);
    if(itr != this->table.end()){
        return true;
    }
    else{
        return false;
    }
}

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