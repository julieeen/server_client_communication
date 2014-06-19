#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define AE (unsigned char)142
#define ae (unsigned char)132
#define OE (unsigned char)153
#define oe (unsigned char)148
#define UE (unsigned char)154
#define ue (unsigned char)129
#define ss (unsigned char)225

/*
 * Liste alle Funktionen
 */

void printHelp() {

	/*
	 * Textfarbe:
	 * \033[x;ym
	 * mit x und y aus:
	 * 	0 to restore default color
	 1 for brighter colors
	 4 for underlined text
	 5 for flashing text
	 30 for black foreground
	 31 for red foreground
	 32 for green foreground
	 33 for yellow (or brown) foreground
	 34 for blue foreground
	 35 for purple foreground
	 36 for cyan foreground
	 37 for white (or gray) foreground
	 40 for black background
	 41 for red background
	 42 for green background
	 43 for yellow (or brown) background
	 44 for blue background
	 45 for purple background
	 46 for cyan background
	 47 for white (or gray) background
	 */

	printf(
			"\n"
					"\t\033[0;31m exit\033[0m - beendet den Client\n"
					"\n"
					"\t\033[0;31m shutdown\033[0m - Beendet den Server und den Client\n"
					"\n"
					"\t\033[0;31m ping\033[0m - Server antwortet mit einem \"Pong\"\n"
					"\n"
					""
					"\t\033[0;31m rechte\033[0m - Erfahre welche Rechte du hast.\n"
					"\n"
					""
					""
					"\t\033[0;31m login(name,passwort)\033[0m - Bekomme mehr Rechte\n"
					"\n"
					"\t\033[0;31m logut\033[0m - Abmelden\n"
					"\n"
					""
					"\t\033[0;31m best()\033[0m - gibt den besten Studenten von allen aus\n"
					"\t\t >> best(itti) - bester ITTIstudent\n"
					"\t\t >> best(mathe) - bester Informatikstudent\n"
					"\t\t >> best(info)  - bester Mathematikstudent\n"
					"\n"
					"\t\033[0;31m create(Vorname,Name,Matrikel,Geburtstag,[Note1,Note2,Note3],Studienfach)\033[0;0m\n"
					"\t\t   zB: create(Albert,Einstein,123456789,14.03.79,[1.0,2.3,3.7], itti)\n"
					"\n"
					"\t\033[0;31m read(Matrikelnummer) - Gibt alle Informationen eines Studenten aus\033[0;0m\n"
					"\t\t   zB: read(123456789)\n"
					"\n"
					"\t\033[0;31m delete(Matrikel)\033[0;0m - entfernt gewuenschten Eintrag aus DB\n"
					"\t\t   zB: delete(123456789)\n"
					""
					"\n"
					"\t\033[0;31m update(Matrikel,Attribut:NeuerWert)\033[0;0m - aktualisiert gewuenschten Eintrag\n"
					"\t\t   zB: update(123456789,Vorname:Hans)\n"
					"\t\t   zB: update(123456789,Noten:[1.0,4.0,1.3])\n"
					"\n"
					"\t\t   Attribute: Vorname/Nachame/Datum/Noten"
					"\n"
					""
					""
					""
					"\n"
					"\t Alle \033[0;4mParameter\033[0;0m muessen folgende \033[0;4mBedingungen\033[0;0m erfuellen:\n"
					"\t\t - Vor-/Namen besten aus Buchstaben\n"
					"\t\t - Matrikelnummern sind 9 Stellig\n"
					"\t\t - Geburtstage werden in der Form DD.MM.YY angegeben\n"
					"\t\t - Noten werden als Listen notiert: [Note1,...,Note?] (Max 10)\n"
					""
					"\n");

}
