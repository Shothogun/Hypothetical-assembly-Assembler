# Hypothetical-assembly-Assembler

This repository is the 1st project from the discipline System Software 2/2019 at Universidade de Brasilia. In this project, a assembler is created to translate a hypothetical assembly language to a machine language module (object file). This translater anaylse fully the lexical, syntactic and semantic aspects required from the code to assembly properly.
A linker is also created to allow programs of up to two modules and generate an executable file from the object files translated by the assembler. Finally, a simulated machine is implemented to run the hyphothetical machine code, executing the translated and linked assembly program.

## Members

Member                     | Univeristy Id
---------------------------|------------------
Otho Teixeira Komatsu      | 170020142
Danilo Inácio dos S. Silva | 170031861

## Technical Description

This program was developed at a Debian Linux distro, especifically at **Linux Ubuntu 18.04.2 LTS**. It's language is C++, compiled with the standard **C++ 14** and with g++. Some more details is avaiable at the makefiles
files, at the directories ```/src_assembler/makefile``` ```/src_linker/makefile``` ```/src_simulator/makefile```.

## Compiling and Running the program Montador

### Compiling Montador

To compile the assembler program, change to the directory */src_assembler/* and by terminal type:

> ```$ make montador```

This will generate the executable file at the directory */exec_assembler/* called ```montador```.

**Note**: Sometimes the compilation conflict the already existing object code files,
 thereby the program doesn't compile correctly. At this case, type ```$ make clean```
 at the terminal.

### Executing Montador

To execute the assembler program, change to the directory */exec_assembler/* and by terminal execute:

> ```$ ./montador <file_name.asm>```

or

> ```$ ./montador <file_name_1.asm> <file_name_2.asm>```

One or two .obj files may be provided for the montador. Substitutes ```<file_name.asm>``` by the name of the desired asm files that will be translated to machine code. These ```.asm``` files shall be located in the directory
 ```/src_assembler/asm_codes/```.

By executing this program, two files will be generated for each input file: to the directory: ```/exec_assembler/obj_files/```, ```file_name.obj``` (the machine code file), and to the directory ```/exec_assembler/pre_files/```, ```file_name.pre``` (the preprocessed source code file).

If the ```.asm``` contains errors, the assembler program will report all errors that the asm code contains.
The report will indicate the line, line number, the type of error(lexical, syntactic and semantic) and the error details at the wrong line from the ```.asm``` file.

**Note 1:** If two files are provided, each module must contain the BEGIN and END directives.

**Note 2:** When the ```.asm``` code's instructions contains errors, the object code will be filled up with garbage code(code ```99```) to preserve the pretended size for the object code(following the valid instructions from the asm code).

**Note 3:**: The line number where an error occurred will be relative to ```.asm``` if it is 
an error in the preprocess directives and will be relative to ```.pre``` for all other errors.


## Compiling and Running the program Ligador

### Compiling Ligador

To compile the linker program, change to the directory */src_linker/* and by terminal type:

> ```$ make ligador```

This will generate the executable file at the directory */exec_linker/*  called ```ligador```.

**Note**: Sometimes the compilation conflict the already existing object code files,
 thereby the program doesn't compile correctly. At this case, type ```$ make clean```
 at the terminal.

### Executing Ligador

To execute the linker program, change to the directory */exec_linker/* and by terminal execute:

> ```$ ./ligador <file_name.obj> <linked_file_name> ```

or

> ```$ ./ligador <file_name_1.obj> <file_name_2.obj> <linked_file_name> ```


One or two .obj files may be provided for the ligador, followed by the name (without file extension) for the linked .obj file. Substitutes ```<file_name.obj>``` by the name of the desired obj files that will be linked. These ```.obj``` files shall be located in the directory ```/exec_assembler/obj_files/```. Replaces ```<linked_file_name>``` with the name that will be assigned to the linked .obj file.
By executing this program, a linked file will be generated in the directory ```/exec_linker/linked_files/```, ```linked_file_name.obj``` (the executable file).

## Compiling and Running the program Simulador

### Compiling Simulador

To compile the simulator program, change to the directory */src_simulator/* and by terminal type:

> ```$ make simulador```

This will generate the executable file at the directory */exec_simulator/*  called ```simulador```.

**Note**: Sometimes the compilation conflict the already existing object code files,
 thereby the program doesn't compile correctly. At this case, type ```$ make clean```
 at the terminal.

### Executing Simulador

To execute the simulator program, change to the directory */exec_simulator/* and by terminal execute:

> ```$ ./simulador <linked_file_name.obj>```

Substitutes ```<linked_file_name.obj>``` by the name of the desired linked .obj file that will be executed. This ```.obj``` file shall be located in the directory ```/exec_linker/linked_files/```.
By executing this program, the object file will be read through it's machine code and the machine simulator will execute its commands.

**Note**: The simulator cannot directly execute .obj files output from the assembler. File must be executable (linked .obj file)
