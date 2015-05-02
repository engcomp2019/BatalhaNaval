#include <allegro.h>
#include <alpng.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "definicoes.h"
#include "cenario.h"

/**************************************************************************
Iniciando structs
***************************************************************************/
//Telas
BITMAP *bmpFundoAgua     = load_bitmap("imagens/estaticos/agua.png",NULL);
stcCenario stcFundoAgua;
setImagemCenario(stcFundoAgua,bmpFundoAgua);

BITMAP *bmpMoveAgua      = load_bitmap("imagens/estaticos/agua_2.png",NULL);
stcCenario stcMoveAgua;
setImagemCenario(stcMoveAgua,bmpMoveAgua);

// Rodapé
BITMAP *bmpRodapeOpcoes  = load_bitmap("imagens/estaticos/rodape.png",NULL);
stcCenario stcRodapeOpcoes;
setImagemCenario(&stcRodapeOpcoes,bmpRodapeOpcoes);
setPosicaoX(&stcRodapeOpcoes,0);
setPosicaoY(&stcRodapeOpcoes,458);

// Tabuleiro
BITMAP *bmpGradeTabuleiro       = load_bitmap("imagens/estaticos/tabuleiro.png",NULL);
stcCenario stcgradeTabuleiro;
setImagemCenario(&stcgradeTabuleiro,bmpGradeTabuleiro);
setPosicaoX(&stcgradeTabuleiro,0);
setPosicaoY(&stcgradeTabuleiro,0);

BITMAP *bmpGradeTabuleiroCores  = load_bitmap("imagens/estaticos/tabuleiro_cores.png",NULL);
stcCenario stcGradeTabuleiroCores;
setImagemCenario(&stcGradeTabuleiroCores,bmpGradeTabuleiroCores);

//Ilhas
BITMAP *bmpIlhaSuperiorEsquerda = load_bitmap("imagens/estaticos/ilhas/top_left.png",NULL);
stcCenario stcIlhaSuperiorEsquerda;
setImagemCenario(&stcIlhaSuperiorEsquerda,bmpIlhaSuperiorEsquerda);
setPosicaoX(&stcIlhaSuperiorEsquerda,-40);
setPosicaoY(&stcIlhaSuperiorEsquerda,0);

BITMAP *bmpIlhaSuperiorDireita  = load_bitmap("imagens/estaticos/ilhas/top_right.png",NULL);
stcCenario stcIlhaSuperiorDireita;
setImagemCenario(&stcIlhaSuperiorDireita,bmpIlhaSuperiorDireita);
setPosicaoX(&stcIlhaSuperiorDireita,525);
setPosicaoY(&stcIlhaSuperiorDireita,0);

BITMAP *bmpIlhaInferiorEsquerda = load_bitmap("imagens/estaticos/ilhas/bottom_left.png",NULL);
stcCenario stcIlhaInferiorEsquerda;
setImagemCenario(&stcIlhaInferiorEsquerda,bmpIlhaInferiorEsquerda);
setPosicaoX(&stcIlhaInferiorEsquerda,0);
setPosicaoY(&stcIlhaInferiorEsquerda,395);

BITMAP *bmpIlhaInferiorDireita  = load_bitmap("imagens/estaticos/ilhas/bottom_right.png",NULL);
stcCenario stcIlhaInferiorDireita;
setImagemCenario(&stcIlhaInferiorDireita,bmpIlhaInferiorDireita);
setPosicaoX(&stcIlhaInferiorDireita,515);
setPosicaoY(&stcIlhaInferiorDireita,375);

//Mouse
BITMAP *bmpCursorMouse          = load_bitmap("imagens/estaticos/mouse.png",NULL);
stcCenario stcCursorMouse;
setImagemCenario(&stcCursorMouse,bmpCursorMouse);

BITMAP *bmpCursorMouseAtaque    = load_bitmap("imagens/estaticos/mouseAtaque.png",NULL);
stcCenario stcCursorMouseAtaque;
setImagemCenario(&stcCursorMouseAtaque,bmpCursorMouseAtaque);


/*Variaveis do jogo*/
int aguaMovimentoX     = 0,
    aguaMovimentoFator = 7,
    aguaMovimentoMin   = 0,
    aguaMovimentoMax   = (38 * aguaMovimentoFator),
    flagMaxX           = 0;

/**/
int batalhaNaval(){

    while(!key[KEY_ESC]){

        desenhaCenario(bmpGradeTabuleiroCores,stcGradeTabuleiroCores);
        desenhaCenario(bmpFundoAgua,stcFundoAgua);

        if(aguaMovimentoX <= aguaMovimentoMax && flagMaxX == 0){
            if(aguaMovimentoX == aguaMovimentoMax){
                flagMaxX = 1;
            }
            aguaMovimentoX++;
        }
        else if(aguaMovimentoX >= aguaMovimentoMin && flagMaxX == 1){

            if(aguaMovimentoX == aguaMovimentoMin){
                flagMaxX = 0;
            }
            aguaMovimentoX--;
        }

          draw_trans_sprite(buffer,moveAgua,-(aguaMovimentoX / aguaMovimentoFator),0);
          desenhaCenario(buffer,stcIlhaSuperiorEsquerda);
          desenhaCenario(buffer,stcIlhaSuperiorDireita);
          desenhaCenario(buffer,stcIlhaInferiorEsquerda);
          desenhaCenario(buffer,stcIlhaInferiorDireita);

          desenhaCenario(buffer,stcRodapeOpcoes);
          desenhaCenario(buffer,stcGradeTabuleiro);
    }
    return 1;
}


    /*
    // Tamanho do cursor do mouse
    int cursorMouseLargura = 28, //Largura do quadro a ser desenhado na tela
        cursorMouseAltura  = 39; //Altura do quadro a ser desenhado na tela

    //Variaveis de controle da movimentacao da agua
    int aguaMovimentoX     = 0,
        aguaMovimentoFator = 7,
        aguaMovimentoMin   = 0,
        aguaMovimentoMax   = (38 * aguaMovimentoFator),
        flagMaxX           = 0;
    */



                  /*blit(fundoAgua, buffer,0,0,0,0,JANELA_LARGURA, JANELA_ALTURA);
              blit(gradeTabuleiroCores, buffer,0,0,0,0,JANELA_LARGURA, JANELA_ALTURA);
              if(aguaMovimentoX <= aguaMovimentoMax && flagMaxX == 0){

                 if(aguaMovimentoX == aguaMovimentoMax){
                    flagMaxX = 1;
                 }
                 aguaMovimentoX++;
              }
              else if(aguaMovimentoX >= aguaMovimentoMin && flagMaxX == 1){

                 if(aguaMovimentoX == aguaMovimentoMin){
                    flagMaxX = 0;
                 }
                 aguaMovimentoX--;
              }

              //draw_trans_sprite(buffer,moveAgua,-(aguaMovimentoX / aguaMovimentoFator),0);

              draw_trans_sprite(buffer,ilhaSuperiorEsquerda,-40,0);
              draw_trans_sprite(buffer,ilhaSuperiorDireita,525,0);
              draw_trans_sprite(buffer,ilhaInferiorEsquerda,0,395);
              draw_trans_sprite(buffer,ilhaInferiorDireita,515,375);

              draw_trans_sprite(buffer,rodapeOpcoes,0,458);
              draw_trans_sprite(buffer,gradeTabuleiro,0,0);

              DefinePosicaoGrade( gradeTabuleiroCores );*/
