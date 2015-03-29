gameTabuleiro[10][10];

struct TRATAEVENTOS {
  
  int  xCentro;
  int  yCentro;
  char temNavio;
  char fogoAtivo;
  char navioDestruido; 
  
}

typedef struct TRATAEVENTOS trataEventos;

trataEventos a1,a2;

a1.xCentro        = ;
a1.yCentro        = ;
a1.temNavio       = 'n' ;
a1.fogoAtivo      = 'n';
a1.navioDestruido = 'n';

a2 = a1;

a2.temNavio = 's';

gameTabuleiro[0][0] = A1;
gameTabuleiro[0][1] = A2;