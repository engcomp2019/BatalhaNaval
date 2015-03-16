//Bibliotecas
#include <allegro.h>
#include <time.h>

//Defines
#define MAX_X 800
#define MAX_Y 600
#define V_MAX_X 0
#define V_MAX_Y 0
#define COLOR_BITS 32
#define VIDEO_CARD GFX_AUTODETECT_WINDOWED
#define DIGI_CARD DIGI_AUTODETECT
#define MIDI_CARD MIDI_AUTODETECT
#define VELOCIDADEJOGO  50

//Funções Extras
int begin(void); 
void closing(void); 
int gameLoop(void); 
void explosao(BITMAP *bmp, int posX, int pos);
void alpng_init(void);
volatile long int counter = 0;
void game_timer(void);
void mira_timer(void);
BITMAP *buffer;


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
  
  install_keyboard();
  
  install_mouse();

  install_timer();
  
  install_int(game_timer, VELOCIDADEJOGO);
  
  srand((unsigned)time(NULL));

  set_color_depth(COLOR_BITS);
  if (set_gfx_mode(VIDEO_CARD, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
  {
    if (set_gfx_mode(GFX_AUTODETECT, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
    {
      allegro_message("Erro ao inicializar o modo grafico");
      return (FALSE);
    }
  }

  if (COLOR_BITS == 32) set_alpha_blender(); // instala o canal alpha (funciona apenas em 32 bits)

 /* if (install_sound(DIGI_CARD, MIDI_CARD, NULL) < 0)
  {
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
    {
      allegro_message("Erro ao inicializar o som");
      return (FALSE);
    }
  }*/

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

  buffer=create_bitmap(800,600);
  
  BITMAP *explosion = load_bitmap("ImagensBatalhaNaval/Explosions.bmp", NULL);
  BITMAP *wallPaper = load_bitmap("ImagensBatalhaNaval/wallPaper.bmp",NULL);
  BITMAP *mouseMira = load_bitmap("ImagensBatalhaNaval/mira.bmp",NULL);
  
  //Explosão
  int explosionInicialX      = 0;
  int explosionInicialY      = 256;
  int explosionImgLargura    = 512;               //Largura da imagem
  int explosionImgAltura     = 448;               //Altura da imagem
  int explosionQuadroLargura = 64;                //Largura do quadro a ser desenhado na tela
  int explosionQuadroAltura  = 64;                //Altura do quadro a ser desenhado na tela
  int explosionImgX          = explosionInicialX; //Posição X inicial da imagem, altera a cada quadro desenhado.
  int explosionImgY          = explosionInicialY; //Posição Y inicial da imagem, altera a cada quadro desenhado
  int explosionX = 0;
  int explosionY = 0;
  int flagAtiva  = 0;
  
  //Mira
  int miraQuadroLargura = 47; //Largura do quadro a ser desenhado na tela
  int miraQuadroAltura  = 47; //Altura do quadro a ser desenhado na tela

  
  
  //Variavies temporarias.
  long int vel_control  = 0;
  long int mira_control = 0;
  
  LOCK_VARIABLE(counter);
  
  LOCK_FUNCTION(game_timer);
  
    
  //While principal
  while (!key[KEY_ESC]){
      
    clear (buffer);
     
    //wallpaper
    blit(wallPaper,buffer,0,0,0,0,800,600);
    
    if( counter >= vel_control){
      
      if(mouse_b & 1 && flagAtiva == 0){
        
        explosionX = mouse_x - (explosionQuadroLargura / 2);
        explosionY = mouse_y - (explosionQuadroAltura / 2);
        flagAtiva  = 1;
        
      }
        
      if(explosionImgX <= (explosionImgLargura - explosionQuadroLargura ) && 
        flagAtiva == 1){
        
        masked_blit(explosion,buffer,explosionImgX,explosionImgY,explosionX,explosionY,explosionQuadroAltura,explosionQuadroLargura);
        
        explosionImgX = explosionImgX + explosionQuadroLargura;
      }
      else if(explosionImgY <= (explosionImgAltura) &&
      flagAtiva == 1){
      
        explosionImgX = 0;
        
        explosionImgY = explosionImgY + explosionQuadroAltura;
        
        masked_blit(explosion,buffer,explosionImgX,explosionImgY,explosionX,explosionY,explosionQuadroAltura,explosionQuadroLargura);
      }
      else {
        explosionImgX = explosionInicialX;
        explosionImgY = explosionInicialY;
        flagAtiva = 0;
      }
    
      /*Desenha mouse na tela com imagem de mira*/
      masked_blit(mouseMira, buffer, 94, 0, mouse_x -23.5, mouse_y -23.5, miraQuadroAltura, miraQuadroLargura);
           
      blit(buffer,screen,0,0,0,0,800,600);
      
      vel_control = counter + 1;
     
    }
  }
  
  return -1;
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
