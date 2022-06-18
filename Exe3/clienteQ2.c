#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define MAXLINE 4096

int main(int argc, char **argv) {

   int            sockfd;
   unsigned int   minhaPorta;
   char           sentence[9], upperSentence[9], meuIP[16];
   struct         sockaddr_in servaddr, my_addr;

   /* Criar socket */
   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket error");
      exit(1);
   }

   /* Informações do servidor */
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(13); 

   if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
      perror("inet_pton error");
      exit(1);
   }

   /* Informações do socket */
   bzero(&my_addr, sizeof(my_addr));
   socklen_t len = sizeof(servaddr);
   getsockname(sockfd, (struct sockaddr *) &my_addr, &len);
   inet_ntop(AF_INET, &servaddr.sin_addr, meuIP, sizeof(meuIP));
   minhaPorta = ntohs(servaddr.sin_port);

   /* Printar informações do socket */
   printf("IP address: %s\n", meuIP);
   printf("Port : %u\n", minhaPorta);

   /* Conectar socket ao servidor */
   if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
      perror("connect error");
      exit(1);
   }

   /* Printar informações do servidor */
   printf("Server IP address: %s\n", argv[1]);
   printf("Server Port : %s\n", argv[2]);

   /* Receber mensagem do servidor */
   recv(sockfd, sentence, 9, 0);

   /* Transformar mensagem do servidor em caixa alta */
   for (int a = 0; a < 9; a++) {
      upperSentence[a] = toupper(sentence[a]);
   }

   /* Enviar mensagem transformada para servidor */
   send(sockfd, upperSentence, strlen(upperSentence), 0);

   /* 
   Tentaiva de enviar as informações do cliente como mensagem através do send

   char string[5];
   sprintf(string, "%u", minhaPorta);

   send(sockfd, meuIP, 16, 0);
   send(sockfd, string, 4, 0);
   */

   exit(0);
}
