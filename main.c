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
#include "jogo.h"
#include "som.h"

//#include <winalleg.h>
//#include "PSerial.h"//Biblioteca para conectar com a porta Serial.
//#include "arduino.h"

// Variaveis Globais
BITMAP  *buffer;
FMOD_SYSTEM *fmodCenario = 0;
FMOD_SYSTEM *fmodAux = 0;
FMOD_SYSTEM *fmodMenu = 0;

// Variaveis Gerais
int inicioJogo     = 0;           // Flag que indica se o jogo foi iniciado
int pauseJogo      = 0;           // Flag que indica se o jogo foi pausado
int fimJogo        = 0;           // Flag que indica se o jogo continua ou nao.
int somCarregado   = 0;           // Flag que indica se o som foi carregado ou nao.
int telaAtual      = TELA_INICIO; // Recebe o valor da tela atual do jogo.
int somLoop        = 0;


volatile int velocidade = 0;    // Recebe o valor incremental da velocidade do jogo.
volatile int totalFps   = 0;	   // Recebe o total de frames por segundo (calculado apenas uma vez a cada segundo).
volatile int frames     = 0;	   // Recebe a quantidade de frames por segundo (incrementado a cada volta do loop principal ).

// Protótipos das funções
void Inicializa();
void Finaliza();
void VelocidadeJogo();
void CalculaFPS();

int definePosicaoGrade( BITMAP *bmp );
void carregaAnimacao( BITMAP *imagem[], char *pasta, int frames );
int  verificaSaida( int opcao );

void populaPreparacao( stcPosicao oPosicaoPreparacao[][TAB_DIM] );
void populaTabuleiroJogo( stcTabuleiroJogo oTabuleiroPlayer[][TAB_DIM] );

void exibeCarregando( BITMAP *local );
int  exibeMenu( BITMAP *local );
int  exibeInicial( BITMAP *origem, BITMAP *destino );
int  exibeJogo();
void exibeInstrucoes( BITMAP *origem, BITMAP *destino );
void exibeResultado( int *resultado );

//Função Main
int main(){

    stcPosicao mtxPosicaoPreparacao[TAB_DIM][TAB_DIM];
    populaPreparacao( mtxPosicaoPreparacao );

    Inicializa();

    // Exibe a mensagem de carregando na tela
    exibeCarregando( screen );

    // Inicialização dos cenários
    BITMAP *telaIntro = load_bitmap( "imagens/estaticos/intro.png", NULL );
    stcCenario intro;
    setImagemCenario( &intro, telaIntro );
    setPosicaoX( &intro, 0 );
    setPosicaoY( &intro, 0 );

    BITMAP *telaInstrucoes = load_bitmap( "imagens/estaticos/instrucoes.png", NULL );
 
    BITMAP *telaPreparacao = load_bitmap( "imagens/estaticos/preparacao.png", NULL );
    stcCenario preparacao;
    setImagemCenario( &preparacao, telaPreparacao );
    setPosicaoX( &preparacao, 0 );
    setPosicaoY( &preparacao, 0 );

    BITMAP *bmpCursorMouse  = load_bitmap( "imagens/estaticos/mouse.png", NULL );

    //Navios da tela de preparação
    BITMAP *bmpNavioTamanho2H       = load_bitmap( "imagens/estaticos/n2-h.png", NULL );
    stcCenario stcNavioTamanho2H;
    setImagemCenario( &stcNavioTamanho2H, bmpNavioTamanho2H );
    setPosicaoX( &stcNavioTamanho2H, getStcPosicaoX( mtxPosicaoPreparacao, P1_NAVIO2H ) );
    setPosicaoY( &stcNavioTamanho2H, getStcPosicaoY( mtxPosicaoPreparacao, P1_NAVIO2H ) );

    BITMAP *bmpNavioTamanho3aH      = load_bitmap( "imagens/estaticos/n3b-h.png", NULL );
    stcCenario stcNavioTamanho3aH;
    setImagemCenario( &stcNavioTamanho3aH, bmpNavioTamanho3aH );
    setPosicaoX( &stcNavioTamanho3aH, getStcPosicaoX( mtxPosicaoPreparacao, P1_NAVIO3AH ) );
    setPosicaoY( &stcNavioTamanho3aH, getStcPosicaoY( mtxPosicaoPreparacao, P1_NAVIO3AH ) );

    BITMAP *bmpNavioTamanho3bH      = load_bitmap( "imagens/estaticos/n3b-h.png", NULL );
    stcCenario stcNavioTamanho3bH;
    setImagemCenario( &stcNavioTamanho3bH, bmpNavioTamanho3bH );
    setPosicaoX( &stcNavioTamanho3bH, getStcPosicaoX( mtxPosicaoPreparacao, P1_NAVIO3BH ) );
    setPosicaoY( &stcNavioTamanho3bH, getStcPosicaoY( mtxPosicaoPreparacao, P1_NAVIO3BH ) );

    BITMAP *bmpNavioTamanho4H       = load_bitmap( "imagens/estaticos/n4-h.png", NULL );
    stcCenario stcNavioTamanho4H;
    setImagemCenario( &stcNavioTamanho4H, bmpNavioTamanho4H );
    setPosicaoX( &stcNavioTamanho4H, getStcPosicaoX( mtxPosicaoPreparacao, P1_NAVIO4H ) );
    setPosicaoY( &stcNavioTamanho4H, getStcPosicaoY( mtxPosicaoPreparacao, P1_NAVIO4H ) );


    //Navios da tela de preparacao na vertical.
    BITMAP *bmpNavioTamanho2V       = load_bitmap( "imagens/estaticos/n2-v.png", NULL );
    stcCenario stcNavioTamanho2V;
    setImagemCenario( &stcNavioTamanho2V, bmpNavioTamanho2V );
    setPosicaoX( &stcNavioTamanho2V, getStcPosicaoX( mtxPosicaoPreparacao, P1_NAVIO2V ) );
    setPosicaoY( &stcNavioTamanho2V, getStcPosicaoY( mtxPosicaoPreparacao, P1_NAVIO2V ) );

    BITMAP *bmpNavioTamanho3aV      = load_bitmap( "imagens/estaticos/n3b-v.png", NULL );
    stcCenario stcNavioTamanho3aV;
    setImagemCenario( &stcNavioTamanho3aV, bmpNavioTamanho3aV );
    setPosicaoX( &stcNavioTamanho3aV, getStcPosicaoX( mtxPosicaoPreparacao, P1_NAVIO3AV ) );
    setPosicaoY( &stcNavioTamanho3aV, getStcPosicaoY( mtxPosicaoPreparacao, P1_NAVIO3AV ) );

    BITMAP *bmpNavioTamanho3bV      = load_bitmap( "imagens/estaticos/n3b-v.png", NULL );
    stcCenario stcNavioTamanho3bV;
    setImagemCenario( &stcNavioTamanho3bV, bmpNavioTamanho3bV );
    setPosicaoX( &stcNavioTamanho3bV, getStcPosicaoX( mtxPosicaoPreparacao, P1_NAVIO3BV ) );
    setPosicaoY( &stcNavioTamanho3bV, getStcPosicaoY( mtxPosicaoPreparacao, P1_NAVIO3BV ) );

    BITMAP *bmpNavioTamanho4V       = load_bitmap( "imagens/estaticos/n4-v.png", NULL );
    stcCenario stcNavioTamanho4V;
    setImagemCenario( &stcNavioTamanho4V, bmpNavioTamanho4V );
    setPosicaoX( &stcNavioTamanho4V, getStcPosicaoX( mtxPosicaoPreparacao, P1_NAVIO4V ) );
    setPosicaoY( &stcNavioTamanho4V, getStcPosicaoY( mtxPosicaoPreparacao, P1_NAVIO4V ) );

    // inicia o loop do jogo
    while( !fimJogo ){

        while( velocidade > 0 ) {

              clear( buffer );

              switch( telaAtual ){

                     case TELA_INICIO:

                          // Testa se esse som já foi carregado
                          if( !somCarregado ){
                             // Inicia o som do cenário
                             FMOD_System_Release( fmodCenario );
                             rest( 250 );
                             //fmodCenario = CarregaSom( "sons/_entrada.mp3", FMOD_LOOP_NORMAL );
                             somCarregado = 1; // Define a flag como som carregado
                          }

                          desenhaCenario( buffer, intro );
                          telaAtual = exibeMenu( buffer );
                          fimJogo = verificaSaida( telaAtual );
                          break;

                     case TELA_PREPARACAO:

                          // Testa se esse som ja foi carregado
                          if( !somCarregado ){
                             // Inicia o som do cenário
                             FMOD_System_Release( fmodCenario );
                             rest( 250 );
                             fmodCenario = CarregaSom( "sons/_preparar.mp3", FMOD_LOOP_OFF );
                             somCarregado = 1; // Define a flag como som carregado
                          }

                          desenhaCenario( buffer, preparacao );
                          /*desenhaCenario( buffer, stcNavioTamanho4H );
                          desenhaCenario( buffer, stcNavioTamanho3bH );
                          desenhaCenario( buffer, stcNavioTamanho3aH );
                          desenhaCenario( buffer, stcNavioTamanho2H );*/

                          desenhaCenario( buffer, stcNavioTamanho4V );
                          desenhaCenario( buffer, stcNavioTamanho3bV );
                          desenhaCenario( buffer, stcNavioTamanho3aV );
                          desenhaCenario( buffer, stcNavioTamanho2V );


                          // Botão Voltar
                          if( mouse_b & 1 && ( mouse_x >= 0 && mouse_x <= 80 ) && ( mouse_y >= 517 && mouse_y <= 550 ) ){
                             somCarregado = 0;        // Define a flag como som descarregado
                             telaAtual = TELA_INICIO; // Define a tela atual com valor da tela de inicio
                          }// Botão Iniciar
                          else if( ( mouse_x >= 395 && mouse_x <= 490 ) && ( mouse_y >= 520 && mouse_y <= 548 ) ){
                             
                             // Altera a imagem do botão iniciar quando passar o cursor do mouse em cima
                             BITMAP *iniHover = load_bitmap( "imagens/estaticos/btn-iniciar.png", NULL );
                             stcCenario iniciar;
                             setImagemCenario( &iniciar, iniHover );
                             setPosicaoX( &iniciar, 386 );
                             setPosicaoY( &iniciar, 516 );
                             desenhaCenario( buffer, iniciar );
                             
                             // Inicia o jogo ao clicar sobre o botão
                             if( mouse_b & 1 ){

                                somCarregado = 0;       // Define a flag como som descarregado
                                telaAtual = TELA_JOGO;  // Define a tela atual com valor da tela do jogo

                             }
                          }

                          break;

                     case TELA_JOGO:
                          
                          if( !inicioJogo ){

                            // Testa se esse som ja foi carregado
                            if( !somCarregado ){

                               // Inicia o som do cenário
                               FMOD_System_Release( fmodCenario );
                               rest( 250 );
                               fmodCenario = CarregaSom( "sons/_oceano2.mp3", FMOD_LOOP_NORMAL );                               
                               somCarregado = 1;
                            }

                            telaAtual = exibeJogo();
                            
                            if( telaAtual == 1 ){
                               inicioJogo = 1;
                            }
                            else if( telaAtual == 0 ){
                               telaAtual = TELA_INICIO; // Define a tela atual com valor da tela de inicio
                            }

                          }                           

                          break;

                     case TELA_INSTRUCOES:
                          
                          clear( buffer );

                          exibeInstrucoes( telaInstrucoes, buffer );

                           if( mouse_b & 1 && ( mouse_x >= 665 && mouse_x <= 790 ) && ( mouse_y >= 517 && mouse_y <= 572 ) ){
                              telaAtual = TELA_INICIO; // Define a tela atual com valor da tela de inicio
                           }

                          break;

                     case TELA_PERDEU:

                          clear( buffer );

                          // Testa se esse som ja foi carregado
                          if( !somCarregado ){
                             // Inicia o som do cenário
                             FMOD_System_Release( fmodCenario );
                             rest( 250 );
                             fmodCenario = CarregaSom( "sons/_preparar.mp3", FMOD_LOOP_OFF );
                             somCarregado = 1;
                          }

                          break;

                     case TELA_GANHOU:

                          break;

              }




              /*definePosicaoGrade( gradeTabuleiroCores );*/


              // Se pressionou a tecla ESC, entao finaliza o jogo.
              if ( key[KEY_ESC] ){
                  somCarregado = 0;
                  fimJogo = 1;
                  //FMOD_System_Release( fmodCenario );
                  telaAtual = TELA_INICIO;
              }

              // Controle dos FPS
              velocidade--; // decrementa o contador de velociddade
              frames++;     // incrementa o contador de frames

        }

        // Exibe o contador de frames na tela
        textprintf_ex( buffer, font, 10, 10, makecol( 255, 0, 0 ), -1, "FPS: %i " , totalFps );

        //Exibe posição X e Y do Mouse
        textprintf_ex( buffer, font, 10, 30, makecol( 255, 0, 0 ), -1, "Mouse X: %d", mouse_x );
        textprintf_ex( buffer, font, 10, 40, makecol( 255, 0, 0 ), -1, "Mouse Y: %d", mouse_y );



        // Desenha mouse na tela com imagem
        draw_trans_sprite( buffer, bmpCursorMouse, mouse_x, mouse_y );

        // Copia todo o conteudo desenhado no buffer para a tela.
        blit( buffer, screen, 0, 0, 0, 0, JANELA_LARGURA, JANELA_ALTURA );

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


    if ( set_gfx_mode( GFX_MODO, JANELA_LARGURA, JANELA_ALTURA, 0, 0 ) < 0 ) {

        allegro_message( "Ocorreu um erro ao inicializar no modo grafico!" );
        exit( -1 );

    }

    if ( install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, NULL ) < 0 ) {

         allegro_message( "Erro ao inicializar o som" );
         exit( -1 );

    }

    install_timer();
    install_keyboard();
    install_mouse();

    LOCK_VARIABLE( velocidade );
    LOCK_FUNCTION( VelocidadeJogo );
    install_int_ex( VelocidadeJogo, BPS_TO_TIMER( FPS_MAXIMO ) );

    LOCK_FUNCTION( CalculaFPS );
    install_int_ex( CalculaFPS, BPS_TO_TIMER( 1 ) );

    // Cria um bitmap com a dimensao definida
    buffer = create_bitmap( JANELA_LARGURA, JANELA_ALTURA );

    // Define um titulo para a janela
    set_window_title( "Batalha Naval - The Arduino Game" );

    // Inicia a comunicação com a porta serial desejada.
    //inicializaSerial( 0, "COM3", 9600 );

}

/*
================================================================================
void Finaliza

Realiza o procedimento de limpeza para finalizar o jogo.

================================================================================
*/

void Finaliza(){

    // Finaliza todas as comunicações com a porta serial.
    //finalizaTodasPortasSeriais();

     clear_keybuf();
     destroy_bitmap( buffer );
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
}END_OF_FUNCTION( VelocidadeJogo );

/*
================================================================================
void CalculaFPS

Executada uma vez por segundo para atualizar o contador de frames por segundo.

================================================================================
*/

void CalculaFPS() {
     totalFps = frames;
     frames = 0;
} END_OF_FUNCTION( CalculaFPS );

/*
================================================================================
void exibeMenu

Exibe o menu na tela inicial.
Jogar, Como Jogar e Sair

================================================================================
*/
int exibeMenu( BITMAP *local ){

    BITMAP *hover = load_bitmap( "imagens/estaticos/menu-item-hover.png", NULL );
    int tela = TELA_INICIO;

    // Define uma estrutura para o menu
    stcCenario menu;
    setImagemCenario( &menu, hover );

   if( ( mouse_x >= 265 && mouse_x <= 385 ) && ( mouse_y >= 339 && mouse_y <= 405 ) ){

      setPosicaoX( &menu, 228 );
      setPosicaoY( &menu, 348 );

      desenhaCenario( local, menu );


      if( mouse_b & 1 ){
          //fmodMenu = CarregaSom( "sons/_mouse_active.mp3", FMOD_LOOP_OFF );
          //rest( 250 );
          //FMOD_System_Release( fmodMenu );
          tela = TELA_PREPARACAO;
          somCarregado = 0;
          exibeCarregando( local );
      }

   }
   else if( ( mouse_x >= 344 && mouse_x <= 562 ) && ( mouse_y >= 426 && mouse_y <= 480 ) ){


      setPosicaoX( &menu, 312 );
      setPosicaoY( &menu, 432 );
      desenhaCenario( local, menu );
      if( mouse_b & 1 ){
          //fmodMenu = CarregaSom( "sons/_mouse_active.mp3", FMOD_LOOP_OFF );
          //rest( 250 );
          //FMOD_System_Release( fmodMenu );
          tela = TELA_INSTRUCOES;
      }

   }
   else if( ( mouse_x >= 523 && mouse_x <= 605 ) && ( mouse_y >= 504 && mouse_y <= 558 ) ){

      setPosicaoX( &menu, 486 );
      setPosicaoY( &menu, 512 );
      desenhaCenario( local, menu );
      if( mouse_b & 1 ){
           //fmodMenu = CarregaSom( "sons/_mouse_active.mp3", FMOD_LOOP_OFF );
           //rest( 250 );
           //FMOD_System_Release( fmodMenu );
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
int exibeInicial( BITMAP *origem, BITMAP *destino ){

}

/*
================================================================================
void exibeJogo

Exibe a tela do jogo e da inicio a batalha.

================================================================================
*/
int exibeJogo(){

  /**************************************************************************
  Matriz para controle
  ***************************************************************************/
  stcTabuleiroJogo stcTabuleiroPlayer[TAB_DIM][TAB_DIM];
  stcTabuleiroJogo stcTabuleiroAdversario[TAB_DIM][TAB_DIM];

  populaTabuleiroJogo( stcTabuleiroPlayer );
  populaTabuleiroJogo( stcTabuleiroAdversario );

  /**************************************************************************
  Iniciando Bitmaps estáticos do cenário
  ***************************************************************************/

  clear( buffer );

  // Água 
  BITMAP *bmpFundoAgua     = load_bitmap( "imagens/estaticos/agua.png", NULL );
  stcCenario stcFundoAgua;
  setImagemCenario( &stcFundoAgua, bmpFundoAgua );
  setPosicaoX( &stcFundoAgua, 0 );
  setPosicaoY( &stcFundoAgua, 0 );

  BITMAP *bmpMoveAgua      = load_bitmap( "imagens/estaticos/agua_2.png", NULL );
  stcCenario stcMoveAgua;
  setImagemCenario( &stcMoveAgua, bmpMoveAgua );

  // Rodapé
  BITMAP *bmpRodapeOpcoes  = load_bitmap( "imagens/estaticos/rodape.png", NULL );
  stcCenario stcRodapeOpcoes;
  setImagemCenario( &stcRodapeOpcoes, bmpRodapeOpcoes );
  setPosicaoX( &stcRodapeOpcoes, 0 );
  setPosicaoY( &stcRodapeOpcoes, 458 );

  // Menu Sair
  BITMAP *bmpMenuSair  = load_bitmap( "imagens/estaticos/sair.png", NULL );
  stcCenario stcMenuSair;
  setImagemCenario( &stcMenuSair, bmpMenuSair );
  setPosicaoX( &stcMenuSair, 0 );
  setPosicaoY( &stcMenuSair, 0 );

  BITMAP *bmpItemHoverSair  = load_bitmap( "imagens/estaticos/sair-item-hover.png", NULL );
  stcCenario stcItemHoverSair;
  setImagemCenario( &stcItemHoverSair, bmpItemHoverSair );

  // Tabuleiro
  BITMAP *bmpGradeTabuleiro       = load_bitmap( "imagens/estaticos/tabuleiro.png", NULL );
  stcCenario stcGradeTabuleiro;
  setImagemCenario( &stcGradeTabuleiro, bmpGradeTabuleiro );
  setPosicaoX( &stcGradeTabuleiro, 0 );
  setPosicaoY( &stcGradeTabuleiro, 0 );

  BITMAP *bmpGradeTabuleiroCores  = load_bitmap( "imagens/estaticos/tabuleiro_cores.png", NULL );
  stcCenario stcGradeTabuleiroCores;
  setImagemCenario( &stcGradeTabuleiroCores, bmpGradeTabuleiroCores );
  setPosicaoX( &stcGradeTabuleiroCores, 0 );
  setPosicaoY( &stcGradeTabuleiroCores, 0 );

  // Ilhas
  BITMAP *bmpIlhaSuperiorEsquerda = load_bitmap( "imagens/estaticos/ilhas/top_left.png", NULL );
  stcCenario stcIlhaSuperiorEsquerda;
  setImagemCenario( &stcIlhaSuperiorEsquerda, bmpIlhaSuperiorEsquerda );
  setPosicaoX( &stcIlhaSuperiorEsquerda, -40 );
  setPosicaoY( &stcIlhaSuperiorEsquerda, 0 );

  BITMAP *bmpIlhaSuperiorDireita  = load_bitmap( "imagens/estaticos/ilhas/top_right.png", NULL );
  stcCenario stcIlhaSuperiorDireita;
  setImagemCenario( &stcIlhaSuperiorDireita, bmpIlhaSuperiorDireita );
  setPosicaoX( &stcIlhaSuperiorDireita, 525 );
  setPosicaoY( &stcIlhaSuperiorDireita, 0 );

  BITMAP *bmpIlhaInferiorEsquerda = load_bitmap( "imagens/estaticos/ilhas/bottom_left.png", NULL );
  stcCenario stcIlhaInferiorEsquerda;
  setImagemCenario( &stcIlhaInferiorEsquerda, bmpIlhaInferiorEsquerda );
  setPosicaoX( &stcIlhaInferiorEsquerda, 0 );
  setPosicaoY( &stcIlhaInferiorEsquerda, 395 );

  BITMAP *bmpIlhaInferiorDireita  = load_bitmap( "imagens/estaticos/ilhas/bottom_right.png", NULL );
  stcCenario stcIlhaInferiorDireita;
  setImagemCenario( &stcIlhaInferiorDireita, bmpIlhaInferiorDireita );
  setPosicaoX( &stcIlhaInferiorDireita, 515 );
  setPosicaoY( &stcIlhaInferiorDireita, 375 );

  // Navios Jogador - Horizontal
  BITMAP *bpmNavioTamanho2HP1       = load_bitmap( "imagens/sprites/barco/normal/navio2H_P1.png", NULL );
  stcCenario stcNavioTamanho2HP1;
  setImagemCenario( &stcNavioTamanho2HP1, bpmNavioTamanho2HP1 );
  setPosicaoX( &stcNavioTamanho2HP1, getStcTabuleiroJogoX( stcTabuleiroPlayer, P1_NAVIO2H ) );
  setPosicaoY( &stcNavioTamanho2HP1, getStcTabuleiroJogoY( stcTabuleiroPlayer, P1_NAVIO2H ) );

  BITMAP *bpmNavioTamanho3aHP1       = load_bitmap( "imagens/sprites/barco/normal/navio3aH_P1.png", NULL );
  stcCenario stcNavioTamanho3aHP1;
  setImagemCenario( &stcNavioTamanho3aHP1, bpmNavioTamanho3aHP1 );
  setPosicaoX( &stcNavioTamanho3aHP1, getStcTabuleiroJogoX( stcTabuleiroPlayer, P1_NAVIO3AH ) );
  setPosicaoY( &stcNavioTamanho3aHP1, getStcTabuleiroJogoY( stcTabuleiroPlayer, P1_NAVIO3AH ) );

  BITMAP *bpmNavioTamanho3bHP1       = load_bitmap( "imagens/sprites/barco/normal/navio3bH_P1.png", NULL );
  stcCenario stcNavioTamanho3bHP1;
  setImagemCenario( &stcNavioTamanho3bHP1, bpmNavioTamanho3bHP1 );
  setPosicaoX( &stcNavioTamanho3bHP1, getStcTabuleiroJogoX( stcTabuleiroPlayer, P1_NAVIO3BH ) );
  setPosicaoY( &stcNavioTamanho3bHP1, getStcTabuleiroJogoY( stcTabuleiroPlayer, P1_NAVIO3BH ) );

  BITMAP *bpmNavioTamanho4HP1       = load_bitmap( "imagens/sprites/barco/normal/navio4H_P1.png", NULL );
  stcCenario stcNavioTamanho4HP1;
  setImagemCenario( &stcNavioTamanho4HP1, bpmNavioTamanho4HP1 );
  setPosicaoX( &stcNavioTamanho4HP1, getStcTabuleiroJogoX( stcTabuleiroPlayer, P1_NAVIO4H ) );
  setPosicaoY( &stcNavioTamanho4HP1, getStcTabuleiroJogoY( stcTabuleiroPlayer, P1_NAVIO4H ) );

  // Navios Adversário - Horizontal
  BITMAP *bpmNavioTamanho2HP2       = load_bitmap( "imagens/sprites/barco/normal/navio2H_P2.png", NULL );
  stcCenario stcNavioTamanho2HP2;
  setImagemCenario( &stcNavioTamanho2HP2, bpmNavioTamanho2HP2 );
  setPosicaoX( &stcNavioTamanho2HP2, getStcTabuleiroJogoX( stcTabuleiroAdversario, P2_NAVIO2H ) );
  setPosicaoY( &stcNavioTamanho2HP2, getStcTabuleiroJogoY( stcTabuleiroAdversario, P2_NAVIO2H ) );

  BITMAP *bpmNavioTamanho3aHP2       = load_bitmap( "imagens/sprites/barco/normal/navio3aH_P2.png", NULL );
  stcCenario stcNavioTamanho3aHP2;
  setImagemCenario( &stcNavioTamanho3aHP2, bpmNavioTamanho3aHP2 );
  setPosicaoX( &stcNavioTamanho3aHP2, getStcTabuleiroJogoX( stcTabuleiroAdversario, P2_NAVIO3AH ) );
  setPosicaoY( &stcNavioTamanho3aHP2, getStcTabuleiroJogoY( stcTabuleiroAdversario, P2_NAVIO3AH ) );

  BITMAP *bpmNavioTamanho3bHP2       = load_bitmap( "imagens/sprites/barco/normal/navio3bH_P2.png", NULL );
  stcCenario stcNavioTamanho3bHP2;
  setImagemCenario( &stcNavioTamanho3bHP2, bpmNavioTamanho3bHP2 );
  setPosicaoX( &stcNavioTamanho3bHP2, getStcTabuleiroJogoX( stcTabuleiroAdversario, P2_NAVIO3BH ) );
  setPosicaoY( &stcNavioTamanho3bHP2, getStcTabuleiroJogoY( stcTabuleiroAdversario, P2_NAVIO3BH ) );

  BITMAP *bpmNavioTamanho4HP2       = load_bitmap( "imagens/sprites/barco/normal/navio4H_P2.png", NULL );
  stcCenario stcNavioTamanho4HP2;
  setImagemCenario( &stcNavioTamanho4HP2, bpmNavioTamanho4HP2 );
  setPosicaoX( &stcNavioTamanho4HP2, getStcTabuleiroJogoX( stcTabuleiroAdversario, P2_NAVIO4H ) );
  setPosicaoY( &stcNavioTamanho4HP2, getStcTabuleiroJogoY( stcTabuleiroAdversario, P2_NAVIO4H ) );


  // Navios Jogador - Vertical
  BITMAP *bmpNavioTamanho2VP1       = load_bitmap( "imagens/sprites/barco/normal/navio2V_P1.png", NULL );
  stcCenario stcNavioTamanho2VP1;
  setImagemCenario( &stcNavioTamanho2VP1, bmpNavioTamanho2VP1 );
  setPosicaoX( &stcNavioTamanho2VP1, getStcTabuleiroJogoX( stcTabuleiroPlayer, P1_NAVIO2V ) );
  setPosicaoY( &stcNavioTamanho2VP1, getStcTabuleiroJogoY( stcTabuleiroPlayer, P1_NAVIO2V ) );

  BITMAP *bmpNavioTamanho3aVP1      = load_bitmap( "imagens/sprites/barco/normal/navio3aV_P1.png", NULL );
  stcCenario stcNavioTamanho3aVP1;
  setImagemCenario( &stcNavioTamanho3aVP1, bmpNavioTamanho3aVP1 );
  setPosicaoX( &stcNavioTamanho3aVP1, getStcTabuleiroJogoX( stcTabuleiroPlayer, P1_NAVIO3AV ) );
  setPosicaoY( &stcNavioTamanho3aVP1, getStcTabuleiroJogoY( stcTabuleiroPlayer, P1_NAVIO3AV ) );

  BITMAP *bmpNavioTamanho3bVP1      = load_bitmap( "imagens/sprites/barco/normal/navio3bV_P1.png", NULL );
  stcCenario stcNavioTamanho3bVP1;
  setImagemCenario( &stcNavioTamanho3bVP1, bmpNavioTamanho3bVP1 );
  setPosicaoX( &stcNavioTamanho3bVP1, getStcTabuleiroJogoX( stcTabuleiroPlayer, P1_NAVIO3BV ) );
  setPosicaoY( &stcNavioTamanho3bVP1, getStcTabuleiroJogoY( stcTabuleiroPlayer, P1_NAVIO3BV ) );

  BITMAP *bmpNavioTamanho4VP1       = load_bitmap( "imagens/sprites/barco/normal/navio4V_P1.png", NULL );
  stcCenario stcNavioTamanho4VP1;
  setImagemCenario( &stcNavioTamanho4VP1, bmpNavioTamanho4VP1 );
  setPosicaoX( &stcNavioTamanho4VP1, getStcTabuleiroJogoX( stcTabuleiroPlayer, P1_NAVIO4V ) );
  setPosicaoY( &stcNavioTamanho4VP1, getStcTabuleiroJogoY( stcTabuleiroPlayer, P1_NAVIO4V ) );

  // Navios Adversário - Vertical
  BITMAP *bmpNavioTamanho2VP2       = load_bitmap( "imagens/sprites/barco/normal/navio2V_P2.png", NULL );
  stcCenario stcNavioTamanho2VP2;
  setImagemCenario( &stcNavioTamanho2VP2, bmpNavioTamanho2VP2 );
  setPosicaoX( &stcNavioTamanho2VP2, getStcTabuleiroJogoX( stcTabuleiroAdversario, P2_NAVIO2V ) );
  setPosicaoY( &stcNavioTamanho2VP2, getStcTabuleiroJogoY( stcTabuleiroAdversario, P2_NAVIO2V ) );

  BITMAP *bmpNavioTamanho3aVP2      = load_bitmap( "imagens/sprites/barco/normal/navio3aV_P2.png", NULL );
  stcCenario stcNavioTamanho3aVP2;
  setImagemCenario( &stcNavioTamanho3aVP2, bmpNavioTamanho3aVP2 );
  setPosicaoX( &stcNavioTamanho3aVP2, getStcTabuleiroJogoX( stcTabuleiroAdversario, P2_NAVIO3AV ) );
          setPosicaoY( &stcNavioTamanho3aVP2, getStcTabuleiroJogoY( stcTabuleiroAdversario, P2_NAVIO3AV ) );

  BITMAP *bmpNavioTamanho3bVP2      = load_bitmap( "imagens/sprites/barco/normal/navio3bV_P2.png", NULL );
  stcCenario stcNavioTamanho3bVP2;
  setImagemCenario( &stcNavioTamanho3bVP2, bmpNavioTamanho3bVP2 );
  setPosicaoX( &stcNavioTamanho3bVP2, getStcTabuleiroJogoX( stcTabuleiroAdversario, P2_NAVIO3BV ) );
  setPosicaoY( &stcNavioTamanho3bVP2, getStcTabuleiroJogoY( stcTabuleiroAdversario, P2_NAVIO3BV ) );

  BITMAP *bmpNavioTamanho4VP2       = load_bitmap( "imagens/sprites/barco/normal/navio4V_P2.png", NULL );
  stcCenario stcNavioTamanho4VP2;
  setImagemCenario( &stcNavioTamanho4VP2, bmpNavioTamanho4VP2 );
  setPosicaoX( &stcNavioTamanho4VP2, getStcTabuleiroJogoX( stcTabuleiroAdversario, P2_NAVIO4V ) );
  setPosicaoY( &stcNavioTamanho4VP2, getStcTabuleiroJogoY( stcTabuleiroAdversario, P2_NAVIO4V ) );

  //Mouse
  BITMAP *bmpCursorMouse          = load_bitmap( "imagens/estaticos/mouse.png", NULL );
  stcCenario stcCursorMouse;
  setImagemCenario( &stcCursorMouse, bmpCursorMouse );

  BITMAP *bmpCursorMouseAtaque    = load_bitmap( "imagens/estaticos/mouseAtaque.png", NULL );
  stcCenario stcCursorMouseAtaque;
  setImagemCenario( &stcCursorMouseAtaque, bmpCursorMouseAtaque );
  
  // Variaveis de controle do movimento da água
  int aguaMovimentoX     = 0, 
      aguaMovimentoFator = 7, 
      aguaMovimentoMin   = 0, 
      aguaMovimentoMax   = ( 38 * aguaMovimentoFator ), 
      flagMaxX           = 0;

  int tipoMouse, posicaoGrade;

    while( !fimJogo ){
      
          // Controle de movimento da água
          if( aguaMovimentoX <= aguaMovimentoMax && flagMaxX == 0 ){
              if( aguaMovimentoX == aguaMovimentoMax ){
                  flagMaxX = 1;
              }
              aguaMovimentoX++;
          }
          else if( aguaMovimentoX >= aguaMovimentoMin && flagMaxX == 1 ){
          
              if( aguaMovimentoX == aguaMovimentoMin ){
                  flagMaxX = 0;
              }
              aguaMovimentoX--;
          }

          // Inicia o som a cada 250 milissegundos
          if( somLoop == 250 ){
             FMOD_System_Release( fmodAux );
             rest( 250 );
             fmodAux = CarregaSom( "sons/_gaivota.mp3", FMOD_LOOP_OFF );
             somLoop = 0;
          }

          // Insere o tabuleiro de cores para controle de posição no cenário
          desenhaCenario( buffer, stcGradeTabuleiroCores );          

          // Insere a água de fundo no cenário 
          desenhaCenario( buffer, stcFundoAgua );

          // Insere a água em movimento no cenário
          setPosicaoX( &stcMoveAgua, -( aguaMovimentoX / aguaMovimentoFator ) );
          desenhaCenario( buffer, stcMoveAgua );
          
          desenhaCenario( buffer, stcGradeTabuleiro );
          desenhaCenario( buffer, stcIlhaSuperiorEsquerda );
          desenhaCenario( buffer, stcIlhaSuperiorDireita );
          desenhaCenario( buffer, stcIlhaInferiorEsquerda );
          desenhaCenario( buffer, stcIlhaInferiorDireita );
          desenhaCenario( buffer, stcNavioTamanho2VP1 );
          desenhaCenario( buffer, stcNavioTamanho3aHP1 );
          desenhaCenario( buffer, stcNavioTamanho3bVP1 );
          desenhaCenario( buffer, stcNavioTamanho4HP1 );

          //Desenha Navios 
          desenhaCenario( buffer, stcNavioTamanho2HP2 );
          desenhaCenario( buffer, stcNavioTamanho3aVP2 );
          desenhaCenario( buffer, stcNavioTamanho3bHP2 );
          desenhaCenario( buffer, stcNavioTamanho4VP2 );
          desenhaCenario( buffer, stcRodapeOpcoes );

          

          // Área do botão Menu
          if( mouse_b & 1 && ( mouse_x >= 671 && mouse_x <= 777 ) && ( mouse_y >= 544 && mouse_y <= 574 ) ){
              pauseJogo = 1;
          }

          posicaoGrade = definePosicaoGrade( bmpGradeTabuleiroCores );
          
          // Desenha na tela o cursor do mouse
          if( posicaoGrade >= 100 && posicaoGrade <= 199 ){
             tipoMouse = 1;
          }
          else{
             tipoMouse = 0;
          }

          // Após clicar no botão Menu, será exibido um alerta.
          if( pauseJogo ){

             desenhaCenario( buffer, stcMenuSair ); 
             tipoMouse = 0;

              // Opção Sim - Volta para o menu inicial
              if( ( mouse_x >= 334 && mouse_x <= 379 ) && ( mouse_y >= 303 && mouse_y <= 333 ) ){
                   setPosicaoX( &stcItemHoverSair, 312 );
                   setPosicaoY( &stcItemHoverSair, 312 );  
                   desenhaCenario( buffer, stcItemHoverSair ); 
                   
                   if( mouse_b & 1 ){
                      clear( buffer );
                      pauseJogo = 0;
                      somCarregado = 0;
                      return 0;
                   }         
              } 
              // Opção Não - Volta para o jogo
              else if( ( mouse_x >= 432 && mouse_x <= 477 ) && ( mouse_y >= 303 && mouse_y <= 333 ) ){
                   setPosicaoX( &stcItemHoverSair, 411 );
                   setPosicaoY( &stcItemHoverSair, 312 );  
                   desenhaCenario( buffer, stcItemHoverSair ); 

                   if( mouse_b & 1 ){
                      pauseJogo = 0;
                   }         
              }         
          }
          
                    

          somLoop++;
          
          if(tipoMouse == 0){
             draw_trans_sprite( buffer, bmpCursorMouse, mouse_x, mouse_y );
          }
          else{
             draw_trans_sprite( buffer, bmpCursorMouseAtaque, mouse_x, mouse_y );  
          }
          
          // Desenha na tela tudo que foi inserido no buffer.
          blit( buffer, screen, 0, 0, 0, 0, JANELA_LARGURA, JANELA_ALTURA );
    }
    return 1;

} END_OF_FUNCTION( exibeJogo );

/*
================================================================================
void exibeInstrucoes

Exibe a tela de instrucoes do jogo e demonstra ao jogador como jogar.

================================================================================
*/
void exibeInstrucoes( BITMAP *origem, BITMAP *destino ){

     stcCenario instrucoes;
     setImagemCenario( &instrucoes, origem );
     setPosicaoX( &instrucoes, 0 );
     setPosicaoY( &instrucoes, 0 );
     desenhaCenario( destino, instrucoes );

} END_OF_FUNCTION( exibeInstrucoes );

/*
================================================================================
void exibeResultado

Exibe a tela de resultado do jogo.
Se o jogador venceu exibe a tela de parabenizacao, caso contrario exibe a tela
de derrota.

================================================================================
*/
void exibeResultado( int *resultado ){

} END_OF_FUNCTION( exibeResultado );

/*
================================================================================
void definePosicaoGrade

.....

================================================================================
*/
int definePosicaoGrade( BITMAP *bmp ){

     int posicao, 
         rgb, 
         xIni = 48, 
         yIni = 180, 
         gradeLinha, 
         gradeColuna;
     
     // Recebe o valor do RGB da localização do mouse na imagem definida.
     rgb = getpixel( bmp, mouse_x, mouse_y );

     // Recebe o valor de rgb e transforma em um valor que vai de 0 a 199
     posicao = ( rgb * ( -1 ) ) - 16777017;
     
     // Exibe o valor na tela da posição atual
     if( posicao >= 0 && posicao <= 199 )
     textprintf_ex( buffer, font, 10, 50, makecol( 255, 0, 0 ), -1, " Posicao: %i " , posicao );

     return posicao;

} END_OF_FUNCTION( definePosicaoGrade );

void exibeCarregando( BITMAP *local ){

    BITMAP *img = load_bitmap( "imagens/estaticos/carregando.png", NULL );

    // Define uma estrutura para o carregando
    stcCenario carregando;
    setImagemCenario( &carregando, img );
    setPosicaoX( &carregando, 0 );
    setPosicaoY( &carregando, 0 );
    desenhaCenario( local, carregando );

}

int verificaSaida( int opcao ){

    if( opcao != TELA_SAIR ){
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
As informações necessarias serao passada atraves de parâmetros.

================================================================================
*/
void carregaAnimacao( BITMAP *imagem[], char *pasta, int frames ){

     int i;
     char caminho[100];

     for( i=0; i < frames; i++ ){

       sprintf( caminho, "imagens/%s/%i.png", pasta, i );
       imagem[i] = load_bitmap( caminho, NULL );

     }
}


/*
================================================================================
void populaPreparacao

 Está função é responsavel por popular a matriz com as posições de X e Y
e os indices de cada uma das posições de acordo com a imagem do tabuleiro
da tela de preparação.

================================================================================
*/

void populaPreparacao( stcPosicao oPosicaoPreparacao[][TAB_DIM] ){

int linha  = 0;
int coluna = 0;
int ind    = 0;

    // Inicio do loop que varre as linhas
    for( linha = 0; linha < 10; linha++ ){

        if( linha != 0 && coluna == 0 ){
            ind ++;
            setStcPosicaoX( oPosicaoPreparacao, oPosicaoPreparacao[linha -1][coluna].posicaoX, linha, coluna );
            setStcPosicaoY( oPosicaoPreparacao, ( oPosicaoPreparacao[linha -1][coluna].posicaoY + 33 ), linha, coluna );
            setStcPosicaoIndice( oPosicaoPreparacao, ind, linha, coluna );
        }

        // Inicio do loop que varre as colunas
        for ( coluna = 0;coluna < 10; coluna++ ){

            if( linha == 0 && coluna == 0 ){
                setStcPosicaoX( oPosicaoPreparacao, 116, linha, coluna );
                setStcPosicaoY( oPosicaoPreparacao, 169, linha, coluna );
                setStcPosicaoIndice( oPosicaoPreparacao, ind, linha, coluna );
            }
            else if( coluna != 0 ){
                ind ++;
                setStcPosicaoX( oPosicaoPreparacao, ( oPosicaoPreparacao[linha][coluna-1].posicaoX + 33 ), linha, coluna );
                setStcPosicaoY( oPosicaoPreparacao, ( oPosicaoPreparacao[linha][coluna-1].posicaoY ), linha, coluna );
                setStcPosicaoIndice( oPosicaoPreparacao, ind, linha, coluna );
            }

        }// Fim do loop que varre as colunas

        coluna = 0;

    }// Fim do loop que varre as linhas
}


/*
================================================================================
void populaTabuleiroJogo

 Está função é responsavel por popular a matriz com as posições de X e Y
e os indices de cada uma das posições de acordo com a imagem dos tabuleiros
principais do jogo.

================================================================================
*/

void populaTabuleiroJogo( stcTabuleiroJogo oTabuleiroJogo[][TAB_DIM] ){

int linha  = 0;
int coluna = 0;
int ind    = 0;

    // Inicio do loop que varre as linhas
    for( linha = 0; linha < 10; linha++ ){

        if( linha != 0 && coluna == 0 ){

            ind++;
            setStcTabuleiroJogoX( oTabuleiroJogo, ( oTabuleiroJogo[linha -1][coluna].posicaoX + 24 ), linha, coluna );
            setStcTabuleiroJogoY( oTabuleiroJogo, ( oTabuleiroJogo[linha -1][coluna].posicaoY + 15 ), linha, coluna );
            setStcTabuleiroJogoIndice( oTabuleiroJogo, ind, linha, coluna );

        }

        // Inicio do loop que varre as colunas
        for ( coluna = 0;coluna < 10; coluna++ ){
            if( linha == 0 && coluna == 0 ){

                setStcTabuleiroJogoX( oTabuleiroJogo, 48, linha, coluna );
                setStcTabuleiroJogoY( oTabuleiroJogo, 30, linha, coluna );
                setStcTabuleiroJogoIndice( oTabuleiroJogo, ind, linha, coluna );
            }
            else if( coluna != 0 ){

                ind ++;
                setStcTabuleiroJogoX( oTabuleiroJogo, ( oTabuleiroJogo[linha][coluna-1].posicaoX + 24 ), linha, coluna );
                setStcTabuleiroJogoY( oTabuleiroJogo, ( oTabuleiroJogo[linha][coluna-1].posicaoY - 15 ), linha, coluna );
                setStcTabuleiroJogoIndice( oTabuleiroJogo, ind, linha, coluna );
            }

        }// Fim do loop que varre as colunas
        coluna = 0;

    }// Fim do loop que varre as linhas
}
