/***********************************************************
*                                                          *
*    PROJETO APC 2 - JOGO DE BATALHA NAVAL COM ARDUINO 			 *
*                                                          *
* Nome: Andre Queiroz					                 RA:	27619994874	*
* Nome: Raiany Araujo					                 RA:	02055024230	*
* Nome: Rodrigo Sene					                  RA:	42108819886 *
* Data de Inicio: 28/02/2015                               *
* Data Final: 25/05/2015		                                 *
***********************************************************/

// Bibliotecas (Arquivos de Cabecalho)


// Prototipos das funcoes

/*
================================================================================

================================================================================
*/

void PopulaTabuleiroAdversario(trataEventos gameTabuleiroAdversario){

    int navioGrande  = 5;
    int navioMedio   = 4;
    int naviopequeno = 3;
    int menorNavio   = 2;
    int indSucesso   = 0;

    while(indSucesso == 0){
        indSucesso = PosicionaNavio(navioGrande,gameTabuleiroAdversario);
    }
    int indSucesso = 0;

    while(indSucesso == 0){
        indSucesso = PosicionaNavio(navioMedio,gameTabuleiroAdversario);
    }
    int indSucesso = 0;

    while(indSucesso == 0){
        indSucesso =  PosicionaNavio(naviopequeno,gameTabuleiroAdversario);
    }
    int indSucesso = 0;

    while(indSucesso == 0){
        indSucesso = PosicionaNavio(naviopequeno,gameTabuleiroAdversario);
    }
    int indSucesso = 0;

    while(indSucesso == 0){
        indSucesso = PosicionaNavio(menorNavio,gameTabuleiroAdversario);
    }
}


/*
char* EventosAdversario(trataEventos gameTabuleiro[][], char *ultimoTiro, int statusUltimoTiro){



}
*/

void geraRound(int *linha, int *coluna){

    linha  = rand() % 9;
    coluna = rand() % 9;

}


/*
 Função responsavel por setar os flags temNavio da struct, criando assim o posicionamento
dos navios na matriz do adversario.
 A função Rand é utilizada para verificar a posição do navio, e tambem se ele será posicionado
na vertical ou horizontal.
 Os navios sempre são posionados da esquerda para direita, e de cima para baixo.
*/
int PosicionaNavio(trataEventos gameTabuleiroAdversario[][],int tamanhoNavio){

    const larguraTabuleiro = 10;
    const alturaTabuleiro  = 10;
    int linha, coluna;
    int flagNavio  = 0;
    int indSucesso = 0;
    int valida     = 0;
    char posicao;

    rand() % 1 == 0 ? posicao = "v" : posicao = "h";

    geraRound(linha, coluna);

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

