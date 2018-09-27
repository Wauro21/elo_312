#include <stdio.h>
#include "funcion.h"
unsigned int multiply_pow_2_v1 (unsigned int number, unsigned int power)
{
  return (number << power);
}

unsigned int multiply_pow_2_v2 (unsigned int number, unsigned int power)
{
  int cont = 0 ;
  int out = number;
  while (cont < power)
  {
    out = out*2;
    cont++;
  }
  return out;
}

void comparar ( unsigned int number, unsigned int power)
{
  printf("V1 -> %u \n", multiply_pow_2_v1(number,power));
  printf("V2 -> %u", multiply_pow_2_v2(number,power));
}

