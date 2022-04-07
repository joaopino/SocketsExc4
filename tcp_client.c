/**********************************************************************
 * CLIENTE liga ao servidor (definido em argv[1]) no porto especificado
 * (em argv[2]), escrevendo a palavra predefinida (em argv[3]).
 * USO: >cliente <enderecoServidor>  <porto>  <Palavra>
 **********************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include <arpa/inet.h>

#define BUF_SIZE 1024

void erro(char *msg);

int count = 0;   //Contador de clientes que estabelecem ligação.

int main(int argc, char *argv[]) {
  char endServer[100];
  int fd;
  struct sockaddr_in addr;
  struct hostent *hostPtr;

  char dest[INET6_ADDRSTRLEN];   //String que iŕa conter o endereço IPv4 em string.

  if (argc != 4) {
    printf("cliente <host> <port> <string>\n");
    exit(-1);
  }

  strcpy(endServer, argv[1]);
  if ((hostPtr = gethostbyname(endServer)) == 0)
    erro("Não consegui obter endereço");

  bzero((void *) &addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
  addr.sin_port = htons((short) atoi(argv[2]));

  if ((fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	  erro("socket");
  if (connect(fd,(struct sockaddr *)&addr,sizeof (addr)) < 0)
	  erro("Connect");

  write(fd, argv[3], 1 + strlen(argv[3]));

  printf("Received from server:\n");

  //inet_ntop(AF_INET, , dest, BUF_SIZE);

  printf("Server received connection from (IP:port) %s:%d; already received %d connections!\n", dest, addr.sin_port, count);

  count++;

  close(fd);
  exit(0);
}

void erro(char *msg) {
  printf("Erro: %s\n", msg);
	exit(-1);
}
