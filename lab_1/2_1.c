
#include "io430.h"
#include <stdio.h>
int numbers[20]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19}; //Declaracion variable global
int number_to_find = 7; //Numero a buscar

int main( void )
{
  //declaracion variables locales
  int cont = 0 ; //Contador
  while (cont <= 19)
  {
    if (numbers[cont] == number_to_find)
    {
      printf("El numero se encontro en el indice: %d",cont);
      break;
    }
    else if (cont == 19)
    {
      printf("No se hayo el numero: %d",number_to_find);
    }
    else 
    {
      cont++; //Se incremente el contador
    }
  }
}
