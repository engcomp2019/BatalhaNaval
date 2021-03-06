/**************************************************************
*                                                             *
*    PROJETO APC 2 - JOGO DE BATALHA NAVAL COM ARDU�NO 			    *
*                                                             *
* Nome: Andre Queiroz					                RA:	27619994874	    *
* Nome: Raiany Araujo					                RA:	02055024230	    *
* Nome: Rodrigo Sene					                RA:	42108819886	     *
* Data de Inicio: 28/02/2015                                  *
* Data Final: 27/05/2015		                                    *
**************************************************************/

// Bibliotecas (Arquivos de Cabecalho)
#include "PSerial.h"
#include <string.h>
#include <stdio.h>
#include <conio.h>


/*
================================================================================
================================================================================
*/
void IniciaSerial(char *porta, int velocidadePorta);
void fechaSerial();
void ArduinoInteracao(int indice);

void ArduinoInteracao(int indice){


	char str[2] = {'0','0'};
	char linha;
 int coluna;

	if(indice >= 0 && indice < 10){
 	   	linha  = '0';
 	   	coluna = indice;		
	}
	else if(indice >= 10 && indice < 20){
      linha  = '1';
    		coluna = indice - 10;
	}
	else if(indice >= 20 && indice < 30){
      linha  = '2';
      coluna = indice - 20;
	}
	else if(indice >= 30 && indice < 40){
      linha  = '3';
      coluna = indice - 30;
	}  	
	else if(indice >= 40 && indice < 50){
      linha  = '4';
      coluna = indice - 40;
	}
	else if(indice >= 50 && indice < 60){
      linha  = '5';
      coluna = indice - 50;
	}
	else if(indice >= 60 && indice < 70){
      linha  = '6';
      coluna = indice - 60;
	}
	else if(indice >= 70 && indice < 80){
      linha  = '7';
      coluna = indice - 70;
	}	
	else if(indice >= 80 && indice < 90){
      linha  = '8';
      coluna = indice - 80;
	}
	else if(indice >= 90 && indice < 100){
      linha  = '9';
      coluna = indice - 90;
	}

	sprintf(str,"%s%d",linha,coluna);

 if(serialConectada(0)){
	    escreveSerial(0,str,2);
 }

}END_OF_FUNCTION(ArduinoInteracao);


void fechaSerial(){
	
	finalizaSerial(0);

}END_OF_FUNCTION(fechaSerial);


void IniciaSerial(char *porta, int velocidadePorta){

	inicializaSerial(0, porta, velocidadePorta);

}END_OF_FUNCTION(IniciaSerial);
