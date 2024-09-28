#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

double BailleyPi(int n) {
  double pi = 0.0;
  for (int i = 0; i < n; i++) {
    pi += ((4.0 / (8.0 * i + 1)) - (2.0 / (8.0 * i + 4.0)) -
           (1.0 / (8.0 * i + 5.0)) - (1.0 / (8.0 * i + 6.0))) *
          (1.0 / pow(16.0, i));
  }
  return pi;
}

int main(int argc, char* argv[]) {
  int n = 5;

  double pi;
  while (1) {
    pi = BailleyPi(n);
    printf("Bailley pi with n = %d: %f\n", n, pi);
    n += 5;
    if (n == 30) {
      break;
    }
  }
  return 0;
}