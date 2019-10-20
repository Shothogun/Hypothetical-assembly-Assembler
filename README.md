# Hypothetical-assembly-Assembler

This repository is the 1st project from the discipline System Software 2/2019 at Universidade de Brasilia. In this project, a assembler is created to translate a hypothetical assembly language to a machine language module (object file). This translater anaylse fully the lexical, syntactic and semantic aspects required from the code to assembly properly. Also, a simulated machine is implemented to run the hyphothetical machine code, executing the translated assembly program.

## Members

Member                | Univeristy Id
----------------------|------------------
Otho Teixeira Komatsu | 170020142
Danilo Inácio         | 

## Technical Description

This program was developed at a Debian Linux distro, especifically at **Linux Ubuntu 18.04.2 LTS**. It's language is C++, compiled with the standard **C++ 14** and with g++. Some more details is avaiable at the makefiles
files, at the directories ```/src_assembler/makefile``` and ```/src_simulator/makefile```.

## Compiling and Running the program Montador

### Compiling Montador

To compile the assembler program, change to the directory */src_assembler/* and by terminal type:

> ```$ make montador```

This will generate the executable file at the directory */exec_assembler/* called ```montador```.

**Note**: Sometimes the compilation conflict the already existing object code files,
 thereby the program doesn't compile correctly. At this case, type ```$ make clean```
 at the terminal.

### Executing

To execute the assembler program, change to the directory */exec_assembler/* and by terminal execute:

> ```$ ./montador <file_name.asm>```

Substitutes ```<file_name.asm>``` by the name of the desired asm file that will be translated to machine code. This ```.asm``` file shall be located in the directory ```/src_assembler/asm_codes/```.

By executing this program, two files will be generated: to the directory ```/exec_assembler/obj_files/```, ```file_name.obj``` (the machine code file), and to the directory ```/exec_assembler/pre_files/```, ```file_name.pre``` (the preprocessed source code file).

If the ```.asm``` contains errors, the assembler program will report all errors that the asm code contains.
The report will indicate the line, line number, the type of error(lexical, syntactic and semantic) and the error details at the wrong line from the ```.asm``` file.

## Compiling and Running the program Simulador

### Compiling Simulador

To compile the assembler program, change to the directory */src_simulator/* and by terminal type:

> ```$ make simulator```

This will generate the executable file at the directory */exec_simulator/*  called ```simulador```.

**Note**: Sometimes the compilation conflict the already existing object code files,
 thereby the program doesn't compile correctly. At this case, type ```$ make clean```
 at the terminal.

### Executing Simulador

To execute the assembler program, change to the directory *exec_assembler* and by terminal execute:

> ```$ ./simulador <file_name.obj>```

Substitutes ```<file_name.obj>``` by the name of the desired obj file translated to machine code that will be executed. This ```.obj``` file shall be located in the directory ```/exec_assembler/obj_files/```.
By executing this program, the object file will be read through it's machine code and the machine simulator will execute its commands.
