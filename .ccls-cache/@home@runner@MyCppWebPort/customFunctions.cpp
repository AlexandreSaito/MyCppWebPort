#include "customFunctions.hpp"

std::vector<std::string> cstf::splitString(std::string content, std::string splitter) {
  std::vector<std::string> list;
  int splitterLength = splitter.length();
  int index = content.find(splitter);
  int startLine = 0;
  if (index == -1) {
    list.push_back(content);
    return list;
  }
  while (index != -1) {
    list.push_back(content.substr(startLine, index - startLine));
    startLine = index + splitterLength;
    index = content.find(splitter, startLine);
  }
  list.push_back(content.substr(startLine, content.length() - startLine));
  return list;
}

bool cstf::string_contains(std::string content, std::string key){
	return content.find(key) != std::string::npos;
}

std::string cstf::replace(std::string content, std::string tag, std::string data){
	size_t start_pos = 0;
	while((start_pos = content.find(tag, start_pos)) != std::string::npos) {
			content.replace(start_pos, tag.length(), data);
			start_pos += tag.length(); // Handles case where 'to' is a substring of 'from'
	}
	return content;
}

std::string cstf::readFile(std::string filePath){
	std::ifstream file;
  std::cout << "Reading file: " + filePath << std::endl;
  file.open(filePath); // open the input file

  std::stringstream strStream;
  strStream << file.rdbuf(); // read the file
  return strStream.str();// str holds the content of the file
}

//https://www.geeksforgeeks.org/encode-ascii-string-base-64-format/
char* cstf::base64Encoder(char input_str[], int len_str)
{
    // Character set of base64 encoding scheme
    char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
      
    // Resultant string
    char *res_str = (char *) malloc(SIZE * sizeof(char));
      
    int index, no_of_bits = 0, padding = 0, val = 0, count = 0, temp;
    int i, j, k = 0;
      
    // Loop takes 3 characters at a time from
    // input_str and stores it in val
    for (i = 0; i < len_str; i += 3)
        {
            val = 0, count = 0, no_of_bits = 0;
  
            for (j = i; j < len_str && j <= i + 2; j++)
            {
                // binary data of input_str is stored in val
                val = val << 8;
                  
                // (A + 0 = A) stores character in val
                val = val | input_str[j];
                  
                // calculates how many time loop
                // ran if "MEN" -> 3 otherwise "ON" -> 2
                count++;
              
            }
  
            no_of_bits = count * 8;
  
            // calculates how many "=" to append after res_str.
            padding = no_of_bits % 3;
  
            // extracts all bits from val (6 at a time)
            // and find the value of each block
            while (no_of_bits != 0)
            {
                // retrieve the value of each block
                if (no_of_bits >= 6)
                {
                    temp = no_of_bits - 6;
                      
                    // binary of 63 is (111111) f
                    index = (val >> temp) & 63;
                    no_of_bits -= 6;        
                }
                else
                {
                    temp = 6 - no_of_bits;
                      
                    // append zeros to right if bits are less than 6
                    index = (val << temp) & 63;
                    no_of_bits = 0;
                }
                res_str[k++] = char_set[index];
            }
    }
  
    // padding is done here
    for (i = 1; i <= padding; i++)
    {
        res_str[k++] = '=';
    }
  
    res_str[k] = '\0';
  
    return res_str;
}
/*json to_json(const T){
	
}

T from_json(const json){
	
}*/