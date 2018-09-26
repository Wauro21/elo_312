
#include <stdio.h>

int numbers[20]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19}; //Declaracion variable global
int number_to_find = 7; //Numero a buscar

void search(int to_find, int n_array[] )
{
  //declaracion variables locales
  int cont = 0 ; //Contador
  while (cont <= 19)
  {
    if (n_array[cont] == to_find)
    {
      printf("El numero se encontro en el indice: %d",cont);
      break;
    }
    else if (cont == 19)
    {
      printf("No se hayo el numero: %d",to_find);
      cont++;
    }
    else 
    {
      cont++; //Se incremente el contador
    }
  }
  return;
}

void main(void)
{
  search(number_to_find,numbers);
}
