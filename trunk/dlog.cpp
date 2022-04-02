#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <map>

#include "application.h"

//
// Logging
//

class Log{
  double last_time = 0;
  std::vector<std::string> messages;

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