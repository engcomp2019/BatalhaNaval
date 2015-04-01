/**************************************************************
*                                                             *
*    PROJETO APC 2 - JOGO DE BATALHA NAVAL COM ARDUÍNO 			  *
*                                                             *
* Nome: Andre Queiroz					                RA:	27619994874	*
* Nome: Raiany Araujo					                RA:	02055024230	*
* Nome: Rodrigo Sene					                RA:	42108819886	*
* Data de Inicio: 28/02/2015                                  *
* Data Final: 25/05/2015		                                  *
**************************************************************/

// Bibliotecas (Arquivos de Cabecalho)


/*
================================================================================
verificaLocalMapa:
Recebe um bitmap e posição do mouse, e retorna a posição 
no tabuleiro em que o mouse está, levando em conta o indice de um tabuleiro
de batalha naval.
================================================================================
*/

char* verificaLocalMapa(BITMAP *mapa, int mouseX, int mouseY){

  int pixel, red, blue, green;
  char indice[3]; 
  pixel = getpixel(mapa, mouseX, mouseY);

  if(pixel == -1){
    indice = "tt";
  }
  
  if(indice != "tt"){
  
    green = getg(pixel);
    blue  = getb(pixel);
    red   = getr(pixel);
    
    if(red == 255 && green == 128 && blue == 0){
      indice = "a1";
    }
    else if (red == 255 && green == 255 && blue == 0){
      indice = "a2";
    }
    else if (red == 255 && green == 0 && blue == 166){
      indice = "b2";
    }
    else if (red == 255 && green == 0 && blue == 0){
      indice = "b2";
    }
    
  }
  return indice;
}




