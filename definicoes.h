#include <allegro.h>

// Defines
#define JANELA_LARGURA 800
#define JANELA_ALTURA 600
#define TAB_DIM 10

#define PROFUNDIDADE_COR 32
#define GFX_MODO GFX_AUTODETECT_WINDOWED

#define FPS_MAXIMO 60

// Define das telas do jogo
#define TELA_INICIO 0
#define TELA_PREPARACAO  1
#define TELA_JOGO  2
#define TELA_INSTRUCOES 3
#define TELA_PERDEU  4
#define TELA_GANHOU 5
#define TELA_SAIR 6

//BITMAP  *buffer;
int i,j;

/*Structs*/
typedef struct{
    int posicaoX, posicaoY, indice;
}stcPosicao;

/***************************************
Sets
****************************************/
void setStcPosicaoX(stcPosicao oLocPreparacao[][TAB_DIM], int x, int linha, int coluna){

    oLocPreparacao[linha][coluna].posicaoX = x;

}END_OF_FUNCTION(setStcPosicaoX);


void setStcPosicaoY(stcPosicao oLocPreparacao[][TAB_DIM], int y, int linha, int coluna){

    oLocPreparacao[linha][coluna].posicaoY = y;

}END_OF_FUNCTION(setStcPosicaoY);


void setStcPosicaoIndice(stcPosicao oLocPreparacao[][TAB_DIM], int indice, int linha, int coluna){

    oLocPreparacao[linha][coluna].indice = indice;

}END_OF_FUNCTION(setStcPosicaoIndice);

/**************************************
Gets
***************************************/

int getStcPosicaoX(stcPosicao oLocPreparacao[][TAB_DIM],int ind){

    for(i = 0; i < 10; i++){
        for(j = 0; j < 10; j ++){

            if(oLocPreparacao[i][j].indice == ind){
                return oLocPreparacao[i][j].posicaoX;
            }
        }
    }
}END_OF_FUNCTION(getStcPosicaoX);

int getStcPosicaoY(stcPosicao oLocPreparacao[][TAB_DIM], int ind){

    for(i = 0; i < 10; i++){
        for(j = 0; j < 10; j ++){

            if(oLocPreparacao[i][j].indice == ind){
                return oLocPreparacao[i][j].posicaoY;
            }
        }
    }
}END_OF_FUNCTION(getStcPosicaoY);

