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



int lire1Par1(int *file, int *dSCl){
	char *unChar=malloc(1);

	int stop=1;
	int total=0;

	while(stop>0){
		errno=0;
		stop = recv(*dSCl,unChar,1,0);
		total+=stop;
		if(stop==-1){
			perror("Erreur recv : ");
			return -1;
		}

		errno=0;
		int err = write(*file,unChar,stop);
		if(err==-1){
			perror("Erreur write() : ");
			return -1;
		}

	}

	close(*file);

	return total;

}


int main(int argc, char const *argv[])
{

	if(argc != 2){
		printf("Utilisation : ./recepfich numeroPort\n");
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

	char *titre=malloc(50);

	int tay = recv(dSClient,titre,50,0);
	if(tay==-1){
		printf("Erreur recv\n");
		exit(EXIT_FAILURE);
	} else { printf("On a recu %d octets\n",tay);}
	

	printf("fichier reçu : %s \n", titre);

	char pathFile[100]="reception/";
	strcat(pathFile,titre);

	int erre = send(dSClient, &tay,sizeof(int),0);
	if(erre==-1){
		perror("Erreur send() : ");
		exit(EXIT_FAILURE);
	} else {printf("On a renvoyé %d octets.\n", erre);}


	errno=0;
	int f = open(pathFile,O_CREAT | O_RDWR, S_IRWXU);
	if(f==-1){
		perror("erreur open() : ");
		exit(EXIT_FAILURE);
	}


	int resultat=lire1Par1(&f,&dSClient);
	if(resultat==-1){
		printf("erreur lire1Par1().\n");
		exit(EXIT_FAILURE);
	}
	printf("On a reçu et écrit : %d octets.\n", resultat);


	close(dSClient);
	close(dS);
	return 0;
}