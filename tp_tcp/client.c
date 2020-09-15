#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/stat.h>



int main(int argc, char const *argv[])
{

	if(argc != 1){
		fprintf(stderr, "Utilisation : ./client\n");
		exit(EXIT_FAILURE);
	}



	struct sockaddr_in adServ;

	int *numPort=malloc(sizeof(int));
	printf("Entrez un numero de port\n");
	scanf("%d", numPort);

	char *addrV4=malloc(sizeof(int));
	printf("Entrez une adresse IPV4\n");
	scanf("%s", addrV4);


	adServ.sin_family = AF_INET;
	adServ.sin_port = htons(*numPort);
	
	errno=0;
	int res = inet_pton(AF_INET, addrV4, &(adServ.sin_addr));
	if(res==0){
		printf("Erreur inet_pton : adresse réseau non valide pour la famille indiquee.\n");
		exit(EXIT_FAILURE);

	} else if(res==-1){
		perror("Erreur inet_pton() : ");
		exit(EXIT_FAILURE);
	}


	
	errno=0;
	int dSock = socket(PF_INET,SOCK_STREAM,0);
	if(dSock==-1){
		perror("Erreur socket() : ");
		exit(EXIT_FAILURE);
	}

	
	socklen_t lgA = sizeof(struct sockaddr_in);

	errno = 0;
	res = connect(dSock,(struct sockaddr *) &adServ,lgA);
	if(res==-1){
		perror("Erreur connect() : ");
		exit(EXIT_FAILURE);
	}


	printf("Veuillez entrer un message a envoyer (sans espace).\n");
	char *msg1=malloc(sizeof(char)*30);
		//msg=argv[1];
		scanf("%s",msg1); // fonctionne mal avec le scanf : envoie seulement le premier mot
		// ceci car scanf interprète l'espace comme un séparateur donc sépare le message
		// en plusieurs chaine de caractere


	// printf("Veuillez entrer un message a envoyer (sans espace).\n");
	// char *msg2=malloc(sizeof(char)*30);
	// scanf("%s",msg2);



	errno=0;
	int env=send(dSock, msg1, strlen(msg1),0);
	if(env==-1){
		perror("Erreur send() : ");
		exit(EXIT_FAILURE);
		} else if(env==0){
		printf("Socket fermee.\n");
		exit(EXIT_FAILURE);
	} else {printf("On a envoyé %d octets.\n",env);}



	// 	errno=0;
	// int env2=send(dSock, msg2, strlen(msg2),0);
	// if(env2==-1){
	// 	perror("Erreur send() : ");
	// 	exit(EXIT_FAILURE);
	// } else if(env2==0){printf("Socket fermee.\n");
	// exit(EXIT_FAILURE);}
	// else {printf("On a envoyé %d octets.\n",env2);}



		free(msg1);
		close(dSock);

		return 0;
	}