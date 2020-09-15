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

	if(argc != 3){
		printf("Utilisation : ./serv numeroPort nbOctetsAttendus\n");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in addrS;

	errno=0;
	int dS = socket(PF_INET,SOCK_STREAM,0);
	if(dS==-1){
		perror("Erreur socket() : ");
		exit(EXIT_FAILURE);
	}

	addrS.sin_family = AF_INET;
	addrS.sin_addr.s_addr = INADDR_ANY;
	addrS.sin_port = htons(atoi(argv[1]));

	errno=0;
	int res = bind(dS,(struct sockaddr *) &addrS,sizeof(struct sockaddr_in));
	if(res==-1){
		perror("Erreur bind() : ");
		exit(EXIT_FAILURE);
	}

	errno=0;
	res=listen(dS,10);
	if(res==-1){
		perror("Erreur listen() : ");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in addrC;
	socklen_t lgA = sizeof(struct sockaddr_in);

	errno=0;
	int dSClient = accept(dS, (struct sockaddr*) &addrC, &lgA);
	if(dSClient==-1){
		perror("Erreur accept() : ");
		exit(EXIT_FAILURE);
	}



	// printf("Pret a recevoir un message. Veuillez ecrire une caractere puis appuyer sur entree pour continuer.\n");
	// char *continuer=malloc(sizeof(char)*2);
	// scanf("%s",continuer);

	int attendu=atoi(argv[2]);
	int recu=0;

	char *msg=malloc(sizeof(char)*attendu);

	while(recu!=attendu){
		recu = recv(dSClient,msg+recu,attendu-recu,0);
		if(recu==-1){
			printf("Erreur recv\n");
			exit(EXIT_FAILURE);
		} else if(recu==0) {
			printf("On a recu : %s\n",msg);
			exit(EXIT_FAILURE);}
		else { printf("On a recu %d octets\n",recu);}

	}

	printf("On a recu : %s\n",msg);

	free(msg);
	close(dSClient);
	close(dS);

	return 0;
}