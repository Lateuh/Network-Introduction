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

int envUDP();

int main(int argc, char **argv){


	if(argc != 1){
		fprintf(stderr, "Utilisation : ./emet\n");
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
	int dSock = socket(PF_INET,SOCK_DGRAM,0);
	if(dSock==-1){
		perror("Erreur socket() : ");
		exit(EXIT_FAILURE);
	}

	
	socklen_t lgA = sizeof(struct sockaddr_in);


	printf("Veuillez entrer un message a envoyer (sans espace).\n");
	char *msg1=malloc(sizeof(char)*30);
	//msg=argv[1];
	scanf("%s",msg1); // fonctionne mal avec le scanf : envoie seulement le premier mot
	// ceci car scanf interprète l'espace comme un séparateur donc sépare le message
	// en plusieurs chaine de caractere


	printf("Veuillez entrer un message a envoyer (sans espace).\n");
	char *msg2=malloc(sizeof(char)*30);
	scanf("%s",msg2);

	errno=0;
	int env = sendto(dSock, msg1, strlen(msg1),0,(struct sockaddr *) &adServ, lgA);
	if(env==-1){
		perror("Erreur sendto() : ");
		exit(EXIT_FAILURE);
	}


	//printf("Message envoyé. On a envoyé %d octets.\n",env);



	errno=0;
	int env2 = sendto(dSock, msg2, strlen(msg2),0,(struct sockaddr *) &adServ, lgA);
	if(env2==-1){
		perror("Erreur sendto() : ");
		exit(EXIT_FAILURE);
	}

	printf("env : %d || env2 : %d\n",env,env2);

	// int tay=-1;
	// errno=0;
	// int recu = recvfrom(dSock, &tay, sizeof(int),0,(struct sockaddr *)&adServ,&lgA);
	// if(recu==-1){
	// 	perror("Erreur recvfrom() : ");
	// 	exit(EXIT_FAILURE);
	// } else {
	// 	if(tay==(env+env2)){
	// 		printf("Tout se passe comme prevu %d.\n",recu);
	// 	}
	// 	else {printf("On a pas la meme taille de donnee envoye (%d) et recu (%d).\n",env+env2,tay);}
	// }

	// printf("On a reçu l'accusé de reception. Le serveur a reçu et renvoyé %d octets.\n",tay);
	
	close(dSock);

	return 0;
}