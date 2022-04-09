#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512	// Tamanho do buffer
#define PORT 9876	// Porto para recepção das mensagens

void erro(char *s) {
	perror(s);
	exit(1);
	}

int main(void) {
  	struct sockaddr_in si_minha, si_outra;

	int s,recv_len;
	socklen_t slen = sizeof(si_outra);
	char buf[BUFLEN];

	char *reservadas[] = {"ola", "saudade", "felicidade", "tristeza", "nao"};

	int val = 0;

	// Cria um socket para recepção de pacotes UDP
	if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		erro("Erro na criação do socket!");
	}

  // Preenchimento da socket address structure
	si_minha.sin_family = AF_INET;
	si_minha.sin_port = htons(PORT);
	si_minha.sin_addr.s_addr = htonl(INADDR_ANY);

	// Associa o socket à informação de endereço
	if(bind(s,(struct sockaddr*)&si_minha, sizeof(si_minha)) == -1) {
		erro("Erro no bind!");
	}

	printf("Servidor à espera de palavras.\n");

	while (1) {   //Inserir novas mensagens sem que o server termine.
		// Espera recepção de mensagem (a chamada é bloqueante)
		if((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_outra, (socklen_t *)&slen)) == -1) {
		erro("Erro no recvfrom!");
		}
		// Para ignorar o restante conteúdo (anterior do buffer)
		buf[recv_len]='\0';

		if (strcmp(buf, "adeus") == 0) {
			exit(0);
		}

		printf("Recebi uma mensagem do cliente com o endereço %s e o porto %d\n", inet_ntoa(si_outra.sin_addr), ntohs(si_outra.sin_port));
		printf("Conteúdo da mensagem recebida: %s\n" , buf);

		for (int i = 0; i < 5; i++) {
			if (strcmp(buf, reservadas[i]) == 0) {
				val = 1;
			}
		}

		if (val == 1) {
			printf("A palavra recebida é reservada!\n");
			sprintf(buf, "A palavra inserida é reservada!\n\n");
			sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &si_outra, slen);
			val = 0;
		}

		else {
			printf("A palavra recebida não é reservada!\n");
			sprintf(buf, "A palavra inserida não é reservada!\n\n");
			sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &si_outra, slen);
		}
	}

	// Fecha socket e termina programa
	close(s);
	return 0;
}
