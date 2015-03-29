int i;

for (i = 0;i < 100; i ++){
  
  if ( (mouse_x >= gameTabuleiro[i].x1 && mouse_x <= gameTabuleiro[i].x23) &&
         ((mouse_y >= gameTabuleiro[i].y14 && mouse_y <= gameTabuleiro[i].y3) ||
           (mouse_y <= gameTabuleiro[i].y14 && mouse_y >= gameTabuleiro[i].y2)) ) ||
            ( (mouse_x >= gameTabuleiro[i].x4 && mouse_x <= gameTabuleiro[i].x23) &&
               ((mouse_y >= gameTabuleiro[i].y14 && mouse_y <= gameTabuleiro[i].y3) ||
                 (mouse_y <= gameTabuleiro[i].y14 && mouse_y >= gameTabuleiro[i].y3)) ){
    
    //TODO Trocar imagem do quadro para "brilhar"
    
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

