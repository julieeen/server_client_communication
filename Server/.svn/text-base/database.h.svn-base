/*
 * database.h
 *
 *  Hier einfach immer die Methodenr�mpfe eintragen wenn Funktionen in database.c dazukommen!
 *  Ist genau so als wenn es oben im c-File steht.
 *  Dann kannst man einfach von anderen c-files auf die Operationen zugreifen.
 */

#define MAXLVs 10 //maximale Anzahl an Noten pro Student

//Pfade der Datenbankdateien
#define MATH_PATH "mathe.dat"
#define ITTI_PATH "itti.dat"
#define INF_PATH "informatik.dat"
#define TEMP_PATH "temp.dat"


struct student /* represents a student */
{
	char fname[20]; /* first name */
	char lname[30]; /* last name */
	long mat; /* matriculation number */
	char bdate[10]; /* date of birth - Format: DD.MM.YY */
	double marks[MAXLVs]; /* array of marks for all courses */
	double avg; /* average mark of student */
	char fach[16]; //the students course
};

int bestGlobal(struct student* gesuchter);
int bestPartial(char d[], struct student* gesuchter);
double average(double marks[MAXLVs]);/* computes the average mark of a student */


char* getStudiengang(long matrikelnr);
//bsp: getStudiengang(123456)

int create(char* vorname, char* name, long mat,char* bday, double noten[], char fach[]);
//Beispiel: create(Hans,Peter,123456,[1,2,3,4],itti)

int update(long mat, char* attribut,char* value);

int delete(long mat);

// read ist reserviert durch socket api
int getStudent(long mat, struct student* gesuchter);
