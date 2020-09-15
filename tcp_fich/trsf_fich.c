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
		fprintf(stderr, "Utilisation : ./trsf_fich \n");
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

	char pathFile[100]="emission/";

	char *fichier = malloc(sizeof(char)*30);
	printf("saisir le nom du fichier a envoyer (ex : momo.txt)\n");
	scanf("%s",fichier);

	strcat(pathFile,fichier);

	errno=0;
	int f = open(pathFile,O_RDONLY);
	if(f==-1){
		perror("erreur open() : ");
		exit(EXIT_FAILURE);
	}



	char *c=malloc(1);
	int boucle=1;
	int compteur=0;

	while(boucle){ // On compte le nombre de char dans le fichier

		errno=0;
		int err=read(f,c,1);

		if(err==-1){
			perror("erreur read() : ");
			exit(EXIT_FAILURE);
		} else if(err==0){ // Fin de fichier
			boucle=0;
		}

		compteur++;

	}

	close(f);

	char *leMessageEntier=malloc(compteur); // On crée un tableau de char qui correspond à la taille du fichier lu

	errno=0;
	f = open(pathFile,O_RDONLY);
	if(f==-1){
		perror("erreur open() : ");
		exit(EXIT_FAILURE);
	}


	boucle=1;
	compteur=0;

	while(boucle){ // On copie le contenu du fichier dans un tableau de char

		errno=0;
		int err=read(f,c,1);

		if(err==-1){
			perror("erreur read() : ");
			exit(EXIT_FAILURE);
		} else if(err==0){ // Fin de fichier
			boucle=0;
		}
		else {
			leMessageEntier[compteur]=*c; // On ajoute le char c à la fin du message
			compteur++; // On avance dans le tableau
		}

	}
	 // printf("j'ai lu : %s\n", leMessageEntier); // Enlever le commentaire pour visualiser le message qui va être envoye

	close(f);

	errno=0;
	int env=send(dSock, fichier, strlen(fichier),0);
	if(env==-1){
		perror("Erreur send() : ");
		exit(EXIT_FAILURE);
	} else if(env==0){printf("Socket fermee.\n");
	exit(EXIT_FAILURE);}
	else {printf("On a envoyé %d octets.\n",env);}


	int qqch;
	errno=0;
	int recu=recv(dSock,&qqch,sizeof(int),0);
	if(recu==-1){
		perror("Erreur recv() : ");
		exit(EXIT_FAILURE);
	} else {
		if(qqch==env){
			printf("Tout se passe comme prevu.\n");
		}
		else {printf("On a pas la meme taille de donnee envoye (%d) et recu (%d).\n",env,qqch);}
	}


	
	errno=0;
	env=send(dSock, leMessageEntier, strlen(leMessageEntier),0);
	if(env==-1){
		perror("Erreur send() : ");
		exit(EXIT_FAILURE);
	} else if(env==0){printf("Socket fermee.\n");
	exit(EXIT_FAILURE);}
	else {printf("On a envoyé %d octets.\n",env);}


	close(dSock);

	return 0;
}