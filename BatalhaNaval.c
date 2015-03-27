// Bibliotecas
#include <allegro.h>
#include <alpng.h>
#include <time.h>
#include <string.h>

// Cabeçalhos
#include "main.h"

// Variaveis
volatile int speed     = 0;                  // Recebe o valor incremental da velocidade do jogo
         int gameState = GAME_STATE_IN_GAME; // Recebe o estado inicial do jogo


//Funções Extras
int  begin(void); 
void closing(void); 
void loadBitmap(BITMAP *imagem[], char *pasta, int frames);
void destroyBitmap(BITMAP *imagem[],int frames);
void alpng_init(void);

//void carregaSom(FMUSIC_MODULE *arquivo);

void gameSpeed();

// Funções das telas do jogo
void gameIntro();
int  gameLoop();
void gameOver();
void gameInstructions();

BITMAP *buffer;
BITMAP *animaExplosao[FPS_EXPLOSAO];
BITMAP *animaFogo[FPS_FOGO];
BITMAP *animaTiroAgua[FPS_TIRO_AGUA];


/*#############################################################################################
  Função Responsavel por controlar a velocidade do jogo.
###############################################################################################*/

void gameSpeed(){

   speed++;
  
}
END_OF_FUNCTION(gameSpeed);


//Função Main
int main(){

  if(!begin()){
  
    closing();
    return 0;
    
  }
  /*gameLoop();
  closing();
  return 0;*/
  
  // inicia loop do jogo
  while(gameState != GAME_STATE_FINISH){
     
     if(gameState == GAME_STATE_IN_GAME) gameLoop();
     
  } // fim do loop do jogo
 
  // remove buffer da memória
//  destroyBitmap(buffer);
  
  
  // Encerra
  closing();
  return 0;
  
}
END_OF_MAIN();

// Tela Inicial
void gameIntro(){
     
     BITMAP *introScreen = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
     
     while (gameState == GAME_STATE_INTRO) {
           
           textprintf_ex(buffer, font, 0,  0, makecol(255,255,255), -1, "Inicio");
           blit(introScreen, buffer, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

           blit(buffer, screen, 0,0, 0,0, SCREEN_WIDTH,SCREEN_HEIGHT);
           
     }
     
     return;
}

// Tela de Instruções
void gameInstructions() {
     while(gameState == GAME_STATE_INSTRUCTIONS) {
        if(key[KEY_ESC]) 
           gameState = GAME_STATE_INTRO;
     }
}

// Tela de GameOver
void gameOver() {
     while(gameState == GAME_STATE_GAMEOVER) {
        if(key[KEY_ESC]) 
           gameState = GAME_STATE_INTRO;
     }
}

/*******************************************************************************************************************
                                                Corpo das funÃ§Ãµes
/*******************************************************************************************************************/

/*#############################################################################################
  - begin : funÃ§Ã£o para iniciar os instaladores do jogo, em caso de algum erro retorna false.
###############################################################################################*/

int begin(void)
{

  allegro_init();
  alpng_init();

  install_keyboard();
  install_mouse();
  install_timer();
  install_int(gameSpeed, MAX_FPS);
  
  srand((unsigned)time(NULL));

  //set_alpha_blender();
  set_color_depth(COLOR_BITS);
  
  if (set_gfx_mode(VIDEO_CARD, SCREEN_WIDTH, SCREEN_HEIGHT, V_SCREEN_X, V_SCREEN_Y) < 0)
  {
    if (set_gfx_mode(GFX_AUTODETECT, SCREEN_WIDTH, SCREEN_HEIGHT, V_SCREEN_X, V_SCREEN_Y) < 0)
    {
      allegro_message("Erro ao inicializar o modo grafico");
      return (FALSE);
    }
  }

  if (COLOR_BITS == 32) set_alpha_blender(); // instala o canal alpha (funciona apenas em 32 bits)



  if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
  {
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
    {
      allegro_message("Erro ao inicializar o som");
      return (FALSE);
    }
  }
  
   set_window_title((char *)"Batalha Naval");

  return (TRUE);
}



/*#############################################################################################
  - LoopDoJogo : Loop principal do jogo.
###############################################################################################*/

int gameLoop(){

  buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
 
  
  // Fundos de Tela
  BITMAP *fundoAgua      = load_bitmap("imagens/estaticos/agua.png",NULL);
  BITMAP *moveAgua       = load_bitmap("imagens/estaticos/agua_2.png",NULL);
  
  // Rodapé
  BITMAP *rodapeOpcoes   = load_bitmap("imagens/estaticos/rodape.png",NULL);
  
  // Tabuleiro
  BITMAP *gradeTabuleiro = load_bitmap("imagens/estaticos/tabuleiro.png",NULL);
  
  // Cursor do Mouse
  BITMAP *cursorMouse    = load_bitmap("imagens/estaticos/mouse.png",NULL);
  
  //Ilhas
  BITMAP *ilhaSuperiorEsquerda = load_bitmap("imagens/estaticos/ilhas/top_left.png",NULL);
  BITMAP *ilhaSuperiorDireita  = load_bitmap("imagens/estaticos/ilhas/top_right.png",NULL);
  BITMAP *ilhaInferiorEsquerda = load_bitmap("imagens/estaticos/ilhas/bottom_left.png",NULL);
  BITMAP *ilhaInferiorDireita  = load_bitmap("imagens/estaticos/ilhas/bottom_right.png",NULL);
    
   // Tamanho do cursor do mouse
  int cursorMouseLargura = 28, //Largura do quadro a ser desenhado na tela
      cursorMouseAltura  = 39, //Altura do quadro a ser desenhado na tela
      countExplosao      = 0,  
      countFogo          = 0,
      countAgua          = 0,
      ativaExplosao      = 0, 
      ativaFogo          = 0,
      ativaAgua          = 0,
      LocalExplosaoX     = 0, 
      LocalExplosaoY     = 0;

  loadBitmap(animaExplosao, "sprites/explosao", FPS_EXPLOSAO);
  loadBitmap(animaFogo, "sprites/fogo", FPS_FOGO);
  loadBitmap(animaTiroAgua, "sprites/agua", FPS_TIRO_AGUA );
  
  //Variavies de controle de tempo de execução.
  long int vel_control   = 0;
  int aguaMovimentoX     = 0, 
      aguaMovimentoFator = 7,
      aguaMovimentoMin   = 0,
      aguaMovimentoMax   = (38 * aguaMovimentoFator),
      flagMaxX           = 0;
         
         
  LOCK_VARIABLE(speed);
  LOCK_FUNCTION(gameSpeed);

  
  //Teste para gerar struct para tratar eventos do tabuleiro
  //##############################################################################################
  typedef struct TRATAEVENTOS {
	  
	  int  xCentro,
	       yCentro,
		     posX1,
	       posX23,
		     posX4,
         posY14,
         posY2,
         posY3;
         

	  char indice[4];
    char temNavio,
         explosaoAtivo,
         fogoAtivo,
         navioDestruido,
         adversario;
	  
	} trataEventos;
   //FIM Struct
   
  //##############################################################################################
  
  //Atribuição de Valores para o vetor de struct 
	trataEventos gameTabuleiro[100];

  int i;
  
  for(i=0;i < 100; i++){
  
    gameTabuleiro[i].posX1   = 0;
    gameTabuleiro[i].posX23  = 0;
    gameTabuleiro[i].posX4   = 0;
    gameTabuleiro[i].posY14  = 0;
    gameTabuleiro[i].posY2   = 0;
    gameTabuleiro[i].posY3   = 0;
  
  }
  
  
	gameTabuleiro[0].xCentro        = 53 ;
	gameTabuleiro[0].yCentro        = 215;
  gameTabuleiro[0].posX1          = 38;
	gameTabuleiro[0].posX23         = 55;
	gameTabuleiro[0].posX4          = 79;
  gameTabuleiro[0].posY14         = 215;
  gameTabuleiro[0].posY2          = 200;
  gameTabuleiro[0].posY3          = 229;
  strcpy( gameTabuleiro[0].indice, "a1");
	gameTabuleiro[0].temNavio       = 'n';
	gameTabuleiro[0].fogoAtivo      = 'n';
	gameTabuleiro[0].explosaoAtivo  = 'n';
	gameTabuleiro[0].navioDestruido = 'n';

	gameTabuleiro[1] = gameTabuleiro[0];

  strcpy( gameTabuleiro[1].indice, "a2");
	gameTabuleiro[1].temNavio = 's';
	gameTabuleiro[1].xCentro  = 79;
	gameTabuleiro[1].yCentro  = 198;
  gameTabuleiro[1].posX1    = 58;
	gameTabuleiro[1].posX23   = 80;
	gameTabuleiro[1].posX4    = 102;
  gameTabuleiro[1].posY14   = 200;
  gameTabuleiro[1].posY2    = 187;
  gameTabuleiro[1].posY3    = 210;
  
  
  //FIM de atribuição de valores
  //TODO For para atribuir esses valores de forma automatica. 
  //##############################################################################################
  //While principal
  while (!key[KEY_ESC]){

    clear(buffer);
    blit(fundoAgua, buffer,0,0,0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    
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
    
    textprintf_ex( buffer, font, 10, 30, makecol(255,0,0), -1, "Movimento X: %d", aguaMovimentoX);


    if(speed >= vel_control){    
    
      textprintf_ex( buffer, font, 10, 10, makecol(255,0,0), -1, "Mouse X: %d", mouse_x);
      textprintf_ex( buffer, font, 10, 20, makecol(255,0,0), -1, "Mouse Y: %d", mouse_y);
          
      /*if(mouse_b & 1 && ativaExplosao == 0){

        ativaExplosao  = 1;
        LocalExplosaoX = mouse_x;
        LocalExplosaoY = mouse_y;

      }   */
      
      /*#############################################################################################################*/
      /*Teste de posicionamento do mouse                                                                             */
      /*#############################################################################################################*/

      for (i = 0;i < 100; i ++){
        
        if ( ((mouse_x >= gameTabuleiro[i].posX1 && mouse_x <= gameTabuleiro[i].posX23) &&
               ((mouse_y >= gameTabuleiro[i].posY14 && mouse_y <= gameTabuleiro[i].posY3) ||
                 (mouse_y <= gameTabuleiro[i].posY14 && mouse_y >= gameTabuleiro[i].posY2))) ||
                 
                  ((mouse_x <= gameTabuleiro[i].posX4 && mouse_x >= gameTabuleiro[i].posX23) &&
                    ((mouse_y >= gameTabuleiro[i].posY14 && mouse_y <= gameTabuleiro[i].posY3) ||
                      (mouse_y <= gameTabuleiro[i].posY14 && mouse_y >= gameTabuleiro[i].posY3))) ){
          
          //TODO Trocar imagem do quadro para "brilhar"
          
          textprintf_ex( buffer, font, 10, 40, makecol(255,0,0), -1, "Voce esta na posicao: %s", gameTabuleiro[i].indice);
          
          if(gameTabuleiro[i].adversario == 's'){
            
            //TODO Trocar Mouse
            
            if(mouse_b & 1){
          
              if(gameTabuleiro[i].temNavio == 'n'){
                
                //TODO tiro na agua recebendo posição X e Y do centro 
                
              }
              else if(gameTabuleiro[i].temNavio == 's' && gameTabuleiro[i].fogoAtivo == 'n'){
                
                //TODO Tiro no navio recebendo posição X e Y do centro 
                
              }
              else if(gameTabuleiro[i].temNavio == 's' && gameTabuleiro[i].fogoAtivo == 's'){
              
                //TODO Bloquear mouse -- Trocar imagem
              
              }
          
            }
            
          } 

        }
        
      }


      /*#############################################################################################################*/
      if(mouse_b & 1 && ativaAgua == 0){

        ativaAgua  = 1;
        LocalExplosaoX = mouse_x - 34; //Largura dividido por dois
        LocalExplosaoY = mouse_y - 60; //altura total

      }

      if(countExplosao < FPS_EXPLOSAO && ativaExplosao == 1){
                      
        draw_trans_sprite(buffer, animaExplosao[countExplosao], LocalExplosaoX, LocalExplosaoY);
        countExplosao ++;
        
        if(countExplosao == (FPS_EXPLOSAO -1) ){

          countExplosao  = 0;
          ativaExplosao  = 0;
          ativaFogo      = 1;

        }
      }
      
      if(countAgua < FPS_TIRO_AGUA && ativaAgua == 1){
                      
        draw_trans_sprite(buffer, animaTiroAgua[countAgua], LocalExplosaoX, LocalExplosaoY);
        countAgua ++;
        
        if(countAgua == (FPS_TIRO_AGUA -1) ){

          countAgua  = 0;
          ativaAgua  = 0;

        }

      }

      if(countFogo < FPS_FOGO && ativaFogo == 1 ){
       

        draw_trans_sprite(buffer, animaFogo[countFogo], LocalExplosaoX, LocalExplosaoY);
        countFogo ++;
      
          if(countFogo == 25){ 
          
            countFogo     = 0;
            ativaExplosao = 0;     
            
          }
      }

      
      /*Desenha mouse na tela com imagem*/
      draw_trans_sprite(buffer, cursorMouse, mouse_x, mouse_y);
        
      blit(buffer, screen, 0,0,0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
      
      vel_control =  speed + 1;
    }


  }
  
  return -1;
}

/*#############################################################################################
  - loadBitmap : Responsavel por carregar na memoria as imagens da pasta informada.
###############################################################################################*/

void loadBitmap(BITMAP *imagem[], char *pasta, int frames){
     
     int i;
     char caminho[100];
              
     for(i=0; i < frames; i++){

       sprintf( caminho, "imagens/%s/%i.png",pasta,i);
       imagem[i] = load_bitmap(caminho, NULL);
     }

}

/*#############################################################################################
  - destroyBitmap : Responsavel por apagar da memoria o Bitmap informado.
###############################################################################################*/
/*
void destroyBitmap(BITMAP *imagem[], int frames){
     
     int i;
              
     for(i=0; i < frames; i++){

       destroyBitmap(imagem[i]);
      
     }

}
*/
/*#############################################################################################
  - closing : Responsavel pelas rotinas de fechamento do jogo..
###############################################################################################*/

void closing(void)
{
  remove_timer();
  remove_mouse();
  remove_sound();
  remove_keyboard();
  allegro_exit();
}
