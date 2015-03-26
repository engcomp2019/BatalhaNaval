int i;

if(mouse_b & 1){

	for (i = 0;i < 100; i ++){
		
		if (mouse_x >= gameTabuleiro[i].xInicio && mouse_x <= gameTabuleiro[i].xFinal) && 
			(mouse_y >= gameTabuleiro[i].yInicio && mouse_y <= gameTabuleiro[i].yFinal ){
			
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