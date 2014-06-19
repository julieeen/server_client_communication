/*
 ============================================================================
 Name        : Server.c
 Author      : 
 Version     :
 Copyright   : Team 4
 Description : Server fur Projekt 1
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
 * Socket API Libary
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/*
 * Datenbank & Parser & Zugriffskontrolle
 */
#include "database.h"
#include "Parser.h"
#include "AccessControl.h"

#define PORT 8000
#define MAXLVs 10

int main(void) {
	printf("Serverprogramm startet\n");

	int sockfd, newsocket;
	struct sockaddr_in serveradresse;
	struct sockaddr_in client_adresse;

	// **********  Socket anfordern  ****************
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error!");
		exit(1);
	}

	/*
	 * Socketopt : REUSEADDR setzen
	 * Aus dem Code von der 2 Hausaufgabe der VL BS
	 */
	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt() error!");
		exit(1);
	}

	/*
	 * Serveradresse konfig. (Adresstyp, Adresse, Port)
	 */
	serveradresse.sin_family = AF_INET; 				// Internet IP Protocol
	serveradresse.sin_port = htons(PORT); 	//Host to Network Byteorder, PORT
	serveradresse.sin_addr.s_addr = INADDR_ANY;		//holt sich die IP

	printf("Server auf %s:%i (Socket: %i)\n", inet_ntoa(serveradresse.sin_addr),
			ntohs(serveradresse.sin_port), sockfd);

	/*
	 * Socketbind, holt konfigurierten Socket vom OS
	 */
	int socklen = sizeof(struct sockaddr_in);
	if ((bind(sockfd, (struct sockaddr *) &serveradresse, socklen)) < 0) {
		perror("bind() error!");
		exit(1);
	}

	/*
	 *	Client kann sich immer weider am Server anmelden und abmelden
	 */
	int shutdown = 0;
	while (shutdown == 0) {
		printf("Server wartet auf neue Verbindung ... \n");
		/*
		 * lauscht auf Socket ob jmd connecten will (max 3)
		 */
		if ((listen(sockfd, 3)) < 0) {
			perror("listen() error!");
			exit(1);
		}

		/*
		 * EmpfŠngt connect und nimmt an.
		 * Dabei wird ein neuer Socket fur diese Kommunikation erstellt
		 */
		int sin_size = sizeof(struct sockaddr_in);
		if ((newsocket = accept(sockfd, (struct sockaddr*) &client_adresse,
				&sin_size)) < 0) {
			perror("accept() error, newsocket kann nicht erstellt werden");
			exit(1);
		}
		printf("Neuer Client verbunden: %i \n", newsocket);

		/*
		 * Ab hier lauft die Kommunikation zw Server & Client in dauerschleife
		 * Der Client gibt jeweils 1 Befehl, der Server macht dann irgendwas,
		 * antwortet dann mit ACK (oder Daten) und wartet dann auch neue Befehle
		 */
		char buf[255];
		while (1) {
			buf[0] = '\0';
			while (recv(newsocket, buf, 255, 0) == 0)
				;
			printf("Nachricht vom Client: %s \n", buf);

			send(newsocket, "ACK\n", 5, 0);

			if (!strcasecmp(buf, "exit")) {
				logout();
				break;

			}

			if (!strcasecmp(buf, "shutdown")) {
				logout();
				shutdown = 1;
				break;
			}

			/*
			 * Datanbankoperation
			 * Ruft den Parser mit dem gesendet Befehlt auf
			 * Antwort vom Parser wird direkt an Client gesendet
			 */
			char* nachricht;
			nachricht = doThis(buf);// Ruft den Paser auf mit dem emfangenen Befehl
			char msg[255];
			strcpy(msg, nachricht);
			printf("Antwort auf Anfrage : %s\n", msg);

			while ((send(newsocket, msg, sizeof(msg), 0)) < 0)
				// Sendet die Antwort des Parsers an den Client
				;

		}

	}
	/*
	 * Beenden des Servers
	 */

	if ((close(newsocket)) == -1) {
		perror("close()");
	}
	if ((close(sockfd)) == -1) {
		perror("close()");
	}
	printf("Serverprogramm beendet\n");
	return 0;
}

