#pragma once

#ifndef MMATH_H
#define MMATH_H

#define pi 3.1415926535897932384626433832795

namespace m{

inline float degrees_to_radians(float degrees) {
  return float(degrees * pi / 180.0f);
}

// u64 pow(u64 base, u64 exponent) {
//     u64 temp = base;
//     for (u64 i = 1; i < exponent; i++) {
//         temp *= base;
//     }
//     return temp;
// }

}
#endif
