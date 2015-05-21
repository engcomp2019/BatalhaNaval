/* [ DOCUMENTAÇÃO ] ===============================================================================================================================================
	
	[ Descrição ] -------------------------------------------------------------------------------------------------------------------------------------------------
	 
	 Este arquivo permite a inicialização, leitura, escrita e finalização da porta serial atraves de um programa escrito na linguagem C (.c) no Windows. 
	 Uma capacidade interessante é a possibilidade de se utilizar mais de uma conexão serial ao mesmo tempo, de forma fácil, amigável e intuitiva.
	 O objetivo do desenvolvimento destas funções é tornar mais didático e simples a utilização de métodos de comunicação entre dispositivos utilizando
	 as portas seriais, tais como placas Arduino, PIC, entre outras.
	 Além da comunicação padrão, a capacidade de se trabalhar com mais de uma porta serial ao mesmo tempo, é especialmente útil a quem deseja utilizar 
	 módulos de bluetooth, por exemplo, ou até mesmo trabalhar com um único software gerenciando a comunicação entre vários dispositivos conectados às 
	 portas COM simultaneamente.


	[ Como utilizar em seu código ] -------------------------------------------------------------------------------------------------------------------------------
	
	 Basta deixar uma cópia deste arquivo no diretório
	 do ser arquivo fonte e incluí-lo em seu código, desta forma:
	
	        #include "PSerial.h"


	[ Funções disponíveis ] ---------------------------------------------------------------------------------------------------------------------------------------
	
	 - void inicializaSerial(u_char idSerial, char *nomePorta, u_int baudRate);			// inicializa a comunicação através da porta serial passada por parâmetro 
	 																					// (Ex.: inicializaSerial(0, "COM3", 9600); - para inicializar a COM3, a 9600bps)
	 
	 - void finalizaSerial(u_char idSerial);											// finaliza a comunicação através da porta serial
	 																					// (Ex.: finalizaSerial(0);)
	 
	 - void finalizaTodasPortasSeriais();                                               //finaliza a comunicação com todas as portas seriais conectadas
                                                                                        // (EX: finalizaTodasPortasSeriais();)
                                                                                        
	 - int leSerial(u_char idSerial, char *buffer, u_int qtdChar);						// faz a leitura de bytes recebidos através da porta serial 
	 																					// (Ex.: leSerial(0, buffer, 10); - para gravar 10 bytes recebidos da COM, no vetor de char "buffer")
	 																					// retorna -1 caso haja falha, e a quantidade de bytes lidos caso haja sucesso
	 
	 - leSerialAteTimeout(u_char idSerial, char *buffer, u_int qtdChar, u_long timeout)	// faz a leitura de bytes recebidos através da porta serial 
	 																					// (Ex.: leSerialAteTimeout(0, buffer, 10, 1000); - para fazer a leitura de até 10 bytes recebidos 
																						// da COM especificada e gravar no vetor de char "buffer" e continuar tentando fazer essa leitura 
																						// até esgotar o tempo especificado em 'timeout' que é em milisegundos)
	 
	 - int escreveSerial(u_char idSerial, char *buffer, u_int qtdChar);					// faz a escrita na porta serial dos bytes contidos no buffer 
	 																					// (Ex.: escreveSerial(0, buffer, 10); - para escrever na serial 10 bytes contidos no vetor "buffer")
	 																					// retorna 0 caso haja falha, e 1 caso haja sucesso
	 
	 - int serialConectada(u_char idSerial);											// simplesmente retorna o status da conexão (1 = conectado | 0 = desconectado)
	 																					// (ex.: (int teste = serialConectada(0);)
	 
	 
	[ Informação importante ] -------------------------------------------------------------------------------------------------------------------------------------
	
	 Quando for tentar endereçar uma porta COMx cujo "número" for maior que 9 (Ex.: COM10, COM11, ... , COM255) 
	 no Windows é necessário especificar a porta dessa forma:
	 
	  "COM10" deverá ser "\\\\.\\COM10"
	  "COM23" deverá ser "\\\\.\\COM23"
	  
	 Obviamente, as portas COMx cujos números forem menores do que 10 (Ex.: COM1, COM2, ... , COM9, etc.) no 
	 Windows também podem ser especificadas da maneira citada acima, porém elas também funcionam se forem 
	 especificadas da forma simples "COM1", "COM2", ... , "COM9".
	 
	 
	[ Notas de autor e versão ] -----------------------------------------------------------------------------------------------------------------------------------
	
	 Este código foi traduzido e adaptado da versão original em: 'http://playground.arduino.cc/Interfacing/CPPWindows'
	 
	 Versão (se fizer modificações, adicione abaixo da última linha disponível, seguindo o padrão de formato)
	 1.0 por Kesede Rodrigues Julio em 2013/05/16
	 2.0 por Yuri Crisostomo Bernardo em 2013/12/28
	 3.0 por Michel Vieira batista em 2015/04/25 - Função finalizaTodasPortasSeriais() implementada para finalizar todas as comunicações.

===================================================================================================================================================================*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#define ARDUINO_WAIT_TIME 2000		// arduino mega pode demorar até 10 segundos para reiniciar
#define QTD_PORTAS 2				// altere aqui a quantidade de portas seriais que deseja trabalhar simultaneamente

typedef struct Pserial{				// criando uma estrutura para armazenas as informações relevantes da conexão serial
  HANDLE hSerial; 					// handler de comunicação Serial
  int conectada;  					// flag da situação da conexão
  COMSTAT status; 					// contém várias informações sobre a conexão
  DWORD erros;    					// mantém a rastreabilidade do último erro
}Pserial;							// nome da estrutura

Pserial serial[QTD_PORTAS];			// instanciando um vetor de estrururas Pserial que serão utilizadas nas funções


// inicializa a comunicação serial com a porta de comunicação recebida por parâmetro
void inicializaSerial(u_char idSerial, char *nomePorta, u_int baudRate){
    
	serial[idSerial].conectada = 0;											// a conexão ainda não está estabelecida

    // tenta conectar-se à porta através de um CreateFile
    serial[idSerial].hSerial = CreateFile(nomePorta, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if(serial[idSerial].hSerial == INVALID_HANDLE_VALUE){					// verifica se algum erro ocorreu
        if(GetLastError() == ERROR_FILE_NOT_FOUND){							// se ocorreu, imprime erro
            printf("ERRO: Handle was not attached. Razao: %s nao disponivel.\n", nomePorta);			// este erro é impresso quando a porta não está disponível (porta não existe, por exemplo)
        }else{
            printf("ERRO!!!");												// imprime um erro "genérico"
        }
    }else{																	// se estiver conectado corretamente
        DCB dcbSerialParams = {0};											// tenta configurar os parâmetros de cominucação da porta
        if (!GetCommState(serial[idSerial].hSerial, &dcbSerialParams)){		// tenta contactar a porta de comunicação verificando se é possível prosseguir (obtendo os parâmetros de configuração atuais)
            printf("Falha na obtencao dos parametros da serial!");			// se não for possível, mostra um erro
        }else{																// sendo possível, define os parâmetros da comunicação serial com a placa
            dcbSerialParams.BaudRate = baudRate; 							// baud rate           = de acordo com o parâmetro recebido em 'baudRate'
            dcbSerialParams.ByteSize = 8;									// bits de paridade    = 8 bits
            dcbSerialParams.StopBits = ONESTOPBIT;							// bits de parada      = 1
            dcbSerialParams.Parity   = NOPARITY;							// paridade            = sem paridade

	        if(!SetCommState(serial[idSerial].hSerial, &dcbSerialParams)){	// aplica os novos parâmetros à porta e verifica se foram aplicados corretamente
	           printf("ALERTA: Nao poderia atribuir parametros para a Porta Serial");					// caso os parâmetros não tenham sido aplicados corretamente, imprime erro
	        }else{															// caso senham sido aplicados corretamente
	            serial[idSerial].conectada = 1;								// define o estado da nossa porta de comunicação como "conectada"
	            Sleep(ARDUINO_WAIT_TIME);									// aguarda 2 segundos (ARDUINO_WAIT_TIME) aguardando a placa arduino reiniciar 
																			// NOTA: funciona bem com Arduino UNO, mas a placa Arduino Mega pode demorar mais do que isso!
	        }
        }
    }
}

// fecha a conexão NOTA: por alguma razão, seu programa pode não conseguir conectar novamente antes de sua aplicação rodar essa função mais uma vez
void finalizaSerial(u_char idSerial){
    if(serial[idSerial].conectada){											// verifica o estado da conexão antes de tentar desconectar
        serial[idSerial].conectada = 0;										// define que a porta não está mais conectada
        CloseHandle(serial[idSerial].hSerial);								// finaliza o handler da comunicação serial
    }
}

// fecha todas as conexões
void finalizaTodasPortasSeriais(){
     int i;
     for(i = 0 ; i < QTD_PORTAS; i++){
          finalizaSerial(i);     
     }
}

// lê os dados através do handler de comunicação serial e armazena no buffer passado por parâmetro.
// se qtdChar for maior do que o total de bytes disponível, será retornado somente o que estava disponível para ser lido.
// NOTA: ao utilizar esta função, certificar-se de aguardar tempo suficiente para o buffer estar carregado com os valores desejados
// a função retornará -1 quando nada puder ser lido
int leSerial(u_char idSerial, char *buffer, u_int qtdChar){
	
    DWORD bytesRead;														// quantidade de bytes que efetivamente estavam prontos para serem lidos
    u_int toRead;															// quantidade de bytes que foram solicitados a serem lidos
    ClearCommError(serial[idSerial].hSerial, &serial[idSerial].erros, &serial[idSerial].status);	// a função 'ClearCommError' foi utilizada para pegar todas as informações de estado da porta serial

    if(serial[idSerial].status.cbInQue > 0){								// verifica se há algo disponível pronto para ser lido, caso haja faz o que segue
        // aqui uma verificação para saber se a quantidade de dados disponível para ser lida é maior do que a quantidade informada no parâmetro 'qtdChar'
		if(serial[idSerial].status.cbInQue > qtdChar){						// se houver mais bytes disponíves do que solicitados
            toRead = qtdChar;												// a quantidade de bytes que devem ser lidos será limitada à quantidade que foi solicitada
        }else{																// se não (caso haja uma quantidade igual ou menor de bytes disponíveis do que solicitados)
            toRead = serial[idSerial].status.cbInQue;						// a quantidade de bytes que devem ser lidos, será exatamente a quantidade de bytes disponíveis
        }
        if(ReadFile(serial[idSerial].hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0){	// tenta ler a quantidade requerida de bytes e retornar o número de bytes lidos corretamente
            return bytesRead;												// retorna o número de bytes lidos corretamente
        }
    }
    return -1;																// caso não haja nada a ser lido ou tenha ocorrido algum erro, retorna -1
}

// lê os dados através do handler de comunicação serial e armazena no buffer passado por parâmetro.
// se qtdChar for maior do que o total de bytes disponível, será retornado somente o que estava disponível para ser lido.
// NOTA: esta função tem como argumento a espera da resposta do dispositivo conectado (preenchendo o buffer da serial) até o tempo definido passado por 'timeout' em milisegundos
// a função retornará -1 quando nada puder ser lido
int leSerialAteTimeout(u_char idSerial, char *buffer, u_int qtdChar, u_long timeout){
	
    DWORD bytesRead;													// quantidade de bytes que efetivamente estavam prontos para serem lidos
    u_int toRead;														// quantidade de bytes que foram solicitados a serem lidos
    ClearCommError(serial[idSerial].hSerial, &serial[idSerial].erros, &serial[idSerial].status);		// a função 'ClearCommError' foi utilizada para pegar todas as informações de estado da porta serial
    long tentativas = 0;												// contador das tentativas de leitura durante o período de tentativas
	int interval = 100;													// tempo de intervalo entre tentavivas de leitura (cada tentativa deverá ocorrer a cada intervalo, e a soma dos 
																		// intervalos não deverá ultrapassar o tempo total estipulado no timeout) isto é necessário para sincronizar o tempo
																		// total do envio dos bytes com o início de uma tentativa de leitura (para que não haja um início de leitura 
																		// precipitado e uma consequente quebra na leitura dos bytes que ainda não foram enviados pelo dispositivo conectado)
																		// NOTA: taxas de transferências mais altas e mensagens mais curtas aceitam tempos de intervalos menores
	
	while ((timeout / interval) > tentativas){							// enquanto houver tentativas restantes (a razão entre a quantidade de tempo, dividido pelo intervalo resulta na quantidade de tentativas disponíveis)
	    ClearCommError(serial[idSerial].hSerial, &serial[idSerial].erros, &serial[idSerial].status);	// a função 'ClearCommError' foi utilizada para pegar todas as informações de estado da porta serial
		if(serial[idSerial].status.cbInQue > 0){						// verifica se há algo disponível pronto para ser lido, caso haja faz o que segue
	        // aqui uma verificação para saber se a quantidade de dados disponível para ser lida é maior do que a quantidade informada no parâmetro 'qtdChar'
			if(serial[idSerial].status.cbInQue > qtdChar){				// se houver mais bytes disponíves do que solicitados
	            toRead = qtdChar;										// a quantidade de bytes que devem ser lidos será limitada à quantidade que foi solicitada
	        }else{														// se não (caso haja uma quantidade igual ou menor de bytes disponíveis do que solicitados)
	            toRead = serial[idSerial].status.cbInQue;				// a quantidade de bytes que devem ser lidos, será exatamente a quantidade de bytes disponíveis
	        }
	        if(ReadFile(serial[idSerial].hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0){	// tenta ler a quantidade requerida de bytes e retornar o número de bytes lidos corretamente
				return bytesRead;										// retorna o número de bytes lidos corretamente
	        }
	    }
	    tentativas++;													// incremento no contador de tentativas
	    Sleep(interval);												// tempo de espera até a próxima tentativa (necessário para fins de sincronismo - cuidado ao mexer neste tempo)
	}
    return -1;															// caso não haja nada a ser lido ou tenha ocorrido algum erro, retorna -1
}

// escreve dados de um buffer através da conexão serial. retorna 1 quando houver sucesso e 0 quando houver falha
int escreveSerial(u_char idSerial, char *buffer, u_int qtdChar){
	
    DWORD bytesSend;																		// quantidade de bytes enviados corretamente, apenas para completar a função (pode ter mais utilidade em futuras revisões)

    if(!WriteFile(serial[idSerial].hSerial, (void *)buffer, qtdChar, &bytesSend, 0)){		// tenta escrever do buffer na porta serial, e caso resulte em algum erro
        ClearCommError(serial[idSerial].hSerial, &serial[idSerial].erros, &serial[idSerial].status);	// utiliza a função 'ClearCommError' para pegar as informações de estado da porta serial
        return 0;																			// e retorna 0 (falha)
    }else{																					// caso tudo ocorra bem
    	return 1;																			// retorna 1 (sucesso)
    }
}

// verifica se a porta serial está conectada
int serialConectada(u_char idSerial){
    return serial[idSerial].conectada;									// simplesmente retorna o status da conexão
}
