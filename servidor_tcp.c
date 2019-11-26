#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

// Definição do tamanho máximo da mensagem que será tratada
#define MAX_INPUT_LENGTH 80

// Protótipos das funções
void server(void *arg);
char* revertString(char *s);

// Função que realiza a troca de letras minúsculas para maiúsculas e altera a ordem da mensagem
char* revertString(char *s) 
{
    int i = 0, j = 0;
    char *aux_string = malloc ( sizeof(char) * strlen(s));

    while (s[i]) 
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
                s[i] = tolower(s[i]);                   // Altera de maíscula para minúscula
        else if (s[i] >= 'a' && s[i] <= 'z')            // Altera de minúscula para maiúscula
                s[i] = toupper(s[i]);
            i++;
    }

    strcpy(aux_string, s);
    // Altera a ordem da frase: primeira letra será a última
    for (i = strlen(aux_string)-1; i >= 0; i--) 
    {
        s[j] = aux_string[i];
	    j++;
    }

    return s;
}

// Função para tratativa de clientes na mesma porta do servidor
void server(void *arg)
{
    // Cria o novo cliente
    int new_client = (*(int *)arg);
    int i = 0;
    char message[MAX_INPUT_LENGTH];
    char* converted_message;

    // Limpa o buffer
    bzero(message, MAX_INPUT_LENGTH);

    // Tratativa de falha para caso a mensagem não tenha chegado ao servidor corretamente 
    if((recv(new_client,message,MAX_INPUT_LENGTH,0)) < 0)
    {
        perror("Nao foi possivel ler a mensagem do cliente!\n");
        exit(1);
    }

    // Tratativa de falha para caso a mensagem não tenha sido encaminhada ao cliente corretamente
    if(send(new_client, revertString(message), MAX_INPUT_LENGTH, 0) < 0)
    {
        perror("Nao foi possivel encaminhar a mensagem ao cliente!\n");
    }

    // Fecha a comunicação entre cliente e servidor
    close(new_client);
}

int main(int argc, char *argv[])
{
    // Verifica se o número de argumentos necessários para execução do programa estão corretos
    if(argc != 2)
    {
        printf("Numero de parametros incorreto: utilizar -> [programa] [porta]\n");
        exit(1);
    }

    int port, sock, socketfd;
    struct sockaddr_in clientaddr, serveraddr;

    // Verificação de qual porta o servidor será aberto
    port = atoi(argv[1]);

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Nao foi possivel criar o socket!\n");
        exit(1);
    }

    // Configurações do servidor
    serveraddr.sin_family = AF_INET;                    // Família do endereço, no caso IPv4
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);     // Recebe acesso de qualquer IP
    serveraddr.sin_port = htons(port);                  // Configuração do número da porta

    // Tratativa de falha para caso o bind tenha sido executado corretamente
    if(bind(sock, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr)) < 0)
    {
        perror("Nao foi possivel estabelecer a conexao!\n");
        exit(1);
    }

    // Servidor consegue escutar até 5 clientes ao mesmo tempo
    if(listen(sock,5) < 0)
    {
        perror("Nao foi possivel encontrar clientes");
        exit(1);
    }

    // Loop para criação de threads e tratativas de falha.
    while (1)
    {
        socklen_t lenaddr = sizeof(clientaddr);
        if((socketfd = accept(sock, (struct sockaddr *)&clientaddr, &lenaddr)) < 0)
        {
            perror("Nao foi possivel aceitar a conexao!\n");
            continue;
        }

        server(&socketfd);
    }
    
}
