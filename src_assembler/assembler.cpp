#include "../include_assembler/assembler.hpp"

///////////////////////////////////
//**   Constructor        
//////////////////////////////////
Assembler::Assembler(char* preprocessed_code_name){
  char preprocessed_file_line [300];
  std::vector<std::string>::iterator it;
  it = this->_pre_file.begin();

  // Tables initialization
  this->_instruction_table = new instruction_table();
  this->_symbol_table = new symbol_table();
  this->_assembling_errors = new error_log();

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
    // Indicates that the .pre file really exist,
    // for further .obj file creation
    this->_exists = true;

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
//**   Destructor        
//////////////////////////////////
Assembler::~Assembler(){
  delete this->_instruction_table;
  delete this->_symbol_table;
  delete this->_assembling_errors;
}

///////////////////////////////////
//**   Assembling        
//////////////////////////////////
void Assembler::Assembling(){

  // Preprocesseed code's line iterated
  std::vector<std::string>::iterator preprocessed_code_line;

  // SECTION DATA code's line iterated
  std::vector<std::string>::iterator section_data_values;

  //
  bool is_SECTION = false;

  // Boolean value that express SECTION TEXT
  // detection at the code line
  bool is_a_SECTION_TEXT = false;

  // Boolean value that express SECTION DATA
  // detection at the code line
  bool is_a_SECTION_DATA = false;

  // Array composed by the matches found in the regex search
  std::smatch matches;

  //////////////////////////////////
  //**   Read instructions ---------
  //////////////////////////////////  
  for(preprocessed_code_line = this->_pre_file.begin();
      preprocessed_code_line < this->_pre_file.end();
      preprocessed_code_line++) { 

    // Check if it's a section
    std::regex SECTION_regex("(SECTION)(\\s)(.*)");
    is_SECTION = std::regex_search(*preprocessed_code_line, matches, SECTION_regex);

    //////////////////////////////////
    //**   SECTION TEXT identifier ---
    ////////////////////////////////// 

    // If SECTION TEXT command is found,
    // the assembler class reads the next lines as 
    // assembly instructions lines

    std::regex TEXT_SECTION_regex("(SECTION)(\\s)(TEXT)");

    // Seek the directive match
    is_a_SECTION_TEXT = std::regex_search (*preprocessed_code_line,
                                            matches,TEXT_SECTION_regex);

    if(is_a_SECTION_TEXT){
      this->_section_identifier = TEXT;

      // Resets its value, since it's SECTION TEXT
      is_a_SECTION_DATA = false;

      this->_SECTION_TEXT_exist = true;

      // Indicates the line's command kind
      this->_line_type_identifier = SECTION_TYPE;
    }

    //////////////////////////////////
    //**   SECTION DATA identifier ---
    ////////////////////////////////// 

    // If SECTION DATA command is found,
    // the assembler class reads the next lines as 
    // assembly instructions lines

    std::regex DATA_SECTION_regex("(SECTION)(\\s)(DATA)");

    // Seek the directive match
    is_a_SECTION_DATA = std::regex_search (*preprocessed_code_line,
                                            matches,DATA_SECTION_regex);

    if(is_a_SECTION_DATA){
      this->_section_identifier = DATA;

      // Resets its value, since it's SECTION DATA
      is_a_SECTION_TEXT = false;

      // Indicates the line's command kind
      this->_line_type_identifier = SECTION_TYPE;
    }

        //////////////////////////////////////////////
        //**   ERROR CASE ----------------------------
        //////////////////////////////////////////////       

    // Check invalid section
    if(is_SECTION && !is_a_SECTION_DATA && !is_a_SECTION_TEXT){
      this->_section_identifier = NONE;
      error invalid_section_error(*preprocessed_code_line, this->_current_line_number, error::error_13);
      this->_assembling_errors->include_error(invalid_section_error);
    }

    // Skips date section for further evaluation
    if(this->_section_identifier == DATA){
      this->_section_data_preprocessed.insert(_section_data_preprocessed.end(),*preprocessed_code_line);
      continue;
    }

    Parser(*preprocessed_code_line);

                        //////////////////////////////////////////////
                        //**   Produce the machine code --------------
                        //////////////////////////////////////////////
    
    IdentifyCommandType();

    /* Debug
    std::cout << preprocessed_code_line;
    */ 

    /*Debug
    this->_symbol_table->PrintTable();
    */

  } // for 

  std::vector<std::string>::iterator section_data_lines;

  for(section_data_lines = this->_section_data_preprocessed.begin();
    section_data_lines < this->_section_data_preprocessed.end();
    section_data_lines++) {
      
    this->_section_identifier = DATA;

    // Resets its value, since it's SECTION DATA
    is_a_SECTION_TEXT = false;

    // Indicates the line's command kind
    this->_line_type_identifier = SECTION_TYPE;

    Parser(*section_data_lines);

                        //////////////////////////////////////////////
                        //**   Produce the machine code --------------
                        //////////////////////////////////////////////
    
    IdentifyCommandType();
  
  }

  vector<string> not_defined = this->_symbol_table->search_not_defined();
  vector<string>::iterator itr1;
  vector<label_occurrence>::iterator itr2;

  
  for(itr1=not_defined.begin(); itr1!=not_defined.end(); itr1++){
    for(itr2=this->_label_occurrences.begin(); itr2!=this->_label_occurrences.end(); itr2++){
      if(*itr1 == itr2->get_label()){
        error undefined_label_error(itr2->get_code_line(), itr2->get_line_number(), error::error_00);
        this->_assembling_errors->include_error(undefined_label_error);
      }
    }  
  }
  

        //////////////////////////////////////////////
        //**   ERROR CASE ----------------------------
        //////////////////////////////////////////////   

  // Notifies an error if SECTION TEXT was not found
  if(!this->_SECTION_TEXT_exist){
    error missing_TEXT_error("", 0, error::error_12);
    this->_assembling_errors->include_error(missing_TEXT_error);
  }
                        //////////////////////////////////////////////
                        //**   Append SECTION DATA in object file-----
                        //////////////////////////////////////////////

  // Corrects the SECTION DATA lines order(inserted in the reversed order)
  std::reverse(this->_section_data_commands.begin(),
               this->_section_data_commands.end()); 

  // Inserts SECTION DATA information to the end of
  // Object file code 
  for(section_data_values = this->_section_data_commands.begin();
      section_data_values < this->_section_data_commands.end();
      section_data_values++) {

    this->_object_file.insert(this->_object_file.begin(), 
                              *section_data_values); 
  }
 
  // Corrects the object code lines order(C++ vectores is inserted in the reversed order)
  std::reverse(this->_object_file.begin(),
               this->_object_file.end()); 



  /*debug
  std::vector<std::string>::iterator it;
  int i = 0;
  for(it = this->_object_file.begin(); it < this->_object_file.end(); it++){
    std::cout << "end " << i << ":\t" <<*it << std::endl;
    i++;
  }
  */
}

void Assembler::Scanner(){
  
    const int MAX_SIZE_TOKEN = 50;
    std::smatch matches;
    std::regex valid_token("(^\\d+$)|(^([a-z]|[A-Z]|_)(\\w+|\\d+)*$)");
    // Characters that separate tokens
    std::regex taps("\\s+|,|:|\\+");
    regex_token_iterator<string::iterator> itr(this->_current_line_string.begin(), this->_current_line_string.end(), taps, -1);
    regex_token_iterator<string::iterator> end;
    string token;
    // Cycles through all tokens identified on the line.
    while (itr != end){
      token = *itr;
      if(token != ""){

        //////////////////////////////////////////////
        //**   ERROR CASE ----------------------------
        //////////////////////////////////////////////      

        // Checks if it is a valid token
        // If not, notify an error
        if(!std::regex_match(token, matches, valid_token) || (token.length() > MAX_SIZE_TOKEN)){
          error error(this->_current_line_string, this->_current_line_number, error::error_10);
          _assembling_errors->include_error(error);
          return;
        }
      }
      itr++;
    }
    return;
}

void Assembler::Parser(std::string code_line ){

  // Indicates that the instruction follows a correct
  // pattern
  bool is_a_regular_instruction = false;

  // Indicates that is a Copy instruction
  bool is_a_COPY_instruction = false;

  // Indicates that is a STOP instruction
  bool is_a_STOP_instruction = false;

  // Indicates that is a SPACE directive
  bool is_a_SPACE_directive = false;

  // Indicates that is a CONST directive
  bool is_a_CONST_directive = false;

  // Indicates that is a label definition
  bool label_definition = false;

  bool two_label_definition = false;

  // Array composed by the matches found in the regex search
  std::smatch matches, matches2;

  // String representing the label being stored
  // in the SPACE directive
  std::string SPACE_label;

  // String representing the constant being 
  // stored in the label in the CONST directive
  std::string CONST_label;

  // Steps one line from preprocessed code
  this->_current_line_number++;

  // Stores the current line processed
  this->_current_line_string = code_line;

  // By default, the line has no
  // instruction type.
  this->_line_type_identifier = NONE;

                    //////////////////////////////////////////////
                    //**   Identify and Validate Tokens
                    //////////////////////////////////////////////

  Scanner();

                    //////////////////////////////////////////////
                    //**   Identify Label at beginning
                    //////////////////////////////////////////////

  // Identifies, eliminates from the code
  // and stores its value into the symbol table
  std::regex label_regex("(^[a-z]|[A-Z]|_)(\\w*|\\d*)(:)(\\s)(.*)");
  std::regex two_label_regex("((^[a-z]|[A-Z]|_)(\\w*|\\d*)(:)\\s*){2,}(\\s)(.*)");
  label_definition = std::regex_search (code_line,
                      matches,label_regex);

  two_label_definition = std::regex_search(code_line, matches2, two_label_regex);

  /*
  // Label
  this->_instruction_operand_1 = matches[1].str() + matches[2].str();

  // Operation
  this->_instruction_operand_2 = matches[5].str();
  */
  // Label 

  //////////////////////////////////////////////
  //**   ERROR CASE ----------------------------
  //////////////////////////////////////////////    
  
  // Check two labels in the same line
  if(two_label_definition){
    // Consider the last label
    // and notify two labels error
    this->_current_label = matches2[3].str() + matches2[4].str();
    code_line = std::regex_replace (code_line,two_label_regex,"$5");
    LabelIdentifier(this->_current_label, LABEL_DEFINITION);
    error two_labels_error(this->_current_line_string, this->_current_line_number, error::error_11);
    this->_assembling_errors->include_error(two_labels_error);
  }
  else if(label_definition) {
    this->_current_label = matches[1].str() + matches[2].str();
    code_line = std::regex_replace (code_line,label_regex,"$5");
    LabelIdentifier(this->_current_label, LABEL_DEFINITION);
    /* Debug
    std::cout << this->_instruction_operand_1 << std::endl;
    */      
  }

                          
                      //////////////////////////////////////////////
                      //**   Separate elements from line
                      //////////////////////////////////////////////

  is_a_regular_instruction = false;
  is_a_COPY_instruction = false;
  is_a_STOP_instruction = false;


  //////////////////////////////////////////////
  //**   Identify regular instruction ---------
  //////////////////////////////////////////////
  
  std::regex INSTRUCTION_regex("(ADD|SUB|MULT|DIV|JMP|JMPN|JMPP|JMPZ|LOAD|STORE|INPUT|OUTPUT)(\\s)(\\w+)(\\+*)(\\d*)");

  // Seek the instruction match
  is_a_regular_instruction = std::regex_search (code_line,
                                            matches,INSTRUCTION_regex);

  // It must ignore COPY instruction and SECTION directive
  if(is_a_regular_instruction && 
    matches[1].str().compare("COPY") != 0 &&
    matches[1].str().compare("SECTION") != 0){
    // Matches pattern at instruction:
    // 0: Instruction match
    // 1: Operator
    // 2: space character
    // 3: Head character from the label
    // 4: Tail from the label
    // 5: Plus character
    // 6: A digit

    this->_instruction_operator = matches[1].str();
    this->_instruction_operand_1 = matches[3].str() + matches[4].str();
    this->_operand_1_offset = matches[6].str();


    StoreLabelOperandOccurrence(this->_instruction_operand_1);

    // Indicates the line's command kind
    this->_line_type_identifier = REGULAR_TYPE;

    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////        

    // Check if it's in the correct section
    if(this->_section_identifier != TEXT){
      error wrong_section_error(this->_current_line_string, this->_current_line_number, error::error_06);
      this->_assembling_errors->include_error(wrong_section_error);
    }
  } // if is_regular_instruction

  //////////////////////////////////////////////
  //**   Identify COPY instruction -------------
  //////////////////////////////////////////////
  std::regex COPY_instruction_regex("(COPY)(\\s)([a-z]|[A-Z]|_)(\\w*|\\d*)(\\+*)(\\d*)(,)([a-z]|[A-Z]|_)(\\w*|\\d*)(\\+*)(\\d*)");

  // Seek the instruction match
  is_a_COPY_instruction = std::regex_search (code_line,
                                            matches,COPY_instruction_regex);

  if(is_a_COPY_instruction){
    // Matches pattern at instruction COPY:
    // 0: Instruction COPY match
    // 1: COPY
    // 2: space character
    // 3: Head character from the label
    // 4: Tail from the label
    // 5: Plus character
    // 6: A digit
    // 7: comma character
    // 8: Head character from the label
    // 9: Tail from the label
    // 10: Plus character
    // 11: A digit


    this->_instruction_operator = matches[1].str();
    this->_instruction_operand_1 = matches[3].str()+ matches[4].str();
    this->_instruction_operand_2 = matches[8].str()+ matches[9].str();
    this->_operand_1_offset = matches[6].str();
    this->_operand_2_offset = matches[11].str();

    /* Debug
    std::cout<< this->_instruction_operand_1 << endl;
    std::cout<< this->_instruction_operand_2 << endl;
    std::cout<< this->_operand_1_offset << endl;
    std::cout<< this->_operand_2_offset << endl;
    */

    StoreLabelOperandOccurrence(this->_instruction_operand_1);
    StoreLabelOperandOccurrence(this->_instruction_operand_2);

    // Indicates the line's command kind
    this->_line_type_identifier = COPY_TYPE;

    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////        

    // Check if it's in the correct section
    if(this->_section_identifier != TEXT){
      error wrong_section_error(this->_current_line_string, this->_current_line_number, error::error_06);
      this->_assembling_errors->include_error(wrong_section_error);
    }

  }          

  //////////////////////////////////////////////
  //**   Identify STOP -------------------------
  //////////////////////////////////////////////
  
  std::regex STOP_instruction_regex("(STOP)");

  // Seek the instruction match
  is_a_STOP_instruction = std::regex_search (code_line,
                                            matches,STOP_instruction_regex);

  if(is_a_STOP_instruction){
    // Matches pattern at instruction STOP:
    // 0: Instruction STOP match

    this->_instruction_operator = matches[0].str();

    // Indicates the line's command kind
    this->_line_type_identifier = STOP_TYPE;

    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////        

    // Check if it's in the correct section
    if(this->_section_identifier != TEXT){
      error wrong_section_error(this->_current_line_string, this->_current_line_number, error::error_06);
      this->_assembling_errors->include_error(wrong_section_error);
    }

  }


  // Code's line is at SECTION TEXT and 
  // has no valid instruction
  if((is_a_regular_instruction == false && 
     is_a_STOP_instruction == false &&
     is_a_COPY_instruction == false)
     && this->_section_identifier == TEXT)
  {     
    this->Error5Verify(code_line);
  }
  
  //////////////////////////////////////////////
  //**   Identify SPACE directive --------------
  //////////////////////////////////////////////
  
  std::regex SPACE_directive_regex("(SPACE)(\\s*)(\\d*)");

  // Seek the instruction match
  is_a_SPACE_directive = std::regex_search (code_line,
                                            matches,SPACE_directive_regex);

  if(is_a_SPACE_directive){
    // Matches pattern at directive SPACE:
    // 0: Directive SPACE match
    // 1: SPACE
    // 2: space character
    // 3: A digit

    this->_instruction_operator = matches[1].str();
    this->_instruction_operand_2 = matches[3].str();

    // Indicates the line's command kind
    this->_line_type_identifier = SPACE_TYPE;

    /* Debug
    std::cout<< "Directive: " << this->_instruction_operator << std::endl;
    std::cout<< "Number   : " << this->_instruction_operand_2 << std::endl;
    */

    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////    
    
    // Check if it's in the correct section
    if(this->_section_identifier != DATA){
      error wrong_section_error(this->_current_line_string, this->_current_line_number, error::error_06);
      this->_assembling_errors->include_error(wrong_section_error);
    }
  }  

  //////////////////////////////////////////////
  //**   Identify CONST directive --------------
  //////////////////////////////////////////////

  std::regex CONST_directive_regex("(CONST)(\\s)(\\d+)");

  // Seek the instruction match
  is_a_CONST_directive = std::regex_search (code_line,
                                            matches,CONST_directive_regex);

  if(is_a_CONST_directive){
    // Matches pattern at directive SPACE:
    // 0: Directive CONST match
    // 1: CONST 
    // 2: Space character
    // 3: A number

    this->_instruction_operator = matches[1].str();
    this->_instruction_operand_2 = matches[3].str();

    // Indicates the line's command kind
    this->_line_type_identifier = CONST_TYPE;

    /* Debug
    std::cout<< "Directive: " << this->_instruction_operator << std::endl;
    std::cout<< "Number  : " << this->_instruction_operand_2 << std::endl;
    */

    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////    

    // Check CONST 0
    if(std::stoi(this->_instruction_operand_2) == 0){
      vector<label_occurrence>::iterator itr;
      // Cycles through the vector of labels used by the DIV instruction
      for(itr = this->_DIV_label_occurrences.begin(); itr!=this->_DIV_label_occurrences.end(); itr++){
        // Notifies an error if the DIV statement uses the LABEL assigned to CONST 0
        if(itr->get_label() == this->_current_label){
          error div_zero_error(itr->get_code_line(), itr->get_line_number(),error::error_07);
          this->_assembling_errors->include_error(div_zero_error);
          // Replaces the operand of CONST for 1
          this->_instruction_operand_2 = "1";
        }
      }
    }

    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////        

    // Check if it's in the correct section
    if(this->_section_identifier != DATA){
      error wrong_section_error(this->_current_line_string, this->_current_line_number, error::error_06);
      this->_assembling_errors->include_error(wrong_section_error);
    }

  } // if is_a_CONST_directive


  // Code's line is at SECTION TEXT and 
  // has no valid instruction
  if((is_a_SPACE_directive == false && 
     is_a_CONST_directive == false)
     && this->_section_identifier == DATA)
  {     
    this->Error4Verify(code_line);
  }
}

void Assembler::StoreLabelOperandOccurrence(std::string label_operand){
  std::smatch matches;
  std::regex JMP_regex("\\bJMP");

  // Stores information about the occurrence of the 
  // DIV instruction to report division by zero errors
  if(this->_instruction_operator == "DIV"){
    label_occurrence occurrence(label_operand, this->_current_line_string, this->_current_line_number);
    this->_DIV_label_occurrences.insert(this->_DIV_label_occurrences.end(), occurrence);
  }
  // Stores information about the occurrence of the 
  // JMP instruction to report JMP errors
  else if(std::regex_search(this->_instruction_operator, matches, JMP_regex)){
    label_occurrence occurrence(label_operand, this->_current_line_string, this->_current_line_number);
    this->_JMP_label_occurrences.insert(this->_JMP_label_occurrences.end(), occurrence);
  }
  // Stores operand label information for possible error notifications
  label_occurrence occurrence(label_operand, this->_current_line_string, this->_current_line_number);
  this->_label_occurrences.insert(this->_label_occurrences.end(), occurrence);

}

void Assembler::IdentifyCommandType(){
  switch (this->_line_type_identifier){
    // Both Directives and regulra instructions type
    // will be treated here
    case REGULAR_TYPE:
      GenerateObjCode(this->_instruction_operator,
                      this->_instruction_operand_1);
      break;

    case COPY_TYPE:
      GenerateObjCode(this->_instruction_operator,
                this->_instruction_operand_1,
                this->_instruction_operand_2);
      break;

    case STOP_TYPE:
      GenerateObjCode(this->_instruction_operator);
      break;
      
    case SPACE_TYPE:
      //! this->_instruction_operand_2: Alloc Size
      GenerateObjCode(this->_instruction_operator,
            this->_instruction_operand_1,
            this->_instruction_operand_2);
      break;

    case CONST_TYPE:
      //! this->_instruction_operand_2: Const value
      GenerateObjCode(this->_instruction_operator,
            this->_instruction_operand_1,
            this->_instruction_operand_2);
      break;

    case SECTION_TYPE:
      break;

    default:
      break;
  }
      
}

void Assembler::GenerateObjCode(std::string instruction, std::string operand1,
                                std::string operand2) {
  //////////////////////////////////////////////
  //**   Generate machine code -----------------
  //////////////////////////////////////////////
  // There's a equivalent instruction at the 
  // instruction table

  // Value tha will be add at object code
  std::string label_value;
  int opcode = this->_instruction_table->get_opcode(instruction);
  int label_const_value,i;
  int space_size;

  // Address at object code. At this function, it's
  // used for location of undefined label at the object code 
  // to the pendency list
  int current_object_code_address;

  // Command located in section type and opcode COPY found
  if(this->_section_identifier == TEXT && opcode != ERROR){
    this->_object_file.insert(this->_object_file.begin(), 
                              to_string(opcode)); 

    //////////////////////////////////////////////
    //**   Labels identify -----------------------
    //////////////////////////////////////////////

    current_object_code_address = this->_object_file.size() + this->_section_data_commands.size();

  
    // Store labels offset 
    if(this->_operand_1_offset.compare("") != 0) {
      this->_address_offset[current_object_code_address][OFFSET] = stoi(this->_operand_1_offset);  
      this->_address_offset[current_object_code_address][LINE] = this->_current_line_number;
    }

    else{
      this->_address_offset[current_object_code_address][OFFSET] = 0;
      this->_address_offset[current_object_code_address][LINE] = this->_current_line_number;
    }

    // Operand 1
    label_value = to_string(LabelIdentifier(operand1, LABEL_OPERAND));
    
    this->_object_file.insert(this->_object_file.begin(), 
                              label_value);
    this->_symbol_table->set_list_address(operand1, current_object_code_address);

    current_object_code_address = this->_object_file.size() + this->_section_data_commands.size();

    // Store labels offset 
    if(this->_operand_2_offset.compare("") != 0) {
      this->_address_offset[current_object_code_address][OFFSET] = stoi(this->_operand_2_offset); 
      this->_address_offset[current_object_code_address][LINE] = this->_current_line_number; 
    }

    else{
      this->_address_offset[current_object_code_address][OFFSET] = 0;
      this->_address_offset[current_object_code_address][LINE] = this->_current_line_number;
    }

    // Operand 2

    label_value = to_string(LabelIdentifier(operand2, LABEL_OPERAND));
    this->_object_file.insert(this->_object_file.begin(), 
                              label_value); 
    this->_symbol_table->set_list_address(operand2, current_object_code_address);

  } //if

  // Command located in DATA type and opcode not found, 
  // that means a directive
  else if(this->_section_identifier == DATA && opcode == ERROR){
    switch (this->_line_type_identifier)
    {
    case SPACE_TYPE:
      // LABEL: SPACE
      // SPACE equals 1 predefined
      if(_instruction_operand_2.compare("") == 0){
        space_size = 1;
      }

      // LABEL: SPACE %NUMBER%
      // Alocates NUMBER amount space
      else {
        space_size = stoi(this->_instruction_operand_2);
      }

      for(i=0; i < space_size; i++){
        this->_section_data_commands.insert(this->_section_data_commands.begin(), 
                      "00"); 
      }
      break;

    case CONST_TYPE:
      label_const_value = stoi(this->_instruction_operand_2);
      this->_section_data_commands.insert(this->_section_data_commands.begin(), 
                              to_string(label_const_value)); 
      break;
    
    default:
      break;
    } // switch
  }// else if
}

void Assembler::GenerateObjCode(std::string instruction, std::string operand1) {
  //////////////////////////////////////////////
  //**   Generate machine code -----------------
  //////////////////////////////////////////////

  // Value tha will be add at object code
  std::string label_value;

  // Address at object code. At this function, it's
  // used for location of undefined label at the object code 
  // to the pendency list
  int current_object_code_address;

  // There's a equivalent instruction at the 
  // instruction table
  int opcode = this->_instruction_table->get_opcode(instruction);
  if(opcode != ERROR){
    this->_object_file.insert(this->_object_file.begin(), 
                              to_string(opcode)); 

    //////////////////////////////////////////////
    //**   Identify label ------------------------
    //////////////////////////////////////////////
  
    current_object_code_address = this->_object_file.size() + this->_section_data_commands.size();

    // Store labels offset 
    if(this->_operand_1_offset.compare("") != 0) {
      this->_address_offset[current_object_code_address][OFFSET] = stoi(this->_operand_1_offset);
      this->_address_offset[current_object_code_address][LINE] = this->_current_line_number;
    }

    else{
      this->_address_offset[current_object_code_address][OFFSET] = 0;
      this->_address_offset[current_object_code_address][LINE] = this->_current_line_number;
    }

    label_value = to_string(LabelIdentifier(operand1, LABEL_OPERAND));
    this->_object_file.insert(this->_object_file.begin(), 
                              label_value);
    this->_symbol_table->set_list_address(operand1, current_object_code_address);

  }
}

void Assembler::GenerateObjCode(std::string instruction) {
  //////////////////////////////////////////////
  //**   Generate machine code -----------------
  //////////////////////////////////////////////
  // There's a equivalent instrution at the 
  // instruction table
  int opcode = this->_instruction_table->get_opcode(instruction);
  if(opcode != ERROR){
    this->_object_file.insert(this->_object_file.begin(), 
                              to_string(opcode)); 

    //////////////////////////////////////////////
    //**   Identify label ------------------------
    //////////////////////////////////////////////

  }

  //////////////////////////////////////////////
  //**   ERROR CASE ----------------------------
  //////////////////////////////////////////////    
}

///////////////////////////////////
//**   MakePreFile        
//////////////////////////////////
void Assembler::MakeObjectFile(char* source_code_name){

  // If the .pre file doesn't exists, assemlber shall not 
  // produce the .object file
  if(!this->_exists) return;

  // The .obj file will be created in the directory
  // exec_assembler/obj_files/
  std::string string_source_code_name = "./obj_files/";

  // Source code name input at the terminal execution
  // concatenated with directory
  string_source_code_name += source_code_name; 

  // Gets the original file name and swap with .obj extention
  std::regex name_regex("(.*)(.asm)");  

  // Replace .pre extention for .obj
   string_source_code_name = std::regex_replace (string_source_code_name,name_regex,"$1.obj");

  // Convert source_code_name string variable to char type
  char char_source_code_name[string_source_code_name.size() + 1];
  strcpy(char_source_code_name, string_source_code_name.c_str());

  // Produce the .obj file with the _object_file vector
  FILE* pre_file = fopen(char_source_code_name, "w+");
  if (pre_file == NULL) perror ("Error opening file");
  std::vector<std::string>::iterator object_file_line;
  for(object_file_line = this->_object_file.begin();
      object_file_line < this->_object_file.end();
      object_file_line++) {
    
    fprintf (pre_file, "%s ", object_file_line->c_str() );
  }
  fclose(pre_file);

}


///////////////////////////////////
//**   LabelIdentifier
//////////////////////////////////

int Assembler::LabelIdentifier(std::string label, int use_type) {
  // Address at object code. At this function, it's
  // used for location of label at the object code 
  int current_object_code_address = this->_object_file.size() + this->_section_data_commands.size();

  // Verifies symbol table contains label 
  if(this->_symbol_table->search(label) == true){
    switch (use_type)
    {
    case LABEL_OPERAND:
      // Verifies if it's defined
      if(this->_symbol_table->get_definition(label)){
        return this->_symbol_table->get_value(label);
      }

      else {
        return this->_symbol_table->get_list_address(label);
      }
      break;
    case LABEL_DEFINITION:
      //////////////////////////////////////////////
      //**   ERROR CASE ----------------------------
      //////////////////////////////////////////////  

      // Verifies if it's defined
      // If already defined, notifies an error
      if(this->_symbol_table->get_definition(label)){
        error repeated_label_error(this->_current_line_string, this->_current_line_number, error::error_01);
        this->_assembling_errors->include_error(repeated_label_error);
      }

      else {
        ResolveLabelValue(label);
      }
      break;
    default:
      break;
    }

  }

  // If it's the first time the label is mentioned
  // inserts at symbol table
  else {
    switch (use_type)
    {
    case LABEL_OPERAND:
      // Default value
      this->_symbol_table->set_value(label, 0);
      // Set as undefined
      this->_symbol_table->set_definition(label, false);
      // Symbol location reference at code
      this->_symbol_table->set_list_address(label, current_object_code_address);

      /* Debug
      std::cout<< this->_symbol_table->search(label) << std::endl;
      */      

      // 
      return -1;

      break;
    
    case LABEL_DEFINITION:
      // Label's addres
      this->_symbol_table->set_value(label, current_object_code_address);
      // Set as defined
      this->_symbol_table->set_definition(label, true);
      // Symbol location reference at code
      this->_symbol_table->set_list_address(label, -1);
      break;
    default:
      break;
    }// switch
  }// else

  return 0;
}


void Assembler::ResolveLabelValue(std::string label){
  // Address at object code. At this function, it's
  // used for location of label at the object code.
  // Must be added to section data because codes are 
  // stored in differents vectors.
  int label_value = this->_object_file.size() + this->_section_data_commands.size();

    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////      

  // Check JMP for SECTION DATA
  if(label_value >= this->_object_file.size()){
    vector<label_occurrence>::iterator itr;
    // Cycles through the vector of labels used by the JMP instructions
    for(itr = this->_JMP_label_occurrences.begin(); itr!=this->_JMP_label_occurrences.end(); itr++){
      // Notifies an error if JMP goes to SECTION DATA
      if(itr->get_label() == label){
        error JMP_wrong_section_error(itr->get_code_line(), itr->get_line_number(),error::error_03);
        this->_assembling_errors->include_error(JMP_wrong_section_error);
      }
    }
  }
  
  // Access last reference to label at object code
  int label_reference = this->_symbol_table->get_list_address(label);

  // Iteration to the next line which reference the label
  // again.
  int next_label_reference;

  // Indicates the address from the label plus the offset 
  // from the operand
  int access_address;

  // Label's size at SPACE command
  int alloc_size;

  // Preprocessed line that occured the error
  std::vector<std::string>::iterator error_line;

  // Reverses object file vector to cascading
  // value substitution process. Thus, 0 index must be
  // the first byte, not the last
  std::reverse(this->_object_file.begin(),
               this->_object_file.end()); 

  while(label_reference != -1) {

    alloc_size = AllocSizeManager(label_reference);

    next_label_reference = stoi(this->_object_file[label_reference]);

    access_address = label_value + this->_address_offset[label_reference][OFFSET];

    //////////////////////////////////////////////////////////////
    //**   Label's storage memory access validation(SPACE range)
    //////////////////////////////////////////////////////////////

    // The offset surpass the alloc maximum, that is alloc-size-1.
    // At offset 0, it's the base address
    if(alloc_size - 1 < this->_address_offset[label_reference][OFFSET]){

      error_line = this->_pre_file.begin()+(this->_address_offset[label_reference][LINE] - 1);

      // ERROR - Out-of-range label access
      error out_range_error(*error_line,
                            this->_address_offset[label_reference][LINE],
                            error::error_16);

      _assembling_errors->include_error(out_range_error);
    /* Debug
    cout<< alloc_size <<endl;
    cout<<this->_address_offset[label_reference][OFFSET] <<endl;
    */
    } //if


    this->_object_file[label_reference] = to_string(access_address);
    label_reference = next_label_reference;
  } // while

  // Goes back to its standard pattern(inverted list)
  std::reverse(this->_object_file.begin(),
              this->_object_file.end()); 

  // Label's address
  this->_symbol_table->set_value(label, label_value);

  // Set as defined
  this->_symbol_table->set_definition(label, true);
}


int Assembler::AllocSizeManager(int label_reference){

  // Express SPACE allocated at the directive
  std::string alloc_size_string = this->_instruction_operand_2;

  // How many bytes SPACE was stored
  int alloc_size_number;

  // Identifies correspond directive
  std::smatch space_matches;
  std::smatch const_matches;
  
  std::regex space_regex("(SPACE)(\\s)(\\d+)");
  std::regex const_regex("(CONST)(\\s)(\\d+)");
  bool space_command = std::regex_search (this->_current_line_string,
                      space_matches,space_regex);
  bool const_command = std::regex_search (this->_current_line_string,
                      const_matches,const_regex);        

  // Some important details from this directive identify
  // process:
  // * If there's no directive valid at this line
  //   it will treat the command as SPACE by default(a regular
  //   label declared at the line)
  //   The invalid directive will be reported at the 
  //   Assembling() function.


  // Store labels alloc size at SPACE command
  if(space_command){
    if(alloc_size_string.compare("") != 0) {
      alloc_size_number = stoi(space_matches[3].str());
      return alloc_size_number;  
    }

    else{
      alloc_size_number = 1;
      return alloc_size_number;
    } // else
  } // if

  // ERROR case - CONST modify operation
  else if(const_command){

    this->Error15Verify(label_reference);

    alloc_size_number = 1;

    return alloc_size_number;
  }


  // Common Label (just 1 alloc size)
  else{
    alloc_size_number = 1;
    return alloc_size_number;
  }
}

void Assembler::Error4Verify(std::string code_line) {
  std::smatch matches;
  std::regex DIRECTIVE_regex("(SECTION|SPACE|CONST|ADD|SUB|MULT|DIV|JMP|JMPN|JMPP|JMPZ|LOAD|STORE|INPUT|OUTPUT|COPY|STOP)(.*)");

  // Check if begins with a valid instruction
  bool directive_exist = std::regex_search (code_line,
                                            matches,DIRECTIVE_regex);


  //////////////////////////////////////////////
  //**   ERROR CASE ----------------------------
  //////////////////////////////////////////////   

  if(!directive_exist){
    error invalid_instruction(code_line,
                          this->_current_line_number,
                          error::error_04);

    _assembling_errors->include_error(invalid_instruction);
  }
}

void Assembler::Error5Verify(std::string code_line) {
  std::smatch matches;
  std::regex INSTRUCTION_regex("(SECTION|SPACE|CONST|ADD|SUB|MULT|DIV|JMP|JMPN|JMPP|JMPZ|LOAD|STORE|INPUT|OUTPUT|COPY|STOP)(.*)");

  // Check if begins with a valid instruction
  bool instruction_exist = std::regex_search (code_line,
                                            matches,INSTRUCTION_regex);


  //////////////////////////////////////////////
  //**   ERROR CASE ----------------------------
  //////////////////////////////////////////////   

  if(!instruction_exist){
    error invalid_instruction(code_line,
                          this->_current_line_number,
                          error::error_05);

    _assembling_errors->include_error(invalid_instruction);
  }
}


void Assembler::Error15Verify(int label_reference){

    /* DEBUG
    cout << "PASSEI" << endl;
    */

    // Line where the refence occurs
    std::vector<std::string>::iterator label_reference_line;
    label_reference_line = this->_pre_file.begin()+(this->_address_offset[label_reference][LINE] - 1);

    // Identify the declared label
    std::smatch label_match;
    std::regex label_regex("([a-z]|[A-Z]|_)(\\w*|\\d*)(:)(\\s)(CONST)(\\s)(\\d+)");
    std::regex_search (this->_current_line_string,
                        label_match,label_regex);

    // Declared label as CONST
    std::string label = label_match[1].str()+label_match[2].str();

    // Analyse operation at CONST labels
    std::smatch modify_const_match;
    
    std::regex copy_regex("(COPY)(\\s)([a-z]|[A-Z]|_)(\\w*|\\d*)(\\+*)(\\d*)(,)([a-z]|[A-Z]|_)(\\w*|\\d*)(\\+*)(\\d*)");
    std::regex store_regex("(STORE)(\\s)(\\w+)");
    std::regex input_regex("(INPUT)(\\s)(\\w+)");

    bool store_command = std::regex_search (*label_reference_line,
                        modify_const_match,store_regex);

    bool input_command = std::regex_search (*label_reference_line,
                        modify_const_match,input_regex);

    bool copy_command = std::regex_search (*label_reference_line,
                    modify_const_match,copy_regex);

    std::string destiny_operand = modify_const_match[3].str()+modify_const_match[4].str();

    // Verifies if destiny operand is the CONST label
    if(copy_command){

      // It's the CONST label as operand
      if(label.compare(destiny_operand) == 0){
        copy_command = true;
      }

      // CONST label is operand. Don't modify
      // it's value
      else
      {
        copy_command = false;
      }
      
    }                       
          
    if(copy_command || store_command || input_command == true) {
      // Preprocessed line that occured the error
      std::vector<std::string>::iterator error_line;
      error_line = this->_pre_file.begin()+(this->_address_offset[label_reference][LINE] - 1);

      // ERROR - Out-of-range label access
      error const_modify_error(*error_line,
                            this->_address_offset[label_reference][LINE],
                            error::error_15);

      _assembling_errors->include_error(const_modify_error);
    }
}