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

char * reverse(char s[20]) {
    int length = strlen(s);
    int c, i, j;

    for (i=0, j=length - 1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }

    return s;
}

int main(int argc, char **argv) {

   int            sockfd;
   char           sentence[20], meuIP[16];
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

   /* Conectar socket ao servidor */
   if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
      perror("connect error");
      exit(1);
   }

   /* Receber mensagem do servidor */
   recv(sockfd, sentence, 20, 0);

   /* Inverter mensagem do servidor */
   printf("Sentence: %s\n", reverse(sentence));

   /* Enviar mensagem transformada para servidor */
   send(sockfd, reverse(sentence), strlen(reverse(sentence)), 0);

   /* 
   Tentaiva de enviar as informações do cliente como mensagem através do send

   char string[5];
   sprintf(string, "%u", minhaPorta);

   send(sockfd, meuIP, 16, 0);
   send(sockfd, string, 4, 0);
   */

   exit(0);
}
