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

int escolherJogada() {
   int jogada;

   printf("Qual será sua jogada?\n1 - Pedra\n2 - Tesoura\n3 - Papel");
   scanf("%d", &jogada);

   return jogada;
}

int quemGanhou(int jogador1, int jogador2) {

   if (jogador1 < jogador2) {
      return 1;
   }

   if (jogador2 < jogador1) {
      return 2;
   }

   else {
      return 0;
   }
}

int main (int argc, char **argv) {

   int            ID = 1;
   char           tmp[4];
   char*          jogadores[20][4];
   int            jogada, jogadaB, jogadorB, resultado;

   int            listenfd, udpfd, connfd, len;
   struct         sockaddr_in servaddr, peer_addr;
   unsigned int   lenPeer = sizeof(peer_addr);
   fd_set         rset;

   jogadores[0][0] = 0;

   /* Criar o socket TCP */
   listenfd = socket(AF_INET, SOCK_STREAM, 0);
   bzero(&servaddr, sizeof(servaddr));

   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servaddr.sin_port = htons(13); 

   bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

   /* Caso dê erro ao escutar uma conexão */
   if (listen(listenfd, LISTENQ) == -1) {
      perror("listen");
      exit(1);
   }



   /* Loop infinito */
   for ( ; ; ) {

      FD_SET(listenfd, &rset);
      nready = select((listenfd + 1), &rset, NULL, NULL, NULL);

      /* Conectar com o cliente */
      if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
         perror("accept");
         exit(1);
      }



      /* --------------------------- */
      /* O JOGO */

      /* Permitir conexão com múltiplos clientes */
      /* Receber informações do cliente e colocar numa lista: ID, IP, Porta, Pontuação = 0 */
      bzero(&peer_addr, sizeof(peer_addr));
      getpeername(connfd, (struct sockaddr*)&peer_addr, &lenPeer);

      tmp = sprintf(tmp, "%d", ID);
      jogadores[ID][0] = tmp;
      jogadores[ID][1] = inet_ntoa(peer_addr.sin_addr);
      jogadores[ID][2] = ntohs(peer_addr.sin_port);
      jogadores[ID][3] = "0";
      ID++;
      jogadores[0][0]++;



      /* JOGO CONTRA SERVIDOR */
      /* Envia opções do servidor e recebe jogada escolhida */
      jogada = (rand() % 3) - 1;
      recv(connfd, jogadaB, 4, 0);

      /* Analisa quem ganhou */
      resultado = quemGanhou(jogadaB, jogada);

      if (resultado == 1) {
         tmp = sprintf(tmp, "%d", (int(jogadores[ID][3]) + 1));
         jogadores[ID][3] = tmp;
      }

      /* Envia quem ganhou */
      send(connfd, resultado, 4, 0);



      /* JOGO CONTRA CLIENTE */
      /* Envia opções de cliente para o cliente [ID, IP, Porta, Pontuação */
      send(connfd, jogadores, sizeof(jogadores), 0);

      /* Recebe o cliente escolhido */
      recv(connfd, jogadorB, 4, 0);

      /* Recebe informação do jogador A sobre quem ganhou */
      recv(connfd, resultado, 4, 0);

      /* Atualiza a pontuação */
      if (resultado == 1) {
         tmp = sprintf(tmp, "%d", (int(jogadores[ID][3]) + 1));
         jogadores[ID][3] = tmp;
      }

      if (resultado == 2) {
         tmp = sprintf(tmp, "%d", (int(jogadores[int(jogadorB)][3]) + 1));
         jogadores[int(jogadorB)][3] = tmp;
      }
      
      close(connfd);
   }

   return(0);
}
