/***********************************************************
*                                                          *
*    PROJETO APC 2 - JOGO DE BATALHA NAVAL COM ARDUINO 			 *
*                                                          *
* Nome: Andre Queiroz					                RA:	27619994874		*
* Nome: Raiany Araujo					                RA:	02055024230		*
* Nome: Rodrigo Sene					                 RA:	42108819886	 *
* Data de Inicio: 28/02/2015                               *
* Data Final: 25/05/2015		                                 *
************************************************************/

#include <allegro.h>

/*
================================================================================
struct stcCenario

Recebe os valores da imagem do cen�rio e sua posi��o em X e Y.

================================================================================
*/
typedef struct{
    
    BITMAP *imagem;
    float posicaoX, posicaoY;
    
}stcCenario;
/* FIM - struct stcCenario ================================================== */


/*
================================================================================
 Prot�tipos das fun��es
================================================================================
*/

void    setImagemCenario(stcCenario *oCenario, BITMAP *img);
BITMAP *getImagemCenario(stcCenario oCenario);
void    setPosicaoX(stcCenario *oCenario, float posX);
float   getPosicaoX(stcCenario oCenario);
void    setPosicaoY(stcCenario *oCenario, float posY);
float   getPosicaoY(stcCenario oCenario);
void    desenhaCenario(BITMAP *oBuffer, stcCenario oCenario);

/* FIM - Prot�tipos das fun��es ============================================= */

/*
================================================================================
void setImagemCenario

Define a imagem do cen�rio na estrutura

@tipo setter
@args *oCenario - ponteiro de estrutura do cen�rio
      *img      - ponteiro do bitmap da imagem do cen�rio

================================================================================
*/

void setImagemCenario(stcCenario *oCenario, BITMAP *img){
    
    oCenario->imagem = img;
    
}
END_OF_FUNCTION(setImagemCenario);

/*
================================================================================
BITMAP *getImagemCenario

Retorna a imagem do cen�rio contido na estrutura

@tipo getter
@args oCenario - estrutura do cen�rio
@return imagem - ponteiro do bitmap da imagem do cen�rio

================================================================================
*/

BITMAP *getImagemCenario(stcCenario oCenario){
    
    return oCenario.imagem;
    
}
END_OF_FUNCTION(getImagemCenario);

/*
================================================================================
void setPosicaoX

Define a posicao X da imagem em rela��o ao cen�rio

@tipo setter
@args *oCenario - ponteiro de estrutura do cen�rio
      posX      - valor da posicao em X

================================================================================
*/

void setPosicaoX(stcCenario *oCenario, float posX){
    
    oCenario->posicaoX = posX;
    
}
END_OF_FUNCTION(setPosicaoX);

/*
================================================================================
float getPosicaoX

Retorna a posicao X da imagem em rela��o ao cen�rio

@tipo getter
@args oCenario  - estrutura do cen�rio
@return posicaoX - valor da posicao em X

================================================================================
*/

float getPosicaoX(stcCenario oCenario){
    
    return oCenario.posicaoX;
    
}
END_OF_FUNCTION(getPosicaoX);

/*
================================================================================
void setPosicaoY

Define a posicao em Y da imagem em rela��o ao cen�rio

@tipo setter
@args *oCenario - ponteiro de estrutura do cen�rio
      posY      - valor da posicao em Y

================================================================================
*/

void setPosicaoY(stcCenario *oCenario, float posY){
    
    oCenario->posicaoY = posY;
    
}
END_OF_FUNCTION(setPosicaoY);

/*
================================================================================
float getPosicaoY

Retorna a posicao em Y da imagem em rela��o ao cen�rio

@tipo getter
@args oCenario  - estrutura do cen�rio
@return posicaoY - valor da posicao em Y

================================================================================
*/

float getPosicaoY(stcCenario oCenario){
    
    return oCenario.posicaoY;
    
}
END_OF_FUNCTION(getPosicaoY);

/*
================================================================================
void desenhaCenario

Desenha o cen�rio no buffer utilizando os argumentos passados.

@tipo draw
@args *oBuffer  - ponteiro do buffer enviado
      oCenario  - estrutura do cen�rio


================================================================================
*/

void desenhaCenario(BITMAP *oBuffer, stcCenario oCenario){

     
     //draw_trans_sprite(bitmap destino, bitmap origem, x do destino, y do destino)
     draw_trans_sprite(oBuffer, getImagemCenario(oCenario), getPosicaoX(oCenario), getPosicaoY(oCenario));    
}
END_OF_FUNCTION(desenhaCenario)

