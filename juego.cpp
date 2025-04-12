#include <stdio.h>
#include <string.h>

class nodo {
public:
	nodo() : yes(0), No(0), dato(0) {}	
	nodo(char *);								
	~nodo();									
	char *dato;								
	nodo *yes;									
	nodo *No;  };

class grafo {
public:
	grafo() : Raiz() {}						
	grafo(char *);							
	~grafo();									
	void VerArbol();                   
	void InsertarAnimal(nodo *, bool, char *, bool, char *);
	nodo *Raiz;                        

private:
	nodo *aux;                         
	char Nombre[256];                  
	nodo *LeeArbol(FILE *);				
	void EscribeArbol(FILE *, nodo*);  
	void Borrar(nodo *);               
	void VerSubArbol(nodo *);				
	int n;  };

nodo::nodo(char * texto)
{	dato = new char[strlen(texto) + 1];	
	strcpy(dato, texto);              
	yes = 0;                             
	No = 0; }

nodo::~nodo()
{	if (dato) delete[] dato;	}

grafo::grafo(char *nombre)
{	FILE *fe;									
	strcpy(Nombre, nombre);             
	fe = fopen(Nombre, "r");            
	if (!fe)                             
		Raiz = new nodo("Perro");	   
	else
	{	Raiz = LeeArbol(fe);             
		fclose(fe);  } }

grafo::~grafo()
{	FILE *fs;									
	fs = fopen(Nombre, "w");				
	EscribeArbol(fs, Raiz);					
	fclose(fs);									
	Borrar(Raiz); }

nodo *grafo::LeeArbol(FILE *fe)
{	char Linea[258];							
	nodo *A;										
	fgets(Linea, 257, fe);					
	while (Linea[strlen(Linea) - 1] < ' ' && Linea[strlen(Linea) - 1] > 0)
		Linea[strlen(Linea) - 1] = 0;
	A = new nodo(&Linea[1]);				
		if (Linea[0] == 'P')						
		{	A->yes = LeeArbol(fe);				
			A->No = LeeArbol(fe); }
	return A;  }

void grafo::EscribeArbol(FILE *fs, nodo *A)
{	if (A->yes)									
	{	fprintf(fs, "P%s\n", A->dato);	
		EscribeArbol(fs, A->yes);			
		EscribeArbol(fs, A->No);  }
	else											
		fprintf(fs, "R%s\n", A->dato);}

void grafo::Borrar(nodo *n)
{	if (n->yes) Borrar(n->yes);				
	if (n->No) Borrar(n->No);   			
	delete n;  }

void grafo::VerArbol()
{	n = 0;                              
	VerSubArbol(Raiz); }
void grafo::VerSubArbol(nodo *A)
{	int i;            						
	for (i = 0; i < n; i++) putchar(' ');
	n += 3;                             
	printf(">%s\n", A->dato);          
	if (A->yes)
	{	printf("S ");							
		VerSubArbol(A->yes);   }
	if (A->No)
	{	printf("N ");							
		VerSubArbol(A->No);    }
	n -= 3;	}

void grafo::InsertarAnimal(nodo *Anterior, bool respant,
	char* Pregunta, bool Resp, char *AnimalNuevo)
{	nodo *P, *Q;					
	if (respant)
	{	P = *&Anterior->yes; }
	else
	{	P = *&Anterior->No;
		Q = new nodo(Pregunta);
		if (respant)
		{	Anterior->yes = Q; }
		else
		{	Anterior->No = Q;
			if (Resp)
			{  Q->No = P; }
			else
			{	Q->yes = P;
				P = new nodo(AnimalNuevo);
				if (Resp)
				{	Q->yes = P; }
				else
				{	Q->No = P;
				}}}}}
bool Respuesta();             

int main()
{	grafo *Datos = new grafo("Datos.pan");	
	nodo *Q, *P, *Anterior;						
	bool Resp, respAnt;				
	char Nuevo[256], Pregunta[256];			
	do 
	{	printf("Piensas en un animal s/n \n");
		Respuesta();			
		Anterior = NULL;		
		P = Datos->Raiz;		
		do							
		{	Q = P;				
			if (P->yes)
			{	printf("%s?\n", P->dato); }
			else
			{	printf("El animal en el que pensabas era un/a s/n: %s ", P->dato);
				Resp = Respuesta();
				if (P->yes)
				{	Anterior = P;
					respAnt = Resp;
					if (Resp) P = P->yes;
					else P = P->No; } }
		} while (Q->yes);			
		if (Resp)
		{	printf("Genial Lo he adivinado!\n"); }
		else						
		{
			printf("No conozco este animal, me indicas cual es \n");
			do						
			{ 	fgets(Nuevo, 255, stdin);
				while (Nuevo[strlen(Nuevo) - 1] < ' ')
					Nuevo[strlen(Nuevo) - 1] = 0;
				printf("Que diferencia a un/a ");
				printf("%s de un/a %s \n ", P->dato, Nuevo);
				fgets(Pregunta, 255, stdin);
				while (Pregunta[strlen(Pregunta) - 1] < ' ')
					Pregunta[strlen(Pregunta) - 1] = 0; 
				printf("Que respuesta se ha de dar a esta pregunta:\n");
				printf("%s?\n", Pregunta);
				printf("Para obtener: %s\n", Nuevo);
				printf("como respuesta: Si o No:\n");
				Resp = Respuesta();			
				printf("Veamos si lo he entendido bien:\n");
				printf("A la pregunta: %s?\n", Pregunta);
				if (Resp)
				{	printf("Si se responde SI se trata de un %s\n", Nuevo);
					printf("y si se responde NO de un %s\n", P->dato); }
				else
				{	printf("Si se responde SI se trata de un %s\n", P->dato);
					printf("y si se responde NO de un %s\n", Nuevo); }
				printf("Es correcto?");
			} while (!Respuesta());	
			Datos->InsertarAnimal(Anterior, respAnt,
				Pregunta, Resp, Nuevo);	}
			printf("Deseas terminar el juego y salir? (S/N)");
	} while (!Respuesta());	
	Datos->VerArbol();	
	delete Datos;			
	return 0;  }

bool Respuesta()
{	char Respuesta[10];  
	fgets(Respuesta, 9, stdin);	
	return Respuesta[0] == 's'; 
}
