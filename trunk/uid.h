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

// incrementing IDs can be useful for testing

#ifdef UID_INCREMENTING
u64 uid_index_size_max_blabha = 0;
#endif

struct Uid{
  u64 id;
  Uid() {
    #ifndef UID_INCREMENTING
      id = RNG::the().randu64(); // 0..u64 max
    #else
      id = uid_index_size_max_blabha++;
    #endif
  };
  std::string to_english() const {
    #ifndef UID_INCREMENTING
      return "uid(" + Noah::the().wordify(id) + ")";
    #else
      return "uid( " + std::to_string(id) + ")";
    #endif
  };
  bool operator<(const Uid &other) const {
    return id < other.id;
  }
  bool operator==(const Uid& other) const {
    return id == other.id;
  }
};

namespace std {

  template <>
  struct hash<Uid>
  {
    std::size_t operator()(const Uid& id) const
    {
      using std::size_t;
      using std::hash;

      // this should just use the default u64 hashing function from std
      // ... hopefully.

      return (hash<u64>()(id.id));
    }
  };

}
