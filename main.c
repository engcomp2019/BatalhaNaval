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
#include "main.h"
//#include "adversario.h"

// Define das telas do jogo
#define TELA_INICIO 0
#define TELA_JOGO  1
#define TELA_INSTRUCOES 2
#define TELA_PERDEU  3
#define TELA_GANHOU 4

// Variaveis Globais
BITMAP  *buffer;

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

void loadBitmap(BITMAP *imagem[], char *pasta, int frames);

typedef struct TRATAEVENTOS {

  int  xCentro,
       yCentro;
  char *indice,
       *tipoNavio;
  int  temNavio,
       explosaoAtivo,
       fogoAtivo,
       destruido;

} trataEventos;

//Função Main
int main(){

    Inicializa();
    // Tamanho do cursor do mouse
    int cursorMouseLargura = 28, //Largura do quadro a ser desenhado na tela
        cursorMouseAltura  = 39; //Altura do quadro a ser desenhado na tela

    char *ponterioPosicao;
    int  pixel;
    int  tipoMouse,trataParada;
    int  linha,coluna;

    // Controla local que a explosão irá ocorrer.
    int LocalExplosaoAguaX = 0,
        LocalExplosaoAguaY = 0,
        LocalExplosaoX     = 0,
        LocalExplosaoY     = 0;

    //Flags para animação de agua.
    int ativaAgua = 0,
        countAgua = 0;
    //Flags para animaçao de explosao
    int ativaExplosao = 0,
        countExplosao = 0;
    //Controla eventos gerais do jogo.
    int flagEventoAtivo = 0;

    //Atribuição de Valores para o vetor de struct
    trataEventos gameTabuleiro[10][10];
    trataEventos gameTabuleiroAdversario[10][10];

    for(linha = 0; linha < 10; linha++){
        for(coluna = 0; coluna < 10; coluna++){

            gameTabuleiro[linha][coluna].temNavio  = 0;
            gameTabuleiro[linha][coluna].destruido = 0;
/*
            switch(linha){
                case 0:
                    sprintf( gameTabuleiro[linha][coluna].indice, "a%i",coluna + 1);
                break;
                case 1:
                    sprintf( gameTabuleiro[linha][coluna].indice, "b%i",coluna + 1);
                break;
                case 2:
                    sprintf( gameTabuleiro[linha][coluna].indice, "c%i",coluna + 1);
                break;
                case 3:
                    sprintf( gameTabuleiro[linha][coluna].indice, "d%i",coluna + 1);
                break;
                case 4:
                    sprintf( gameTabuleiro[linha][coluna].indice, "e%i",coluna + 1);
                break;
                case 5:
                    sprintf( gameTabuleiro[linha][coluna].indice, "f%i",coluna + 1);
                break;
                case 6:
                    sprintf( gameTabuleiro[linha][coluna].indice, "g%i",coluna + 1);
                break;
                case 7:
                    sprintf( gameTabuleiro[linha][coluna].indice, "h%i",coluna + 1);
                break;
                case 8:
                    sprintf( gameTabuleiro[linha][coluna].indice, "i%i",coluna + 1);
                break;
                case 9:
                    sprintf( gameTabuleiro[linha][coluna].indice, "j%i",coluna + 1);
                break;
            }*/
        }
    }

    gameTabuleiro[0][0].indice    = "a1";
    gameTabuleiro[0][0].temNavio  = 1;
    gameTabuleiro[0][0].xCentro   = 60;
    gameTabuleiro[0][0].yCentro   = 40;

    gameTabuleiro[0][1].indice    = "a2";
    gameTabuleiro[0][1].temNavio  = 0;
    gameTabuleiro[0][1].xCentro   = 95;
    gameTabuleiro[0][1].yCentro   = 165;

    gameTabuleiro[1][0].indice    = "b1";
    gameTabuleiro[1][0].temNavio  = 1;
    gameTabuleiro[1][0].destruido = 1;

    gameTabuleiro[1][1].indice    = "b2";

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
    BITMAP *cursorMouseAtaque    = load_bitmap("imagens/estaticos/mouseAtaque.png",NULL);

    //Ilhas
    BITMAP *ilhaSuperiorEsquerda = load_bitmap("imagens/estaticos/ilhas/top_left.png",NULL);
    BITMAP *ilhaSuperiorDireita  = load_bitmap("imagens/estaticos/ilhas/top_right.png",NULL);
    BITMAP *ilhaInferiorEsquerda = load_bitmap("imagens/estaticos/ilhas/bottom_left.png",NULL);
    BITMAP *ilhaInferiorDireita  = load_bitmap("imagens/estaticos/ilhas/bottom_right.png",NULL);

    //Animações
    BITMAP *animaTiroAgua[FPS_TIRO_AGUA];
    loadBitmap(animaTiroAgua, "sprites/agua", FPS_TIRO_AGUA );

    BITMAP *animaExplosao[FPS_EXPLOSAO];
    loadBitmap(animaExplosao, "sprites/explosao", FPS_TIRO_AGUA );

    // inicia o loop do jogo
    while(!fimJogo){

        while(velocidade > 0) {

              clear(buffer);
              draw_trans_sprite(buffer,gradeTabuleiroCores,0,0);
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
              draw_trans_sprite(buffer,gradeTabuleiro,0,0);

            //Teste posição mouse com cores//

            pixel = getpixel(gradeTabuleiroCores, mouse_x, mouse_y);
            ponterioPosicao = verificaLocalMapa(pixel);


            for(linha = 0; linha < 10; linha ++){
                for(coluna = 0; coluna < 10; coluna ++){

                    if(gameTabuleiro[linha][coluna].indice == ponterioPosicao){

                        tipoMouse = 1;//Inicio a váriavel que controla a imagem do mouse.

                        if(gameTabuleiro[linha][coluna].temNavio == 1 && gameTabuleiro[linha][coluna].destruido == 1){
                            //Não deixa atirar no local.
                                tipoMouse = 0;
                            }

                        if(mouse_b & 1 && flagEventoAtivo == 0){
                        //Verifica se houve clique do mouse no quadro(Botão direito).

                            if(gameTabuleiro[linha][coluna].temNavio == 1 && gameTabuleiro[linha][coluna].destruido == 0){
                            //Ativr animação de explosão.
                                ativaExplosao = 1;
                                LocalExplosaoX = gameTabuleiro[linha][coluna].xCentro; //Largura dividido por dois
                                LocalExplosaoY = gameTabuleiro[linha][coluna].yCentro; //altura total
                            }

                            else if(gameTabuleiro[linha][coluna].temNavio == 0){
                            //Ativr animação de explosão na agua.
                                ativaAgua = 1;
                                flagEventoAtivo = 1;
                                LocalExplosaoAguaX = gameTabuleiro[linha][coluna].xCentro - 34; //Largura dividido por dois
                                LocalExplosaoAguaY = gameTabuleiro[linha][coluna].yCentro - 60; //altura total
                            }
                        }
                        textprintf_ex( buffer, font, 10, 20, makecol(255,0,0), -1, "Local: %s" , gameTabuleiro[linha][coluna].indice);
                        trataParada = 1;
                        break;
                    }
                    else{
                        tipoMouse   = 0;
                        trataParada = 0;
                    }
                }

                if(trataParada == 1){
                    break;
                }
            }

            if(countAgua < FPS_TIRO_AGUA && ativaAgua == 1){

                draw_trans_sprite(buffer, animaTiroAgua[countAgua], LocalExplosaoAguaX, LocalExplosaoAguaY);
                countAgua ++;

                if(countAgua == (FPS_TIRO_AGUA -1) ){

                  countAgua  = 0;
                  ativaAgua  = 0;
                  flagEventoAtivo = 0;
                }

            }


            if(countExplosao < FPS_EXPLOSAO && ativaExplosao == 1){

            draw_trans_sprite(buffer, animaExplosao[countExplosao], LocalExplosaoX, LocalExplosaoY);
            countExplosao ++;

                if(countExplosao == (FPS_EXPLOSAO -1) ){

                  countExplosao  = 0;
                  ativaExplosao  = 0;

                }
            }

            if(tipoMouse == 1){
                // Desenha mouse de ataque na tela caso seja inimigo
                draw_trans_sprite(buffer, cursorMouseAtaque, mouse_x, mouse_y);
            }
            else{
                // Desenha mouse na tela com imagem
                draw_trans_sprite(buffer, cursorMouse, mouse_x, mouse_y);
            }

              // Se pressionou a tecla ESC, entao finaliza o jogo.
              if (key[KEY_ESC]) fimJogo = 1;

              // Controle dos FPS
              velocidade--; // decrementa o contador de velociddade
              frames++;     // incrementa o contador de frames

        }

        // Exibe o contador de frames na tela
        textprintf_ex( buffer, font, 10, 10, makecol(255,0,0), -1, "FPS: %i " , totalFps);

        //Exibe posição X e Y do Mouse
        textprintf_ex( buffer, font, 10, 30, makecol(255,0,0), -1, "Mouse X: %d", mouse_x);
        textprintf_ex( buffer, font, 10, 40, makecol(255,0,0), -1, "Mouse Y: %d", mouse_y);

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
     frames   = 0;
} END_OF_FUNCTION(CalculaFPS);








void PopulaTabuleiroAdversario(trataEventos gameTabuleiroAdversario[10][10]){

    int navioGrande  = 5;
    int navioMedio   = 4;
    int naviopequeno = 3;
    int menorNavio   = 2;
    int indSucesso   = 0;

    while(indSucesso == 0){
        indSucesso = PosicionaNavio(navioGrande,gameTabuleiroAdversario);
    }
    indSucesso = 0;

    while(indSucesso == 0){
        indSucesso = PosicionaNavio(navioMedio,gameTabuleiroAdversario);
    }
    indSucesso = 0;

    while(indSucesso == 0){
        indSucesso =  PosicionaNavio(naviopequeno,gameTabuleiroAdversario);
    }
    indSucesso = 0;

    while(indSucesso == 0){
        indSucesso = PosicionaNavio(naviopequeno,gameTabuleiroAdversario);
    }
    indSucesso = 0;

    while(indSucesso == 0){
        indSucesso = PosicionaNavio(menorNavio,gameTabuleiroAdversario);
    }
}


/*
char* EventosAdversario(trataEventos gameTabuleiro[][10], char *ultimoTiro, int statusUltimoTiro){



}
*/

void geraRound(int *linha, int *coluna){

    *linha  = rand() % 9;
    *coluna = rand() % 9;

}
/*
 Função responsavel por setar os flags temNavio da struct, criando assim o posicionamento
dos navios na matriz do adversario.
 A função Rand é utilizada para verificar a posição do navio, e tambem se ele será posicionado
na vertical ou horizontal.
 Os navios sempre são posionados da esquerda para direita, e de cima para baixo.
*/
int PosicionaNavio(trataEventos gameTabuleiroAdversario[][10],int tamanhoNavio){

    const larguraTabuleiro = 10;
    const alturaTabuleiro  = 10;
    int linha, coluna;
    int flagNavio  = 0;
    int indSucesso = 0;
    int valida     = 0,
        verificaPosicao;
    char posicao;

    posicao = (rand() % 1 == 0) ? "v" : "h";

    geraRound(&linha, &coluna);

    while (valida != 1 ){

        if(posicao == "v" && linha < (alturaTabuleiro - tamanhoNavio)){
            valida = 0;
        }else if(posicao == "h" && coluna < (larguraTabuleiro - tamanhoNavio)){
            valida = 0;
        }
        else{
            geraRound(linha, coluna);
        }
    }

    //posicionando navios
    if(gameTabuleiroAdversario[linha][coluna].temNavio == "n"){

        //Pociciona navio na horizontal
        if(posicao == "h"){
            for(coluna; coluna < (coluna + tamanhoNavio); coluna ++){

                if(gameTabuleiroAdversario[linha][coluna].temNavio == "s"){
                    flagNavio = 1;
                    break;
                }
            }

            if (flagNavio == 0){

                for(coluna; coluna < (coluna + tamanhoNavio); coluna ++){

                    gameTabuleiroAdversario[linha][coluna].temNavio = "s";

                }

                indSucesso = 1;
            }
        }
        //Pociciona navio na vertical
        else{

            for(linha;linha < (linha + tamanhoNavio); linha ++){

                if(gameTabuleiroAdversario[linha][coluna].temNavio == "s"){

                    flagNavio = 1;
                    break;
                }
            }
            if (flagNavio == 0){

                for(linha;linha < (linha + tamanhoNavio); linha ++){

                    gameTabuleiroAdversario[linha][coluna].temNavio = "s";
                }

                indSucesso = 1;
            }
        }
    }

    return indSucesso;
}



void loadBitmap(BITMAP *imagem[], char *pasta, int frames){

     int i;
     char caminho[100];

     for(i=0; i < frames; i++){

       sprintf( caminho, "imagens/%s/%i.png",pasta,i);
       imagem[i] = load_bitmap(caminho, NULL);
     }

}
