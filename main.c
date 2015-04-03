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
#include "IndiceTab.h"

// Variaveis Globais
BITMAP  *buffer;



// Define das telas do jogo
#define TELA_INICIO 0
#define TELA_JOGO  1
#define TELA_INSTRUCOES 2
#define TELA_PERDEU  3
#define TELA_GANHOU 4

// Variaveis Gerais
int fimJogo = 0; // Flag que indica se o jogo continua ou nao.
int telaAtual = TELA_INICIO;  // Recebe a tela atual do jogo.

volatile int velocidade = 0; // Recebe o valor incremental da velocidade do jogo.
volatile int totalFps   = 0;	// Recebe o total de frames por segundo (calculado apenas uma vez a cada segundo).
volatile int frames     = 0;	// Recebe a quantidade de frames por segundo (incrementado a cada volta do loop principal ).

// Prototipos das funcoes
void Inicializa();
void Finaliza();
void VelocidadeJogo();
void CalculaFPS();

void exibeInicial();
void exibeJogo();
void exibeInstrucoes();
void exibeResultado(int *resultado);

//Função Main
int main(){

    Inicializa();

    char *testPos;

    //Variaveis de controle da movimentacao da agua
    int aguaMovimentoX     = 0,
        aguaMovimentoFator = 7,
        aguaMovimentoMin   = 0,
        aguaMovimentoMax   = (38 * aguaMovimentoFator),
        flagMaxX           = 0;

    // Fundos de Tela
    BITMAP *fundoAgua            = load_bitmap("imagens/estaticos/agua.png",NULL);
    BITMAP *moveAgua             = load_bitmap("imagens/estaticos/agua_2.png",NULL);

    // Rodapé
    BITMAP *rodapeOpcoes         = load_bitmap("imagens/estaticos/rodape.png",NULL);

    // Tabuleiro
    BITMAP *gradeTabuleiro       = load_bitmap("imagens/estaticos/tabuleiro.png",NULL);
    BITMAP *gradeTabuleiroCores  = load_bitmap("imagens/estaticos/tabuleiro_cores.png",NULL);

    // Cursor do Mouse
    BITMAP *cursorMouse          = load_bitmap("imagens/estaticos/mouse.png",NULL);

    //Ilhas
    BITMAP *ilhaSuperiorEsquerda = load_bitmap("imagens/estaticos/ilhas/top_left.png",NULL);
    BITMAP *ilhaSuperiorDireita  = load_bitmap("imagens/estaticos/ilhas/top_right.png",NULL);
    BITMAP *ilhaInferiorEsquerda = load_bitmap("imagens/estaticos/ilhas/bottom_left.png",NULL);
    BITMAP *ilhaInferiorDireita  = load_bitmap("imagens/estaticos/ilhas/bottom_right.png",NULL);

    // Tamanho do cursor do mouse
    int cursorMouseLargura = 28, //Largura do quadro a ser desenhado na tela
        cursorMouseAltura  = 39; //Altura do quadro a ser desenhado na tela

    // inicia o loop do jogo
    while(!fimJogo){

        while(velocidade > 0) {

              clear(buffer);
              blit(fundoAgua, buffer,0,0,0,0,JANELA_LARGURA, JANELA_ALTURA);

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

              draw_trans_sprite(buffer,ilhaSuperiorEsquerda,-40,0);
              draw_trans_sprite(buffer,ilhaSuperiorDireita,525,0);
              draw_trans_sprite(buffer,ilhaInferiorEsquerda,0,395);
              draw_trans_sprite(buffer,ilhaInferiorDireita,515,375);

              draw_trans_sprite(buffer,rodapeOpcoes,0,458);
              draw_trans_sprite(buffer,gradeTabuleiro,30,40);
              draw_trans_sprite(buffer,gradeTabuleiroCores,25,62);

              // Se pressionou a tecla ESC, entao finaliza o jogo.
              if (key[KEY_ESC]) fimJogo = 1;

              // Controle dos FPS
              velocidade--; // decrementa o contador de velociddade
              frames++;     // incrementa o contador de frames

        }

        // Exibe o contador de frames na tela
        textprintf_ex( buffer, font, 10, 10, makecol(255,0,0), -1, " FPS: %i " , totalFps);


        //Teste posição mouse com cores//

        testPos = verificaLocalMapa(gradeTabuleiroCores, mouse_x, mouse_y);

        textprintf_ex( buffer, font, 10, 20, makecol(255,0,0), -1, " Indice: %c " ,testPos);

        // Desenha mouse na tela com imagem
        draw_trans_sprite(buffer, cursorMouse, mouse_x, mouse_y);

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
void exibeInicial

Exibe a tela inicial do jogo com as opções possíveis.
Jogar, Como Jogar e Sair

================================================================================
*/
void exibeInicial(){}

/*
================================================================================
void exibeJogo

Exibe a tela do jogo e da inicio aos procedimentos necessarios.
Monta cenario, verifica posicao das embarcacoes do jogador...

================================================================================
*/
void exibeJogo(){}

/*
================================================================================
void exibeInstrucoes

Exibe a tela de instrucoes do jogo e demonstra ao jogador como jogar.

================================================================================
*/
void exibeInstrucoes(){}

/*
================================================================================
void exibeResultado

Exibe a tela de resultado do jogo.
Se o jogador venceu exibe a tela de parabenizacao, caso contrario exibe a tela
de derrota.

================================================================================
*/
void exibeResultado(int *resultado){}


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



