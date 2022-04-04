#pragma once

#include "mtypedefs.h"
#include "rng.h"
#include "noah.h"

// this is a unique id, commonly called a UID

// implemented this from a tutorial from the Primal Game Engine Programming series:
  // https://youtu.be/rT599NDbkN4



constexpr u64 uid_bits{24};
constexpr u64 index_bits{sizeof(u64) * 8 - uid_bits};
constexpr u64 index_mask{ (u64{1} << index_bits) - 1};
constexpr u64 uid_mask{ (u64{1} << uid_bits) - 1};



struct Uid{
  u64 id;
  Uid() {
    id = RNG::the().randu64(); // 0..u64 max
  };
  std::string to_english() {
    Noah::the().wordify(id);
  };
};