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
  STRUCTS
******************************************************************************/
typedef struct{

    int indice,
        posicaoX,
        posicaoY,
        tipoNavio;

}stcTabuleiroJogo;

/******************************************************************************
  SETS
******************************************************************************/
void setStcTabuleiroJogoIndice(stcTabuleiroJogo oTabJogo[][TAB_DIM], int ind, int linha, int coluna){

    oTabJogo[linha][coluna].indice = ind;

}END_OF_FUNCTION(setStcTabuleiroJogoIndice);


void setStcTabuleiroJogoX(stcTabuleiroJogo oTabJogo[][TAB_DIM], int x, int linha, int coluna){

    oTabJogo[linha][coluna].posicaoX = x;

}END_OF_FUNCTION(setStcTabuleiroJogoX);


void setStcTabuleiroJogoY(stcTabuleiroJogo oTabJogo[][TAB_DIM], int y, int linha, int coluna){

    oTabJogo[linha][coluna].posicaoY = y;

}END_OF_FUNCTION(setStcTabuleiroJogoY);


void setStcTabuleiroJogoTipoNavio(stcTabuleiroJogo oTabJogo[][TAB_DIM], int tpNavio, int linha, int coluna){

    oTabJogo[linha][coluna].tipoNavio = tpNavio;

}END_OF_FUNCTION(setStcTabuleiroJogoTipoNavio);


/******************************************************************************
  GETS
******************************************************************************/

int getStcTabuleiroJogoX(stcTabuleiroJogo oTabJogo[][TAB_DIM],int ind){

int i,j;

    for(i = 0; i < 10; i++){
        for(j = 0; j < 10; j ++){

            if(oTabJogo[i][j].indice == ind){
                return oTabJogo[i][j].posicaoX;
            }
        }
    }
}END_OF_FUNCTION(getStcTabuleiroJogoX);


int getStcTabuleiroJogoY(stcTabuleiroJogo oTabJogo[][TAB_DIM],int ind){

int i,j;

    for(i = 0; i < 10; i++){
        for(j = 0; j < 10; j ++){

            if(oTabJogo[i][j].indice == ind){
                return oTabJogo[i][j].posicaoY;
            }
        }
    }
}END_OF_FUNCTION(getStcTabuleiroJogoY);


int getStcTabuleiroJogoIndice(stcTabuleiroJogo oTabJogo[][TAB_DIM],int x, int y){

int i,j;

    for(i = 0; i < 10; i++){
        for(j = 0; j < 10; j ++){

            if(oTabJogo[i][j].posicaoX == x && oTabJogo[i][j].posicaoY == y){
                return oTabJogo[i][j].indice;
            }
        }
    }
    return -1;
}END_OF_FUNCTION(getStcTabuleiroJogoY);


int getStcTabuleiroJogoTipoNavio(stcTabuleiroJogo oTabJogo[][TAB_DIM],int ind){

int i,j;

    for(i = 0; i < 10; i++){
        for(j = 0; j < 10; j ++){

            if(oTabJogo[i][j].indice == ind){
                return oTabJogo[i][j].tipoNavio;
            }
        }
    }
}END_OF_FUNCTION(getStcTabuleiroJogoY);

