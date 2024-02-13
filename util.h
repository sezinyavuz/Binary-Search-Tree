#pragma once
#include <algorithm>
#include <sstream>
#define error_exit(fmt, ...) do { fprintf(stderr, "%s error: " fmt, __func__, ##__VA_ARGS__); exit(1); } while(0);

// Function to compare two strings lexicographically
int compareLexicographically(std::string str1, std::string str2) {
  // Convert both strings to lowercase
  std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
  std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);

  // Compare the strings using the STL function std::lexicographical_compare
  if (std::lexicographical_compare(str1.begin(), str1.end(), str2.begin(), str2.end())) {
    return -1; // str1 comes before str2 in lexicographic order
  } else if (std::lexicographical_compare(str2.begin(), str2.end(), str1.begin(), str1.end())) {
    return 1; // str1 comes after str2 in lexicographic order
  } else {
    return 0; // str1 and str2 are equal
  }
}
int max(int a, int b)
{
    return (a > b)? a : b;
}
// take from https://stackoverflow.com/a/68117217
std::vector<std::string> StringToVector(std::string theString, 
        char separator){
 
    // Create a vector
    std::vector<std::string> vecsWords;
    
    // A stringstream object receives strings separated
    // by a space and then spits them out 1 by 1
    std::stringstream ss(theString);
    
    // Will temporarily hold each word in the string
    std::string sIndivStr;
    
    // While there are more words to extract keep
    // executing
    // getline takes strings from a stream of words stored
    // in the stream and each time it finds a blanks space
    // it stores the word proceeding the space in sIndivStr
    while(getline(ss, sIndivStr, separator)){
        
        // Put the string into a vector
        vecsWords.push_back(sIndivStr);
    }
    
    return vecsWords;
}