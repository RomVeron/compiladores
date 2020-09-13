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


/*********************HASH************************/
entrada *tabla;				//declarar la tabla de simbolos
int tamTabla=TAMHASH;		//utilizado para cuando se debe hacer rehash
int elems=0;				//utilizado para cuando se debe hacer rehash

int h(const char* k, int m)
{
	unsigned h=0,g;
	int i;
	for (i=0;i<strlen(k);i++)
	{
		h=(h << 4) + k[i];
		if ( (g=h&0xf0000000) ){
			h=h^(g>>24);
			h=h^g;
		}
	}
	return h%m;
}

void initTabla()
{	
	int i=0;
	
	tabla=(entrada*)malloc(tamTabla*sizeof(entrada));
	for(i=0;i<tamTabla;i++)
	{
		tabla[i].compLex=-1;
	}
}

int esprimo(int n)
{
	int i;
	for(i=3;i*i<=n;i+=2)
		if (n%i==0)
			return 0;
	return 1;
}

int siguiente_primo(int n)
{
	if (n%2==0)
		n++;
	for (;!esprimo(n);n+=2);

	return n;
}

//en caso de que la tabla llegue al limite, duplicar el tamaño
void rehash()
{
	entrada *vieja;
	int i;
	vieja=tabla;
	tamTabla=siguiente_primo(2*tamTabla);
	initTabla();
	for (i=0;i<tamTabla/2;i++)
	{
		if(vieja[i].compLex!=-1)
			insertar(vieja[i]);
	}		
	free(vieja);
}

//insertar una entrada en la tabla
void insertar(entrada e)
{
	int pos;
	if (++elems>=tamTabla/2)
		rehash();
	pos=h(e.lexema,tamTabla);
	while (tabla[pos].compLex!=-1)
	{
		pos++;
		if (pos==tamTabla)
			pos=0;
	}
	tabla[pos]=e;

}
//busca una clave en la tabla, si no existe devuelve NULL, posicion en caso contrario
entrada* buscar(const char *clave)
{
	int pos;
	pos=h(clave,tamTabla);
	while(tabla[pos].compLex!=-1 && strcmp(tabla[pos].lexema,clave)!=0 )
	{
		pos++;
		if (pos==tamTabla)
			pos=0;
	}
	return &tabla[pos];
}

void insertTablaSimbolos(const char *s, int n)
{
	entrada e;
	strcpy(e.lexema,s);
	// sprintf(e.lexema,s);
	e.compLex=n;
	insertar(e);
}

void initTablaSimbolos()
{
	int i;
	const char *vector[]={
		"program",
		"type",
		"var",
		"array",
		"begin",
		"end",
		"do",
		"to",
		"downto",
		"then",
		"of",
		"function",
		"procedure", 
		"integer", 
		"real", 
		"boolean", 
		"char", 
		"for", 
		"if", 
		"else", 
		"while", 
		"repeat", 
		"until", 
		"case", 
		"record", 
		"writeln",
		"write",
		"const"
};
 	for (i=0;i<28;i++)
	{
		insertTablaSimbolos(vector[i],i+256);
	}
	insertTablaSimbolos(",",COMA);
	insertTablaSimbolos(":",DOS_PUNTOS);
	insertTablaSimbolos("{",L_LLAVE);
	insertTablaSimbolos("}",R_LLAVE);
	insertTablaSimbolos("[",L_CORCHETE);
	insertTablaSimbolos("]",R_CORCHETE);
	insertTablaSimbolos("true",PR_TRUE);
	insertTablaSimbolos("false",PR_FALSE);
	insertTablaSimbolos(":",DOS_PUNTOS);
}