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

  // Boolean value that express SECTION
  // detection at the code line
  bool is_SECTION = false;
  
  // Boolean value that express BEGIN
  // detection at the code line
  bool is_BEGIN = false;

  // Boolean value that express END
  // detection at the code line
  bool is_END = false;

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

    //////////////////////////////////
    //**   BEGIN identifier ----------
    ////////////////////////////////// 

    std::regex BEGIN_regex("((^[a-z]|[A-Z]|_)(\\w*|\\d*)(:))*(BEGIN)(\\n)");
    is_BEGIN = std::regex_search(*preprocessed_code_line, matches, BEGIN_regex);
    if(is_BEGIN){
      // Remove BEGIN in current line
      *preprocessed_code_line = std::regex_replace (*preprocessed_code_line,BEGIN_regex,"$3");
    }

    //////////////////////////////////
    //**   END identifier -----------
    ////////////////////////////////// 

    std::regex END_regex("(END)(\\n)");
    is_END = std::regex_search(*preprocessed_code_line, matches, END_regex);    
    if(is_END){
      break;
    }

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

    // Skips data section for further evaluation
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

        //////////////////////////////////////////////
        //**   Update the definition table -----------
        //////////////////////////////////////////////  
  std::map<string, int>::iterator element;
  /*Debug
  std::cout<<"Definition table:"<<std::endl;
  */
  for(element = this->_definition_table.begin(); element != this->_definition_table.end();
                                            element++){
    element->second = this->_symbol_table->get_value(element->first);
    /*Debug
    std::cout<<"label: "<<element->first<<" value: "<<element->second<<std::endl;
    */
    
  }

  /* Debug
  std::map<string, std::vector<int>>::iterator element2;
  std::vector<int>::iterator element3;
  std::cout<<"Usage table:"<<std::endl;
  for(element2 = this->_usage_table.begin(); element2 != this->_usage_table.end(); element2++){
    std::cout<<"label: "<<element2->first<<" positions: ";
    for(element3 = element2->second.begin(); element3 != element2->second.end(); element3++){
      std::cout<<*element3;
    }
    std::cout<<std::endl;
  }
  std::cout<<std::endl;
  */

        //////////////////////////////////////////////
        //**   ERROR CASE ----------------------------
        //////////////////////////////////////////////   

  vector<string> not_defined = this->_symbol_table->search_not_defined();
  vector<string>::iterator itr1;
  vector<label_occurrence>::iterator itr2;

  // Notifies an error if label is no defined
  for(itr1=not_defined.begin(); itr1!=not_defined.end(); itr1++){
    for(itr2=this->_label_occurrences.begin(); itr2!=this->_label_occurrences.end(); itr2++){
      if(*itr1 == itr2->get_label()){
        error undefined_label_error(itr2->get_code_line(), itr2->get_line_number(), error::error_00);
        this->_assembling_errors->include_error(undefined_label_error);
        if((itr2->get_instruction_operator() == "JMP") ||
          (itr2->get_instruction_operator() == "JMPZ") ||
          (itr2->get_instruction_operator() == "JMPP") ||
          (itr2->get_instruction_operator() == "JMPN")){
          error undefined_label_error(itr2->get_code_line(), itr2->get_line_number(), error::error_02);
          this->_assembling_errors->include_error(undefined_label_error);
        }    
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
 
  // Corrects the object code lines order(C++ vectors is inserted in the reversed order)
  std::reverse(this->_object_file.begin(),
               this->_object_file.end()); 

  /*Debug
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
    std::regex valid_token("(^\\d+$)|(^([a-z]|[A-Z]|_)(\\w+|\\d+)*$)|((^\\-*)(0x|0X|)(\\d+$))");
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

  // Indicates that is a SECTION directive
  bool is_a_SECTION = false;

  // Indicates that is a label definition
  bool label_definition = false;

  bool two_label_definition = false;

  bool extern_label_definition = false;

  bool public_label_definition = false;

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
                    //**   Identify EXTERN Label 
                    //////////////////////////////////////////////                    

  std::regex extern_label_regex("(^[a-z]|[A-Z]|_)(\\w*|\\d*)(:)(\\s)(EXTERN)");
  extern_label_definition = std::regex_search (code_line, matches, extern_label_regex);
  if(extern_label_definition){
    this->_current_label = matches[1].str() + matches[2].str();
    LabelIdentifier(this->_current_label, EXTERN_LABEL);
    return;
  }
                    //////////////////////////////////////////////
                    //**   Identify PUBLIC Label 
                    //////////////////////////////////////////////   
  std::regex public_label_regex("(PUBLIC)(\\s)(^[a-z]|[A-Z]|_)(\\w*|\\d*)(\\n)");
  public_label_definition = std::regex_search (code_line, matches, public_label_regex);
  if(public_label_definition){
    this->_current_label = matches[3].str() + matches[4].str();
    LabelIdentifier(this->_current_label, PUBLIC_LABEL);
    return;
  }
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
    code_line = std::regex_replace (code_line,two_label_regex,"$6");
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
  //**   Identify SECTION diretive ------------
  //////////////////////////////////////////////

  std::regex SECTION_regex("(SECTION)(\\s)(DATA|TEXT)");

  // Seek the SECTION match
  is_a_SECTION = std::regex_search (code_line,
                                    matches,SECTION_regex);

  if(is_a_SECTION){
    return;
  }

  //////////////////////////////////////////////
  //**   Identify regular instruction ---------
  //////////////////////////////////////////////
  
  std::regex INSTRUCTION_regex("(SECTION|ADD|SUB|MULT|DIV|JMP|JMPN|JMPP|JMPZ|LOAD|STORE|INPUT|OUTPUT)(\\s)(^[a-z]|[A-Z]|_)(\\w*|\\d*)(\\+\\d+|\\n)");

  // Seek the instruction match
  is_a_regular_instruction = std::regex_search (code_line,
                                                matches,INSTRUCTION_regex);

  // It must ignore COPY instruction and SECTION directive
  if(is_a_regular_instruction && 
    matches[1].str().compare("COPY") != 0 &&
    matches[1].str().compare("SECTION") != 0){

    std::regex INSTRUCTION_regex("(ADD|SUB|MULT|DIV|JMP|JMPN|JMPP|JMPZ|LOAD|STORE|INPUT|OUTPUT)(\\s)(\\w+)(\\+*)(\\d*)");

    // Seek the instruction match
    is_a_regular_instruction = std::regex_search (code_line,
                                                  matches,INSTRUCTION_regex);

    // Matches pattern at instruction:
    // 0: Instruction match
    // 1: Operator
    // 2: space character
    // 3: label
    // 4: plus character
    // 5: number


    this->_instruction_operator = matches[1].str();
    this->_instruction_operand_1 = matches[3].str();
    this->_operand_1_offset = matches[5].str();

    // Verifies invalid argument occurence
    this->ModifyAdressLabelVerify(code_line);


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
  std::regex COPY_instruction_regex("(COPY)(\\s)([a-z]|[A-Z]|_)(\\w*|\\d*)(\\+*)(\\d*)(,)([a-z]|[A-Z]|_)(\\w*|\\d*)(\\+*)(\\d*)(\\n*)$");

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

    // Verifies invalid argument occurence
    this->ModifyAdressLabelVerify(code_line);

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
  
  std::regex STOP_instruction_regex("(STOP)(\\n*)$");

  // Seek the instruction match
  is_a_STOP_instruction = std::regex_search (code_line,
                                            matches,STOP_instruction_regex);

  if(is_a_STOP_instruction){
    // Matches pattern at instruction STOP:
    // 0: Instruction STOP match
    this->_instruction_operator = matches[1].str();
    
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
    this->Error8Verify(code_line);
    this->Error9Verify(code_line);
    this->Error14Verify(code_line);
    this->InvalidInstructionWrite(code_line);
  }

  
  //////////////////////////////////////////////
  //**   Identify SPACE directive --------------
  //////////////////////////////////////////////
  
  std::regex SPACE_directive_regex("(^SPACE)(\\s\\d*|\\s{0})(\\n$)");

  // Seek the instruction match
  is_a_SPACE_directive = std::regex_search (code_line,
                                            matches,SPACE_directive_regex);                                        

  if(is_a_SPACE_directive){  
    if(this->_section_identifier == DATA){
      // If instruction is correct, third regex
      // group is always a number
      std::regex CORRECT_SPACE_directive_regex("(^SPACE)(\\s*)(\\d*)(\\n$)");

      // Seek the instruction match
      is_a_SPACE_directive = std::regex_search (code_line,
                                                matches,CORRECT_SPACE_directive_regex);

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
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////    
    
    // Check if it's in the correct section
    else{
      error wrong_section_error(this->_current_line_string, this->_current_line_number, error::error_06);
      this->_assembling_errors->include_error(wrong_section_error);
    }
  }  

  //////////////////////////////////////////////
  //**   Identify CONST directive --------------
  //////////////////////////////////////////////

  std::regex CONST_directive_regex("(CONST)(\\s)(\\-*)(0x|0X|)(\\d+)");

  // Seek the instruction match
  is_a_CONST_directive = std::regex_search (code_line,
                                            matches,CONST_directive_regex);

  if(is_a_CONST_directive){ 
    if(this->_section_identifier == DATA){
      /*Debug
      cout << code_line << endl;
      */
      // Matches pattern at directive SPACE:
      // 0: Directive CONST match
      // 1: CONST 
      // 2: Space character
      // 3: A number

      this->_instruction_operator = matches[1].str();
      this->_instruction_operand_2 = matches[3].str() + matches[4].str() + matches[5].str();

      /*Debug
      cout << this->_instruction_operator << endl;
      cout << this->_instruction_operand_2 << endl;
      */

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
      long CONST_value = std::stol (this->_instruction_operand_2, nullptr, 0);
      if(CONST_value == 0){
        vector<label_occurrence>::iterator itr;
        // Cycles through the vector of labels used by the DIV instruction
        for(itr = this->_label_occurrences.begin(); itr!=this->_label_occurrences.end(); itr++){
          // Notifies an error if the DIV statement uses the LABEL assigned to CONST 0
          if((itr->get_label() == this->_current_label) && (itr->get_instruction_operator() == "DIV")){
            error div_zero_error(itr->get_code_line(), itr->get_line_number(),error::error_07);
            this->_assembling_errors->include_error(div_zero_error);
          }
        }
      }
    }

    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////        

    // Check if it's in the correct section
    else{
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
    /* Debug
      std::cout << this->_current_line_string << endl;  
      std::cout << this->_current_line_number << endl;                                          
    */
    this->Error4Verify(code_line);
    this->Error8Verify(code_line);
    this->Error14Verify(code_line);
    this->InvalidInstructionWrite(code_line);
  }
}

void Assembler::StoreLabelOperandOccurrence(std::string label_operand){

  // Stores operand label information for possible error notifications
  label_occurrence occurrence(label_operand, this->_instruction_operator, this->_current_line_string, this->_current_line_number);
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
  // Absolute value
  this->_bit_map.push_back(0);                          

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
    // Relative value
    this->_bit_map.push_back(1);                          
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
    // Relative value
    this->_bit_map.push_back(1);                           
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
        // Absolute value
        this->_bit_map.push_back(0);               
      }
      break;

    case CONST_TYPE:
      label_const_value = (int) std::stol (this->_instruction_operand_2, nullptr, 0);;
      this->_section_data_commands.insert(this->_section_data_commands.begin(), 
                              to_string(label_const_value)); 
      // Absolute value
      this->_bit_map.push_back(0);
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
    // Absolute value
    this->_bit_map.push_back(0);
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
    // Relative value
    this->_bit_map.push_back(1);

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
    // Absolute value                           
    this->_bit_map.push_back(0);
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

  // Gets the file name without extention
  std::string prog_name = std::regex_replace(source_code_name,name_regex,"$1");
  
  // Convert source_code_name string variable to char type
  char char_source_code_name[string_source_code_name.size() + 1];
  strcpy(char_source_code_name, string_source_code_name.c_str());

  // Produce the .obj file with the _object_file vector
  FILE* obj_file = fopen(char_source_code_name, "w+");
  if (obj_file == NULL) perror ("Error opening file");

  // Headear informations in .obj file
  fprintf (obj_file, "H: %s\n", prog_name.c_str());
  fprintf (obj_file, "H: %d\n", (int)this->_object_file.size());
  fprintf (obj_file, "H: ");

  for(auto bit : this->_bit_map){
    fprintf(obj_file, "%d", bit);
  }

  fprintf (obj_file, "\n");

  // Code
  fprintf(obj_file, "T: ");
  std::vector<std::string>::iterator object_file_line;
  for(object_file_line = this->_object_file.begin();
      object_file_line < this->_object_file.end();
      object_file_line++) {
    
    fprintf (obj_file, "%s ", object_file_line->c_str() );
  }
  fprintf (obj_file, "\n");  

  if(this->_is_MODULE){
    // Definition table
    if(this->_definition_table.size() > 0){
      fprintf (obj_file, "DEFINITION TABLE:\n");
      fprintf (obj_file, "VALUE     SYMBOL\n");
    }
    std::map<std::string, int>::iterator definition_table_line;
    for(definition_table_line = this->_definition_table.begin(); definition_table_line != this->_definition_table.end(); definition_table_line++){
      fprintf (obj_file, "%08d  ", definition_table_line->second);
      fprintf (obj_file, "%s\n", definition_table_line->first.c_str());
    }

    // Usage table
    if(this->_usage_table.size() > 0){
      fprintf (obj_file, "USAGE TABLE:\n");
      fprintf (obj_file, "ADDRESS   SYMBOL\n");
    }
    std::map<std::string, std::vector<int>>::iterator usage_table_line;
    std::vector<int>::iterator usage_table_col;
    for(usage_table_line = this->_usage_table.begin(); usage_table_line != this->_usage_table.end(); usage_table_line++){
      for(usage_table_col = usage_table_line->second.begin(); usage_table_col != usage_table_line->second.end(); usage_table_col++){
        fprintf (obj_file, "%08d  ", *usage_table_col);
        fprintf (obj_file, "%s\n", usage_table_line->first.c_str());
      }  
    }
  }
  fclose(obj_file);
}


///////////////////////////////////
//**   LabelIdentifier
//////////////////////////////////

int Assembler::LabelIdentifier(std::string label, int use_type) {
  // Address at object code. At this function, it's
  // used for location of label at the object code 
  int current_object_code_address = this->_object_file.size() + this->_section_data_commands.size();

  std::vector<int> usage_lines;
  
  // Verifies symbol table contains label 
  if(this->_symbol_table->search(label) == true){
    switch (use_type)
    {
    case LABEL_OPERAND:
      // Verifies if it's defined
      if(this->_symbol_table->get_definition(label)){
        if(this->_symbol_table->get_extern(label)){
          this->_usage_table[label].push_back(current_object_code_address);
        }
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

        // It's a address label
        if(this->_section_identifier == TEXT){
          this->_address_labels.insert(_address_labels.begin(),label);
        }
      }

      this->Error15Verify(label);
      break;
    case PUBLIC_LABEL:
      this->_definition_table.insert(pair<string, int>(label, NULL));
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

      // It's a address label
      if(this->_section_identifier == TEXT){
        this->_address_labels.insert(_address_labels.begin(),label);
      }

      this->Error15Verify(label);

      break;
    case EXTERN_LABEL:
      // Label's addres
      this->_symbol_table->set_value(label, 0);
      // Set as defined
      this->_symbol_table->set_definition(label, true);
      // Symbol location reference at code
      this->_symbol_table->set_list_address(label, -1);
      // Set as extern
      this->_symbol_table->set_extern(label, true);
      // Insert label in usage table
      this->_usage_table.insert(pair<string, std::vector<int>>(label, usage_lines));
      
      break;
    case PUBLIC_LABEL:
      this->_definition_table.insert(pair<string, int>(label, NULL));
      
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
  uint label_value = this->_object_file.size() + this->_section_data_commands.size();

    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////      

  // Check JMP for SECTION DATA
  if(label_value > this->_object_file.size()){
    vector<label_occurrence>::iterator itr;
    // Cycles through the vector of labels used by the JMP instructions
    for(itr = this->_label_occurrences.begin(); itr!=this->_label_occurrences.end(); itr++){
      // Notifies an error if JMP goes to SECTION DATA
      if((itr->get_label() == label) && ((itr->get_instruction_operator() == "JMP") ||
      (itr->get_instruction_operator() == "JMPZ") ||
      (itr->get_instruction_operator() == "JMPP") ||
      (itr->get_instruction_operator() == "JMPN"))){
        error JMP_wrong_section_error(itr->get_code_line(), itr->get_line_number(),error::error_03);
        this->_assembling_errors->include_error(JMP_wrong_section_error);
        error JMP_invalid_label_error(itr->get_code_line(), itr->get_line_number(),error::error_02);
        this->_assembling_errors->include_error(JMP_invalid_label_error);
      }
    }
  }

  // Check JMP for SPACE, CONST, SECTION TEXT and SECTION DATA
  std::smatch matches;
  std::regex invalid_destiny_regex("(.*)(SPACE|CONST|SECTION\\sTEXT|SECTION\\sDATA)([^:])");

  if((label_value < this->_object_file.size()) && std::regex_search(this->_current_line_string, matches, invalid_destiny_regex)){
    vector<label_occurrence>::iterator itr;
    for(itr = this->_label_occurrences.begin(); itr!=this->_label_occurrences.end(); itr++){
      // Notifies an error if JMP goes to SECTION DATA
      if((itr->get_label() == label) && ((itr->get_instruction_operator() == "JMP") ||
      (itr->get_instruction_operator() == "JMPZ") ||
      (itr->get_instruction_operator() == "JMPP") ||
      (itr->get_instruction_operator() == "JMPN"))){
        error JMP_invalid_label_error(itr->get_code_line(), itr->get_line_number(),error::error_02);
        this->_assembling_errors->include_error(JMP_invalid_label_error);
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
    alloc_size = AllocSizeManager();

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


int Assembler::AllocSizeManager(){

  // Express SPACE allocated at the directive
  std::string alloc_size_string;

  // How many bytes SPACE was stored
  int alloc_size_number;

  // Identifies correspond directive
  std::smatch space_matches;
  std::smatch const_matches;
  
  std::regex space_regex("(^[a-z]|[A-Z]|_)(\\w*|\\d*)(:)(\\s)(SPACE)(\\s)(\\d+)");
  std::regex const_regex("(^[a-z]|[A-Z]|_)(\\w*|\\d*)(:)(\\s)(CONST)(\\s)(\\d+)");
  bool space_command = std::regex_search (this->_current_line_string,
                      space_matches,space_regex);
  bool const_command = std::regex_search (this->_current_line_string,
                      const_matches,const_regex);        

  alloc_size_string = space_matches[7].str();

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
      alloc_size_number = stoi(alloc_size_string);
      return alloc_size_number;  
    }

    else{
      alloc_size_number = 1;
      return alloc_size_number;
    } // else
  } // if

  // ERROR case - CONST modify operation
  else if(const_command){
    alloc_size_number = 1;

    return alloc_size_number;
  }


  // Common Label (just 1 alloc size)
  else{
    alloc_size_number = 1;
    return alloc_size_number;
  }
}

void Assembler::LinkOffsetLabels(){
  std::map<int, int[2]>::iterator extern_label;

  // Object code address where occurs
  // the offset labels calls
  int obj_code_address;

  // Offset value written at object code's
  // extern labels locations
  int offset;

  // Value stored at the object code file
  std::string obj_code;

  for(extern_label = this->_address_offset.begin(); 
      extern_label != this->_address_offset.end();
      ++extern_label)
  {
    obj_code_address = extern_label->first;
    offset = extern_label->second[OFFSET];
    obj_code = this->_object_file[obj_code_address];

    // Verifies if the offset case is a 
    // extern label use
    if(obj_code.compare("0") == 0){
      // Writes offset value if extern labels uses it
      this->_object_file[obj_code_address] = to_string(offset);
    }
  }
}

void Assembler::Error4Verify(std::string code_line) {
  std::smatch matches;
  std::regex DIRECTIVE_regex("(SECTION|SPACE|CONST|ADD|SUB|MULT|DIV|JMP|JMPN|JMPP|JMPZ|LOAD|STORE|INPUT|OUTPUT|COPY|STOP)(?!\\d|\\w)(.*)");

  // Check if begins with a valid instruction
  bool instruction_exist = std::regex_search (code_line,
                                            matches,DIRECTIVE_regex);


  if(!instruction_exist){
    error invalid_instruction(code_line,
                          this->_current_line_number,
                          error::error_04);

    _assembling_errors->include_error(invalid_instruction);
  }
}

void Assembler::Error5Verify(std::string code_line) {
  std::smatch matches;

  // Invalid any other character that follows the instruction
  std::regex INSTRUCTION_regex("(SECTION|SPACE|CONST|ADD|SUB|MULT|DIV|JMP|JMPN|JMPP|JMPZ|LOAD|STORE|INPUT|OUTPUT|COPY|STOP)(?!\\d|\\w)(.*)");

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

void Assembler::Error8Verify(std::string code_line) {
  // Express if instruction is with a correct
  // correspond operands amount 
  bool correct_operands_amount = true;
  std::smatch matches;
  std::regex INSTRUCTION_regex("(SPACE|CONST|ADD|SUB|MULT|DIV|JMP|JMPN|JMPP|JMPZ|LOAD|STORE|INPUT|OUTPUT|COPY|STOP)(?!\\d|\\w)(.*)");

  // Check if begins with a valid instruction
  bool instruction_exist = std::regex_search (code_line,
                                              matches,INSTRUCTION_regex);
  
  // Only analyse valid instruction
  if(instruction_exist) {
    std::string operand = matches[2].str() + matches[3].str();
    std::smatch test_match;

    // Don't notify directives at SECTION TEXT
    if(matches[1].compare("COPY") == 0 && 
      this->_section_identifier == TEXT){  
      // Verifies if it's a 2 operands operation
      std::regex copy_operand_regex("^(\\s*)(\\w+|\\d+)(,)(\\w+|\\d+)$");
      correct_operands_amount = std::regex_search (operand,
                                                test_match,copy_operand_regex);      
    }

    else if(matches[1].compare("STOP") == 0 && 
            this->_section_identifier == TEXT){      
      // Verifies if it's a 0 operands operation
      std::regex stop_operand_regex("^$");

      correct_operands_amount = std::regex_search (operand,
                                                test_match,stop_operand_regex);                                            
    } // if STOP

    // Don't notify instructions at SECTION DATA
    else if(matches[1].compare("CONST") == 0 &&
            this->_section_identifier == DATA) {        
      // Verifies if it's a 2 operands operation
      std::regex const_operand_regex("^(\\s)(\\w+)$");

      correct_operands_amount = std::regex_search (operand,
                                                test_match,const_operand_regex);  
    } // if CONST 

    // Don't notify instructions at SECTION DATA
    else if( matches[1].compare("SPACE") == 0 &&
            this->_section_identifier == DATA) {
      std::regex space_operand_regex("(^\\s\\w*$|^\\s\\w*\\n$|^\\s{0}\\n$)");

      correct_operands_amount = std::regex_search (operand,
                                                test_match,space_operand_regex);  
    } // if SPACE

    // Any other valid instruction at SECTION TEXT
    else if (matches[1].compare("CONST") != 0 &&
             matches[1].compare("SPACE") != 0 &&
            this->_section_identifier == TEXT){
      // Verifies if it's a 1 operand operation
      std::regex regular_instrucion_operand_regex("^(\\s)(\\w*|\\d*)(\\+*)(\\d*)$");

      correct_operands_amount = std::regex_search (operand,
                                                test_match,regular_instrucion_operand_regex);      
    }

    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////   


    if(correct_operands_amount == false){
      error invalid_instruction(code_line,
                            this->_current_line_number,
                            error::error_08);

      _assembling_errors->include_error(invalid_instruction);
    }

  } // if instruction_exist 
}

void Assembler::Error9Verify(std::string code_line) {
  bool correct_operands_types = true;
  std::smatch matches;
  std::regex INSTRUCTION_regex("(ADD|SUB|MULT|DIV|JMP|JMPN|JMPP|JMPZ|LOAD|STORE|INPUT|OUTPUT|COPY)(\\s)(.*)");

  // Check if begins with a valid instruction
  bool valid_instruction = std::regex_search (code_line,
                                              matches,INSTRUCTION_regex);

  if(valid_instruction){
    std::string instruction_operator = matches[1].str();
    std::string instruction_operand = matches[3].str();

    if(instruction_operator.compare("COPY") == 0 &&
      this->_section_identifier == TEXT){

      std::regex copy_regex("((^\\d+)(\\D+$)|\\D+)");


      // Characters that separate tokens
      std::regex taps("\\s|,");
      regex_token_iterator<string::iterator> itr(instruction_operand.begin(), instruction_operand.end(), taps, -1);
      regex_token_iterator<string::iterator> end;

      std::string token;
      // Cycles through all tokens identified on the line.
      while (itr != end){
        token = *itr;
        if(token != ""){
          // Checks if it is a valid operand type(not a number types)
          if(!std::regex_match(token, matches, copy_regex)){
            correct_operands_types = false;
          }
        }
        itr++;
      }
    }


    else if(instruction_operator.compare("STOP") != 0 &&
            this->_section_identifier == TEXT){
              
      std::regex operand_type_regex("((^\\d+)(\\D+$)|\\D+)");
      // Characters that separate tokens
      std::regex taps("\\s");
      regex_token_iterator<string::iterator> itr(instruction_operand.begin(), instruction_operand.end(), taps, -1);
      regex_token_iterator<string::iterator> end;

      std::string token;
      // Cycles through all tokens identified on the line.
      while (itr != end){
        token = *itr;
        if(token != ""){
          // Checks if it is a valid operand type(not a number types)
          if(!std::regex_match(token, matches, operand_type_regex)){
            correct_operands_types = false;
          }
        }
        itr++;
      }

    }


    //////////////////////////////////////////////
    //**   ERROR CASE ----------------------------
    //////////////////////////////////////////////   


    if(correct_operands_types == false){
      error invalid_instruction(code_line,
                                this->_current_line_number,
                                error::error_09);

      _assembling_errors->include_error(invalid_instruction);
    }
  } // if valid instruction                                              
}

void Assembler::Error14Verify(std::string code_line){
  bool correct_argument = true;
  std::smatch matches;
  std::regex directive_regex("(CONST|SPACE)(\\s)(.*)");

  // Check if begins with a valid directive 
  bool valid_directive = std::regex_search (code_line,
                                              matches,directive_regex);

  // Case 1: Directive error case
  if(valid_directive && this->_section_identifier == DATA
    && matches[3].compare("") != 0){

    std::string directive_operator = matches[1].str();
    std::string directive_operand = matches[3].str();

    std::regex operand_regex("(^[0-9])(\\d*)");
    // Characters that separate tokens
    std::regex taps("\\s");
    regex_token_iterator<string::iterator> itr(directive_operand.begin(), directive_operand.end(), taps, -1);
    regex_token_iterator<string::iterator> end;

    std::string token;
    // Cycles through all tokens identified on the line.
    while (itr != end){
      token = *itr;
      if(token != ""){
        // Checks if it is a valid operand type(not a number types)
        if(!std::regex_match(token, matches, operand_regex)){
          correct_argument = false;
        }
      }
      itr++;
    }
  }

  // Case 2: Label+number error

  std::regex label_offset_regex("(\\w+)(\\s)(\\w+)(\\+)(\\w+)");

  // Check if begins with a valid directive 
  bool label_offset_case = std::regex_search (code_line,
                                              matches,label_offset_regex);

  if(label_offset_case){
    std::string offset = matches[5].str();

    std::regex label_offset_regex("(\\d+)");

    // Check if begins with a valid directive 
    correct_argument = std::regex_search (offset,
                                               matches,label_offset_regex);
  }                                              


  //////////////////////////////////////////////
  //**   ERROR CASE ----------------------------
  //////////////////////////////////////////////   


  if(correct_argument == false){
    error invalid_instruction(code_line,
                              this->_current_line_number,
                              error::error_14);

    _assembling_errors->include_error(invalid_instruction);
  }

}

void Assembler::ModifyAdressLabelVerify(std::string code_line){
  bool error_occurred = false;
  std::smatch matches;
  std::regex INSTRUCTION_regex("(STORE|INPUT|COPY)(\\s)(.*)");

  std::regex_search (code_line, matches, INSTRUCTION_regex);

  std::string instruction_operator  = matches[1].str();
  std::string instruction_operand  =  matches[3].str();

  

  if((instruction_operator.compare("STORE") == 0 ||
     instruction_operator.compare("INPUT") == 0) &&
     this->_section_identifier == TEXT){
      std::list<std::string>::iterator it = std::find (this->_address_labels.begin(),
                                                         this->_address_labels.end(), 
                                                         instruction_operand);

      // This label is a address_label.
      // Invalid modify operation.
      if(it != this->_address_labels.end()){
        error_occurred = true;
      }

  }

  else if(instruction_operator.compare("COPY") == 0 && this->_section_identifier == TEXT){
    std::regex INSTRUCTION_regex("(COPY)(\\s)([a-z]|[A-Z]|_)(\\w*|\\d*)(\\+*)(\\d*)(,)([a-z]|[A-Z]|_)(\\w*|\\d*)(\\+*)(\\d*)");
    bool regular_copy = std::regex_search (code_line, matches, INSTRUCTION_regex);
    
    if(regular_copy){
      std::string destiny_operator = matches[8].str() + matches[9].str();

      std::list<std::string>::iterator it = std::find (this->_address_labels.begin(),
                                                         this->_address_labels.end(), 
                                                         destiny_operator);

      // This label is a address_label.
      // Invalid modify operation.
      if(it != this->_address_labels.end()){
        error_occurred = true;
      }
    }
  }
  
  //////////////////////////////////////////////
  //**   ERROR CASE ----------------------------
  //////////////////////////////////////////////   


  if(error_occurred){
    error invalid_instruction(code_line,
                              this->_current_line_number,
                              error::error_14);

    _assembling_errors->include_error(invalid_instruction);
  }


}

void Assembler::Error15Verify(std::string label){
  // Identify the declared label
  std::smatch label_match;
  std::regex label_regex("([a-z]|[A-Z]|_)(\\w*|\\d*)(:)(\\s)(CONST)(\\s)(\\d+)");
  bool CONST_label =  std::regex_search (this->_current_line_string,
                      label_match,label_regex);

  

  // Don't analyse when is not a CONST
  // label
  if (!CONST_label){
    return;
  }

  for(std::vector<label_occurrence>::iterator it = this->_label_occurrences.begin(); 
      it < this->_label_occurrences.end(); ++it)
  {
    // Verifies code operations that uses
    // the defined label and at SECTION TEXT
    if(it->get_label().compare(label) == 0){

      // Line where the reference occurs
      std::string label_reference_line = it->get_code_line();
      int label_reference_line_number = it->get_line_number();
      
      /* Debug
      cout << label_reference_line << endl;
      */

      // Analyse operation at CONST labels
      std::smatch modify_const_match;
      
      std::regex copy_regex("(COPY)(\\s)([a-z]|[A-Z]|_)(\\w*|\\d*)(\\+*)(\\d*)(,)([a-z]|[A-Z]|_)(\\w*|\\d*)(\\+*)(\\d*)");
      std::regex store_regex("(STORE)(\\s)(\\w+)");
      std::regex input_regex("(INPUT)(\\s)(\\w+)");

      bool store_command = std::regex_search (label_reference_line,
                          modify_const_match,store_regex);

      bool input_command = std::regex_search (label_reference_line,
                          modify_const_match,input_regex);

      bool copy_command = std::regex_search (label_reference_line,
                      modify_const_match,copy_regex);

      std::string destiny_operand = modify_const_match[8].str()+modify_const_match[9].str();

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
        // ERROR - Out-of-range label access
        error const_modify_error(label_reference_line,
                              label_reference_line_number,
                              error::error_15);

        _assembling_errors->include_error(const_modify_error);
      }          
    } //if(it->get_label.compare(label) == 0)
  }//for
}

void Assembler::InvalidInstructionWrite(std::string code_line){
  std::smatch matches;
  std::regex size_2_regex("(CONST|SPACES|ADD|SUB|MULT|DIV|JMP|JMPN|JMPP|JMPZ|LOAD|STORE|INPUT|OUTPUT)(\\s)(.*)");

  // Check if begins with a valid instruction
  bool size_2_operation = std::regex_search (code_line,
                                              matches,size_2_regex);

  std::regex size_3_regex("(COPY)(\\s)(.*)");

  // Check if begins with a valid instruction
  bool size_3_operation = std::regex_search (code_line,
                                              matches,size_3_regex);

  
  std::regex size_1_regex("(STOP)(\\s)(.*)");

  // Check if begins with a valid instruction
  bool size_1_operation = std::regex_search (code_line,
                                              matches,size_1_regex);
                                                                                            
  if(size_2_operation){
    this->_object_file.insert(this->_object_file.begin(),"99");
    this->_object_file.insert(this->_object_file.begin(),"99");
  }

  else if(size_3_operation){
    this->_object_file.insert(this->_object_file.begin(),"99");
    this->_object_file.insert(this->_object_file.begin(),"99");
    this->_object_file.insert(this->_object_file.begin(),"99");
  }

  else if(size_1_operation){
    this->_object_file.insert(this->_object_file.begin(),"99");
  }

  else {
    this->_object_file.insert(this->_object_file.begin(),"99");
    this->_object_file.insert(this->_object_file.begin(),"99");
  }                                                                                    
}
