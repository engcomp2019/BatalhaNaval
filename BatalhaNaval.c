//Bibliotecas
#include <allegro.h>
#include <alpng.h>
#include <time.h>
#include <string.h>

//Defines
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define V_SCREEN_X 0
#define V_SCREEN_Y 0
#define COLOR_BITS 32
#define VIDEO_CARD GFX_AUTODETECT_WINDOWED
#define DIGI_CARD DIGI_AUTODETECT
#define MIDI_CARD MIDI_AUTODETECT
#define VELOCIDADEJOGO  50

#define FRAMES_EXPLOSAO 41
#define FRAMES_FOGO 26

//Funções Extras
int begin(void); 
void closing(void); 
int gameLoop(void); 
void carregaBitmap(BITMAP *imagem[], char *pasta, int *frames);
void DestruirBitmap(BITMAP *imagem[],int *frames);
void alpng_init(void);
volatile long int counter = 0;
void game_timer(void);

BITMAP *buffer;
BITMAP *animaExplosao[FRAMES_EXPLOSAO];
BITMAP *animaFogo[FRAMES_FOGO];

//Função Main
int main(){

  if(!begin()){
  
    closing();
    return 0;
  }
  
  gameLoop();
  
  closing();
  
  return 0;
}
END_OF_MAIN();

/*******************************************************************************************************************
                                                Corpo das funções
/*******************************************************************************************************************/

/*#############################################################################################
  - begin : função para iniciar os instaladores do jogo, em caso de algum erro retorna false.
###############################################################################################*/

int begin(void)
{

  allegro_init();
  alpng_init();

  install_keyboard();
  install_mouse();
  install_timer();
  install_int(game_timer, VELOCIDADEJOGO);
  
  srand((unsigned)time(NULL));

  set_alpha_blender();
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

  if (install_sound(DIGI_CARD, MIDI_CARD, NULL) < 0)
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
  Funções Responsaveis por criar uma variaveis  temporizadas.
###############################################################################################*/

void game_timer(void){

   counter++;
  
}
END_OF_FUNCTION(game_timer);

/*#############################################################################################
  - LoopDoJogo : Loop principal do jogo.
###############################################################################################*/

int gameLoop(void){

  buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
  
  // Fundos de Tela
  BITMAP *fundoAgua = load_bitmap("imagens/estaticos/agua.png",NULL);
  
  // Rodap�
  BITMAP *rodapeOpcoes = load_bitmap("imagens/estaticos/rodape.png",NULL);
  
  // Tabuleiro
  BITMAP *gradeTabuleiro = load_bitmap("imagens/estaticos/tabuleiro.png",NULL);
  
  // Cursor do Mouse
  BITMAP *cursorMouse = load_bitmap("imagens/estaticos/mouse.png",NULL);
  
  //Ilhas
  BITMAP *ilhaSuperiorEsquerda = load_bitmap("imagens/estaticos/ilhas/top_left.png",NULL);
  BITMAP *ilhaSuperiorDireita = load_bitmap("imagens/estaticos/ilhas/top_right.png",NULL);
  BITMAP *ilhaInferiorEsquerda = load_bitmap("imagens/estaticos/ilhas/bottom_left.png",NULL);
  
  
  
   // Tamanho do cursor do mouse
  int cursorMouseLargura = 28; //Largura do quadro a ser desenhado na tela
  int cursorMouseAltura  = 39; //Altura do quadro a ser desenhado na tela
  int countExplosao = 0, countFogo  = 0;
  int ativaExplosao = 0, ativaFogo  = 0;
  int LocalExplosaoX = 0, LocalExplosaoY = 0;
  

  carregaBitmap(animaExplosao, "sprites/explosao", FRAMES_EXPLOSAO);
  carregaBitmap(animaFogo, "sprites/fogo", FRAMES_FOGO);
  
  //Variavies de controle de tempo de execu��o.
  long int vel_control  = 0;
  
  LOCK_VARIABLE(counter);
  LOCK_FUNCTION(game_timer);

  //While principal
  while (!key[KEY_ESC]){

    clear(buffer);
    blit(fundoAgua, buffer,0,0,0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    draw_trans_sprite(buffer,ilhaSuperiorEsquerda,0,0);
    draw_trans_sprite(buffer,ilhaSuperiorDireita,525,0);
    draw_trans_sprite(buffer,ilhaInferiorEsquerda,0,395);
    draw_trans_sprite(buffer,rodapeOpcoes,0,458);
    draw_trans_sprite(buffer,gradeTabuleiro,20,60);
    


    if(counter >= vel_control){
    
      textprintf_ex( buffer, font, 10, 10, makecol(255,0,0), -1, "Mouse X: %d", mouse_x);
      textprintf_ex( buffer, font, 10, 20, makecol(255,0,0), -1, "Mouse Y: %d", mouse_y);
          
      if(mouse_b & 1 && ativaExplosao == 0){

        ativaExplosao = 1;
        LocalExplosaoX = mouse_x;
        LocalExplosaoY = mouse_y;

      }   

      if(countExplosao < FRAMES_EXPLOSAO && ativaExplosao == 1){
                      
        draw_trans_sprite(buffer, animaExplosao[countExplosao], LocalExplosaoX, LocalExplosaoY);
        countExplosao ++;
        
        if(countExplosao == (FRAMES_EXPLOSAO -1) ){

          countExplosao  = 0;
          ativaExplosao  = 0;
          ativaFogo      = 1;

        }
      }

      if(countFogo < FRAMES_FOGO && ativaFogo == 1 ){
       

        draw_trans_sprite(buffer, animaFogo[countFogo], LocalExplosaoX, LocalExplosaoY);
        countFogo ++;
      
          if(countFogo == 25){
            countFogo = 0;
            ativaExplosao = 0;      
          }
      }

      
      /*Desenha mouse na tela com imagem de mira*/
      draw_trans_sprite(buffer, cursorMouse, mouse_x-(cursorMouseLargura/2), mouse_y-(cursorMouseAltura/2));
        
      blit(buffer, screen, 0,0,0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
      vel_control =  counter + 1;
    }


  }
  
  return -1;
}

/*#############################################################################################
  - carregaBitmap : Responsavel por carregar na memoria as imagens da pasta informada.
###############################################################################################*/

void carregaBitmap(BITMAP *imagem[], char *pasta, int *frames){
     
     int i;
     char caminho[100];
              
     for(i=0; i < frames; i++){

       sprintf( caminho, "imagens/%s/%i.png",pasta,i);
       imagem[i] = load_bitmap(caminho, NULL);
     }

}

/*#############################################################################################
  - DestruirBitmap : Responsavel por apagar da memoria o Bitmap informado.
###############################################################################################*/

void DestruirBitmap(BITMAP *imagem[],int *frames){
     
     int i;
              
     for(i=0; i < frames; i++){

       destroy_bitmap(imagem[i]);
      
     }

}

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
