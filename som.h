/***********************************************************
*                                                          *
*    PROJETO APC 2 - JOGO DE BATALHA NAVAL COM ARDUÍNO 			 *
*                                                          *
* Nome: Andre Queiroz					                RA:	27619994874		*
* Nome: Raiany Araujo					                RA:	02055024230		*
* Nome: Rodrigo Sene					                 RA:	42108819886	 *
* Data de Inicio: 28/02/2015                               *
* Data Final: 25/05/2015		                                 *
************************************************************/

// Bibliotecas (Arquivos de Cabecalho)
#include <fmod.h>

// Protótipo da função
FMOD_SYSTEM *CarregaSom(char *arquivo, FMOD_MODE modo);

/*
================================================================================
FMOD_SYSTEM CarregaSom

Executa o arquivo de audio informado atraves do parametro e o modo.

================================================================================
*/

FMOD_SYSTEM *CarregaSom(char *arquivo, FMOD_MODE modo){

    FMOD_SYSTEM *fmodSistema = 0;
    FMOD_SOUND *fmodSom      = 0;
    FMOD_CHANNEL *fmodCanal  = 0;
    FMOD_RESULT fmodResultado;
    
    
    fmodResultado = FMOD_System_Create(&fmodSistema);
    fmodResultado = FMOD_System_Init(fmodSistema, 32, FMOD_INIT_NORMAL, NULL);
    fmodResultado = FMOD_System_CreateSound(fmodSistema, arquivo, modo, 0, &fmodSom);
    fmodResultado = FMOD_System_PlaySound(fmodSistema, FMOD_CHANNEL_FREE, fmodSom, 0, &fmodCanal);
    
    return fmodSistema;
    
}
END_OF_FUNCTION(CarregaSom);
