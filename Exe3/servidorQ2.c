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

#define LISTENQ 10

int main (int argc, char **argv) {
   int    listenfd, connfd, len;
   struct sockaddr_in servaddr, peer_addr;
   char   sentence[] = "lowercase";

   time_t rawtime;
   struct tm * timeinfo;

   FILE *fptr;
   fptr = fopen("clients.txt", "w");

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

      /* Instante de conexão */
      time(&rawtime);
      timeinfo = localtime(&rawtime);
      fprintf(fptr, "Connection Time: %s", asctime(timeinfo));

      /* Enviar sentença e receber com nova formatação */
      len = strlen(sentence);
      send(connfd, sentence, len, 0);
      recv(connfd, sentence, len, 0);

      /* Receber informações IP e Porta */
      bzero(&peer_addr, sizeof(peer_addr));
      unsigned int lenPeer = sizeof(peer_addr);
      getpeername(connfd, (struct sockaddr*)&peer_addr, &lenPeer);

      /* Escrever no IP e Porta no arquivo */
      fprintf(fptr, "Client IP address: %s\n", inet_ntoa(peer_addr.sin_addr));
      fprintf(fptr, "Client Port: %d\n", ntohs(peer_addr.sin_port));

      printf("Sentence: %s\n", sentence);
      close(connfd);

      /* Instante de desconexão */
      time(&rawtime);
      timeinfo = localtime(&rawtime);
      fprintf(fptr, "Disconnection Time: %s", asctime(timeinfo));

      /*
      printf("Client IP address: %s\n", inet_ntoa(peer_addr.sin_addr));
      printf("Client Port: %d\n", ntohs(peer_addr.sin_port));
      */
         
      /* 
      Tentaiva de receber as informações IP e Porta do cliente como mensagem através do recv

      char meuIP[16], minhaPorta[5];

      recv(connfd, meuIP, 16, 0);
      recv(connfd, minhaPorta, 4, 0);

      printf("Client IP address: %s\n", meuIP);
      printf("Client Port: %s\n", minhaPorta);
      */
   }

   fclose(fptr);
   return(0);
}
