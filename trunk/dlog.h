#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>

#include "application.h"

//
// Logging
//

#define LOG(n) Log::the().push(std::to_string(n));

class Print{
public:
  static void heading(std::string h) {
    std::cout << "\n\n#\n# " << h << " \n#\n\n";
  }
  static void heading2(std::string h) {
    std::cout << "\n\n# " << h << " \n";
  }
};

class Log{
public:
  static Log& the() {
    static Log* my_log = nullptr;
    if (!my_log) {
      my_log = new Log;
    }
    return *my_log;
  };
  double last_time = 0;
  std::vector<std::string> messages;
  std::vector<std::string> unique_messages;

  void log_once(std::string message) {
    if (std::binary_search(unique_messages.begin(), unique_messages.end(), message)) {
      return;
    }
    unique_messages.push_back(message);
    std::sort(unique_messages.begin(), unique_messages.end());
    push(message);
  }

  void lognow(std::string message) {
    std::cout << std::fixed << std::setprecision(6) << message << "\n";
  };
  void poplog() {
    lognow(messages.back());
    messages.pop_back();
  };
  void tick() {
    if (messages.size() == 0) {
      return;
    }
    auto time = Application::the().time;
    if (time > last_time + 0.1f) {
      poplog();
      last_time = time;
    }
  }
  void push(std::string message) {
    messages.push_back(message);
  }
  void push_unique(std::string message) {
    for (int i = 0; i < messages.size(); i++) {
      if (messages[i] == message) {
        return;
      }
    }
    messages.push_back(message);
  }
  template<class template_key, class template_value>
  void push_map(std::string prefix, std::map<template_key, template_value> m) {
    std::string temp = prefix + " {\n";
    
    for (auto const& [key, val] : m) {
      temp += " \t" + std::to_string(key) + ": \t" + std::to_string(val) + ", \n";
    }

    temp += "}\n";

    push(temp);
  }
}; // short for "debug log"
