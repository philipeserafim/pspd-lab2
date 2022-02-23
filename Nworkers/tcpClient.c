/* ******************************/
/* FGA/Eng. Software/ FRC       */
/* Prof. Fernando W. Cruz       */
/* Codigo: tcpClient2.c         */
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
#include <math.h>
#include <pthread.h>

#define MAX_SIZE 80
#define VETOR_SIZE 10000

struct struct_st
{
  int min, max;
  float vetor[VETOR_SIZE];
  float maior;
  float menor;
  pthread_t tid;
};

void* maior_menor(struct struct_st * p) {
	struct sockaddr_in address;
	
	address.sin_family = AF_INET; /* config. socket p. internet*/
	ladoServ.sin_addr.s_addr = inet_addr(argv[1]);
	ladoServ.sin_port = htons(atoi(argv[2]));
	
}

void procuraMaiorMenor(void * p){
	struct struct_st* a = (struct struct_st *) p;
}

int main(int argc, char *argv[])
{
	struct sockaddr_in ladoServ; /* contem dados do servidor 	*/
	int sd;											 /* socket descriptor              */
	int n, k;										 /* num caracteres lidos do servidor */
	char bufout[MAX_SIZE];			 /* buffer de dados enviados  */
	float vetor[VETOR_SIZE];
	float maiorMenor[2];
	struct struct_st *t;

	/*Gerador do vetor de 10k posições do lado do cliente*/
	for (int i = 0; i < VETOR_SIZE; i++)
	{
		vetor[i] = sqrt(pow((i - VETOR_SIZE / 2), 2));
	}

	/* confere o numero de argumentos passados para o programa */
	if (argc < 3)
	{
		printf("uso correto: %s <ip_do_servidor> <porta_do_servidor>\n", argv[0]);
		exit(1);
	}

	t = malloc((argc - 2) * sizeof(struct struct_st));

	memset((char *)&ladoServ, 0, sizeof(ladoServ)); /* limpa estrutura */
	memset((char *)&bufout, 0, sizeof(bufout));			/* limpa buffer */

	ladoServ.sin_family = AF_INET; /* config. socket p. internet*/
	ladoServ.sin_addr.s_addr = inet_addr(argv[1]);
	ladoServ.sin_port = htons(atoi(argv[2]));

	for(int i = 0; i < (argc-2); i++){
		pthread_create(&t[i].tid, NULL, &funcao, (&t[i]));
	}
	for(int i = 0; i < (argc-2); i++){
		pthread_join(t[i].tid, NULL);
	}

	/* Cria socket */
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0)
	{
		fprintf(stderr, "Criacao do socket falhou!\n");
		exit(1);
	}

	/* Conecta socket ao servidor definido */
	if (connect(sd, (struct sockaddr *)&ladoServ, sizeof(ladoServ)) < 0)
	{
		fprintf(stderr, "Tentativa de conexao falhou!\n");
		exit(1);
	}
	/*while para comunicar com servidor*/
	while (1)
	{
		if (send(sd, &vetor, VETOR_SIZE * sizeof(float), 0) < 0)
		{
			perror("Falha no envio");
		}
		else
		{
			printf("Sucesso no envio\n");
		}

		if (recv(sd, &maiorMenor, 2 * sizeof(float), 0) < 0)
		{
			perror("Falha na resposta\n");
		}
		else
		{
			printf("Maior: %f | Menor: %f\n", maiorMenor[0], maiorMenor[1]);
			close(sd);
			break;
		}
	} /* fim while */

	printf("------- encerrando conexao com o servidor -----\n");
	return (0);
} /* fim do programa */
