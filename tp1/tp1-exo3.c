#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<arpa/inet.h>


int main(int argc, char** argv){
	

	struct addrinfo addrHints; // On crée une structure d'adresse qui contient les éléments de comparaison pour getaddrinfo()
	struct addrinfo *addrRes, *aux; // Pointeur sur la structure d'adresse resultat de getaddrinfo()
	struct sockaddr *addr; // Pointeur sur la structure générale qui stocke l'addresse
	struct sockaddr_in *addr4; // Pointeur sur la structure qui stocke les addresses IPV4
	char *addrText; // Pointeur sur l'adresse au format texte

	char *host=malloc(sizeof(char)*100); // On alloue 100 octets
	//char *serv=malloc(sizeof(char)*100);

	if(argc!=1){
		fprintf(stderr, "Un seul argument : %s\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	addrHints.ai_family=AF_INET; // on limite aux addresse IPV4
	addrHints.ai_socktype=0; // getaddrinfo() peut renvoyer des adresses de n'importe quel type de socket
	addrHints.ai_protocol=0; // getaddrinfo() peut renvoyer des adresses de n'importe quel type d'adresse
	addrHints.ai_flags=0; // Pas de flags nécessaires
	addrHints.ai_addrlen=0; // Toutes les tailles d'adresses acceptées
	addrHints.ai_addr=NULL;
	addrHints.ai_canonname=NULL;
	addrHints.ai_next=NULL;

	printf("Veuillez entrer un nom d'hote. (ex : nike.com , apple.com , reebok.com ...)\n");
	char *nomHote=malloc(sizeof(char)*100); // Hote saisi par l'utilisateur
	scanf("%s",nomHote);
	

	int err = getaddrinfo(nomHote,NULL,&addrHints,&addrRes);

	if(err!=0){
		fprintf(stderr, "Erreur getaddrinfo : %s\n", gai_strerror(err));
		exit(EXIT_FAILURE);
	}
	
	printf("L'hote %s a pour adresses : \n", nomHote);

	for(aux = addrRes; aux != NULL; aux = aux->ai_next){ // On parcourt toutes les adresses mise dans addrRes par getaddrinfo()
		if(aux->ai_family == AF_INET){// Si c'est une adresse IPV4

			// Récupération des adresses pour le nom d'hote entré par l'utilisateur
			addrText = malloc(aux->ai_addrlen); // On alloue à addrText la taille mémoire équivalente à la taille de l'adresse contenue dans aux
			addr = aux->ai_addr; // On attribue l'adresse de aux à addr (la struct générale qui stocke les adresses)
			addr4 = (struct sockaddr_in *) addr; // addr4 stocke l'adresse IPV4
			inet_ntop(AF_INET,&addr4->sin_addr, addrText, aux->ai_addrlen); // On converti l'adresse binaire contenue dans addr4 au format text dans addrText
			printf("- %s ", addrText); // On affiche l'adresse au format texte
			
			// Récupération nom d'hote pour chaque IP
			err = getnameinfo(addr,aux->ai_addrlen,host,100,NULL,0,NI_NAMEREQD);
			if(err!=0){
				fprintf(stderr, "Erreur getnameinfo : %s\n", gai_strerror(err));
				exit(EXIT_FAILURE);
			}
			printf("de nom : %s\n", host);
		}
	}


	// On libère l'espace mémoire alloué
	freeaddrinfo(addrRes);
	freeaddrinfo(aux);
	free(addrText);
	free(host);
	free(nomHote);


	return 0;
	
}