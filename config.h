#include <stdio.h>
#define CFG_VALOR 99

int valor;

void teste(){
     
     printf("Informe um valor: ");
     scanf("%i", &valor);
     
     if(valor > 0){
         printf("%i",valor);          
     }else{
         printf("%i", CFG_VALOR);        
     }
     
     printf("\n");
     teste(); 
}
