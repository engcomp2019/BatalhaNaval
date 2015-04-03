/***********************************************************
*                                                          *
*    PROJETO APC 2 - JOGO DE BATALHA NAVAL COM ARDUINO 			 *
*                                                          *
* Nome: Andre Queiroz					                 RA:	27619994874	*
* Nome: Raiany Araujo					                 RA:	02055024230	*
* Nome: Rodrigo Sene					                  RA:	42108819886 *
* Data de Inicio: 28/02/2015                               *
* Data Final: 25/05/2015		                                 *
***********************************************************/

// Bibliotecas (Arquivos de Cabecalho)
#include <allegro.h>

// Prototipos das funcoes

/*
================================================================================
char *verificaLocalMapa:

Recebe um bitmap e posicao do mouse, e retorna a posicao
no tabuleiro em que o mouse esta, levando em conta o indice de um tabuleiro
de batalha naval.

================================================================================
*/

char* verificaLocalMapa(int pixel){

  int red, blue, green;

  if(pixel == -1){
    return "tt";
  }

  if(pixel != -1){

    green = getg(pixel);
    blue  = getb(pixel);
    red   = getr(pixel);

    if(red == 255 && green == 128 && blue == 0){
      return "a1";
    }
    else if (red == 255 && green == 255 && blue == 0){
      return "a2";
    }
    else if (red == 255 && green == 0 && blue == 166){
      return "b2";
    }
    else if (red == 216 && green == 0 && blue == 0){
      return "b2";
    }

  }
}





