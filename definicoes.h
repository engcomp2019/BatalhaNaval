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


//Testes Navios

#define P1_NAVIO2H  0
#define P1_NAVIO3AH 21
#define P1_NAVIO3BH 35
#define P1_NAVIO4H  80

#define P1_NAVIO2V  8
#define P1_NAVIO3AV 21
#define P1_NAVIO3BV 45
#define P1_NAVIO4V  60

#define P2_NAVIO2H  05
#define P2_NAVIO3AH 21
#define P2_NAVIO3BH 35
#define P2_NAVIO4H  80

#define P2_NAVIO2V  8
#define P2_NAVIO3AV 21
#define P2_NAVIO3BV 45
#define P2_NAVIO4V  60

//Portas Arduino
#define VELOCIDADEPORTA 9600
#define PORTA "COM3"
