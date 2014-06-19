/*
 * Parser.h
 *
 * Parser soll Befehle in Form von Strings
 * zb "erstelleStudent(Tom,Mas,Note1,Not2,Not3)"
 * zerlegen und entsprechende Operationen ausführen
 *
 * Alle Funktionen liefern einen String zurück,
 * der genau so an den Client übergeben wird.
 *
 */
#ifndef PARSER_H_
#define PARSER_H_

char* doThis(char cmd[]);
int isALetter(char value);
int isANumber(char value);


#endif /* PARSER_H_ */
