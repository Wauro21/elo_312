/***************************************************
* Nombre del modulo: keyboard.c
*
* Modulo creado para la asignatura Elo312
* Laboratorio de Estructura de Computadores
* del departamento de Electrónica de la Universidad
* Técnica Federico Santa María. El uso o copia
* está permitido y se agracede mantener el nombre 
* de los creadores.
*
* Escrito inicialmente el 01/01/2004 Por Michael Kusch & Wolfgang Freund
* Modificado el 24/09/2014           Por Mauricio Solís & Andrés Llico.
*
* Descripción del módulo:
* Modulo driver teclado matricial
* Contiene las funciones que permiten manejar el
* teclado matricial.
***************************************************/

/*  Include section
*
***************************************************/
#include "keyboard.h"
#include "display.h"
#include <stdio.h>

/*  Defines section
*
***************************************************/
typedef unsigned char uchar;

/*  Local Function Prototype Section
*
***************************************************/
void keyboard_button_action(char);


/*  Global Variables Section
*
***************************************************/
uchar BUTMAT_CLK = 0;           // cuántas interrupciones se lleva presionado el botón
uchar BUTMAT_PREV = 0xFF;       // tecla de la lectura anterior, FF => ninguna.
char tecla_vieja=100;
char estado;
char letras1[16] = {'A','B','C',' ','D','E','F',' ','G','H','I',' ','\b',' ','\n',' '};
char letras2[16] = {'J','K','L',' ','M','N','O',' ','P','Q','R',' ','\b',' ','\n',' '};
char letras3[16] = {'S','T','U',' ','V','W','X',' ','Y','Z',' ',' ','\b',' ','\n',' '};
char numeros[16] = {'1','2','3',' ','4','5','6',' ','7','8','9',' ','\b','0','\n',' '};

////////////////////////////////////////
//       RUTINAS IMPLEMENTADAS        //
//                                    //

/**************************************************
* Nombre    		: void keyboard_init(void)
* returns			: void
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Inicializa los puertos del 
* driver del teclado.
**************************************************/
void keyboard_init(void)
{
    PTOUT = 0x00;
    PTDIR = 0x00;
}

/**************************************************
* Nombre    		: void keyboard_tec_matrix(void)
* returns			: void
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Lee la matriz de botones y 
* llama a la función keyboard_button_action() cuando
* detecta que se ha presionado una tecla (o se ha mantenido
* presionada una tecla).
* Esta función debe ser llamada cada cierta cantidad de tiempo.
**************************************************/
void keyboard_tec_matrix(void)
{
    uchar BUTMAT = 0xFF;        // tecla de 0 a 15
    uchar k, i, temp;

    temp = 0x00;
    for(PTDIR = 0x10; PTDIR > 0; PTDIR <<= 1) {
        asm("NOP"); asm("NOP");
        asm("NOP"); asm("NOP");
        asm("NOP");
        k = (((~PTIN) & 0x0F) | PTDIR);
        if ( (k & 0x0F) )	           // si hay algún bit de columna en 0, entonces if es verdadero
            temp = k;	               // y se guarda el valor de la columna y filas activas
    }

    if (temp) {	                       // hubo tecla presionada?
        switch	(temp & 0xF0) {
        case BIT4:
            BUTMAT = 0;	 // fila 1
            break;
        case BIT5:
            BUTMAT = 4;	 // fila 2
            break;
        case BIT6:
            BUTMAT = 8;	 // fila 3
            break;
        case BIT7:
            BUTMAT = 12; // fila 4
            break;
        }

        switch	(temp & 0x0F) {
        case BIT0:
            asm("NOP");
            BUTMAT += 0; // col 1
            break;
        case BIT1:
            BUTMAT += 1; // col 2
            break;
        case BIT2:
            BUTMAT += 2; // col 3
            break;
        case BIT3:
            BUTMAT += 3; // col 4
            break;
        }

        if (BUTMAT != BUTMAT_PREV) {              // botón presionado es distinto al anterior?
            BUTMAT_PREV = BUTMAT;
            BUTMAT_CLK = 0;
            keyboard_button_action(BUTMAT);
        }
        else {
            if (++BUTMAT_CLK == 100) {            // presionada por más de 100*10[ms] (1 [s])?
                keyboard_button_action(BUTMAT);   // actuar como si se volvió a presionar el botón
                BUTMAT_CLK -= 33;                 // por cada 0.5 [s] que siga presionado el botón
            }
        }
    }
    else
        BUTMAT_PREV = 0xFF;                       // ningún botón presionado
}
//                                    //
////////////////////////////////////////


////////////////////////////////////////
//     RUTINAS NO IMPLEMENTADAS       //
//                                    //

/**************************************************
* Nombre    		: void keyboard_button_action(char arg1)
* returns			: void
* arg1				: Tecla presionada
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Realiza una accion segun la
tecla que ha sido presionada.
**************************************************/
void keyboard_button_action(char tecla)
{

	if((tecla==3)||(tecla==7)||(tecla==11)||(tecla==15))
	{
	  estado = tecla;
	  tecla_vieja = tecla;
	}
	else if(tecla_vieja!=tecla)
	{
	  if((tecla!=12)||(tecla!=14))
	  	tecla_vieja = tecla;
	switch(estado)
	{
	  case(3):			
//			printf("%c",letras1[tecla]);
			uart_0_send(letras1[tecla]);
			break;
		case(7):
//		  	printf("%c",letras2[tecla]);
			uart_0_send(letras2[tecla]);
		  break;
		case(11):
//			printf("%c",letras3[tecla]);
			uart_0_send(letras3[tecla]);
		  break;
		case(15):
//			printf("%c",numeros[tecla]);
			uart_0_send(numeros[tecla]);
		  break;
	}
	
	} 
}

