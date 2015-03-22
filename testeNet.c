#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>

#define MAX_X 640
#define MAX_Y 480
#define V_MAX_X 0
#define V_MAX_Y 0
#define COLOR_BITS 32
#define VIDEO_CARD GFX_AUTODETECT_WINDOWED
#define DIGI_CARD DIGI_AUTODETECT
#define MIDI_CARD MIDI_AUTODETECT
#define FPS 10		// Treco estranho, olhe no site o porque de eu ter feito isso

// ESTRUTURAS--------------------------------------------------------------
typedef struct SEQUENCE_
{
	BITMAP **frame;
	int frame_max;
	int frame_num;
	int block_anim;
} SEQUENCE;

//-------------------------------------------------------------------------
// FUNÇÕES PRINCIPAIS
int begin(void); 
void closing(void); 
void gameloop(void); 
volatile long int counter = 0;
void game_timer(void);
int init_sequence(SEQUENCE *,int );
int animate(BITMAP **, SEQUENCE *, int, int, int);

BITMAP *buffer;

int main(int argc, char **argv)
{
	if (!begin())
	{
		closing();
		return -1;
	}

	gameloop();

	closing();
	return 0;

}
END_OF_MAIN();

int begin(void)
{

	allegro_init();

	install_keyboard();
	install_mouse();

	install_timer();

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

	if (COLOR_BITS == 32)	set_alpha_blender(); // instala o canal alpha (funciona apenas em 32 bits)

	if (install_sound(DIGI_CARD, MIDI_CARD, NULL) < 0)
	{
		if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
		{
			allegro_message("Erro ao inicializar o som");
			return (FALSE);
		}
	}


	LOCK_VARIABLE(counter);
	LOCK_FUNCTION(game_timer);
	install_int(game_timer, 1000/FPS);

	return (TRUE);
}

void game_timer(void)
{
    counter++;
}
END_OF_FUNCTION(game_timer);

void gameloop(void)
{
	SEQUENCE bolinha_azul;
	long long int vel_control = 0;
	int x;
	
	if (!init_sequence(&bolinha_azul, 8))
		return;
	
	buffer = create_bitmap(MAX_X,MAX_Y);
	clear(buffer);
	
	for (x = 0; x < bolinha_azul.frame_max;x++)
	{
		bolinha_azul.frame[x] = create_bitmap(50,50);
	}
	
	for (x = 0; x < bolinha_azul.frame_max;x++)
	{
		clear_to_color(bolinha_azul.frame[x], makecol(255,0,255));
	}
	
	circlefill(bolinha_azul.frame[0],bolinha_azul.frame[0]->w/2,bolinha_azul.frame[0]->h/2,15,makecol(0,0,255));
	circlefill(bolinha_azul.frame[1],bolinha_azul.frame[1]->w/2,bolinha_azul.frame[1]->h/2,15,makecol(0,0,200));
	circlefill(bolinha_azul.frame[2],bolinha_azul.frame[2]->w/2,bolinha_azul.frame[2]->h/2,15,makecol(0,0,150));
	circlefill(bolinha_azul.frame[3],bolinha_azul.frame[3]->w/2,bolinha_azul.frame[3]->h/2,15,makecol(0,0,100));
	circlefill(bolinha_azul.frame[4],bolinha_azul.frame[4]->w/2,bolinha_azul.frame[4]->h/2,15,makecol(0,0,50));
	circlefill(bolinha_azul.frame[5],bolinha_azul.frame[5]->w/2,bolinha_azul.frame[5]->h/2,15,makecol(0,0,100));
	circlefill(bolinha_azul.frame[6],bolinha_azul.frame[6]->w/2,bolinha_azul.frame[6]->h/2,15,makecol(0,0,150));
	circlefill(bolinha_azul.frame[7],bolinha_azul.frame[7]->w/2,bolinha_azul.frame[7]->h/2,15,makecol(0,0,200));
	
	circlefill(bolinha_azul.frame[0],bolinha_azul.frame[0]->w/2,bolinha_azul.frame[0]->h/2,10,makecol(0,100,50));
	circlefill(bolinha_azul.frame[1],bolinha_azul.frame[1]->w/2,bolinha_azul.frame[1]->h/2,10,makecol(0,100,100));
	circlefill(bolinha_azul.frame[2],bolinha_azul.frame[2]->w/2,bolinha_azul.frame[2]->h/2,10,makecol(0,100,150));
	circlefill(bolinha_azul.frame[3],bolinha_azul.frame[3]->w/2,bolinha_azul.frame[3]->h/2,10,makecol(0,100,200));
	circlefill(bolinha_azul.frame[4],bolinha_azul.frame[4]->w/2,bolinha_azul.frame[4]->h/2,10,makecol(0,100,255));
	circlefill(bolinha_azul.frame[5],bolinha_azul.frame[5]->w/2,bolinha_azul.frame[5]->h/2,10,makecol(0,100,200));
	circlefill(bolinha_azul.frame[6],bolinha_azul.frame[6]->w/2,bolinha_azul.frame[6]->h/2,10,makecol(0,100,150));
	circlefill(bolinha_azul.frame[7],bolinha_azul.frame[7]->w/2,bolinha_azul.frame[7]->h/2,10,makecol(0,100,100));
	
	
	// Looping do Jogo-------------------------------
	do
	{
		
		if (counter >= vel_control)
		{
			// Controles----------------------------------
			
			
			
			// Desenhando na tela-------------------------
			if (!animate(&buffer, &bolinha_azul, key[KEY_SPACE], MAX_X/2, MAX_Y/2))
				draw_sprite(buffer, bolinha_azul.frame[0],MAX_X/2, MAX_Y/2);
			
			//Re-desenhando a tela (com Double Buffer)-------------------------
			blit(buffer, screen, 0, 0, 0, 0,MAX_X,MAX_Y);
			clear(buffer);
			
			
			vel_control = counter + 1;
		}
		
		if (key[KEY_ESC])    break;
	}while(1);
}

int init_sequence(SEQUENCE *seq, int max)
{
	(*seq).frame_max = max;
	(*seq).frame = (BITMAP **) malloc( (*seq).frame_max * sizeof(BITMAP *));
	
	if (!(*seq).frame)
		return 0;
	
	(*seq).frame_num = 0;
	(*seq).block_anim = FALSE;
	
	return 1;
}

int animate(BITMAP **buffer, SEQUENCE *seq, int key, int x, int y)
{
	if ( (key) && (!(*seq).block_anim) )
		(*seq).block_anim = TRUE;
	
	if ((*seq).block_anim)
	{
		draw_sprite(*buffer, (*seq).frame[(*seq).frame_num ],x,y);
		
		if ((*seq).frame_num >= ((*seq).frame_max - 1) )
		{
			(*seq).frame_num = 0;
			(*seq).block_anim = FALSE;
		}
		else
			(*seq).frame_num++;
		
		return 1;
	}
	
	return 0;
}


void closing(void)
{
	remove_timer();
	remove_mouse();
	remove_sound();
	remove_keyboard();
	allegro_exit();
}
