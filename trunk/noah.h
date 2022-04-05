#pragma once

#include <string>
#include <vector>

#include <iostream>
#include <fstream>


#include "mtypedefs.h"

class Noah{
public:
  static Noah& the() {
    static Noah* my_noah = nullptr;
    if (!my_noah) {
      my_noah = new Noah;
    }
    return *my_noah;
  };

  std::vector<std::string> m_words;

  Noah() {
    std::cout << "\nConstructing Noah's Dictionary of the English Language ...\n";

    const std::string dictionary_filepath = 
      "resources/noah-webster/noah_webster_1913_dictionary_uniqueWords_cleanedUp.txt";
    
    std::ifstream stream(dictionary_filepath);
    
    if (!stream.is_open()) {
      std::cout << "dictionary not found!" << "\n\t" << dictionary_filepath << "\n" << "\n";
    }

    // TODO how much memory does this use? (it's at least 2 megabytes)

    std::string line;
    while (getline(stream, line)) {
      m_words.push_back(line);
    }
  }

  // convert u64 to a series of english words

  std::string wordify(u64 number) {
    u64 remainder = number;
    std::string output = "";

    if (m_words.size() < 2) {
      std::cout << "can't wordify without a dictionary!\n";
      return "";
    }

    while (remainder > m_words.size()) {
      output += m_words[remainder % m_words.size()] + " ";
      remainder /= m_words.size();
    }

    output += m_words[remainder % m_words.size()];

    return output;
  };
};
