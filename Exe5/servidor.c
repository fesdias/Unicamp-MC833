#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LISTENQ 10

int main (int argc, char **argv) {

   int    listenfd, connfd, len;
   struct sockaddr_in servaddr, peer_addr;
   char   sentence[20000];

   /* Caso dê erro ao criar o socket */
   if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("socket");
      exit(1);
   }

   /* Cria arquivo descritor */
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family      = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servaddr.sin_port        = htons(13);   

   /* Caso dê erro ao realizar a união entre o nome e o socket */
   if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
      perror("bind");
      exit(1);
   }

   /* Caso dê erro ao escutar uma conexão */
   if (listen(listenfd, LISTENQ) == -1) {
      perror("listen");
      exit(1);
   }

   /* Loop infinito */
   for ( ; ; ) {

      /* Caso dê erro ao aceitar uma conexão */
      if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
         perror("accept");
         exit(1);
      }

      /* Receber frase do cliente */
      recv(connfd, sentence, len, 0);

      /* Enviar frase para o cliente */
      len = strlen(sentence);
      send(connfd, sentence, len, 0);

      /* Receber informações IP e Porta */
      bzero(&peer_addr, sizeof(peer_addr));
      unsigned int lenPeer = sizeof(peer_addr);
      getpeername(connfd, (struct sockaddr*)&peer_addr, &lenPeer);
      printf("Client IP address: %s\n", inet_ntoa(peer_addr.sin_addr));
      printf("Client Port: %d\n", ntohs(peer_addr.sin_port));

      close(connfd);
   }

   return(0);
}
