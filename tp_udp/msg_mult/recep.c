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


int recUDP();


int main(int argc, char const *argv[])
{

	if(argc != 3){
		printf("Utilisation : ./recep 'numeroPort' nbOctetsAttendus\n");
		exit(EXIT_FAILURE);
	}


	struct sockaddr_in addrS;

	addrS.sin_family = AF_INET;
	addrS.sin_addr.s_addr = INADDR_ANY;
	addrS.sin_port = htons(atoi(argv[1]));


	errno=0;
	int dS = socket(PF_INET,SOCK_DGRAM,0);
	if(dS==-1){
		perror("Erreur socket() : ");
		exit(EXIT_FAILURE);
	}


	errno=0;
	int res = bind(dS,(struct sockaddr *) &addrS,sizeof(struct sockaddr_in));
	if(res==-1){
		perror("Erreur bind() : ");
		exit(EXIT_FAILURE);
	}



	struct sockaddr_in addrC;
	socklen_t lgA = sizeof(struct sockaddr_in);

	
	
	char *msg=malloc(sizeof(char)*atoi(argv[2]));


	printf("Veuillez ecrire une caractere puis appuyer sur entree pour continuer.\n");
	char *continuer=malloc(sizeof(char)*2);
	scanf("%s",continuer);

	errno=0;
	res=recvfrom(dS, msg, *argv[2],0,(struct sockaddr *)&addrC,&lgA);
	if(res==-1){
		perror("Erreur recvfrom() : ");
		exit(EXIT_FAILURE);
	}


	 printf("On a reçu %d octets : %s\n",res, msg);
	 int taille=strlen(msg);
	 printf("taille msg : %d\n",taille);
	
	// errno=0;
	// res=sendto(dS,&taille,sizeof(int),0,(struct sockaddr *)&addrC,lgA);
	// if(res==-1){
	// 	perror("Erreur sendto() : ");
	// 	exit(EXIT_FAILURE);
	// }

	// printf("On a renvoyé %d octets.\n",res);

	close(dS);

	return 0;
}