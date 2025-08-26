#include "stdio.h"
#include <string.h>

float Q_rsqrt(float number) {
  // 快速求平方根倒数算法
  const float threehalfs = 1.5f;
  float x2 = number * 0.5f;
  float y = number;
  long i;

  memcpy(&i, &y, sizeof(i));
  i = 0x5f3759df - (i >> 1);
  memcpy(&y, &i, sizeof(y));
  y = y * (threehalfs - (x2 * y * y));
  y = y * (threehalfs - (x2 * y * y));
  return y;
}

float q_sqrt_fast(float number){
  
  return 1/Q_rsqrt(number);
}

int main() {
  float x = 2;
  printf("Q_rsqrt(%f) = %f !\n", x, Q_rsqrt(x));
  printf("sqrt(%f) = %f !\n",x,q_sqrt_fast(x));
  return 0;
}
