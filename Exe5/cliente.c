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
#include <poll.h>

int main(int argc, char **argv) {

   int            sockfd;
   ssize_t        ret_read;
   struct         sockaddr_in servaddr, my_addr;
   char           meuIP[16];
   char           fileIN[20000], fileOUT[20000];
   struct         pollfd input[1];

   input[0].fd = 0;
   input[0].events = POLLIN;
   
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

   /*ENVIAR AQUIVO DE ENTRADA */
   do {
      poll(input, 1, 1);
      ret_read = read(0, fileIN, 20000);
      send(sockfd, fileIN, strlen(fileIN), 0);

      recv(sockfd, fileOUT, 20, 0);
      fputs(fileOUT, stdout);

   } while (ret_read != 0);


   exit(0);
}
