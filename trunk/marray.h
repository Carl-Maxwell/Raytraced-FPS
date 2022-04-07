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
    m_size = start_size;
  };
  ~Array() {
    delete[] m_data;
  }

  void push_back(T element) {
    if (m_length < m_size) {
      m_data[m_length] = element;
      m_length++;
    } else {
      std::cout << "Error! m::array() not enough space! push_back() when length: " << m_length << "\tsize: " << m_size << "\n";
      // TODO crash application
    }
  }
  void grow(u64 desiredSize) {
    m_data = new T[desiredSize];
    m_size = desiredSize;

    if (m_length) {
      std::cout << "Error! Called grow on marray but marray had data in it!\n";
    }
  }
  T* operator[](u64 index) {
    return &m_data[index];
  }
  T& back() { return m_data[m_length-1]; }

  u64 length() { return m_length; }
};

}