#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

int main (int argc, char **argv) {
   int    listenfd, connfd, len, backlog, i;
   struct sockaddr_in servaddr, peer_addr;
   char   words[10][20] = { "tiger", "lion", "elephant", "zebra", "horse", "camel", "deer", "crocodile", "rabbit", "cat"};

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

   /* Ler valor de backlog */
   backlog = 0;
   printf("Enter backlog value: ");
   scanf("%i", &backlog);

   /* Caso dê erro ao realizar a união entre o nome e o socket */
   if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
      perror("bind");
      exit(1);
   }

   /* Caso dê erro ao escutar uma conexão */
   if (listen(listenfd, backlog) == -1) {
      perror("listen");
      exit(1);
   }

   /* Loop infinito */
   for ( ; ; ) {

      sleep(10);

      /* Caso dê erro ao aceitar uma conexão */
      if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
         perror("accept");
         exit(1);
      }

      /* Enviar sentença e receber com nova formatação */
      i = rand() % 10;
      len = strlen(words[i]);

      send(connfd, words[i], len, 0);

      /* Receber informações IP e Porta */
      bzero(&peer_addr, sizeof(peer_addr));
      unsigned int lenPeer = sizeof(peer_addr);
      getpeername(connfd, (struct sockaddr*)&peer_addr, &lenPeer);

      printf("Client IP address: %s\n", inet_ntoa(peer_addr.sin_addr));
      printf("Client Port: %d\n", ntohs(peer_addr.sin_port));
      printf("Sentence: %s\n", words[i]);

      if (strcmp(words[i], "bye") == 0) {
         close(connfd);
      }

      recv(connfd, words[i], len, 0);
   }

   return(0);
}
