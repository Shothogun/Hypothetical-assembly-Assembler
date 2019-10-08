#include "../include_assembler/assembler.hpp"

///////////////////////////////////
//**   Constructor        
//////////////////////////////////
Assembler::Assembler(char* preprocessed_code_name){
  char preprocessed_file_line [300];
  std::vector<std::string>::iterator it;
  it = this->_pre_file.begin();

  // Gets the original file name and swap with .pre extention
  std::regex name_regex("(.*)(.asm)");  

  // Preprocessed codes are located at the current directory ./pre_files/
  std::string string_preprocessed_code_name = "./pre_files/";

  // Preprocessed code name input at the terminal execution
  // concatenated with directory
  string_preprocessed_code_name += preprocessed_code_name;

  // Replace .asm extention for .pre
  string_preprocessed_code_name = std::regex_replace (string_preprocessed_code_name,name_regex,"$1.pre");  

  // Convert preprocessed_code_name string variable to char type
  char char_object_code_name[string_preprocessed_code_name.size() + 1];
  strcpy(char_object_code_name, string_preprocessed_code_name.c_str());

  // Open the chosen preprocessed code file
  FILE* preprocessed_code = fopen(char_object_code_name, "r");
  if (preprocessed_code == NULL) perror ("Error opening file");
  else {
    // Until reach end of file
    while (!feof (preprocessed_code) ) {
      // Gets 100 characters from the source code line
      if ( fgets (preprocessed_file_line , 100 , preprocessed_code) == NULL ) break;
      
      // Fills the object_file vector with the asm source code 
      it = this->_pre_file.insert(it, preprocessed_file_line);
    }
    fclose (preprocessed_code);
   }

  // Corrects the preprocessed code lines order(inserted in the reversed order)
  std::reverse(this->_pre_file.begin(),
               this->_pre_file.end());

  /*debug
  for(it = this->_pre_file.begin(); it < this->_pre_file.end(); it++){
    std::cout << *it ;
  }
  */
}

///////////////////////////////////
//**   Assembling        
//////////////////////////////////
void Assembler::Assembling(){

  // Preprocesseed code's line iterated
  std::vector<std::string>::iterator preprocessed_code_line;

  // Boolean value that express SECTION TEXT
  // detection at the code line
  bool is_a_SECTION_TEXT = false;

  // Boolean value that express SECTION DATA
  // detection at the code line
  bool is_a_SECTION_DATA = false;

  // Indicates that the instruction follows a correct
  // pattern
  bool regular_instruction = false;

  // Indicates that is a Copy instruction
  bool COPY_instruction = false;

  // Array composed by the matches found in the regex search
  std::smatch matches;

  // Equivalent to the operator from the code's line
  // instructions
  std::string instruction_operator;

  // Equivalent to the operator from the code's line
  // instructions
  std::string instruction_operand;

  // Equivalent to the operator from the COPY's
  // instructions
  std::string instruction_operand_1;
  std::string instruction_operand_2;

  for(preprocessed_code_line = this->_pre_file.begin();
      preprocessed_code_line < this->_pre_file.end();
      preprocessed_code_line++) {    
    // Steps one line from preprocessed code
    this->_current_line++;
        
    //////////////////////////////////
    //**   SECTION TEXT identifier ---
    ////////////////////////////////// 

    // If SECTION TEXT command is found,
    // the assembler class reads the next lines as 
    // assembly instructions lines

    std::regex TEXT_SECTION_regex("(SECTION)(\\s)(TEXT)");

    // Seeks the EQU directive match
    is_a_SECTION_TEXT = std::regex_search (*preprocessed_code_line,
                                           matches,TEXT_SECTION_regex);

    if(is_a_SECTION_TEXT){
      this->_section_identifier = TEXT;
    }

    //////////////////////////////////
    //**   SECTION DATA identifier ---
    ////////////////////////////////// 

    // If SECTION DATA command is found,
    // the assembler class reads the next lines as 
    // assembly instructions lines

    std::regex DATA_SECTION_regex("(SECTION)(\\s)(DATA)");

    // Seeks the EQU directive match
    is_a_SECTION_DATA = std::regex_search (*preprocessed_code_line,
                                           matches,DATA_SECTION_regex);

    if(is_a_SECTION_DATA){
      this->_section_identifier = DATA;
    }

    //////////////////////////////////////////////
    //**   Identify regular instruction ---------
    //////////////////////////////////////////////
    
    std::regex INSTRUCTION_regex("(\\w+)(\\s)([a-z]|[A-Z]|_)(\\w*|\\d*)");

    // Seeks the EQU directive match
    regular_instruction = std::regex_search (*preprocessed_code_line,
                                              matches,INSTRUCTION_regex);  

    if(regular_instruction){
      // Matches pattern at instruction:
      // 0: Instruction match
      // 1: Operator
      // 2: space character
      // 3: Head character from the label
      // 4: Tail from the label

      instruction_operator = matches[1].str();
      instruction_operand = matches[3].str() + matches[4].str();

      std::cout << instruction_operator << " ";
      std::cout << instruction_operand << std::endl;
    }

    //////////////////////////////////////////////
    //**   Identify COPY instruction -------------
    //////////////////////////////////////////////
    std::regex COPY_instruction_regex("(COPY)(\\s)(\\w+)(,)(\\w+)");

    // Seeks the EQU directive match
    COPY_instruction = std::regex_search (*preprocessed_code_line,
                                              matches,COPY_instruction_regex);

    if(COPY_instruction){
      // Matches pattern at instruction COPY:
      // 0: Instruction COPY match
      // 1: COPY
      // 2: space character
      // 3: operand 1
      // 4: comma ',' character
      // 5: operand 2

      instruction_operator = matches[1].str();
      instruction_operand_1 = matches[3].str();
      instruction_operand_2 = matches[5].str();

      // Its a COPY instruction, not a regular instruction
      regular_instruction = false;
    }                                              

    //////////////////////////////////////////////
    //**   Identify STOP -------------------------
    //////////////////////////////////////////////

    //////////////////////////////////////////////
    //**   Identify SPACE directive --------------
    //////////////////////////////////////////////

    //////////////////////////////////////////////
    //**   Identify CONST directive --------------
    //////////////////////////////////////////////

    //////////////////////////////////////////////
    //**   ERROR verify --------------------------
    //////////////////////////////////////////////

    //////////////////////////////////////////////
    //**   Produce the machine code --------------
    //////////////////////////////////////////////


  /* Debug
  std::cout << *preprocessed_code_line;
  */ 
  }
}