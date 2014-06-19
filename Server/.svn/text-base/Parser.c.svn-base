/*
 * Parser.c
 *
 * Parser soll Befehle in Form von Strings
 * zb "erstelleStudent(Tom,Mas,Note1,Not2,Not3)"
 * zerlegen und entsprechende Operationen ausf�hren
 *
 * Alle Funktionen liefern einen String zur�ck,
 * der genau so an den Client �bergeben wird.
 *
 *
 */
#include "database.h"
#include "AccessControl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student stud; //statischer Dummy Student, auf dem die Ausgaben von Read/Best abgespeichert werden
char antwort[255]; //Antwortstring zur R�ckgabe an den Server

/*
 * return 1 if given char is a letter, 0 if not
 */
int isALetter(char value) {
	// ASCII --> 65: A - 90: Z, 97:a - 122:z
	if ((64 < value) && (value < 91)) {
		return 1;
	}
	if ((96 < value) && (value < 123)) {
		return 1;
	}
	return 0;
}

/*
 * return 1 if given char is a number, 0 if not
 */
int isANumber(char value) {
	// ASCII --> 48:0 - 57:9
	if ((47 < value) && (value < 58)) {
		return 1;
	}
	return 0;
}

/*
 * Bekommt ein Kommando als String und bearbeitet diesen
 * Der String 'antwort' wird returned.
 */
char* doThis(char* cmd) {
	/* Erkennung der gewuenschten Methode */

	/* Ausgabe des besten Studenten (entweder allgemein oder einer Fachrichtung */
	if (!strcasecmp(cmd, "ping")) { //Testmethode, um Kommunikation Client<->Server zu testen
		return "pong";
	}

	/*
	 * *********************  Funktion Best  ********************************
	 * Rechte: 0, Antwrotet mit besten Studenten
	 */
	if (!strncasecmp(cmd, "best", 4)) {
		if (!strcasecmp(cmd, "best()")) { //der beste alle Studenten
			if (bestGlobal(&stud) != -1) { //speichert den besten Studenten auf stud und prueft gleichzeitig den Fehlerfall
				char cache[] =
						"Bester Student ist %s %s mit Notendurchschnitt: %.2lf";
				sprintf(antwort, cache, stud.fname, stud.lname, stud.avg);
				printf("%s", antwort);
				return antwort;
			} else {
				return "Lesefehler";
			}
		}
		if (!strcasecmp(cmd, "best(itti)")) { //der beste itti Student
			if (bestPartial("itti", &stud) != -1) {
				char cache[] =
						"Bester ITTI.-Student ist %s %s mit Notendurchschnitt: %.2lf";
				sprintf(antwort, cache, stud.fname, stud.lname, stud.avg);
				printf("%s", antwort);
				return antwort;
			} else {
				return "Lesefehler";
			}
		}
		if (!strcasecmp(cmd, "best(info)")) { //der beste info Student
			if (bestPartial("info", &stud) != -1) {
				char cache[] =
						"Bester INF.-Student ist %s %s mit Notendurchschnitt: %.2lf";
				sprintf(antwort, cache, stud.fname, stud.lname, stud.avg);
				printf("%s", antwort);
				return antwort;
			} else {
				return "Lesefehler";
			}
		}
		if (!strcasecmp(cmd, "best(mathe)")) { //der beste mathe Student
			if (bestPartial("mathe", &stud) != -1) {
				char cache[] =
						"Bester Math.-Student ist %s %s mit Notendurchschnitt: %.2lf";
				sprintf(antwort, cache, stud.fname, stud.lname, stud.avg);
				printf("%s", antwort);
				return antwort;
			} else {
				return "Lesefehler";
			}
		}

		return "Dieser Studiengang ist nicht bekannt.";
	}

	/*
	 *  ***********************  Funktion CREATE  ************************
	 *  Nur Admin : 111, Fuegt einen Eintrag der DB hinzu
	 */
	if (!strncasecmp(cmd, "create(", 7)) {

		if (getStatus() != 111) { //prueft, ob Admin eingeloggt ist
			return "create() ist nur Admins erlaubt.";
		}

		char vorname[64];
		char nachname[64];
		char matrikel_string[10];
		long matrikel;
		char bday[9];
		double noten[MAXLVs] = { 0 };
		char fach[32];

		int i = 6;
// Vorname parsen
		int versatz = ++i;
		while (cmd[i] != ',') {
			if (!isALetter(cmd[i])) {
				return "Namen bestehen nur aus Buchstaben.";
			}
			vorname[i - versatz] = cmd[i];
			i++;
			if (i > 50) {
				return "1. Parameter zu lang!";
			}
		}
		vorname[i - versatz] = '\0';

// Nachname parsen
		versatz = ++i;
		while (cmd[i] != ',') {
			if (!isALetter(cmd[i])) {
				return "Namen bestehen nur aus Buchstaben.";
			}
			nachname[i - versatz] = cmd[i];
			i++;
			if (i > 100) {
				return "2. Parameter zu lang!";
			}
		}
		nachname[i - versatz] = '\0';

// Matrikel parsen
		versatz = ++i;
		while (cmd[i] != ',') {
			matrikel_string[i - versatz] = cmd[i];
			if ((i - versatz > 8) && (cmd[i + 1] != ',')) {
				return "Matrikel hat 9 Stellen (1)";
			}
			if ((i - versatz < 8) && (cmd[i + 1] == ',')) {
				return "Matrikel hat 9 Stellen (2)";
			}
			if (!isANumber(cmd[i])) {
				return "Matrikelnummer besteht nur aus Zahlen.";
			}
			i++;
		}
		//matrikel_string[i - versatz] = '\0';
		matrikel = atoi(matrikel_string);
		if (matrikel < 100000000)
			return "Matrikel darf keine fuehrenden Nullen haben.";

// Geburtstag parsen
		versatz = ++i;
		while (cmd[i] != ',') {
			bday[i - versatz] = cmd[i];
			i++;
		}
		if (i - versatz != 8) {
			return "Geburtstagsangaben haben insgesamt 8 Stellen.";
		}
		int j;
		for (j = 0; j < 8; j++) { //ueberprueft richtigen Aufbau des Geburtsdatums
			if (j == 2 || j == 5) {
				if (bday[j] != '.') {
					return "Geburtsdaten haben das Format DD.MM.YY\n";
				}
			} else {
				if (!isANumber(bday[j])) {
					return "Geburtsdaten haben das Format DD.MM.YY\n";
				}
			}
		}
		char* p;
		int day, month;
		day = strtol(bday, &p, 10);
		month = strtol((p + 1), NULL, 10);

		if (day > 31 || month > 12) {
			return "Kein g�ltiges Geburtsdatum";
		}
		bday[i - versatz] = '\0';
		//long geburtstag = atoi(btag);//Geburtstag ist immer ohne f�hrende Nullen!

		versatz = ++i;
		if (cmd[i] != '[') {
			return "Noten nur in Form von [1.3,2.7,3.0,4.0] angeben (max. 10)";
		}

// Noten parsen
		versatz = ++i;
		char tempnote[4];				// Noten haben immer 3 zeichen
		int x = 0; 						// Anzahl der Noten
		while (cmd[i] != ']') {			// Bis zum Ende der Noten also ] - Noten
			if ((!isANumber(cmd[i])) || (!isANumber(cmd[i + 2]))) {
				return "Noten sind immer mit Vorkomma.Nachkomma anzugeben zB \"1.0\".";
			}
			if (i > 200)
				return "Notenliste nicht korrekt!";
			char cache[] = "%c.%c";
			sprintf(tempnote, cache, cmd[i], cmd[i + 2]);
			double checknote = atof(tempnote);
			if ((checknote < 1.0) || (checknote > 5.0))
				return "Noten muessen zwischen 1.0 und 5.0 liegen.";
			noten[x] = checknote;
			x++;
			i = i + 4;						//Sprung zu n�chsten Note
			if (cmd[i - 1] == ']') {			// Notenliste is zuende
				break;
			}
		}

//Studienfach parsen
		versatz = ++i;
		while (cmd[i] != ')' && cmd[i] != ' ') {
			fach[i - versatz] = cmd[i];
			i++;
		}
		fach[i - versatz] = '\0';

		if ((strcmp(fach, "itti")) && (strcmp(fach, "info"))
				&& (strcmp(fach, "mathe"))) {
			return "Nur \"itti\",\"info\" oder \"mathe\" als Studiengaeng maeglich.";
		}

// Doppelte Eintraege verhindern.
		if ((getStudent(matrikel, &stud))) {
			return "Dieser Eintrag existiert schon in der Datenbank";
		}

		/*
		 *		printf("Datensatz: Vorname: %s, Nachname: %s, Matrikel: %li mit %i Noten in Datenbank %s\n",
		 *				vorname, nachname, matrikel, x, fach);
		 */

// DB Operation aufrufen
		if (create(vorname, nachname, matrikel, bday, noten, fach) < 0) {
			return "Fehler beim eintragen in die Datenbank.";
		}
		return "Datensatz wurde erfolgreich in die Datenbank eingetragen.";

	}

	/*
	 *  ***********************  Funktion DELETE  ************************
	 *	Rechte: 111, entfernt einen Eintrag aus der DB
	 */
	if (!strncmp(cmd, "delete(", 7)) {

		if (getStatus() != 111) {
			return "delete() ist nur Admins erlaubt.";
		}

// Matrikel parsen
		int i = 0;
		char matrikel_string[9];
		while (cmd[i + 7] != ')') {
			if (!isANumber(cmd[i + 7])) {
				return "Matrikelnummer besteht nur aus Zahlen.";
			}
			matrikel_string[i] = cmd[i + 7];
			if ((i > 8) && (cmd[i + 7 + 1] != ')')) {
				return "Matrikel hat 9 Stellen (zu kurz)";
			}
			if ((i < 8) && (cmd[i + 7 + 1] == ')')) {
				return "Matrikel hat 9 Stellen (zu lang)";
			}
			i++;
		}
		long matrikel = atol(matrikel_string);

// DB Operation aufrufen
		if (delete(matrikel) > 0) {
			return "Eintrag wurde erfolgreich aus der DB entfernt.";
		}
		return "Eintrag konnte nicht aus der DB entfernt werden.";
	}

	/*
	 * **************************** Funktion READ  ***********************
	 * Funktioniert wie delete!
	 */
	if (!strncmp(cmd, "read(", 5)) {

		/*
		 * Studenten duerfen nur sich selbst lesen, Admins durfen alles lesen
		 * Aktueller Student : getUser(), vor der Rueckgabe vergleich.
		 */
		char user_matrikel[32];
		if (getStatus() != 111) {
			getIdentity(user_matrikel);
			if (!strcmp(user_matrikel, "000000000")) {
				return "read() ist nur Admins und Studenten erlaubt";
			}
		}

		int i = 0;
		char matrikel_string[9];
		while (cmd[i + 5] != ')') {
			if (!isANumber(cmd[i + 5])) {
				return "Matrikelnummer besteht nur aus Zahlen.";
			}
			matrikel_string[i] = cmd[i + 5];
			if ((i > 8) && (cmd[i + 5 + 1] != ')')) {
				return "Matrikel hat max. 9 Stellen";
			}
			if ((i < 8) && (cmd[i + 5 + 1] == ')')) {
				return "Matrikel besteht aus 9 Stellen";
			}
			i++;
		}
		long matrikel = atol(matrikel_string);
		/*
		 Wenn nicht Admin:
		 Usermatrikel und Studentenmatrikel pruefen
		 */
		if (getStatus() != 111) {
			getIdentity(user_matrikel);
			long user_matrikel_long = atol(user_matrikel);
			if (user_matrikel_long != matrikel) {
				return "Studenten ist der Zugriff nur auf ihre eigenen Daten gestattet.";
			}
		}
		if (getStudent(matrikel, &stud) == 1) {

			sprintf(antwort,
					"\nName: %s %s\nStudienfach: %s\nGeburtsdatum: %s\nDurchschnitt: %.2f\n",
					stud.fname, stud.lname, stud.fach, stud.bdate, stud.avg);
			char notenout[64] = "Noten:";
			int i;
			for (i = 0; i < MAXLVs; i++) {
				if (stud.marks[i] >= 1) {
					char singlenote[5];
					sprintf(singlenote, " %.1lf", stud.marks[i]);
					strcat(notenout, singlenote);
				}
			}
			strcat(antwort, notenout);
			strcat(antwort, "\n");
			return antwort;
		}
		return "Student nicht gefunden";

	}

	/*
	 *  ************************** Funktion UPDATE ***********************
	 *  Rechte 111, Veraendert die Daten eines Eintrags
	 */

	if (!strncasecmp(cmd, "update(", 7)) {
		// Rechtekontrolle
		if (getStatus() != 111) {
			return "update() ist nur Admins gestattet.";
		}
//Matrikel Parsen
		int i = 0;
		char matrikel_string[9];
		while (cmd[i + 7] != ',') {
			if (!isANumber(cmd[i + 7])) {
				return "Matrikelnummer besteht nur aus Zahlen.";
			}
			matrikel_string[i] = cmd[i + 7];
			if ((i > 8) && (cmd[i + 7 + 1] != ',')) {
				return "Matrikel hat max. 9 Stellen";
			}
			if ((i < 8) && (cmd[i + 7 + 1] == ',')) {
				return "Matrikel besteht aus 9 Stellen";
			}
			if (isALetter(cmd[i + 7])) {
				return "MAtrikel muss aus Nummern bestehen";
			}
			i++;
		}

		long matrikel = atol(matrikel_string);
		i++;
//Attribut parsen
		char attribut[32];
		int versatz = i;
		while (cmd[i + 7] != ':') {
			attribut[i - versatz] = cmd[i + 7];
			i++;
		}
		attribut[i - versatz] = '\0';
		i++;

// Attribut pr�fen
		if (strcasecmp(attribut, "vorname") && strcasecmp(attribut, "nachname")
				&& strcasecmp(attribut, "noten")
				&& strcasecmp(attribut, "datum")) {
			return "Gueltige Attribute sind: \n"
					"- Vorname\n"
					"- Nachname\n"
					"- Noten\n"
					"- Datum\n";
		}

		char value[32];
		versatz = i;
		while (cmd[i + 7] != ')') {
			if ((!strcasecmp(attribut, "vorname"))
					|| (!strcasecmp(attribut, "nachname"))) { //Wenn Vorname oder Nachname: Ueberpruefen ob nur Buchstaben
				if (!isALetter(cmd[i + 7])) {
					return "Namen bestehen nur aus Buchstaben";
				}
			}

			value[i - versatz] = cmd[i + 7];
			i++;
		}

		if (!strcasecmp(attribut, "datum")) { //Wenn Geburtstag, Ueberpruefen auf gueltiges Datum
			char* p;
			int day, month;
			day = strtol(value, &p, 10);
			month = strtol((p + 1), NULL, 10);

			if ((day > 31) || (month > 12)) {
				return "Kein gueltiges Geburtsdatum";
			}
		}
		value[i - versatz] = '\0';

// DB Operation aufrufen
		if (update(matrikel, attribut, value) > 0) {
			return "Eintrag erfolgreich modifiziert";
		}
		return "Eintrag konnte nicht modifiziert werden.";

	}

	/*
	 * ************************* Funktion RECHTE *************************
	 * rechte: 0, Gibt die Aktuellen Rechte aus
	 */
	if (!strcasecmp(cmd, "rechte")) {
		char cache[] = "Deine Rechte: %i";
		sprintf(antwort, cache, getStatus());
		return antwort;
	}

	/*
	 * ************************* Funktion LOGIN ***************************
	 * rechte: 0, ermoeglicht seine Rechte zu erhoehen
	 */
	if (!strncmp(cmd, "login(", 6)) {
		if (getStatus() > 0) {
			return "Mehrfachanmeldung nicht moeglich.";
		}
// USername parsen
		int i = 6;
		char givenName[32];
		char givenPwd[32];
		int versatz = 6;
		while (cmd[i] != ',') {
			givenName[i - versatz] = cmd[i];
			i++;
			if (i > 30) {
				return "Fehler in der Namensangabe!";
			}
		}
		givenName[i - versatz] = '\0';
		i++;

// Passwort parsen
		versatz = i;
		while (cmd[i] != ')') {
			givenPwd[i - versatz] = cmd[i];
			i++;
			if (i > 60) {
				return "Fehler in der Passwortangabe!";
			}
		}
		givenPwd[i - versatz] = '\0';

// DB Operation aufrufen
		int value = login(givenName, givenPwd);

		switch (value) {
		case 0: {
			char cache[] = "Erfolgreich angemeldet als user: %s";
			sprintf(antwort, cache, givenName);
			return antwort;
		}
		case 1:
			return "Falsches Passwort!";
		case -1:
			return "Kein Benutzerprofil gefunden.";
		}
	}

	/*
	 * ******************** Funktion LOGOUT ***********************
	 * rechte 0, Ermoeglicht ein sicheres Logout
	 */
	if (!strcasecmp(cmd, "logout")) {
		if (getStatus() == 0) {
			return "Keine abmeldung erforderlich.";
		}
		logout();
		return "Erfolgreich abgemeldet.";
	}

	return "Operation nicht bekannt. Probiere \"help\"";
}

