#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

// Definição do tamanho máximo da mensagem que será tratada
#define MAX_INPUT_LENGTH 80

void main(int argc, char *argv[])
{
    // Verifica se o número de argumentos necessários para execução do programa estão corretos
    if(argc != 3)
    {
        printf("Numero de parametros incorreto: utilizar -> [programa] [IP] [porta]\n");
        exit(1);
    }

    int fd, num_bytes = 0, msg_size;
    char message[MAX_INPUT_LENGTH];
    struct sockaddr_in server;
    int port = atoi(argv[2]);

    // Zera o buffer das mensagens
    bzero(message, MAX_INPUT_LENGTH);
    // Aguarda a captação do teclado para encaminhar a mensagem ao servidor
    printf("Entre com a mensagem: ");
    fgets(message, MAX_INPUT_LENGTH, stdin);
      
    msg_size = strlen(message);
    
    // Remove quebra de linha da msg
	if (message[--msg_size] == '\n')
	    message[msg_size] = '\0';

    // Abre comunicação com o servidor utilizando protocolo TCP/IP e executa tratativa de sucesso
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Nao foi possivel criar o socket!\n");
        exit(1);
    }

    // Configurações do servidor
    server.sin_family = AF_INET;                    // Família do endereço, no caso IPv4
    server.sin_port = htons(port);                  // Configuração da porta
    server.sin_addr.s_addr = inet_addr(argv[1]);    // Configuração do IP
    bzero(&(server.sin_zero), 8);                   //

    // Loop para tentativas de conexão ao servidor
    for(int i = 0; i < 5; i++)
    {
        if(connect(fd, (struct sockaddr *)&server, sizeof(server)) == 0)
            break;

        // Tratativa de falha para caso a conexão nao seja estabelecida corretamente   
        else
        {
            perror("Nao foi possivel conectar ao servidor!\n");
            fflush(stdin);
            exit(1);
        }   
    }

    // Tratativa de falha em caso da mensagem não ser encaminhada corretamente ao servidor
    if(send(fd, message, MAX_INPUT_LENGTH, 0) < 0)
    {
        printf("Mensagem nao foi enviada!\n");
        exit(1);
    }

    // Limpa a variável que recebe o número de bytes da mensagem e limpa o vetor message
    num_bytes = 0;
    bzero(message, MAX_INPUT_LENGTH);
    // Loop de aguardo de chegada da mensagem
    while(num_bytes <= 0)
        num_bytes = recv(fd, message, MAX_INPUT_LENGTH, 0);

    // Imprime a mensagem alterada pelo servidor
    printf("Servidor: %s\n", message);

    close(fd);
    return 0;
}
