#pragma once

#include <iostream>

#include "mtypedefs.h"

namespace m{

template <typename T>
class Array{
public:
  T* m_data;
  u64 m_size;
  u64 m_length;

  Array(): m_length(0), m_size(0) {};
  Array(int start_size): m_length(0) {
    m_data = new T[start_size];
    size = start_size;
  };

  void push_back(T element) {
    if (m_length < size) {
      m_data[m_length] = element;
      length++;
    } else {
      std::cout << "Error! m::array() not enough space! push_back() when length: " << m_length << "\tsize: " << m_size << "\n";
      // TODO crash application
    }
  }

  u64 length() { return length; }
};

}