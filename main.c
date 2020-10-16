	
//	PROGETTO API 2018 - INTERPRETE DI TM
//	LORENZO PRADA 10529212

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#define ACCEPT "acc"
#define MAX_TRANSITION "max"

//#define INPUT "input_pub.txt"
#define INPUT "input_public1.txt"
//#define INPUT "prova.txt"



//	------------------------------------------------------------------------------------------------------------------------------------------------------		STRUTTURE DATI

//	Struct che contiene un elemento della funzione di transizione (star)

typedef struct star {
	int curr_state;
	char read;
	char write;
	char move;
	int next_state;
	struct star* next_star;

} Star;

// 	Struct che contiene uno degli stati finali

typedef struct end_state {
	int end_state_value;
	struct end_state* next_end_state; 
} End_state;

//	Struct con un puntatore a memory_tape_char e al nodo successivo per salvare tutte le stringhe in input

typedef struct list_of_memory_tape {
	char* data_from_file;
	struct list_of_memory_tape* next_saved_tape;
} List_of_memory_tape;

//	Struct che contiene i vari tape nel momento dell'esecuzione

typedef struct tm_cfg {
	struct tm_cfg* prev;
	int            active_state;
	int            pos_tape;
	char          *tape;
	struct tm_cfg* next;
} TM_cfg;

//	Dati forniti dal file input e dati globali

int ttl = 0;
Star* testa_funzione = NULL;
End_state* testa_end_state = NULL;
List_of_memory_tape* testa_lista_nastri = NULL;
TM_cfg* p_head_tm_cfg= NULL;


//	------------------------------------------------------------------------------------------------------------------------------------------------------		INIT FUNZIONI

void get_start ();				// 	Funzione di prova che stampa Hello Mr

void get_input_new ();			//	Tentativo numero 5

void print_star ();				//	Stampa la lista in cui salvo la funzione di transizione

void print_memory_tape ();		//	Stampa i nastri di memoria che contengon le stringhe da eseguire sulla TM

void execution ();				//	Esegue i nastri di memoria e restituisce 0 se non accetta, 1 se accetta, 2 se non termina (=U)
int tm_step (TM_cfg* );
int tm_run ();

int is_final_state (int);

int remove_tm_cfg (TM_cfg* );
void remove_all_tm_cfgs ();

void print_tm_cfg ();
void print_all_tm_cfgs ();


//	------------------------------------------------------------------------------------------------------------------------------------------------------		MAIN

int main() {

	get_input_new ();

	//print_star ();

	//print_memory_tape ();

	//execution();

	//get_start ();

	return 0;
}

//	------------------------------------------------------------------------------------------------------------------------------------------------------		FUNZIONI

/*

************************************************************************************************************************************************************	get_start

void get_start ()

funzione prova che stampa solo hello Mr

*/

void get_start () {
	printf("Hello Mr\n");
	return;
}

/*

************************************************************************************************************************************************************	get_input_new

void get_input_new ()

Funzione che apre il file di input e importa in diverse strutture dati tutti i dati necessari alla costruzione della TM e all'esecuzione delle stringhe

*/

void get_input_new () {

	Star* new_star = NULL;
	Star* old_star = NULL;
	End_state* new_end_state = NULL;
	End_state* old_end_state = NULL;
	List_of_memory_tape* new_memory_tape = NULL;
	List_of_memory_tape* old_memory_tape = NULL;
	//FILE* input = fopen (INPUT, "r");
	char get_string[1000];
	char c = 'a';

	//if (input==NULL) {													//	Errore FOPEN
	//	printf("Errore FOPEN nella funzione get_input_new\n");
	//	return;
	//}

	//printf("File di input aperto\n");

	fgets (get_string, sizeof(get_string), stdin);						// 	Burn "tr" dallo stream
	fgets (get_string, sizeof(get_string), stdin);						//	Inizio acquisizione dei dati per la funzione di transizione

	while (strncmp(get_string, ACCEPT,3) != 0 ) {						// 	Se l'ultima stringa letta è uguale ad "acc" mi fermo
		new_star = (Star* )malloc(sizeof(Star));
		if (new_star == NULL) {											//	Errore malloc
			printf ("Errore malloc nella funzione get_input\n");
			return;
		}
		new_star->next_star=NULL;

		if (testa_funzione==NULL) testa_funzione=new_star;

		sscanf (get_string, "%d %c %c %c %d ", &new_star->curr_state, &new_star->read, &new_star->write, &new_star->move, &new_star->next_state);

		if (old_star!=NULL) old_star->next_star=new_star;				// 	Occhio agli spazi dentro la scanf, sono fondamentali
		old_star=new_star;

		fgets (get_string, sizeof(get_string), stdin);					//	Acquisizione della stringa successiva per il prossimo ciclo
	} 

	//printf("Funzione di transizione importata in una lista\n");
	
	fgets (get_string, sizeof(get_string), stdin);
	while (strncmp(get_string, MAX_TRANSITION,3) != 0 ) {

		new_end_state = (End_state* )malloc(sizeof(End_state));
		if (new_end_state == NULL) {											//	Errore malloc
			printf ("Errore malloc nella funzione get_input\n");
			return;
		}
		new_end_state->next_end_state = NULL;
		
		if (testa_end_state == NULL) testa_end_state = new_end_state;

		if (get_string!=MAX_TRANSITION) {						

			new_end_state->end_state_value = atoi (get_string);
		//	printf("Importato sato finale numero    %d\n",new_end_state->end_state_value);

			if (old_end_state!=NULL) old_end_state->next_end_state=new_end_state;
			old_end_state = new_end_state;
		}
		
		fgets (get_string, sizeof(get_string), stdin);
	}

	fgets (get_string, sizeof(get_string), stdin);
	sscanf (get_string, "%d", &ttl);									//	Acquisizione del numero massimo di transizioni
	//printf ("Numero massimo di transizioni %d\n",ttl);

	fgets (get_string, sizeof(get_string), stdin); get_string[strlen(get_string)-1]='\0'; 		//	Acquisizione di run e tolgo il carattere EOL alla fine
	
	int output;


	while (fgets (get_string, sizeof(get_string), stdin) != NULL) {
		get_string[strlen(get_string)-1]='\0'; 
		output = tm_run(get_string);


		if (output == 2) 
			printf("U\n");
		else 
			printf("%d\n", output);
		/*new_memory_tape = (List_of_memory_tape* )malloc(sizeof(List_of_memory_tape));
		if (new_memory_tape == NULL){
				printf ("Errore malloc nella funzione get_input\n");
				return;
			}
		new_memory_tape->next_saved_tape=NULL;

		if (testa_lista_nastri == NULL) testa_lista_nastri = new_memory_tape;

		if (old_memory_tape!=NULL) old_memory_tape->next_saved_tape = new_memory_tape;
		old_memory_tape = new_memory_tape;
	
		new_memory_tape->data_from_file = (char* )malloc (strlen(get_string)+1);
		strcpy(new_memory_tape->data_from_file,get_string);
		*/
	}
	return;
}

/*

************************************************************************************************************************************************************	print_star

void print_star ()

Funzione che scorre la funzione di transizione e stampa il contenuto di ogni nodo

*/

void print_star () {
    Star* p_star = testa_funzione;										//	Creo un puntatore nuovo per non modificare la testa della funzione
	printf("Stampa della funzione di transizione...\n");

	while (p_star != NULL){
		printf("%d %c %c %c %d\n", p_star->curr_state, p_star->read, p_star->write, p_star->move, p_star->next_state);
		p_star = p_star -> next_star;
	}

	return;
}

/*

************************************************************************************************************************************************************	execution

void execution ()

Funzione che scorre la lista dei nastri di memoria e, uno per uno, li esegue

*/

void execution () {

	List_of_memory_tape* riga = testa_lista_nastri;
	int output = -1;
	int cont = 1;
	FILE* output_file = fopen("TM_output.txt", "w");								//	Creo un file per salvare gli output delle computazioni

	while (riga!=NULL) {
		output = tm_run(riga->data_from_file);


		printf("Computazione stringa %d:\n",cont);									//	Stampa dei valori resituiti dalla stringa computata
		fprintf(output_file, "Computazione stringa %d:\n",cont);

		if (output == 2) {
			printf("U\n\n");
			fprintf(output_file,"U\n\n");
		}

		else {
			printf("%d\n\n", output);
			fprintf(output_file, "%d\n\n", output);
		}

		riga = riga->next_saved_tape;
		cont++;
	}
	fclose(output_file);
	return;
}

/*

************************************************************************************************************************************************************	tm_run

int tm_run (char* )

Funzione che riceve come parametro una stringa da eseguire, la processa e restituisce il seguente output:
-	0 se la stringa è accettata dalla TM: 
		se una delle computazioni arriva in uno stato finale
-	1 se la stringa non è accettata dalla TM:
		se tutte le computazioni parallele giungono in uno stato (non finale) in cui non è possibile eseguire alcuna transizione
-	2 se la stringa non termina:
		se il ttl scade e la stringa non è accettata nè non accettata

*/

int tm_run(char * tape) {
	TM_cfg* p_tm_cfg=NULL;
	TM_cfg* p_prov_tm_cfg = NULL;
    int local_ttl;
    int status=2;

    local_ttl=ttl;
    while (local_ttl>0 && status==2) {

    	if (p_head_tm_cfg==NULL) {								//	Creo la prima configurazione se ho una stringa nuova
    		p_tm_cfg=(TM_cfg* )malloc(sizeof(TM_cfg));
	   		p_tm_cfg->prev=NULL;
    		p_tm_cfg->active_state=0;
    		p_tm_cfg->pos_tape=0;
    		p_tm_cfg->tape=(char* )malloc(strlen(tape)+1);
        	strcpy(p_tm_cfg->tape,tape);
      		p_tm_cfg->next=NULL;
    		p_head_tm_cfg=p_tm_cfg;    
    	}
  // print_tm_cfg(p_head_tm_cfg);

    	while (p_tm_cfg!=NULL && status==2) {					//	Scorro tutta la lista delle configurazioni, facendo fare ad ognuna uno step
    		p_prov_tm_cfg = p_tm_cfg->next;
    		status = tm_step(p_tm_cfg);							//	Lo step di una configurazione può creare più di una nuova configurazione
        	p_tm_cfg = p_prov_tm_cfg;
	   	}

    	if (status == 1) {
    		remove_all_tm_cfgs ();
    		return 1;
    	}
    	if (status == 0) return 0;
    	
    	p_tm_cfg=p_head_tm_cfg;
    	local_ttl--;
   // 	printf("Local_ttl=%d\n\n",local_ttl);
    }
    return 2;
}

/*

************************************************************************************************************************************************************	tm_step

int tm_step (Tm_cfg* )

Funzione che scorre tutte le configurazioni attualmente presenti e, per ognuna di esse, controlla se nella funzione di transizione esiste una 
mossa eseguibile. 
Se esiste la esegue creando un nodo precedente a quello corrente modificandolo opportunamente, se non esiste restituisce 0 e cancella il nodo 
che contiene la configurazione non accettata

*/

int tm_step(TM_cfg* p_tm_cfg) {
	Star* star=testa_funzione;
	TM_cfg* p_tm_cfg_new=NULL;
	char* temp_tape = NULL;
	int ret=0;
  	if (is_final_state (p_tm_cfg->active_state)==1) return 1;
  	while (star!=NULL) {

  		if (star->curr_state == p_tm_cfg->active_state && star->read == p_tm_cfg->tape[p_tm_cfg->pos_tape]) {			//	Se trovo una mossa che posso fare, la faccio

    		p_tm_cfg_new=(TM_cfg* )malloc(sizeof(TM_cfg));																		// 	Read
  			p_tm_cfg_new->prev=NULL;
    		p_tm_cfg_new->next=NULL;

    		p_tm_cfg_new->tape=(char* )malloc(strlen(p_tm_cfg->tape)+1);														//	Write
       		strcpy (p_tm_cfg_new->tape,p_tm_cfg->tape);
      		p_tm_cfg_new->tape[p_tm_cfg->pos_tape]=star->write;
 	     	p_tm_cfg_new->next=p_tm_cfg;

 	     	if (p_tm_cfg->prev == NULL) {							//	Quando allaccio il nuovo nodo distinguo il caso in cui è il primo
 	     		p_tm_cfg_new->next = p_tm_cfg;
 	     		p_tm_cfg->prev = p_tm_cfg_new;
 	     		p_head_tm_cfg=p_tm_cfg_new;
 	     	}

   			else {													//	Dal caso in cui ne esiste un altro
   				p_tm_cfg->prev->next = p_tm_cfg_new;
   				p_tm_cfg_new->prev = p_tm_cfg->prev;
   				p_tm_cfg_new->next = p_tm_cfg;
   				p_tm_cfg->prev = p_tm_cfg_new;
     	 		}

   				p_tm_cfg_new->active_state=star->next_state;																	//	Move
    			switch (star->move) {	
    				case 'R': 	p_tm_cfg_new->pos_tape=(p_tm_cfg->pos_tape+1);
    							if (p_tm_cfg_new->pos_tape == strlen(p_tm_cfg_new->tape)) {			//	Se vado oltre il nastro a destra
    								temp_tape = (char* )malloc(strlen(p_tm_cfg->tape)+2);
									strcpy (temp_tape, p_tm_cfg->tape);
    								strcat (temp_tape, "_");
    								p_tm_cfg_new->tape = temp_tape;
    							}
    							break;
    				case 'L': 	
    							p_tm_cfg_new->pos_tape=(p_tm_cfg->pos_tape-1);
    							if (p_tm_cfg_new->pos_tape == -1) {								//	Se vado oltre il nastro a sinistra
    								p_tm_cfg_new->pos_tape = 0;
    								temp_tape = (char* )malloc(strlen(p_tm_cfg_new->tape)+2);
    								strcpy(temp_tape,"_");
    								strcat(temp_tape, p_tm_cfg_new->tape);
    								p_tm_cfg_new->tape = temp_tape;
    							}
    							break;     
    				default : 	p_tm_cfg_new->pos_tape=p_tm_cfg->pos_tape;
    							break;
    			}
     	 	p_tm_cfg->prev = p_tm_cfg_new;
      		if (is_final_state (p_tm_cfg_new->active_state)==1) return 1;						//	Accetto la stringa se sono in uno stato finale
  		}
  	
  		star=star->next_star;
 	}

	ret=remove_tm_cfg (p_tm_cfg);

return ret;
}

/*

************************************************************************************************************************************************************	print_memory_tape

void print_memory_tape ()

Funzione che scorre la lista con i puntatori ai nastri di memoria, salvati in array, e li stampa uno per uno

*/

void print_memory_tape () {
	List_of_memory_tape* riga = testa_lista_nastri;
	printf("Stampa dei nastri di memoria acquisiti\n");

	while (riga!=NULL) {
		printf("%s\n",riga->data_from_file);
		riga = riga->next_saved_tape;
	}
	printf("Fine nastri memoria\n");
	return;
}

/*

************************************************************************************************************************************************************	is_final_state

int is_final_state (int)

Funzione che riceve in input uno stato della TM e controlla se esso è uno stato finale o no. 
Restituisce 1 se lo stato in input è finale, 0 se non lo è

*/

int is_final_state (int state) {
  End_state* p_end_state=testa_end_state;  
  while (p_end_state!=NULL) {
  	if (p_end_state->end_state_value==state) return 1;
  	p_end_state=p_end_state->next_end_state;
  }
  return 0;
}

/*

************************************************************************************************************************************************************	remove_tm_cfg

int remove_tm_cfg (TM_cfg* )

Funzione che riceve in input un puntatore a una configurazione della TM e lo elimina, aggiustando i puntatori ai nodi precedenti e successivi.
Restituisce 0 se cancella l'unico nodo della lista delle configurazioni, 2 altrimenti

*/

int remove_tm_cfg (TM_cfg* p_tm_cfg) {

	free(p_tm_cfg->tape);

	if (p_tm_cfg->next==NULL && p_tm_cfg->prev==NULL) {											//	Se elimino l'unica configurazione, la stringa non è accettata e restituisco 0
  		p_head_tm_cfg = NULL;
		free (p_tm_cfg);
  		return 0;
  	}
	
	else if (p_tm_cfg->next!=NULL && p_tm_cfg->prev==NULL) {									//	Se la configurazione è la prima della lista
		p_tm_cfg->next->prev = NULL;
  		p_head_tm_cfg = p_tm_cfg->next;
		free (p_tm_cfg);
  	}

  	else if (p_tm_cfg->prev != NULL && p_tm_cfg->next==NULL) {									//	Se la configurazione è l'ultima della lista
		p_tm_cfg->prev->next = NULL;
		free (p_tm_cfg);
		//printf("AAAA");print_tm_cfg(p_tm_cfg);
  	}

  	else if (p_tm_cfg->prev!=NULL && p_tm_cfg->next!=NULL) {									//	Se la configurazione è in mezzo ad altre 2
  		p_tm_cfg->prev->next = p_tm_cfg->next;
  		p_tm_cfg->next->prev = p_tm_cfg->prev;
		free (p_tm_cfg);
  	}

  	else 	printf("Delete nothig\n");

  	return 2;	
}

/*

************************************************************************************************************************************************************	remove_all_tm_cfgs

void remove_all_tm_cfgs ()

Funzione che esegue la funzione remove_tm_cfg su tutte le configurazioni della TM

*/

void remove_all_tm_cfgs () {
	while (p_head_tm_cfg != NULL) {
		remove_tm_cfg (p_head_tm_cfg);
	}
	return;
}

/*

************************************************************************************************************************************************************	print_tm_cfg

void print_tm_cfg (TM_cfg* )

Funzione che riceve in input un puntatore a una configurazione della TM e la stampa 

*/
void print_tm_cfg (TM_cfg* p_tm_cfg) {
		printf("Stato %d\t",p_tm_cfg->active_state);
		printf("Posizione %d\t",p_tm_cfg->pos_tape);
		printf("%s\n",p_tm_cfg->tape);
		return;
}

/*

************************************************************************************************************************************************************	print_all_tm_cfgs

void print_all_tm_cfgs ()

Funzione che esegue la funzione print_tm_cfg su tutti i nodi della lista delle configurazioni

*/

void print_all_tm_cfgs () {
	TM_cfg* temp_cfg = p_head_tm_cfg;
	int cont = 0;
	while (temp_cfg!=NULL) {
		cont++;
		printf("Contatore %d\t",cont);								//	Il contatore serve a sapere quante configurazioni ci sono nel momento della stampa
		print_tm_cfg(temp_cfg);
		temp_cfg = temp_cfg->next;
	}
	return;
}

/*

************************************************************************************************************************************************************

*/
