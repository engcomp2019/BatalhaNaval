
//colocação das bibliotecas básicas e extras
#include <stdio.h>
#include <stdlib.h>
#include <fmod.h>
#include <fmod_errors.h>   
#include <allegro.h>
#include <stdlib.h>
#include <winalleg.h>

int main(int argc, char *argv[])
{
    allegro_init(); // inicia o allegro
   set_color_depth(32); // inicia a tela com a profundidade de cor em 32 bits
    int res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0); // detecta a placa de video e inicializa a tela com a dimensão de 640*480


    FSOUND_SAMPLE *samp1 = 0; //cria um ponteiro para armazenar o som em memória
   
 
    if (FSOUND_GetVersion() < FMOD_VERSION) // verificação da versão do fmod caso a versão do FSOUND for menor que a do FMOD retorna uma menssagem de erro
    {
        printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
        return 1;
    }

    /*
        Seleciona a saída de audio
    */
    FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
       


    /*
        Seleção do driver
    */

     
   FSOUND_GetOutput(); // indentifica o tipo de saida
   FSOUND_GetMixer(); // indentifica o mixer
   FSOUND_SetDriver(0); // seta o driver de som que vai ser usado 
 
                 
    /*
        Inicializando o FMOD
    */
    if (!FSOUND_Init(44100, 32, FSOUND_INIT_GLOBALFOCUS)) // se o valor do FSOUND_Init for 0 execute o tratamento de erro
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return 1;
    }

     
    /*
        Carrengando o Sample
    */

    /* PCM,44,100 Hz, 32 Bit, Mono ou uma mp3 ou outros formatos suportados pelo fmod*/
    samp1 = FSOUND_Sample_Load(FSOUND_UNMANAGED, "01 - blink-182 - Anthem Part Two.mp3", FSOUND_NORMAL | FSOUND_HW2D, 0, 0);   /* hardware? why not, just to show it can be done */
    if (!samp1)
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return 1;
    }

   // aqui fala qual maneira o sample ira tocar caso falhe excute o tratamento de erro
    if(!FSOUND_Sample_SetMode(samp1, FSOUND_LOOP_OFF))// o loop normal toca a musica continuamente ate o programa fechar
   {
   printf("Error!\n");   
    printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
   }
   // aqui sera tocado o sample ,caso falhe, execute o tratamento de erro
   if(!FSOUND_PlaySound(FSOUND_FREE, samp1))
   {
   printf("Error!\n");   
   printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
   }
   
    Sleep(60000);  // executa o programa durante 60 segundos     


    /*
        limpando a memoria e fechando o fmod
    */
    FSOUND_Sample_Free(samp1); // limpa a memoria ultilizada pelo ponteiro do sample
   

    FSOUND_Close();  // encerra a API FMOD
 

    return 0;
}
END_OF_MAIN();
