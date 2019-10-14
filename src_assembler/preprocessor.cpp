#include "../include_assembler/preprocessor.hpp"
#include <string>

///////////////////////////////////
//**   Constructor        
//////////////////////////////////
Preprocessor::Preprocessor(char* source_code_name){
  char asm_line_code [300];
  std::vector<std::string>::iterator it;
  it = this->_preprocessed_file.begin();

  // Source codes are locates in the directory /sources/asm_codes/
  // outside current /exec_assembler/ directory(where the program is executed)
  std::string string_source_code_name = "../src_assembler/asm_codes/";

  // Source code name input at the terminal execution
  // concatenated with directory
  string_source_code_name += source_code_name;

  // Convert source_code_name string variable to char type
  char char_source_code_name[string_source_code_name.size() + 1];
  strcpy(char_source_code_name, string_source_code_name.c_str());

  // Open the chosen source code file
  FILE* source_code = fopen(char_source_code_name, "r");
  if (source_code == NULL) perror ("Error opening file");
  else {
    // Indicates that the .asm file really exist,
    // for further .pre file creation
    this->_exists = true;

    // Until reach end of file
    while (!feof (source_code) ) {
      // Gets 100 characters from the source code line
      if ( fgets (asm_line_code , 100 , source_code) == NULL ) break;
      
      // Fills the preprocessed_file vector with the asm source code 
      it = this->_preprocessed_file.insert(it, asm_line_code);

    }
    fclose (source_code);
   }

  // Corrects the source code lines order(inserted in the reversed order)
  std::reverse(this->_preprocessed_file.begin(),
               this->_preprocessed_file.end());
}

///////////////////////////////////
//**   Preprocessing        
//////////////////////////////////
void Preprocessor::Preprocessing(){

  // Iterator from the vector. Reference to a string value
  std::vector<std::string>::iterator source_code_line;
  // Array composed by the matches found in the regex search
  std::smatch matches;

  // Boolean value to verify directive occurence
  int matched_EQU, matched_IF, matched_empty_line, matched_label;

  // Label value condition to the IF directive
  int label_value_IF_condition;

  // Line number in file;
  int line_counter;

  // Elements from Directives command
  std::string label;
  int label_value;

  // Next source code line reference, used for 
  // erase next line in IF directives condition true
  std::vector<std::string>::iterator next_source_code_line;

  // Source code_line without unnecessary characters
  std::string clean_source_code_line;

  // Iterates each line from source code(not processed yet)
  for (source_code_line = this->_preprocessed_file.begin(), line_counter=1;
       source_code_line < this->_preprocessed_file.end();
       source_code_line++, line_counter++){

    //////////////////////////////////
    //**   Unsesitive case filter ---
    //////////////////////////////////  
    
    std::for_each(source_code_line->begin(), source_code_line->end(), [](char & c) {
		  c = ::toupper(c);
	  });

    ///////////////////////////////////
    //**   Exclude garbage characters
    ///////////////////////////////////

    //**   Exclude empty lines
    std::regex empty_line_regex("(^\\n$)|(^\\s)(\\s+)(\\n)");
    matched_empty_line = std::regex_search (*source_code_line,matches,empty_line_regex);

    if(matched_empty_line){
      this->_preprocessed_file.erase(source_code_line);

      // After exclude, we should go back to the previous line,
      // to not pass by the next lines
      source_code_line--;
      continue;
    }

    //**   Exclude spaces between tokens
    clean_source_code_line = "";
    std::regex tokens_regex("(\\S+)");
    std::regex_search (*source_code_line,matches,tokens_regex);
    std::string s = *source_code_line;

    // Get each token from the line
    while (std::regex_search (s, matches,tokens_regex)) {
      // Concatenates each token with space
      clean_source_code_line = clean_source_code_line + matches.str() + " ";

      // Get the next subarray excluding processed tokens
      s = matches.suffix().str();
    }

    // Erases last space and add a new line
    clean_source_code_line.erase(clean_source_code_line.end()-1);
    clean_source_code_line += '\n';
    *source_code_line = clean_source_code_line;

    ///////////////////////////////////
    //**   Only Comments line erasing -
    ///////////////////////////////////

    std::regex only_comments_regex("(^\\s*)(;[^].*)");  

    if(std::regex_search (*source_code_line,matches,only_comments_regex)){
      
      // Exclude only Comments line
      this->_preprocessed_file.erase(source_code_line);

      // After exclude, we should go back to the previous line,
      // to not pass by the next lines
      source_code_line--;

      continue;
    }

    ///////////////////////////////////
    //**   Comments erasing   --------   
    ///////////////////////////////////

    std::regex comments_regex("(\\s*)(;[^].*)");  

    // Gets the comment and erases it(replace by nothing)
    *source_code_line = std::regex_replace (*source_code_line,comments_regex,"");

    ///////////////////////////////////
    //**   Ajust Label+Number pattern
    ///////////////////////////////////
    std::regex sum_label_regex("(^[a-z]|[A-Z]|_)(\\w*|\\d*)(\\s*)(\\+)(\\s*)(\\d+)");  

    // Get the sum between label and number offset and dispose them
    // without space
    *source_code_line = std::regex_replace (*source_code_line,sum_label_regex,"$1$2$4$6");

    ///////////////////////////////////
    //**   Ajust COPY operators -------
    ///////////////////////////////////
    // Matches pattern at instruction COPY:
    // 0: Instruction COPY match
    // 1: COPY
    // 2: space character
    // 3: Label
    // 4: Plus character
    // 5: A digit
    // 6: space character
    // 7: comma character
    // 8: space character
    // 9: Label
    // 10: Plus character
    // 11: A digit

    std::regex COPY_regex("(COPY)(\\s)(\\w+)(\\+*)(\\d*)(\\s)(,)(\\s)(\\w+)(\\+*)(\\d*)");  

    // Get the COPY operator and its operands to format 
    // to the default pattern(COPY operand_1,operand_2)
    *source_code_line = std::regex_replace (*source_code_line,COPY_regex,"$1 $3$4$5,$9$10$11");

    ///////////////////////////////////
    //**   Directives execution -------      
    ///////////////////////////////////

    // Check for invalid token in preprocessing directives
    std::regex IF_EQU_expression("(.*)(EQU|IF)(.*)");
    std::regex valid_token("(^\\d+$)|(^([a-z]|[A-Z]|_)(\\w+|\\d+)*$)");
    std::regex taps("\\s+|,|:");
    regex_token_iterator<string::iterator> itr(source_code_line->begin(), source_code_line->end(), taps, -1);
    regex_token_iterator<string::iterator> end;
    string strn;
    bool invalid_token = false;
    std::regex label_expression_regex("(^[a-z]|[A-Z]|_)(\\w+|\\d+)(:)(\\s*)(\\n)");

    if(std::regex_search(*source_code_line, matches, IF_EQU_expression)){
      while (itr != end){
        if((*itr!= "EQU") && (*itr!="IF") && (*itr!="")){
          strn = *itr;
          if(!std::regex_match(strn, matches, valid_token)){
            
            error error(*source_code_line, line_counter, error::error_10);
            _preprocessing_errors.include_error(error);
            if(source_code_line == _preprocessed_file.begin()){
              matched_label = 0;
            }
            else{
              matched_label = std::regex_search(*(source_code_line-1), matches, label_expression_regex);
            }
            invalid_token = true;
            break;
          }
        }
        itr++;
      }
      if(invalid_token){
        if(matched_label){
          this->_preprocessed_file.erase(source_code_line-1);
          source_code_line--;
        }
        this->_preprocessed_file.erase(source_code_line);
        source_code_line--;
        continue;
      }
    }

    // Check argument type in EQU 

    std::regex EQU("(.*)(EQU)(.*)");
    std::regex regex_number("^\\s*\\d+\\s*\\n*$");
    std::regex regex_label("^\\s*([a-z]|[A-Z]|_)(\\w+|\\d+)(:)?\\s*\\n*$");
    std::string number;
    matched_EQU = std::regex_search (*source_code_line,matches,EQU);
    number = matches[3].str();
    if(matched_EQU && !std::regex_match(number, matches , regex_number)){
      
      error error(*source_code_line, line_counter, error::error_14);
      _preprocessing_errors.include_error(error);
      
      if(source_code_line == _preprocessed_file.begin()){
        matched_label = 0;
      }
      else{
        matched_label = std::regex_search(*(source_code_line-1), matches, label_expression_regex);
      }
      if(matched_label){
        this->_preprocessed_file.erase(source_code_line-1);
        source_code_line--;
      }
      this->_preprocessed_file.erase(source_code_line);
      source_code_line--;
      continue;
    }

    // Check argument type in IF
    std::regex IF("(.*)(IF)(.*)");
    matched_IF = std::regex_search (*source_code_line,matches,IF);
    label = matches[3].str();
    if(matched_IF && !std::regex_match(label, matches , regex_label)){
      
      error error(*source_code_line, line_counter, error::error_14);
      _preprocessing_errors.include_error(error);
      if(source_code_line == _preprocessed_file.begin()){
        matched_label = 0;
      }
      else{
        matched_label = std::regex_search(*(source_code_line-1), matches, label_expression_regex);
      }
      if(matched_label){
        this->_preprocessed_file.erase(source_code_line-1);
        source_code_line--;
      }      
      this->_preprocessed_file.erase(source_code_line);
      source_code_line--;
      continue;      
    }
    //**   EQU execution     

<<<<<<< HEAD
    //CASE 1: EQU with two label on the same line

    std::regex EQU_two_labels("(((^[a-z]|[A-Z]|_)(\\w+|\\d+)(:)\\s*){2,})\\s+(EQU)\\s+(\\d+)");

    matched_EQU = std::regex_search (*source_code_line,matches,EQU_two_labels);
    if(matched_EQU){
      // Matches pattern at the directive EQU:
      // 0: EQU directive match
      // 1: Labels found
      // 2: Last label
      // 3: Head character from de last label
      // 4: Tail from the last label
      // 5: Character ':'
      // 6: 'EQU'
      // 7: A Number

      label = matches[3].str() + matches[4].str();
      label_value = stoi(matches[7]);
      
      error error(*source_code_line, line_counter, error::error_11);
      _preprocessing_errors.include_error(error);

      this->_equ_values.insert(std::pair<std::string,int>(label, label_value)); 

      // Exclude EQU directive line
      this->_preprocessed_file.erase(source_code_line);
      
      source_code_line--;

      continue;
    }

    //CASE 2: EQU with one label on the same line or two, 
    //one on the same line as the EQU and the other on the previous line

    std::regex EQU_expression_regex("(^[a-z]|[A-Z]|_)(\\w+|\\d+)(:)\\s+(EQU)\\s+(\\d+)");
    
=======
    std::regex EQU_expression_regex("(^[a-z]|[A-Z]|_)(\\w*|\\d*)(:)\\s+(EQU)\\s+(\\d+)");
>>>>>>> 36adf5d538d79b879d4e342885199a2b400ef8ec

    // Seeks the EQU directive match
    matched_EQU = std::regex_search (*source_code_line,matches,EQU_expression_regex);

    // If a EQU directive is found
    if(matched_EQU){
      // Matches pattern at the directive EQU:
      // 0: EQU directive match
      // 1: Head character from the label
      // 2: Tail from the label
      // 3: Character ':'
      // 4: 'EQU'
      // 5: A Number
  
      label = matches[1].str() + matches[2].str();
      label_value = stoi(matches[5]);
      

      // Search the label on the previous line
      if(source_code_line == _preprocessed_file.begin()){
        matched_label = 0;
      }
      else{
        matched_label = std::regex_search(*(source_code_line-1), matches, label_expression_regex);
      }
      // Matches pattern at the label:
      // 0: label match
      // 1: Head character from the label
      // 2: Tail from the label
      
      // If the previous line contained one label, notify two labels error
      // and only considers the last label
      if(matched_label){
        error error(*source_code_line, line_counter, error::error_11);
       _preprocessing_errors.include_error(error);
      }

      // If the label has already been declared, do not change the value and notify the error
      if(this->_equ_values.count(label) > 0){
        error error(*source_code_line, line_counter, error::error_01);
        _preprocessing_errors.include_error(error);
      }
      // Otherwise, add label declaration
      else{
        this->_equ_values.insert(std::pair<std::string,int>(label, label_value));        
      }

      if(matched_label){
        this->_preprocessed_file.erase(source_code_line-1);
        source_code_line--;
      }
      // Exclude EQU directive line
      this->_preprocessed_file.erase(source_code_line);

      // After exclude, we should go back to the previous line,
      // to not pass by the next lines
      source_code_line--;

      continue;

      /* Debug
      std::cout <<  this->_equ_values["TRIANGULO"] << std::endl;
      */

      /* Debug
      for (auto sub_match:matches) std::cout << sub_match << std::endl;
      */
    } // if1
    

    //CASE 3: EQU with label on the previous line or whidout label

    std::regex EQU_without_label("(EQU)\\s+(\\d+)(\\s*)(\\n)");
    
    // Seeks the EQU directive match
    matched_EQU = std::regex_search (*source_code_line,matches,EQU_without_label);

    if(matched_EQU){
      // Matches pattern at the directive EQU:
      // 0: EQU directive match
      // 1: 'EQU'
      // 2: A Number

      label_value = stoi(matches[2]);
      
      // Search the label on the previous line
      if(source_code_line == _preprocessed_file.begin()){
        matched_label = 0;
      }
      else{
        matched_label = std::regex_search(*(source_code_line-1), matches, label_expression_regex);
      }
      // Matches pattern at the label:
      // 0: label match
      // 1: Head character from the label
      // 2: Tail from the label


      if(matched_label){
        label = matches[1].str() + matches[2].str();
        // Exclude EQU label line
        this->_preprocessed_file.erase(source_code_line-1);
        this->_equ_values.insert(std::pair<std::string,int>(label, label_value)); 
        source_code_line--;
      }
      
      // Exclude EQU directive line
      this->_preprocessed_file.erase(source_code_line);
      source_code_line--;
      
      continue;

    }
    
    //**   IF execution      
    std::regex IF_expression_regex("(IF)(\\s)([a-z]|[A-Z]|_)(\\w*|\\d*)");

    // Seeks the EQU directive match
    matched_IF = std::regex_search (*source_code_line,matches,IF_expression_regex);

    // If a IF directive is found
    if(matched_IF){
      // Matches pattern at the directive EQU:
      // 0: IF directive match
      // 1: 'IF'
      // 2: ' '
      // 3: Head from the label name
      // 4: Tail from the label name

      label = matches[3].str() + matches[4].str();
      

      // If label is found, check IF condition
      if(this->_equ_values.count(label) > 0){
        label_value_IF_condition = this->_equ_values[label];
      }
      // Otherwise, consider the IF condition false, and notify the error
      else{
        label_value_IF_condition = 1;
        error error(*source_code_line, line_counter, error::error_00);
        _preprocessing_errors.include_error(error);
      }

      /* Debug
      std::cout <<  label << std::endl;
      */

      /* Debug
      std::cout << *source_code_line << std::endl;
      std::cout << label_value_IF_condition << std::endl;
      */

      // If label value is 0, preprocess should erase next line
      if(!label_value_IF_condition){
        this->_preprocessed_file.erase(source_code_line);
        source_code_line--;
      }

      // Exclude IF directive line
      this->_preprocessed_file.erase(source_code_line);
      
      // After exclude, we should go back to the previous line,
      // to not pass by the next lines
      source_code_line--;
      continue;

      /* Debug
      for (auto sub_match:matches) std::cout << sub_match << std::endl;
      */
    } // if
  } // for

  /* Degug
  std::vector<std::string>::iterator it;
  for (it=this->_preprocessed_file.begin(); it < this->_preprocessed_file.end(); ++it){
  	std::cout << *it;
  }
  */

  // Display all errors found
  _preprocessing_errors.display(error_log::DETAILED);

}

///////////////////////////////////
//**   MakePreFile        
//////////////////////////////////
void Preprocessor::MakePreFile(char* source_code_name){

  // If the .asm file doesn't exists, preprocessor shall not 
  // produce the .pre file
  if(!this->_exists) return;

  // The .pre file will be created in the directory
  // exec_assembler/pre_files/
  std::string string_source_code_name = "./pre_files/";

  // Source code name input at the terminal execution
  // concatenated with directory
  string_source_code_name += source_code_name; 

  // Gets the original file name and swap with .pre extention
  std::regex name_regex("(.*)(.asm)");  

  // Replace .asm extention for .pre
   string_source_code_name = std::regex_replace (string_source_code_name,name_regex,"$1.pre");

  // Convert source_code_name string variable to char type
  char char_source_code_name[string_source_code_name.size() + 1];
  strcpy(char_source_code_name, string_source_code_name.c_str());

  // Produce the .pre file with the _preprocessed_file vector
  FILE* pre_file = fopen(char_source_code_name, "w+");
  if (pre_file == NULL) perror ("Error opening file");
  std::vector<std::string>::iterator pre_file_line;
  for(pre_file_line = this->_preprocessed_file.begin();
      pre_file_line < this->_preprocessed_file.end();
      pre_file_line++) {
    
    fprintf (pre_file, "%s", pre_file_line->c_str() );
  }
  fclose(pre_file);

}