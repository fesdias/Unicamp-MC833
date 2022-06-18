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

int escolherJogador(char (*jogadores)[3], int row) {
   
   int i, j, oponente;

   for(i = 0; i < row; i++) {

         printf("%d ", i);

         for(j = 0; j < 3; j++) {
            printf("%s ", jogadores[i][j]);
         }

         printf("\n");
   }

   printf("Qual o número do seu oponente?");
   scanf("%d", &oponente);

   return oponente;
}

int escolherJogada() {
   int jogada;

   printf("Qual será sua jogada?\n1 - Pedra\n2 - Tesoura\n3 - Papel");
   scanf("%d", &jogada);

   return jogada;
}

int quemGanhou(int jogador1, int jogador2) {

   // Caso dê empate
   if (jogador1 - jogador2 == 0) {
      return 0;
   }

   // Caso jogador 1 vença
   if (jogador1 - jogador2 == -1 || jogador1 = jogador2 == 2) {
      return 1;
   }

   // Caso jogador 2 vença
   if (jogador1 - jogador2 == 1 || jogador1 = jogador2 == -2) {
      return 2;
   }

   else {
      return 0;
   }
}

void printResultado(int resultado) {

   if (resultado == 1) {
      printf("Parabéns! Você derrotou o seu oponente.\nNos vemos no próximo jogo ;)\n");
   }

   if (resultado == 2) {
      printf("Vixi! Seu oponente venceu essa partida.\nBoa sorte no próximo jogo :(\n");
   }

   if (resultado == 0) {
      printf("Belo embate! Pena que acabou em empate.\nMais sorte na próxima vez :)\n");
   }

   else {
      printf("Alguma coisa deu errado com o seu resultado\nO final desse jogo será pra sempre um mistério\n");
   }
}

int enviarResultado(int resultado) {
   if (resultado == 1) {
      return 2;
   }
   if (resultado == 2) {
      return 1;
   }
   else {
      return resultado;
   }
}

int main(int argc, char **argv) {

   /* CRIAÇÃO E CONEXÃO SOCKETS */

   /* ----------- */
   /* CONEXÃO TCP */



   int            sockfd;
   ssize_t        ret_read;
   struct         sockaddr_in servaddr, my_addr;
   char           meuIP[16];


   /* Informações do socket */
   bzero(&my_addr, sizeof(my_addr));
   socklen_t len = sizeof(servaddr);
   getsockname(sockfd, (struct sockaddr *) &my_addr, &len);
   inet_ntop(AF_INET, &servaddr.sin_addr, meuIP, sizeof(meuIP));
   
   /* Criar socket TCP */
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

      /* Conectar socket ao servidor */
   if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
      perror("connect error");
      exit(1);
   }



   /* ----------- */
   /* CONEXÃO UDP */


   /* SERVIDOR UDP */
    int udpfd;
    fd_set rset;
    struct sockaddr_in UDPservaddr, jAaddr;
    bzero(&UDPservaddr, sizeof(UDPservaddr));
  
    // Create a UDP Socket
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);        
    UDPservaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    UDPservaddr.sin_port = htons(PORT);
    UDPservaddr.sin_family = AF_INET; 
   
    // bind server address to socket descriptor
    bind(udpfd, (struct sockaddr*)&UDPservaddr, sizeof(UDPservaddr));
    FD_ZERO(&rset);





   /* --------------------------- */
   /* O JOGO */

   int oponente, ID, command, jogada, jogadaB, resultado;
   char* jogadores;

   /* Criar ID Único (pedir para servidor) */
   recv(sockfd, ID, 4, 0);
   command = 0;

   /* LOOP Interface: Jogo contra servidor ou contra cliente */
   do {

      FD_SET(udpfd, &rset);
      printf("Quem será seu adversário?\n1 - A Máquina\n2 - Outro jogador\n0 - Sair do Jogo\n");
      scanf("%d", command);

      /* Se é o jogador B - server */
      if (FD_ISSET(udpfd, &rset)) {

         printf("Você foi escolhido por um oponente - chegou o momento da batalha.");

         /* Envia escolha para jogador A */
         jogadaB = escolherJogada();
         sendto(udpfd, (int*)jogadaB, 4, 0, (struct sockaddr*)&jAaddr, sizeof(jAaddr));

         /* Recebe informação do jogador A sobre quem ganhou */
         len = sizeof(jAaddr);
         recvfrom(udpfd, resultado, 4, 0, (struct sockaddr*)&jAaddr, &len);
         printResultado(resultado);

         /* volta começo loop */
         continue;
      }

      if (command == 0) {
         break;
      }

      /* JOGO CONTRA SERVIDOR */
      if (command == 1) {

         /* Recebe opções do servidor e envia jogada escolhida */
         jogada = escolherJogada();
         send(sockfd, jogada, 4, 0);

         /* Recebe resultado do jogo */
         recv(sockfd, resultado, 4, 0);
         printResultado(resultado);

         /* volta começo loop */
         continue;
      }
      
      /* JOGO CONTRA CLIENTE */
      if (command == 2) {

         /* Recebe do servidor opções de outros clientes */
         recv(sockfd, jogadores, );
         oponente = escolherJogador(jogadores, int(jogadores[0][0]));

         /* Envia para os servidor o cliente escolhido */
         send(sockfd, oponente, 4, 0);

         /* CONEXÃO UDP - conecta-se ao jogador escolhido */
         int udpfd;
         struct sockaddr_in j2addr;
            
         // clear servaddr
         bzero(&j2addr, sizeof(j2addr));
         j2addr.sin_addr.s_addr = inet_addr(jogadores[oponente][1]);
         j2addr.sin_port = htons(jogadores[oponente][2]);
         j2addr.sin_family = AF_INET;
            
         // create datagram socket
         udpfd = socket(AF_INET, SOCK_DGRAM, 0);
            
         // connect to server
         if (connect(udpfd, (struct sockaddr *)&j2addr, sizeof(j2addr)) < 0) {
            printf("\n Error : Connect Failed \n");
            exit(0);
         }

         /* Se é o jogador A - cliente */
         /* Escolhe sua jogada e envia para jogador B escolher tbm */
         jogada = escolherJogada();
         recvfrom(udpfd, jogadaB, 4, MSG_CONFIRM, (const struct sockaddr *) &j2addr, sizeof(j2addr));


         /* Analisa quem ganhou */
         resultado = quemGanhou(jogada, jogadaB);
         printResultado(resultado);

         /* Envia quem ganhou para jogador B e servidor */
         sendto(udpfd, int(enviarResultado(resultado)), 4, MSG_CONFIRM, (const struct sockaddr *) &j2addr, sizeof(j2addr));
         send(sockfd, resultado, 4, 0);

         /* volta começo loop */
         continue;
      }

      else {
         printf("Comando inválido\nQuem será seu adversário?\n1 - A Máquina\n2 - Outro jogador\n0 - Sair do Jogo\n");
         scanf("%d", command);
      }

   } while (command != 0);

   exit(0);
}
