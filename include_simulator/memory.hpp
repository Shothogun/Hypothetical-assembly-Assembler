#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <regex>

// Macro for number of bytes in the memory
#define   MEM_SIZE      512

// This memory will be contiguos a 512 bytes stored
// i.e, 216 word of 16 bits
extern int mem[MEM_SIZE];

// PC register of 16 bits
extern int16_t PC;

// ACC register of 16 bitsint fetch();
extern int16_t ACC;

// This function will read each byte from the object file
// and fill the memory with it
void LoadProgram(std::string object_code);

void fetch_execute_cycle();

#endif