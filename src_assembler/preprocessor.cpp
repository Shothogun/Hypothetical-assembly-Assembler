#include "../include_assembler/preprocessor.hpp"

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

void Preprocessor::Preprocessing(){

  // Iterator from the vector. Reference to a string value
  std::vector<std::string>::iterator source_code_line;
  // Array composed by the matches found in the regex search
  std::smatch matches;

  // Boolean value to verify directive occurence
  int matched_EQU, matched_IF, matched_empty_line;

  // Label value condition to the IF directive
  int label_value_IF_condition;

  // Elements from Directives command
  std::string label;
  int label_value;

  // Next source code line reference, used for 
  // erase next line in IF directives condition true
  std::vector<std::string>::iterator next_source_code_line;

  // Source code_line without unnecessary characters
  std::string clean_source_code_line;

  // Iterates each line from source code(not processed yet)
  for (source_code_line = this->_preprocessed_file.begin();
       source_code_line < this->_preprocessed_file.end();
       source_code_line++){

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
    //**   Comments erasing   --------   
    ///////////////////////////////////

    ///////////////////////////////////
    //**   Ajust COPY operators -------
    ///////////////////////////////////

    ///////////////////////////////////
    //**   Directives execution -------      
    ///////////////////////////////////

    //**   EQU execution     

    std::regex EQU_expression_regex("(^[a-z]|[A-Z]|_)(\\w+|\\d+)(:)\\s+(EQU)\\s+(\\d+)");

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

      this->_equ_values.insert(std::pair<std::string,int>(label, label_value));

      /* Debug
      std::cout <<  this->_equ_values["TRIANGULO"] << std::endl;
      */

      /* Debug
      for (auto sub_match:matches) std::cout << sub_match << std::endl;
      */
     continue;
    } // if
    
    //**   IF execution      
    std::regex IF_expression_regex("(IF)(\\s)([a-z]|[A-Z]|_)(\\w+|\\d+)");

    // Seeks the EQU directive match
    matched_IF = std::regex_search (*source_code_line,matches,IF_expression_regex);

    // If a IF directive is found
    if(matched_IF){
      // Matches pattern at the directive EQU:
      // 0: EQU directive match
      // 1: 'IF'
      // 2: ' '
      // 3: Head from the label name
      // 4: Tail from the label name

      label = matches[3].str() + matches[4].str();
      
      label_value_IF_condition = this->_equ_values[label];

      /* Debug
      std::cout <<  label << std::endl;
      */


      /* Debug
      std::cout << *source_code_line << std::endl;
      std::cout << label_value_IF_condition << std::endl;
      */

      // Exclude IF directive line
      this->_preprocessed_file.erase(source_code_line);

      // If label value is 0, preprocess should erase next line
      if(!label_value_IF_condition){
        this->_preprocessed_file.erase(source_code_line);
      }

      // After exclude, we should go back to the previous line,
      // to not pass by the next lines
      source_code_line--;
      continue;

      /* Debug
      for (auto sub_match:matches) std::cout << sub_match << std::endl;
      */
    } // if
  } // for

  // Degug
  std::vector<std::string>::iterator it;
  for (it=this->_preprocessed_file.begin(); it < this->_preprocessed_file.end(); ++it){
  	std::cout << *it;
  }
  //

}