/* *****************************/
/* FGA / Eng. Software / FRC   */
/* Prof. Fernando W. Cruz      */
/* Codigo: tcpServer2.c	       */
/* Alterado para PSPD-LAB2 por: */
/* Guilherme Fernandes 180018019*/
/* Philipe Serafim     160141842*/
/* ******************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define QLEN 5      /* tamanho da fila de clientes  */
#define MAX_SIZE 80 /* tamanho do buffer */
#define VETOR_SIZE 10000
#define THREAD_N 2

float menorMaior[2];
float vetor[VETOR_SIZE];

struct struct_st
{
  int min, max;
  float vetor[VETOR_SIZE];
  float maior;
  float menor;
  pthread_t tid;
};

void *procura(void *p)
{
  struct struct_st *a = (struct struct_st *)p;

  for (int i = a->min; i < a->max; i++)
  {
    if (vetor[i] > a->maior)
    {
      a->maior = vetor[i];
    }
    if (vetor[i] < a->menor)
    {
      a->menor = vetor[i];
    }
  }
}

int atende_cliente(int descritor, struct sockaddr_in endCli, int msgRecebida)
{
  char bufin[MAX_SIZE];
  int n;
  float maior = -99999;
  float menor = 99999;
  struct struct_st t1, t2;

  while (1)
  {
    msgRecebida = recv(descritor, &vetor, VETOR_SIZE * sizeof(float), 0);
    if (msgRecebida == 0)
    {
      printf("Conexão encerrada\n");
      close(descritor);
      exit(0);
    }
    else if (msgRecebida < 0)
    {
      perror("Mensagem não recebida\n");
    }
    else
    {
      printf("Mensagem recebida\n");

      t1.min = 0;
      t1.max = 5000;
      t2.min = t1.max;
      t2.max = VETOR_SIZE;
      pthread_create(&t1.tid, NULL, &procura, (void *) &t1);
      pthread_create(&t2.tid, NULL, &procura, (void *) &t2);

      pthread_join(t1.tid, NULL);
      pthread_join(t2.tid, NULL);

      /*Coloca os valores em um string para poder enviar através de um único send()*/
      if(t1.menor < t2.menor)
        menorMaior[0] = t1.menor;
      else
        menorMaior[0] = t2.menor;
      
      if(t1.maior > t2.maior)
       menorMaior[1] = t1.maior;
      else
        menorMaior[1] = t2.maior;
      if (send(descritor, &menorMaior, VETOR_SIZE * sizeof(float) * 2, 0) < 0)
      {
        perror("Não foi possível responder");
      }
      else
      {
        break;
      }
    }
  } /* fim while */
  fprintf(stdout, "Encerrando conexao com %s:%u ...\n\n", inet_ntoa(endCli.sin_addr), ntohs(endCli.sin_port));
  close(descritor);
} /* fim atende_cliente */

int main(int argc, char *argv[])
{
  struct sockaddr_in endServ; /* endereco do servidor   */
  struct sockaddr_in endCli;  /* endereco do cliente    */
  int sd, novo_sd;            /* socket descriptors */
  int pid, alen, n, msgRecebida;

  if (argc < 3)
  {
    printf("Digite IP e Porta para este servidor\n");
    exit(1);
  }
  memset((char *)&endServ, 0, sizeof(endServ)); /* limpa variavel endServ    */
  endServ.sin_family = AF_INET;                 /* familia TCP/IP   */
  endServ.sin_addr.s_addr = inet_addr(argv[1]); /* endereco IP      */
  endServ.sin_port = htons(atoi(argv[2]));      /* PORTA	    */

  /* Cria socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0)
  {
    fprintf(stderr, "Falha ao criar socket!\n");
    exit(1);
  }

  /* liga socket a porta e ip */
  if (bind(sd, (struct sockaddr *)&endServ, sizeof(endServ)) < 0)
  {
    fprintf(stderr, "Ligacao Falhou!\n");
    exit(1);
  }

  /* Ouve porta */
  if (listen(sd, QLEN) < 0)
  {
    fprintf(stderr, "Falhou ouvindo porta!\n");
    exit(1);
  }

  printf("Servidor ouvindo no IP %s, na porta %s ...\n\n", argv[1], argv[2]);
  /* Aceita conexoes */
  alen = sizeof(endCli);
  for (;;)
  {
    /* espera nova conexao de um processo cliente ... */
    if ((novo_sd = accept(sd, (struct sockaddr *)&endCli, &alen)) < 0)
    {
      fprintf(stdout, "Falha na conexao\n");
      exit(1);
    }
    fprintf(stdout, "Cliente %s: %u conectado.\n", inet_ntoa(endCli.sin_addr), ntohs(endCli.sin_port));
    atende_cliente(novo_sd, endCli, msgRecebida);
  } /* fim for */
  return 0;
} /* fim do programa */