#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

/* TEST - Main für die Datenbankfunktionalität
 * int main()
 {
 FILE *inf, *itti, *math; // points to one of the database files
 inf = fopen("././src/informatik.dat", "r"); // opens informatik db in reading mode
 if (inf == NULL)
 printf("Fehlerinf");
 itti = fopen("././src/itti.dat", "r"); // opens itti db in reading mode
 if (itti == NULL)
 printf("Fehleritti");
 math = fopen("././src/mathe.dat", "r"); // opens math db in reading mode
 if (math == NULL)
 printf("Fehlermath");
 int input; // chooses an option (see below)
 struct student best; // stores the student with the lowest average mark
 printf("\nBitte waehlen:\n1 - Bester Student(Gesamt)\n2 - Bester Student(Informatik)\n3 - Bester Student(Itti)\n4 - Bester Student(Mathe)\n5 - Beenden\n");
 scanf("%d", &input);
 switch (input)
 {
 case 1:
 best = bestGlobal(inf, itti, math);
 break;
 case 2:
 best = bestPartial(inf);
 break;
 case 3:
 best = bestPartial(itti);
 break;
 case 4:
 best = bestPartial(math);
 break;
 case 5:
 exit(1);
 }
 printf("\nName: %s %s\nMatrikelnummer: %ld\nGeburtsdatum: %ld\nDurchschnittsnote: %lf\n", best.fname, best.lname, best.mat, best.bdate, best.avg);
 fclose(inf);
 fclose(itti);
 fclose(math);
 exit(1);
 } */

//markstoArray speichert die Noten aus string in das Array marks
void markstoArray(char *string, double *marks) {
	char markString2[4]; /* stores one mark at a time as a string (to parse it as a double) */
	int num; /* indicates the number of marks a student has */
	int i, j, k;
	num = strlen(string) / 4; /* computes the number of marks (one mark equals 4 charakters, for example " 2.5") */
	for (i = 0; i < num; i++) /* this double "for" loop parses the markArray string and stores every mark in the mark array of a student */
	{
		for (j = 0; j < sizeof(markString2); j++) {
			markString2[j] = string[(4 * i + j)];
		}
		sscanf(markString2, " %lf", &marks[i]);
	}
	for (k = num; k < MAXLVs; k++) //fills up the rest of the Array with zeros
		sscanf(" 0.0", " %lf", &marks[k]);
}

int create(char* vorname, char* name, long mat, char* bday, double noten[],
		char fach[]) {
	FILE *db;
	int i;
	if (!(strcmp(fach, "itti")))
		db = fopen(ITTI_PATH, "a"); // opens itti db in append mode
	else if (!(strcmp(fach, "info")))
		db = fopen(INF_PATH, "a"); // opens inf db in append mode
	else if (!(strcmp(fach, "mathe")))
		db = fopen(MATH_PATH, "a"); // opens math db in append mode
	else
		return -1;
	if (db == NULL) {
		return -1;
	}
	fprintf(db, "%ld ", mat); //Eintragung der Matrikelnummer
	fprintf(db, "%s ", vorname); //Eintragung des Vornamen
	fprintf(db, "%s ", name); //Eintragung des Nachnamens
	fprintf(db, "%s ", bday); //Eintragung des Geburtsdatums
	fprintf(db, "%.2lf", average(noten)); //Eintragung der Durchschnittsnote
	for (i = 0; i < MAXLVs; i++) {
		if (noten[i] >= 1)
			fprintf(db, " %.1f", noten[i]); //Eintragung der Einzelnoten
	}
	fprintf(db, "\n");
	fclose(db);
	return 1;

}
int deletePart(long mat, char fach[]) { //deletes a student by copying all other students into a temporary file and overwriting the original one
	FILE *temp, *org;
	long delmat;
	int ret = 0;
	temp = fopen(TEMP_PATH, "w+"); //opens temp file in writing, than reading mode
	if (!(strcmp(fach, "itti")))
		org = fopen(ITTI_PATH, "r"); // opens itti db in reading mode
	else if (!(strcmp(fach, "info")))
		org = fopen(INF_PATH, "r"); // opens inf db in reading mode
	else if (!(strcmp(fach, "mathe")))
		org = fopen(MATH_PATH, "r"); // opens math db in reading mode
	else
		return -1;
	int lines = 0;
	while ((fscanf(org, "%*[^\n]"), fscanf(org, "%*c")) != EOF) //counts the number of lines in a file
		lines++;
	rewind(org);
	int i;
	for (i = 0; i < lines; i++) {
		fscanf(org, "%ld ", &delmat); //scans matrikelnumber of current line
		char move[128];
		fgets(move, 128, org); //scans rest of the line
		if (delmat == mat) //dont copy the line if matrikelnumbers match
			ret = 1;
		else {
			fprintf(temp, "%ld ", delmat); //writes matrikelnumber to tempfile
			fputs(move, temp); //writes rest of the line to tempfile
		}
	}
	//here begins the overwriting of org with temp
	rewind(temp); //sets pointer to beginning of file
	fclose(org);
	if (!(strcmp(fach, "itti")))
		org = fopen(ITTI_PATH, "w"); // opens itti db in writing mode, erasing the original file
	else if (!(strcmp(fach, "info")))
		org = fopen(INF_PATH, "w"); // opens inf db in writing mode
	else if (!(strcmp(fach, "mathe")))
		org = fopen(MATH_PATH, "w"); // opens math db in writing mode
	else
		return -1;
	int c;
	int j = 0;
	while (j < lines && (c = fgetc(temp)) != EOF) {
		if (c == '\n')
			j++;
		fputc(c, org);
	}
	fclose(temp);
	fclose(org);
	return ret;
}

/* delete ruft nacheinander die Untermethode deletePart
 * auf allen 3 Teildatenbanken auf, bis der Datensatz gefunden ist
 */
int delete(long mat) {
	int del = deletePart(mat, "itti");
	if (del == 1)
		return 1;
	del = deletePart(mat, "mathe");
	if (del == 1)
		return 1;
	del = deletePart(mat, "info");
	if (del == 1)
		return 1;
	return 0;
}

/*
 * 0 wenn nicht gefunden
 * -1 im Fehlerfall
 * 1 wenn gefunden, Parameter gesuchter wird dann belegt.
 */
int getStudentPart(char* fach, long mat, struct student* gesuchter) {
	FILE *db;
	long searchMat;
	char marks[(MAXLVs * 4) + 1]; //to temporarily store the notes of a student in a string
	if (!(strcmp(fach, "itti")))
		db = fopen(ITTI_PATH, "r"); // opens itti db in reading mode
	else if (!(strcmp(fach, "info")))
		db = fopen(INF_PATH, "r"); // opens inf db in reading mode
	else if (!(strcmp(fach, "mathe")))
		db = fopen(MATH_PATH, "r"); // opens math db in reading mode
	if (db == NULL)
		return -1;
	for (;;) {
		fscanf(db, "%ld ", &searchMat);
		if (feof(db)) //Bei Dateiende Abbruch der Suchschleife
			break;
		if (searchMat == mat) {
			gesuchter->mat = searchMat;
			fscanf(db, "%s %s %s %lf", gesuchter->fname, gesuchter->lname,
					gesuchter->bdate, &gesuchter->avg); //Auslesen des Trefferdatensatzes
			fgets(marks, (MAXLVs * 4) + 1, db);
			markstoArray(marks, gesuchter->marks); //stores the students marks
			return 1;
		}
		fscanf(db, "%*[^\n]s"); //wenn nicht der gesuchte Datensatz: Cursor rueckt weiter bis zum naechsten
	}
	return 0;
}

int getStudent(long mat, struct student* gesuchter) {
	int res;
	res = getStudentPart("mathe", mat, gesuchter); //sucht in Mathe db
	if (res != 0) {
		strcpy(gesuchter->fach, "mathe");
		return res;
	}
	res = getStudentPart("itti", mat, gesuchter); //sucht in Itti db
	if (res != 0) {
		strcpy(gesuchter->fach, "itti");
		return res;
	}
	res = getStudentPart("info", mat, gesuchter); //sucht in Info db
	if (res != 0) {
		strcpy(gesuchter->fach, "info");
		return res;
	}
	return 0;
}

/*
 *	update bekommt matrikelnummer und zu verÔøΩnderndes Attribut, sowie
 *	die gewollten Daten. zB update(123456,vorname,hans)
 */

int update(long mat, char* attribut, char* value) {
	int err = -1;
	struct student s;
	err = getStudent(mat, &s); //Zu veränderner Student wird geholt
	if (err != 1)
		return err;
	err = deletePart(mat, s.fach); //Datensatz des Studenten wird aus DB gelöscht
	if (err != 1)
		return err;
	//neuer Datensatz wird angelegt entsprechend des angegebenen Attributs
	if (!strcasecmp(attribut, "vorname"))
		err = create(value, s.lname, mat, s.bdate, s.marks, s.fach);
	else if (!strcasecmp(attribut, "nachname"))
		err = create(s.fname, value, mat, s.bdate, s.marks, s.fach);
	else if (!strcasecmp(attribut, "datum"))
		err = create(s.fname, s.lname, mat, value, s.marks, s.fach);
	else if (!strcasecmp(attribut, "noten")) { //Notenupdate durch Uebergabe der neuen Notenliste: [1.4,2.3,1.2,...]
		char notenString[255];
		double newmarks[MAXLVs];
		int i = 1;
		if (value[0] != '[')
			return -1;
		while (value[i] != ']') {
			if (value[i] == ')')
				return -1;
			notenString[i - 1] = value[i];
			i++;
		}
		markstoArray(notenString, newmarks);
		err = create(s.fname, s.lname, mat, s.bdate, newmarks, s.fach);
	}
	return err;
}

/* 1 wenn bester Student gefunden
 * -1 wenn Fehler
 */
int bestGlobal(struct student* gesuchter) {
	struct student sitti = { }, sinfo = { }, smath = { };
	int err;
	err = bestPartial("itti", &sitti); //best of itti students
	if (err == -1)
		return err;
	err = bestPartial("info", &sinfo);	//best of informatik students
	if (err == -1)
		return err;
	err = bestPartial("mathe", &smath); //best of math students
	if (err == -1)
		return err;
	//comparision of the average marks of all the part. best students and linking of the best of them to "gesuchter"
	if (sitti.avg <= sinfo.avg && sitti.avg <= smath.avg) {
		*gesuchter = sitti;
	} else if (sinfo.avg <= sitti.avg && sinfo.avg <= smath.avg) {
		*gesuchter = sinfo;
	} else {
		*gesuchter = smath;
	}
	return 1;
}

/* 1 wenn bester Student gefunden
 * -1 wenn Fehler
 */
int bestPartial(char d[], struct student* gesuchter) {
	FILE *data;
	if (!(strcmp(d, "itti")))
		data = fopen(ITTI_PATH, "r"); // opens itti db in reading mode
	else if (!(strcmp(d, "info")))
		data = fopen(INF_PATH, "r"); // opens informatik db in reading mode
	else if (!(strcmp(d, "mathe")))
		data = fopen(MATH_PATH, "r"); // opens math db in reading mode
	if (data == NULL) {
		return -1;
	}

	struct student stemp = { }; /* temporarily student */
	gesuchter->avg = 5.0;
	for (;;) { /*endless loop to scan all lines of the database */
		fscanf(data, "%ld %s %s %s %lf", &stemp.mat, stemp.fname, stemp.lname,
				stemp.bdate, &stemp.avg); /* scans one line of data file (except for the marks) */
		fscanf(data, "%*[^\n]"); //jumps to next line of file
		if (stemp.avg <= gesuchter->avg) /* compares the temporarily best student with the one just scanned and eventually replaces it */
			*gesuchter = stemp;
		if (feof(data)) /* jumps out of infinite loop at end of file */
			break;
	}
	fclose(data);
	return 1;
}

double average(double marks[]) {
	int i;
	int number = 0; /* actual number of marks in the array */
	double add = 0; /* addition of all marks */
	for (i = 0; i < (MAXLVs); i++) {
		if (marks[i] == 0.0) /* leaves the loop after reading the last course mark */
			break;
		add = add + marks[i];
		number++;
	}
	return (add / number);
}
