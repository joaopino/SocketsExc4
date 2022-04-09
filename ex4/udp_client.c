#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#define BUFLEN 1024    // Tamanho do buffer

void erro(char *s) {
    perror(s);
    exit(1);
}

int main(int argc, char *argv[]) {
    struct sockaddr_in meu_serv;

    int s, recv_len;

    if (argc != 4) {
        char msg[BUFLEN];
        sprintf(msg, "cliente <endereço IP servidor> <porto> <palavra>");
        printf("Try----->%s", msg);
        exit(0);
    }

    char *HOSTNAME = *(argv + 1);
    int PORT = atoi(*(argv + 2));
    char *MSG = *(argv + 3);

    if (gethostbyname(HOSTNAME) == 0) {
        fprintf(stderr, "Host não encontrado!\n");
        exit(-1);
    }

    memset((void *) &meu_serv, 0, sizeof(meu_serv));

    // Preenchimento da socket address structure
    meu_serv.sin_family = AF_INET;
    meu_serv.sin_port = htons(PORT);
    meu_serv.sin_addr.s_addr = htonl(INADDR_ANY);

    socklen_t slen = sizeof(meu_serv);

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        erro("Erro na criação de socket!");
    }

    if (connect(s, (struct sockaddr *) &meu_serv, slen) < 0) {
        erro("Erro na conexão!");
    }

    char buf[BUFLEN];
    sendto(s, MSG, strlen(MSG), 0, (struct sockaddr *) &meu_serv, slen);

    if (strcmp(MSG, "adeus") == 0) {
			exit(0);
	}

    recv_len = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &meu_serv, &slen);

    printf("Recebi uma mensagem do servidor com o endereço %s e o porto %d\n", inet_ntoa(meu_serv.sin_addr), ntohs(meu_serv.sin_port));

    printf("%s", buf);

    // Fecha a socket e termina programa
    close(s);
    return 0;
}
