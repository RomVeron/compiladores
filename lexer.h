/************Librerias************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/************Definiciones************/

//Codigos
#define L_LLAVE			200
#define STRING 			201
#define DOS_PUNTOS		202
#define L_CORCHETE		203
#define NUMBER			204
#define COMA			205
#define PR_FALSE		206
#define R_CORCHETE		207
#define R_LLAVE			208
#define PR_NULL			209
#define PR_TRUE			210
#define LITERAL_NUM		211
#define LITERAL_CADENA	213


// Fin Codigos
#define TAMBUFF 	5
#define TAMLEX 		50
#define TAMHASH 	101

/************* Estructuras ********************/

typedef struct entrada{
	//definir los campos de 1 entrada de la tabla de simbolos
	int compLex;
	char lexema[TAMLEX];	
	struct entrada *tipoDato; // null puede representar variable no declarada	
	// aqui irian mas atributos para funciones y procedimientos...
	
} entrada;

typedef struct {
	int compLex;
	entrada *pe;
} token;

/************* Prototipos ********************/
void insertar(entrada e);
entrada* buscar(const char *clave);
void initTabla();
void initTablaSimbolos();
void sigLex();

