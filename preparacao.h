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

/******************************************************************************
preparacao.h

Objetivo: Unificar as funçes e objetos utilizados para criação e manutenção
          da lógica dentro da tela de preparação do jogo.
******************************************************************************/


/******************************************************************************
  STRUCTS
******************************************************************************/
typedef struct{
    int posicaoX, posicaoY, indice;
}stcPosicao;

/******************************************************************************
  SETS
******************************************************************************/
void setStcPosicaoX(stcPosicao oLocPreparacao[][TAB_DIM], int x, int linha, int coluna){

    oLocPreparacao[linha][coluna].posicaoX = x;

}END_OF_FUNCTION(setStcPosicaoX);


void setStcPosicaoY(stcPosicao oLocPreparacao[][TAB_DIM], int y, int linha, int coluna){

    oLocPreparacao[linha][coluna].posicaoY = y;

}END_OF_FUNCTION(setStcPosicaoY);


void setStcPosicaoIndice(stcPosicao oLocPreparacao[][TAB_DIM], int indice, int linha, int coluna){

    oLocPreparacao[linha][coluna].indice = indice;

}END_OF_FUNCTION(setStcPosicaoIndice);

/******************************************************************************
  GETS
******************************************************************************/

int getStcPosicaoX(stcPosicao oLocPreparacao[][TAB_DIM],int ind){

int i,j;

    for(i = 0; i < 10; i++){
        for(j = 0; j < 10; j ++){

            if(oLocPreparacao[i][j].indice == ind){
                return oLocPreparacao[i][j].posicaoX;
            }
        }
    }
}END_OF_FUNCTION(getStcPosicaoX);

int getStcPosicaoY(stcPosicao oLocPreparacao[][TAB_DIM], int ind){

int i,j;

    for(i = 0; i < 10; i++){
        for(j = 0; j < 10; j ++){

            if(oLocPreparacao[i][j].indice == ind){
                return oLocPreparacao[i][j].posicaoY;
            }
        }
    }
}END_OF_FUNCTION(getStcPosicaoY);
