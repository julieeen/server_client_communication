#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AccessControl.h"

/*
 * AccessControl sorgt sich um die Rechteverteilung
 *
 * In unserem Fall:
 * READ.UPDATE/CREATE.DELETE
 * 111. Dozent - read, create, update, delete
 * 1xx. Student - read (nur sich selbst)
 * 000. Gast - best
 *
 * Client kann Server fragen was er darf.
 * Client kann mehr Rechte vom Server einfordern (login(name,pw))
 * --> gilt bis zum nächsten exit
 *
 * Aus der Aufgabenstellung, (Frage: Normale Benutzer haben mehr Rechte als Studenten.)
 * Administratoren mit xxx user-ID haben volle Zugriffsrechte,
 * d.h. dürfen Dateien einfügen und editieren, während normale
 * Benutzer nur Lesezugriff haben und jeder Student nur seine
 * eigenen Datensätze sehen kann.
 */

// Hat Werte zwischen 0 - 111
int rights = 0;
char usermatrikel[32];
FILE* file;

/*
 * Gibt den aktuellen Wert von rights aus
 */
int getStatus() {
	return rights;
}

/*
 * Halet Namen und die Matrikel des angemeldetetn Nutzers
 */
void getIdentity(char matrikel[]){
	if(rights == 0){
		strcpy(matrikel, "000000000");	//No Identity
	}
	else{
		strcpy(matrikel,usermatrikel);
	}
}

/*
 * Hashed Passworter
 */
char* cryptofoo(char foo[]) {
	char* value = malloc(sizeof(char)*32);
	int i = 0;
	int p = 0;
	int versatz = 0;
	int magic = (strlen(foo));
	while (p < 30) {
		switch (p % 4) {
		case 1:
			value[p] = (foo[i] + magic) % 10 + 97;
			break;
		case 2:
			value[p] = (foo[i] + magic) % 10 + 48;
			break;
		default:
			value[p] = (foo[i] + magic) % 10 + 68;
		}
		if (i == strlen(foo)) {
			i = -1;
			versatz++;
			magic = magic + versatz;
		}
		p++;
		i++;
	}
	value[30] = '\0';
	return value;
}

/*
 * Ermoeglicht ein login, indem es rights manipuliert
 * 0, 	Login erfolgreich
 * 1, 	Passwort falsch
 * -1,	Login nicht erfolgreich
 */
int login(char* name, char* pwd) {
	char getName[32];
	char getPwd[32];
	char getStatus[32];
	char getMatrikel[32];

	if(rights != 0){
		return -1;
	}

	file = fopen(PASSWD_PATH, "r");

	if(file == NULL){
		perror("file not found.\n");
	}

	while (!feof(file)) {
		fscanf(file, "%s %s %s %s", getName, getPwd, getStatus,getMatrikel);
		if (!strcmp(name, getName)) {
			if (!strcmp(cryptofoo(pwd), getPwd)) {
				rights = atoi(getStatus);
				strcpy(usermatrikel,getMatrikel);
				fclose(file);
				return 0;
			}
			fclose(file);
			return 1;
		}
	}
	fclose(file);
	return -1;
}

/*
 * Meldet den User ab, indem rights = 0 gestzt wird
 */
void logout(){
	rights = 0;
}




