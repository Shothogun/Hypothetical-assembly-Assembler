# Hypothetical-assembly-Assembler
This repository is the 1st project from the discipline System Software 2/2019 at Universidade de Brasilia. In this project, a assembler simulator is created to translate a hypothetical assembly language to a machine language module (object file).

# Compiling and Running the program Montador

## Compiling

To compile the assembler program, change to the directory */src_assembler/* and by terminal type:

> ```$ make montador```

This will generate the executable file at the directory */exec_assembler/*.

## Executing

To execute the assembler program, change to the directory *exec_assembler* and by terminal execute:

> ```$ ./montador <file_name.asm>```

Substitutes ```<file_name.asm>``` by the name of the desired asm file that will be translated to machine code.
This ```.asm``` file shall be located in the directory ```/src_assembler/asm_codes/```.
By executing this program, two files will be generated to the directory ```/exec_assembler/output_files/```, ```file_name.obj``` (the machine code file) and ```file_name.pre``` (the preprocessed source code file).
