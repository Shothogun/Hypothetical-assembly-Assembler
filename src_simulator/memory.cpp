#include "../include_simulator/memory.hpp"

int mem[MEM_SIZE];
int16_t PC = 0;
int16_t ACC = 0;

typedef enum {
  ADD = 1, SUB, MULT, DIV, JMP, JMPN, JMPP, JMPZ, COPY,
  LOAD, STORE, INPUT, OUTPUT, STOP

} opcodes;


void LoadProgram(std::string object_code) {
  int address = 0;
  std::smatch string_bytes_sequence;
  std::regex pattern("(\\S+)");

  // Get array of bytes separated by space
  // Begin iterator
  auto bytes_sequence_begin = std::sregex_iterator(object_code.begin(),
                                                  object_code.end(),
                                                  pattern);

  // Default end iterator
  auto bytes_sequence_end = std::sregex_iterator();

  // Go through the set of bytes read
  for(std::sregex_iterator i = bytes_sequence_begin; i != bytes_sequence_end; ++i){
    // Gets the byte as a string
    std::smatch byte = *i;
    std::string string_byte = byte.str();
    
    // Converts string into int
    int int_byte = std::stoi(string_byte);

    // Stores it at the memory
    mem[address] = int_byte;

    address++;
  }
}

void fetch_execute_cycle(){
  int PC_opcode = mem[PC];
  int OP1, OP2;

  while(PC_opcode != STOP){
    OP1 = mem[PC+1];
    OP2 = mem[PC+2];
    switch (mem[PC])
    {
    case ADD:
      ACC = ACC + mem[OP1];
      PC += 2;
      break;
    
    case SUB:
      ACC = ACC - mem[OP1];
      PC += 2;
      break;
    
    case MULT:
      ACC = ACC * mem[OP1];
      PC += 2;
      break;

    case DIV:
      ACC = ACC / mem[OP1];
      PC += 2;
      break;

    case JMP:
      PC = OP1;
      break;

    case JMPN:
      if(ACC<0)
        PC = OP1;
      else
        PC += 2;
      break;

    case JMPP:
      if(ACC>0)
        PC = OP1;
      else
        PC += 2;      
      break;

    case JMPZ:
      if(ACC==0)
        PC = OP1;
      else
        PC += 2;
      break;

    case COPY:
      mem[OP2] = mem[OP1];
      PC += 3;
      break;

    case LOAD:
      ACC = mem[OP1];
      PC += 2;
      break;


    case STORE:
      mem[OP1] = ACC;
      PC += 2;
      break;


    case INPUT:
      std::cout << "Favor, inserir um valor númerico:";
      std::cin >> mem[OP1];
      PC += 2;
      break;

    case OUTPUT:
      std::cout << "O valor de saı́da é: " << mem[OP1] << std::endl;
      PC += 2;
      break;

    case STOP:
      break;

    default:
      break;
    } // switch

    PC_opcode = mem[PC];
    std::cout << "PC    : " << PC << std::endl;
    std::cout << "ACC   : " << ACC<< std::endl;
    std::cout << "ADDR1 : " << OP1<< std::endl;
    std::cout << "CONT  : " << mem[OP1]<< std::endl;

    if(PC_opcode == COPY){
      std::cout << "ADDR2 : " << OP2<< std::endl;
      std::cout << "CONT  : " << mem[OP2]<< std::endl;
    }

    std::cout << std::endl;
  } // while

}
