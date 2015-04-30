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
#include "som.h"

// Variaveis Globais
BITMAP  *buffer;
FMOD_SYSTEM *fmodSis = 0;

// Variaveis Gerais
int fimJogo        = 0;           // Flag que indica se o jogo continua ou nao.
int somCarregado   = 0;           // Flag que indica se o som foi carregado ou nao.
int telaAtual      = TELA_INICIO; // Recebe o valor da tela atual do jogo.
    
volatile int velocidade = 0; // Recebe o valor incremental da velocidade do jogo.
volatile int totalFps   = 0;	// Recebe o total de frames por segundo (calculado apenas uma vez a cada segundo).
volatile int frames     = 0;	// Recebe a quantidade de frames por segundo (incrementado a cada volta do loop principal ).

// Protótipos das funções
void Inicializa();
void Finaliza();
void VelocidadeJogo();
void CalculaFPS();

void exibeCarregando();
int  exibeMenu(BITMAP *local);
void exibeInicial(BITMAP *origem, BITMAP *destino);
void exibeJogo();
void exibeInstrucoes();
void exibeResultado(int *resultado);


void definePosicaoGrade(BITMAP *bmp);
void carregaAnimacao(BITMAP *imagem[], char *pasta, int frames);
int  verificaSaida(int opcao);

//Função Main
int main(){
    
    Inicializa();
    
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
    
    // Fundos de Tela
    //BITMAP *fundoAgua            = load_bitmap("imagens/estaticos/agua.png",NULL);
    //BITMAP *moveAgua             = load_bitmap("imagens/estaticos/agua_2.png",NULL);

    // Rodapé
    //BITMAP *rodapeOpcoes         = load_bitmap("imagens/estaticos/rodape.png",NULL);

    // Tabuleiro
    //BITMAP *gradeTabuleiro       = load_bitmap("imagens/estaticos/tabuleiro.png",NULL);
    //BITMAP *gradeTabuleiroCores  = load_bitmap("imagens/estaticos/tabuleiro_cores.png",NULL);

    // Cursor do Mouse
    BITMAP *cursorMouse            = load_bitmap("imagens/estaticos/mouse.png",NULL);
    BITMAP *cursorMouseAtaque      = load_bitmap("imagens/estaticos/mouseAtaque.png",NULL);

    //Ilhas
    //BITMAP *ilhaSuperiorEsquerda = load_bitmap("imagens/estaticos/ilhas/top_left.png",NULL);
    //BITMAP *ilhaSuperiorDireita  = load_bitmap("imagens/estaticos/ilhas/top_right.png",NULL);
    //BITMAP *ilhaInferiorEsquerda = load_bitmap("imagens/estaticos/ilhas/bottom_left.png",NULL);
    //BITMAP *ilhaInferiorDireita  = load_bitmap("imagens/estaticos/ilhas/bottom_right.png",NULL);
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
    
    

    // inicia o loop do jogo
    while(!fimJogo){
        
        while(velocidade > 0) {

              clear(buffer);
              
              switch(telaAtual){
                     
                     case TELA_INICIO:

                          // Testa se esse som já foi carregado
                          if(!somCarregado){
                             // Inicia o som da tela de inicio no sistema
                             fmodSis = CarregaSom("sons/_entrada.mp3", FMOD_LOOP_NORMAL);
                             somCarregado = 1;
                          }
                          desenhaCenario(buffer, intro);
                          telaAtual = exibeMenu(buffer);
                          fimJogo = verificaSaida(telaAtual);
                          break;
                     
                     case TELA_JOGO:
                          
                          break;
                          
                     case TELA_INSTRUCOES:

                          desenhaCenario(buffer, instrucoes);

                          if(mouse_b && (mouse_x >= 648 && mouse_x <= 778) && (mouse_y >= 520 && mouse_y <= 583)){
                             telaAtual = TELA_INICIO;                                      
                          }
                          
                          break; 
                             
                     case TELA_PERDEU:
                          
                          break; 
                          
                     case TELA_GANHOU:
                          
                          break;                      
                                
              }
               
                             
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
    
    // Exibe a mensagem de carregando na tela
    exibeCarregando();
    
    
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
       
      /*fmodSis = CarregaSom("sons/_mouse_over.mp3", FALSE);
      rest(250);
      FMOD_System_Release(fmodSis); */ 
        
      setPosicaoX(&menu, 228);
      setPosicaoY(&menu, 348);
      desenhaCenario(local, menu);
      if(mouse_b){
          tela = TELA_JOGO;
          somCarregado = 0;
      }
               
   }
   else if((mouse_x >= 349 && mouse_x <= 608) && (mouse_y >= 426 && mouse_y <= 491)){
      // Testa se esse som já foi carregado
                         
      setPosicaoX(&menu, 312);
      setPosicaoY(&menu, 432);
      desenhaCenario(local, menu);
      if(mouse_b){ 
          tela = TELA_INSTRUCOES;
          somCarregado = 0;
      }
         
   }
   else if((mouse_x >= 585 && mouse_x <= 660) && (mouse_y >= 507 && mouse_y <= 549)){
                        
      setPosicaoX(&menu, 544);
      setPosicaoY(&menu, 512);
      desenhaCenario(local, menu); 
      if(mouse_b){
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

void exibeCarregando(){

    BITMAP *img = load_bitmap("imagens/estaticos/carregando.png", NULL);

    // Define uma estrutura para o carregando
    stcCenario carregando;
    setImagemCenario(&carregando, img);
    setPosicaoX(&carregando, 0);
    setPosicaoY(&carregando, 0);
    desenhaCenario(screen, carregando);  

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
As informações necessarias serao passada atraves de parâmetros.

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


