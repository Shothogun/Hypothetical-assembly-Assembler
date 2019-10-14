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
}

///////////////////////////////////
//**   Assembling        
//////////////////////////////////
void Assembler::Assembling(){

  // Preprocesseed code's line iterated
  std::vector<std::string>::iterator preprocessed_code_line;

  // SECTION DATA code's line iterated
  std::vector<std::string>::iterator section_data_values;

  // Boolean value that express SECTION TEXT
  // detection at the code line
  bool is_a_SECTION_TEXT = false;

  // Boolean value that express SECTION DATA
  // detection at the code line
  bool is_a_SECTION_DATA = false;

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

  // Array composed by the matches found in the regex search
  std::smatch matches;

  // String representing the label being stored
  // in the SPACE directive
  std::string SPACE_label;

  // String representing the constant being 
  // stored in the label in the CONST directive
  std::string CONST_label;

  //////////////////////////////////
  //**   Read instructions ---------
  //////////////////////////////////  
  for(preprocessed_code_line = this->_pre_file.begin();
      preprocessed_code_line < this->_pre_file.end();
      preprocessed_code_line++) {    
                        //////////////////////////////////////////////
                        //**   Identify Label at beginning
                        //////////////////////////////////////////////

    // Identifies, eliminates from the code
    // and stores its value into the symbol table
    std::regex label_regex("(^[a-z]|[A-Z]|_)(\\w*|\\d*)(:)(\\s)(.*)");
    label_definition = std::regex_search (*preprocessed_code_line,
                        matches,label_regex);

    this->_instruction_operand_1 = matches[1].str() + matches[2].str();


    if(label_definition) {
      *preprocessed_code_line = std::regex_replace (*preprocessed_code_line,label_regex,"$5");
      LabelIdentifier(this->_instruction_operand_1, LABEL_DEFINITION);
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

    // Steps one line from preprocessed code
    this->_current_line++;
        
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
    //**   Identify regular instruction ---------
    //////////////////////////////////////////////
    
    std::regex INSTRUCTION_regex("(\\w+)(\\s)([a-z]|[A-Z]|_)(\\w*|\\d*)");

    // Seek the instruction match
    is_a_regular_instruction = std::regex_search (*preprocessed_code_line,
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

      this->_instruction_operator = matches[1].str();
      this->_instruction_operand_1 = matches[3].str() + matches[4].str();

      // Indicates the line's command kind
      this->_line_type_identifier = REGULAR_TYPE;
    }

    //////////////////////////////////////////////
    //**   Identify COPY instruction -------------
    //////////////////////////////////////////////
    std::regex COPY_instruction_regex("(COPY)(\\s)(\\w+)(,)(\\w+)");

    // Seek the instruction match
    is_a_COPY_instruction = std::regex_search (*preprocessed_code_line,
                                              matches,COPY_instruction_regex);

    if(is_a_COPY_instruction){
      // Matches pattern at instruction COPY:
      // 0: Instruction COPY match
      // 1: COPY
      // 2: space character
      // 3: operand 1
      // 4: comma ',' character
      // 5: operand 2

      this->_instruction_operator = matches[1].str();
      this->_instruction_operand_1 = matches[3].str();
      this->_instruction_operand_2 = matches[5].str();

      // Indicates the line's command kind
      this->_line_type_identifier = COPY_TYPE;
    }                                              

    //////////////////////////////////////////////
    //**   Identify STOP -------------------------
    //////////////////////////////////////////////
    
    std::regex STOP_instruction_regex("(STOP)");

    // Seek the instruction match
    is_a_STOP_instruction = std::regex_search (*preprocessed_code_line,
                                              matches,STOP_instruction_regex);

    if(is_a_STOP_instruction){
      // Matches pattern at instruction STOP:
      // 0: Instruction STOP match

      this->_instruction_operator = matches[0].str();

      // Indicates the line's command kind
      this->_line_type_identifier = STOP_TYPE;
    }  

    //////////////////////////////////////////////
    //**   Identify SPACE directive --------------
    //////////////////////////////////////////////
    
    std::regex SPACE_directive_regex("(SPACE)(\\s*)(\\d*)");

    // Seek the instruction match
    is_a_SPACE_directive = std::regex_search (*preprocessed_code_line,
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
    }  

    //////////////////////////////////////////////
    //**   Identify CONST directive --------------
    //////////////////////////////////////////////

    std::regex CONST_directive_regex("(CONST)(\\s)(\\d+)");

    // Seek the instruction match
    is_a_CONST_directive = std::regex_search (*preprocessed_code_line,
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
    }
                        //////////////////////////////////////////////
                        //**   ERROR verify --------------------------
                        //////////////////////////////////////////////

                        //////////////////////////////////////////////
                        //**   Produce the machine code --------------
                        //////////////////////////////////////////////
    
  IdentifyCommandType();

  /* Debug
  std::cout << *preprocessed_code_line;
  */ 

  /*Debug
  this->_symbol_table->PrintTable();
  */

  } // for 

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

void Assembler::IdentifyCommandType(){
  switch (this->_line_type_identifier){
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

      // Operand 1
      label_value = to_string(LabelIdentifier(operand1, LABEL_OPERAND));
      this->_object_file.insert(this->_object_file.begin(), 
                                label_value);
      this->_symbol_table->set_list_address(operand1, current_object_code_address);

      current_object_code_address = this->_object_file.size() + this->_section_data_commands.size();

      // Operand 1

      label_value = to_string(LabelIdentifier(operand2, LABEL_OPERAND));
      this->_object_file.insert(this->_object_file.begin(), 
                                label_value); 
      this->_symbol_table->set_list_address(operand2, current_object_code_address);

    }

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
      }
    }

    else {

    }

    //////////////////////////////////////////////
    //**   ERROR case ----------------------------
    //////////////////////////////////////////////
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
      label_value = to_string(LabelIdentifier(operand1, LABEL_OPERAND));
      this->_object_file.insert(this->_object_file.begin(), 
                                label_value);
      this->_symbol_table->set_list_address(operand1, current_object_code_address);

    }

    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////
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
      // Verifies if it's defined
      if(this->_symbol_table->get_definition(label)){
        // ERROR case
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
    }
  }
}

int Assembler::ResolveLabelValue(std::string label){
  // Address at object code. At this function, it's
  // used for location of label at the object code.
  // Must be added to section data because codes are 
  // stored in differents vectors.
  int label_value = this->_object_file.size() + this->_section_data_commands.size();

  // Access last reference to label at object code
  int label_reference = this->_symbol_table->get_list_address(label);

  int next_label_reference;

  // Reverses object file vector to cascading
  // value substitution process. Thus, 0 index must be
  // the first byte, not the last
  std::reverse(this->_object_file.begin(),
               this->_object_file.end()); 

  while(label_reference != -1) {
    next_label_reference = stoi(this->_object_file[label_reference]);
    this->_object_file[label_reference] = to_string(label_value);
    label_reference = next_label_reference;
  }

  // Goes back to its standard pattern(inverted list)
  std::reverse(this->_object_file.begin(),
              this->_object_file.end()); 

  // Label's address
  this->_symbol_table->set_value(label, label_value);

  // Set as defined
  this->_symbol_table->set_definition(label, true);
}