#pragma once

#ifndef RNG_H
#define RNG_H

#include <random>

#include "mtypedefs.h"

class RNG{
public:
  static RNG& the() {
    static RNG* my_rng = nullptr;
    if (!my_rng) {
      my_rng = new RNG;
    }
    return *my_rng;
  };

  std::default_random_engine engine;
  std::uniform_int_distribution<u64> dist;
  void setup_rng() {
    // dist.seed((unsigned long)Application::the().startTime);
  }

  //
  // Random Integers
  //

  i32 die(i32 sides) {
    return die(1, sides);
  }
  i32 die(i32 n, i32 sides) {
    i32 sum = 0;
    for (i32 i=0; i < n; n++) {
      sum += (_rngul() % sides) + 1;
    }
    return sum;
  }
  i32 randi32(i32 max = std::numeric_limits<i32>::max()) { // 0..max
    return _rngul() % max;
  }
  u64 randu64() {
    return _rngul();
  }
  u64 _rng() {
    return dist(engine);
  }
  u64 _rngul() { // RNG unsigned long
    return _rng();
    // u64 data = 0;

    // const u64 base = 2;
    // const u64 useful_bits = 32;

    // for (u64 bits = 0; bits < 64; bits += 32) {
    //   data |= (rand() & m::pow(base, useful_bits)) << bits;
    // }
    // return data;

    // TODO how random is this? Is it biased towards one thing or another?
  }

  //
  // random floats
  //

  float randf() { // rand float
    return float(_rngd());
  }
  float randf(float min, float max) { // min..max
    double range = max-min;
    return float( _rngd() * double(range) + double(min) );
  }
  // meant this to be f64, dunno if that's what double is?
  double _rngd() { // 0..1, RNG double
    return (double( _rngul()%10'000'000'000'000'001 ) / 10'000'000'000'000'000.0f);

    // note the off by 1 that comes from modulus ( for example rand()%4 is 0..3,
      // so for 1d4 you'd need rand()%5 )
  }
};

#endif