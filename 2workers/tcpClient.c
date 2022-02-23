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
#include <unistd.h>
#include <pthread.h>

#define MAX_SIZE 80
#define VETOR_SIZE 10000

typedef struct server {
	int size;
	float * vetor;
    char host[16];
    char porta[6];
}
server;

typedef struct maior_menor {
    float menor;
    float maior;
}
maior_menor;

maior_menor maior_menor_server(server s) {
    struct sockaddr_in ladoServ;
	char bufout[MAX_SIZE];
    int sd;
    float result[2];
	

 	memset((char *)&ladoServ, 0, sizeof(ladoServ)); /* limpa estrutura */
	memset((char *)&bufout, 0, sizeof(bufout));	

    ladoServ.sin_family = AF_INET;
    ladoServ.sin_addr.s_addr = inet_addr(s.host);
    ladoServ.sin_port = htons(atoi(s.porta));

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

    send(sd, s.vetor, VETOR_SIZE, 0);
    recv(sd, result, sizeof(result), 0);
    close(sd);

    maior_menor res;
	res.menor = result[0];
	res.maior = result[1];
    return res;
}

void * maior_menor_thread(void * arg) {
    server * s = (server * ) arg;
    maior_menor * res = (maior_menor * ) malloc(sizeof(maior_menor));
    maior_menor r = maior_menor_server( * s);
    res -> menor = r.menor;
    res -> maior = r.maior;

    pthread_exit(res);
}

int main(int argc, char *argv[])
{
	struct sockaddr_in ladoServ; /* contem dados do servidor 	*/
	int sd;											 /* socket descriptor              */
	int n, k;										 /* num caracteres lidos do servidor */
	char bufout[MAX_SIZE];			 /* buffer de dados enviados  */
	float vetor[VETOR_SIZE];
	float maiorMenor[2];

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

	int n_server = atoi(argv[1]);

	server *s = malloc(n_server*sizeof(server));

	for(int i = 3; i < n_server; i++) {
		s->host[i] = argv[i];
		s->porta[i+1] = atoi(argv[i+1]);
		i++;
	}

	int item = VETOR_SIZE / n_server;
	int items_missing = VETOR_SIZE % n_server;

	    for (int i = 0; i < n_server; i++) {
        if (i == n_server - 1)
            s[i].size = item + items_missing;
        else
            s[i].size = item;

        s[i].vetor = (float * ) malloc(sizeof(float) * s[i].size);

        for (int j = 0; j < s[i].size; j++)
            s[i].vetor[j] = vetor[i * item + j];
    }


	pthread_t * threads = malloc(sizeof(pthread_t) * n_server);

    for (int i = 0; i < n_server; i++) {
        pthread_create( & threads[i], NULL, maior_menor_thread, & s[i]);
    }

    maior_menor ** results = (maior_menor ** ) malloc(sizeof(maior_menor * ) * n_server);
    for (int i = 0; i < n_server; i++) {
        pthread_join(threads[i], (void ** ) & results[i]);
    }

    server final_server;

    final_server.size = n_server * 2;
    final_server.vetor = (float * ) malloc(sizeof(server) * (n_server * 2));
    strcpy(final_server.host, s[0].host);
    strcpy(final_server.porta, s[0].porta);

    for (int i = 0; i < n_server; i++) {
        final_server.vetor[i * 2] = results[i] -> menor;
        final_server.vetor[i * 2 + 1] = results[i] -> maior;
    }

    maior_menor res = maior_menor_server(final_server);
    printf("$ minor: %f major: %f\n", res.menor, res.maior);

    free(final_server.vetor);
    free(threads);
    free(results);

    for (int i = 0; i < n_server; i++) {
        free(s[i].vetor);
    }
	
    free(s);

	return 0;
} /* fim do programa */
