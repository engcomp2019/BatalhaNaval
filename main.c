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

// Bibliotecas (Arquivos de Cabecalho)
#include <allegro.h>
#include <alpng.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "definicoes.h"
#include "cenario.h"
#include "preparacao.h"
//#include "som.h"

// Variaveis Globais
BITMAP  *buffer;
//FMOD_SYSTEM *fmodCenario = 0;
//FMOD_SYSTEM *fmodMenu = 0;

// Variaveis Gerais
int inicioJogo     = 0;           // Flag que indica se o jogo foi iniciado
int fimJogo        = 0;           // Flag que indica se o jogo continua ou nao.
int somCarregado   = 0;           // Flag que indica se o som foi carregado ou nao.
int telaAtual      = TELA_INICIO; // Recebe o valor da tela atual do jogo.

volatile int velocidade = 0;    // Recebe o valor incremental da velocidade do jogo.
volatile int totalFps   = 0;	// Recebe o total de frames por segundo (calculado apenas uma vez a cada segundo).
volatile int frames     = 0;	// Recebe a quantidade de frames por segundo (incrementado a cada volta do loop principal ).

// Protótipos das funções
void Inicializa();
void Finaliza();
void VelocidadeJogo();
void CalculaFPS();
int batalhaNaval();

void exibeCarregando(BITMAP *local);
int  exibeMenu(BITMAP *local);
void exibeInicial(BITMAP *origem, BITMAP *destino);
void exibeJogo();
void exibeInstrucoes();
void exibeResultado(int *resultado);


void definePosicaoGrade(BITMAP *bmp);
void carregaAnimacao(BITMAP *imagem[], char *pasta, int frames);
int  verificaSaida(int opcao);

void populaPreparacao(stcPosicao oPosicaoPreparacao[][TAB_DIM]);


//Função Main
int main(){

    stcPosicao mtxPosicaoPreparacao[TAB_DIM][TAB_DIM];
    populaPreparacao(mtxPosicaoPreparacao);

    Inicializa();

    // Exibe a mensagem de carregando na tela
    exibeCarregando(screen);

    // Inicialização dos cenários
    BITMAP *telaIntro = load_bitmap("imagens/estaticos/intro.png", NULL);
    stcCenario intro;
    setImagemCenario(&intro, telaIntro);
    setPosicaoX(&intro, 0);
    setPosicaoY(&intro, 0);

    BITMAP *telaInstrucoes = load_bitmap("imagens/estaticos/instrucoes.png", NULL);
    stcCenario instrucoes;
    setImagemCenario(&instrucoes, telaInstrucoes);
    setPosicaoX(&instrucoes, 0);
    setPosicaoY(&instrucoes, 0);

    BITMAP *telaPreparacao = load_bitmap("imagens/estaticos/preparacao.png", NULL);
    stcCenario preparacao;
    setImagemCenario(&preparacao, telaPreparacao);
    setPosicaoX(&preparacao, 0);
    setPosicaoY(&preparacao, 0);

    BITMAP *bmpCursorMouse  = load_bitmap("imagens/estaticos/mouse.png",NULL);

    //Navios da tela de preparação
    BITMAP *bmpNavioTamanho2H       = load_bitmap("imagens/estaticos/n2-h.png",NULL);
    stcCenario stcNavioTamanho2H;
    setImagemCenario(&stcNavioTamanho2H, bmpNavioTamanho2H);
    setPosicaoX(&stcNavioTamanho2H, getStcPosicaoX(mtxPosicaoPreparacao,0));
    setPosicaoY(&stcNavioTamanho2H, getStcPosicaoY(mtxPosicaoPreparacao,0));

    BITMAP *bmpNavioTamanho3aH      = load_bitmap("imagens/estaticos/n3b-h.png",NULL);
    stcCenario stcNavioTamanho3aH;
    setImagemCenario(&stcNavioTamanho3aH,bmpNavioTamanho3aH);
    setPosicaoX(&stcNavioTamanho3aH, getStcPosicaoX(mtxPosicaoPreparacao,3));
    setPosicaoY(&stcNavioTamanho3aH, getStcPosicaoY(mtxPosicaoPreparacao,0));

    BITMAP *bmpNavioTamanho3bH      = load_bitmap("imagens/estaticos/n3b-h.png",NULL);
    stcCenario stcNavioTamanho3bH;
    setImagemCenario(&stcNavioTamanho3bH,bmpNavioTamanho3bH);
    setPosicaoX(&stcNavioTamanho3bH, getStcPosicaoX(mtxPosicaoPreparacao,7));
    setPosicaoY(&stcNavioTamanho3bH, getStcPosicaoY(mtxPosicaoPreparacao,0));

    BITMAP *bmpNavioTamanho4H       = load_bitmap("imagens/estaticos/n4-h.png",NULL);
    stcCenario stcNavioTamanho4H;
    setImagemCenario(&stcNavioTamanho4H,bmpNavioTamanho4H);
    setPosicaoX(&stcNavioTamanho4H, getStcPosicaoX(mtxPosicaoPreparacao,0));
    setPosicaoY(&stcNavioTamanho4H, getStcPosicaoY(mtxPosicaoPreparacao,1));


    //Navios da tela de preparacao na vertical.
    BITMAP *bmpNavioTamanho2V       = load_bitmap("imagens/estaticos/n2-v.png",NULL);
    stcCenario stcNavioTamanho2V;
    setImagemCenario(&stcNavioTamanho2V,bmpNavioTamanho2V);

    BITMAP *bmpNavioTamanho3aV      = load_bitmap("imagens/estaticos/n3b-v.png",NULL);
    stcCenario stcNavioTamanho3aV;
    setImagemCenario(&stcNavioTamanho3aV, bmpNavioTamanho3aV);

    BITMAP *bmpNavioTamanho3bV      = load_bitmap("imagens/estaticos/n3b-v.png",NULL);
    stcCenario stcNavioTamanho3bV;
    setImagemCenario(&stcNavioTamanho3bV, bmpNavioTamanho3bV);

    BITMAP *bmpNavioTamanho4V       = load_bitmap("imagens/estaticos/n4-v.png",NULL);
    stcCenario stcNavioTamanho4V;
    setImagemCenario(&stcNavioTamanho4V, bmpNavioTamanho4V);

    // inicia o loop do jogo
    while(!fimJogo){

        while(velocidade > 0) {

              clear(buffer);

              switch(telaAtual){

                     case TELA_INICIO:

                          // Testa se esse som já foi carregado
                          if(!somCarregado){
                             // Inicia o som do cenário
                             //FMOD_System_Release(fmodCenario);
                             //rest(250);
                             //fmodCenario = CarregaSom("sons/_entrada.mp3", FMOD_LOOP_NORMAL);
                             somCarregado = 1;
                          }

                          desenhaCenario(buffer, intro);
                          telaAtual = exibeMenu(buffer);
                          fimJogo = verificaSaida(telaAtual);
                          break;

                     case TELA_PREPARACAO:

                          // Testa se esse som ja foi carregado
                          if(!somCarregado){
                             // Inicia o som do cenário
                             //FMOD_System_Release(fmodCenario);
                             //rest(250);
                             //fmodCenario = CarregaSom("sons/_preparar.mp3", FMOD_LOOP_OFF);
                             somCarregado = 1;
                          }

                          desenhaCenario(buffer, preparacao);
                          desenhaCenario(buffer, stcNavioTamanho4H);
                          desenhaCenario(buffer, stcNavioTamanho3bH);
                          desenhaCenario(buffer, stcNavioTamanho3aH);
                          desenhaCenario(buffer, stcNavioTamanho2H);


                          // Bot�o Voltar
                          if(mouse_b && (mouse_x >= 0 && mouse_x <= 80) && (mouse_y >= 517 && mouse_y <= 550)){
                             somCarregado = 0;
                             telaAtual = TELA_INICIO;
                          }// Bot�o Iniciar
                          else if(mouse_b && (mouse_x >= 395 && mouse_x <= 490) && (mouse_y >= 520 && mouse_y <= 548)){
                             telaAtual = TELA_JOGO;
                          }
                          //fimJogo = verificaSaida(telaAtual);

                          break;

                     case TELA_JOGO:

                          //telaAtual = batalhaNaval();
                          if(!inicioJogo){
                            telaAtual = batalhaNaval();
                            inicioJogo = 1;
                          }
                          /*
                          if(!inicioJogo){

                             somCarregado = 0;
                             // Exibe a mensagem de carregando na tela
                             exibeCarregando(screen);
                             inicioJogo = 1;
                             FMOD_System_Release(fmodCenario);
                          }

                           // Testa se esse som já foi carregado
                          if(!somCarregado){
                             // Inicia o som do cenário
                             rest(250);
                             fmodCenario = CarregaSom("sons/_oceano.mp3", FMOD_LOOP_NORMAL);
                             somCarregado = 1;
                          }

                          blit(gradeTabuleiroCores, buffer,0,0,0,0,JANELA_LARGURA, JANELA_ALTURA);
                          blit(fundoAgua, buffer,0,0,0,0,JANELA_LARGURA, JANELA_ALTURA);
                        */

                          break;

                     case TELA_INSTRUCOES:

                          desenhaCenario(buffer, instrucoes);

                          if(mouse_b && (mouse_x >= 626 && mouse_x <= 756) && (mouse_y >= 504 && mouse_y <= 558)){
                             telaAtual = TELA_INICIO;
                          }

                          break;

                     case TELA_PERDEU:

                          break;

                     case TELA_GANHOU:

                          break;

              }




              /*DefinePosicaoGrade( gradeTabuleiroCores );*/


              // Se pressionou a tecla ESC, entao finaliza o jogo.
              if (key[KEY_ESC]){
                  somCarregado = 0;
                  //FMOD_System_Release(fmodCenario);
                  telaAtual = TELA_INICIO;
              }

              // Controle dos FPS
              velocidade--; // decrementa o contador de velociddade
              frames++;     // incrementa o contador de frames

        }

        // Exibe o contador de frames na tela
        textprintf_ex( buffer, font, 10, 10, makecol(255,0,0), -1, "FPS: %i " , totalFps);

        //Exibe posição X e Y do Mouse
        textprintf_ex( buffer, font, 10, 30, makecol(255,0,0), -1, "Mouse X: %d", mouse_x);
        textprintf_ex( buffer, font, 10, 40, makecol(255,0,0), -1, "Mouse Y: %d", mouse_y);

        // Desenha mouse na tela com imagem
        draw_trans_sprite(buffer, bmpCursorMouse, mouse_x, mouse_y);

        // Copia todo o conteudo desenhado no buffer para a tela.
        blit(buffer, screen, 0, 0, 0, 0, JANELA_LARGURA, JANELA_ALTURA);

    } // finaliza o loop do jogo


    // Executa todas as finalizacoes necessarias
    Finaliza();


  // Encerra
  return 0;

}
END_OF_MAIN();


/*
================================================================================
void Inicializa

Realiza os procedimentos necessarios para iniciar o jogo.
Inicia bibliotecas, realiza verificacoes fundamentais, seta posicoes iniciais...

================================================================================
*/

void Inicializa() {

    allegro_init();
    alpng_init();

    set_color_depth( PROFUNDIDADE_COR );

    if ( PROFUNDIDADE_COR == 32 ){
        set_alpha_blender(); // instala o canal alpha
    }


    if ( set_gfx_mode(GFX_MODO, JANELA_LARGURA, JANELA_ALTURA, 0, 0) < 0 ) {

        allegro_message("Ocorreu um erro ao inicializar no modo grafico!");
        exit(-1);

    }

    if ( install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL ) < 0) {

         allegro_message("Erro ao inicializar o som");
         exit(-1);

    }

    install_timer();
    install_keyboard();
    install_mouse();

    LOCK_VARIABLE(velocidade);
    LOCK_FUNCTION(VelocidadeJogo);
    install_int_ex(VelocidadeJogo, BPS_TO_TIMER(FPS_MAXIMO));

    LOCK_FUNCTION(CalculaFPS);
    install_int_ex(CalculaFPS, BPS_TO_TIMER(1));

    // Cria um bitmap com a dimensao definida
    buffer = create_bitmap(JANELA_LARGURA, JANELA_ALTURA);

    // Define um titulo para a janela
    set_window_title("Batalha Naval - The Arduino Game");

}

/*
================================================================================
void Finaliza

Realiza o procedimento de limpeza para finalizar o jogo.

================================================================================
*/

void Finaliza() {
     clear_keybuf();
     destroy_bitmap(buffer);
     remove_timer();
     remove_mouse();
     remove_sound();
     remove_keyboard();
     allegro_exit();
}

/*
================================================================================
void Velocidade

Incrementa o contador da velocidade do jogo.

================================================================================
*/

void VelocidadeJogo() {
     velocidade++;
}END_OF_FUNCTION(VelocidadeJogo);

/*
================================================================================
void CalculaFPS

Executada uma vez por segundo para atualizar o contador de frames por segundo.

================================================================================
*/

void CalculaFPS() {
     totalFps = frames;
     frames = 0;
} END_OF_FUNCTION(CalculaFPS);

/*
================================================================================
void exibeMenu

Exibe o menu na tela inicial.
Jogar, Como Jogar e Sair

================================================================================
*/
int exibeMenu(BITMAP *local){

    BITMAP *hover = load_bitmap("imagens/estaticos/menu-item-hover.png", NULL);
    int tela = TELA_INICIO;

    // Define uma estrutura para o menu
    stcCenario menu;
    setImagemCenario(&menu, hover);

   if((mouse_x >= 265 && mouse_x <= 385) && (mouse_y >= 339 && mouse_y <= 405)){

      setPosicaoX(&menu, 228);
      setPosicaoY(&menu, 348);

      desenhaCenario(local, menu);


      if(mouse_b & 1){
          //fmodMenu = CarregaSom("sons/_mouse_active.mp3", FMOD_LOOP_OFF);
          //rest(250);
          //FMOD_System_Release(fmodMenu);
          tela = TELA_PREPARACAO;
          somCarregado = 0;
          exibeCarregando(local);
      }

   }
   else if((mouse_x >= 349 && mouse_x <= 608) && (mouse_y >= 426 && mouse_y <= 491)){


      setPosicaoX(&menu, 312);
      setPosicaoY(&menu, 432);
      desenhaCenario(local, menu);
      if(mouse_b & 1){
          //fmodMenu = CarregaSom("sons/_mouse_active.mp3", FMOD_LOOP_OFF);
          //rest(250);
          //FMOD_System_Release(fmodMenu);
          tela = TELA_INSTRUCOES;
      }

   }
   else if((mouse_x >= 585 && mouse_x <= 660) && (mouse_y >= 507 && mouse_y <= 549)){

      setPosicaoX(&menu, 544);
      setPosicaoY(&menu, 512);
      desenhaCenario(local, menu);
      if(mouse_b & 1){
           //fmodMenu = CarregaSom("sons/_mouse_active.mp3", FMOD_LOOP_OFF);
           //rest(250);
           //FMOD_System_Release(fmodMenu);
           tela = TELA_SAIR;
           somCarregado = 0;
      }
   }

   return tela;

}

/*
================================================================================
void exibeInicial

Exibe a tela inicial do jogo com as opções possíveis.
Jogar, Como Jogar e Sair

================================================================================
*/
void exibeInicial(BITMAP *origem, BITMAP *destino){

}

/*
================================================================================
void exibeJogo

Exibe a tela do jogo e da inicio aos procedimentos necessarios.
Monta cenario, verifica posicao das embarcacoes do jogador...

================================================================================
*/
void exibeJogo(){


} END_OF_FUNCTION(exibeJogo);

/*
================================================================================
void exibeInstrucoes

Exibe a tela de instrucoes do jogo e demonstra ao jogador como jogar.

================================================================================
*/
void exibeInstrucoes(){

} END_OF_FUNCTION(exibeInstrucoes);

/*
================================================================================
void exibeResultado

Exibe a tela de resultado do jogo.
Se o jogador venceu exibe a tela de parabenizacao, caso contrario exibe a tela
de derrota.

================================================================================
*/
void exibeResultado(int *resultado){

} END_OF_FUNCTION(exibeResultado);

/*
================================================================================
void DefinePosicaoGrade

.....

================================================================================
*/
void DefinePosicaoGrade(BITMAP *bmp){

     int rgb,
         xIni = 48,
         yIni = 180,
         gradeLinha,
         gradeColuna;

     rgb = getpixel(bmp, mouse_x, mouse_y);
     rgb = (rgb * (-1)) - 16777017;

     if( rgb >= 0 && rgb < 200 ){

         textprintf_ex( buffer, font, 10, 50, makecol(255,0,0), -1, " Cor: %i " , rgb);

     }

} END_OF_FUNCTION(DefinePosicaoGrade);

void exibeCarregando(BITMAP *local){

    BITMAP *img = load_bitmap("imagens/estaticos/carregando.png", NULL);

    // Define uma estrutura para o carregando
    stcCenario carregando;
    setImagemCenario(&carregando, img);
    setPosicaoX(&carregando, 0);
    setPosicaoY(&carregando, 0);
    desenhaCenario(local, carregando);

}

int verificaSaida(int opcao){

    if(opcao != TELA_SAIR){
         return 0;
    }
    else{
         return 1;
    }


}

/*
================================================================================
void carregaAnimacao

Carrega diversos arquivos PNG para se criar uma animacao.
As informa��es necessarias serao passada atraves de par�metros.

================================================================================
*/
void carregaAnimacao(BITMAP *imagem[], char *pasta, int frames){

     int i;
     char caminho[100];

     for(i=0; i < frames; i++){

       sprintf( caminho, "imagens/%s/%i.png",pasta,i);
       imagem[i] = load_bitmap(caminho, NULL);

     }

}


void populaPreparacao(stcPosicao oPosicaoPreparacao[][TAB_DIM]){

int linha  = 0;
int coluna = 0;
int ind    = 0;

    // Inicio do loop que varre as linhas
    for(linha = 0; linha < 10; linha++){
        // Inicio do loop que varre as colunas
        for (coluna = 0;coluna < 10; coluna++){

            if(linha == 0 && coluna == 0){
                setStcPosicaoX(oPosicaoPreparacao, 116, linha, coluna);
                setStcPosicaoY(oPosicaoPreparacao, 169, linha, coluna);
                setStcPosicaoIndice(oPosicaoPreparacao, ind, linha, coluna);
                ind ++;
            }
            else{
                setStcPosicaoX(oPosicaoPreparacao,(oPosicaoPreparacao[linha][coluna-1].posicaoX + 33),linha,coluna);
                setStcPosicaoY(oPosicaoPreparacao,(oPosicaoPreparacao[linha][coluna-1].posicaoY + 33),linha,coluna);
                setStcPosicaoIndice(oPosicaoPreparacao, ind, linha, coluna);
                ind ++;
            }

        }// Fim do loop que varre as colunas

    }// Fim do loop que varre as linhas
}


/*
================================================================================
void batalhaNaval

Carrega diversos arquivos PNG para se criar uma animacao.
As informa��es necessarias serao passada atraves de par�metros.

================================================================================
*/

int batalhaNaval(){

  /**************************************************************************
  Iniciando structs
  ***************************************************************************/

  /**************************************************************************
  Iniciando Bitmaps
  ***************************************************************************/

  clear(buffer);
  //Telas
  BITMAP *bmpFundoAgua     = load_bitmap("imagens/estaticos/agua.png",NULL);
  stcCenario stcFundoAgua;
  setImagemCenario(&stcFundoAgua,bmpFundoAgua);

  BITMAP *bmpMoveAgua      = load_bitmap("imagens/estaticos/agua_2.png",NULL);
  stcCenario stcMoveAgua;
  setImagemCenario(&stcMoveAgua,bmpMoveAgua);

  // Rodap�
  BITMAP *bmpRodapeOpcoes  = load_bitmap("imagens/estaticos/rodape.png",NULL);
  stcCenario stcRodapeOpcoes;
  setImagemCenario(&stcRodapeOpcoes,bmpRodapeOpcoes);
  setPosicaoX(&stcRodapeOpcoes,0);
  setPosicaoY(&stcRodapeOpcoes,458);

  // Tabuleiro
  BITMAP *bmpGradeTabuleiro       = load_bitmap("imagens/estaticos/tabuleiro.png",NULL);
  stcCenario stcGradeTabuleiro;
  setImagemCenario(&stcGradeTabuleiro,bmpGradeTabuleiro);
  setPosicaoX(&stcGradeTabuleiro,0);
  setPosicaoY(&stcGradeTabuleiro,0);

  BITMAP *bmpGradeTabuleiroCores  = load_bitmap("imagens/estaticos/tabuleiro_cores.png",NULL);
  stcCenario stcGradeTabuleiroCores;
  setImagemCenario(&stcGradeTabuleiroCores,bmpGradeTabuleiroCores);
  setPosicaoX(&stcGradeTabuleiroCores,0);
  setPosicaoY(&stcGradeTabuleiroCores,0);

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

  //Navios
  BITMAP *bpmNavioTamanho2H       = load_bitmap("imagens/sprites/barco/normal/navio2H.png",NULL);
  stcCenario stcNavioTamanho2H;
  setImagemCenario(&stcNavioTamanho2H, bpmNavioTamanho2H);
  setPosicaoX(&stcNavioTamanho2H, 48);
  setPosicaoY(&stcNavioTamanho2H, 180);

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

    while(!key[KEY_ESC]){

        desenhaCenario(bmpGradeTabuleiroCores,stcGradeTabuleiroCores);
        blit(bmpFundoAgua, buffer,0,0,0,0,JANELA_LARGURA, JANELA_ALTURA);

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

          draw_trans_sprite(buffer,bmpMoveAgua,-(aguaMovimentoX / aguaMovimentoFator),0);
          desenhaCenario(buffer,stcGradeTabuleiro);
          desenhaCenario(buffer,stcIlhaSuperiorEsquerda);
          desenhaCenario(buffer,stcIlhaSuperiorDireita);
          desenhaCenario(buffer,stcIlhaInferiorEsquerda);
          desenhaCenario(buffer,stcIlhaInferiorDireita);
          desenhaCenario(buffer,stcNavioTamanho2H);
          desenhaCenario(buffer,stcRodapeOpcoes);


          draw_trans_sprite(buffer, bmpCursorMouse, mouse_x, mouse_y);

          textprintf_ex( buffer, font, 10, 10, makecol(255,0,0), -1, "Mouse X: %d", mouse_x);
          textprintf_ex( buffer, font, 10, 20, makecol(255,0,0), -1, "Mouse Y: %d", mouse_y);

          blit(buffer, screen, 0, 0, 0, 0, JANELA_LARGURA, JANELA_ALTURA);

    }
    return 1;
}
