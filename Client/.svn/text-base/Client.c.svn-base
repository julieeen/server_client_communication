/*
 ============================================================================
 Name        : Client.c
 Author      : 
 Version     :
 Copyright   :
 Description : Client for Networkprogramming
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "Help.h"

#define SERVER_PORT 8000
#define SERVER_ARDR "127.0.0.1"

void deleteWhitespace(char* text);


int main(void) {
	printf("Clientprogramm startet\n");

	int sockfd;
	struct sockaddr_in serveradresse;
	//struct sockaddr_in client_adresse;

	// **********  Socket anfordern  ****************
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("EXIT: Socket error!\n");
		exit(1);
	}

	serveradresse.sin_family = AF_INET; 				//Internet IP Protocol
	serveradresse.sin_port = htons(SERVER_PORT); 	//Host to Network Byteorder
	serveradresse.sin_addr.s_addr = inet_addr(SERVER_ARDR);

	//printf("Port: %d\n", ntohs(serveradresse.sin_port) );
	//printf("Adresse: %s \n\n", inet_ntoa(serveradresse.sin_addr));

	printf("Verbunden mit %s:%i (Socket: %i)\n",inet_ntoa(serveradresse.sin_addr),
			ntohs(serveradresse.sin_port),sockfd);

	int socklen = sizeof(serveradresse);

	// ************ Verbindung aufbauen **************
	// Connect verlangt : Socket, Adresse, AdresslŠnge
	// Serveradresse bisher vom Typ: sockaddr_in
	if(connect(sockfd, (struct sockaddr *) &serveradresse, socklen) < 0){
		perror("Cant find Server.\n");
		return 1;
	}

	printf("Benutze \"help\" um die Hilfe aufzurufen, max 254 Zeichen\n"); // max 74 auch mit scanf(), gets()
	char input[255];
	while (1) {
		input[0] = '\0';

		printf("<< ");
		if(fgets(input,255,stdin) < 0){
			perror("fgets() error");
			continue;
		}

		if( strlen(input) > 254){
			perror("Eingabe hat zu viele Zeichen!");
			continue;
		}

		deleteWhitespace(input);

		int len = strlen(input);
		if (input[len - 1] == '\n')
			input[len - 1] = '\0';

		if (!strcmp(input, "help")) {
			printHelp();
			continue;
		}

		/*
		 * Sendet Kommando
		 */
		//printf("Sendet msg (%s) an Server ... ", input);
		while ((send(sockfd, input, sizeof(input), 0)) < 0);
		//printf(" ok!\n");

		/*
		 * EmpfŠngt BestŠtigung fŸr das Kommando
		 */
		//printf("Waitng for ACK ");
		char answer[5];
		while (recv(sockfd, answer, 5, 0) == 0);
		//printf("...  get: %s\n", answer);


/*
 * *************************************************************************
 */

		/*
		 * Befehle die auch Client betreffen
		 */
		if (!strncmp(input, "exit", 4)) {
			printf("Verbindung wird beendet ... \n");
			break; 					// verlasse While(1)
		}

		if (!strncmp(input, "shutdown", 4)) {
			printf("Verbindung und Server werden beendet ...\n");
			break;
		}





		/*
		 * Wenn kein Systembefehl, kommt eine Antwort auf die Anfrage
		 */
		char result[255];
		// printf("Warte auf Antwort ...\n");
		while (recv(sockfd, result, sizeof(result), 0) == 0);
		printf(">> %s\n", result);

	}

	/*
	 * Beenden der Kommunikation
	 */


	close(sockfd);

	printf("Clientprogramm beendet\n");
	return EXIT_SUCCESS;
}


/*
 * Delete all Whitespace
 */
void deleteWhitespace(char* text){

	int textsize = strlen(text);
	char tmp[textsize];
	strcpy(tmp,text);

	int i = 0;
	int p = 0;
	while(i < textsize){
		text[p] = tmp[i];
		if(tmp[i] == 32){
			p--;
		}
		i++;
		p++;
	}
	text[i-(i-p)] = '\0';
}

