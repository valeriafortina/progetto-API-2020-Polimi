#include <stdio.h>
#include <string.h>
//#include <math.h>
#include <malloc.h>

#define MAX_STRINGA 30
#define MAX_NASTRO 2097152
#define RED 0
#define BLACK 1

#define BLOCCO 512


//variabili globali

typedef struct __lista_stato_accettazione {
	long valore;
	struct __lista_stato_accettazione *succ;
} lista_stato_accettazione;

typedef struct __vettore_regola {
	char scrittura;
	char mossa;
	long stato_arrivo;
	short ultimo;
	struct __vettore_regola *succ;
} vettore_regola;


typedef struct __albero_lett {
	struct __albero_lett *padre;
	struct __albero_lett *left;
	struct __albero_lett *right;
	struct __vettore_regola *figlio;
	long lettera; 	//chiave, salvo i char in ascii
	short colore;
} albero_lett;


typedef struct __albero {		//sentinella � NULL
	struct __albero *padre;
	struct __albero *left;
	struct __albero *right;
	struct __albero_lett *figlio;
	long stato;	//chiave
	short colore; //0 rosso, 1 nero
} albero;

//funzioni

void left_rotate1(albero **T, albero *x)
{
	albero *y, *t;
	y = x->right;
	x->right = y->left;

	if(y->left != NULL)
	{
		t = y->left;
		t->padre = x;
	}
	y->padre = x->padre;
	t = x->padre;	//serve per l'else if
	if(x->padre == NULL)
	{
		*T = y;		//T.root = y secondo il libro
	}
	else if(x == t->left)
	{
		t->left = y;
	}
	else
	{
		t->right = y;
	}
	y->left = x;
	x->padre = y;

}

void right_rotate1(albero **T, albero *x)
{
	albero *y, *t;
	y = x->left;
	x->left = y->right;

	if(y->right != NULL)
	{
		t = y->right;
		t->padre = x;
	}
	y->padre = x->padre;
	t = x->padre; //per l'else if
	if(x->padre == NULL)
	{
		*T = y;
	}
	else if(x == t->right)
	{
		t->right = y;
	}
	else
	{
		t->left = y;
	}
	y->right = x;
	x->padre = y;
}

void left_rotate2(albero_lett **T, albero_lett *x)
{
	albero_lett *y, *t;
	y = x->right;
	x->right = y->left;

	if(y->left != NULL)
	{
		t = y->left;
		t->padre = x;
	}
	y->padre = x->padre;
	t = x->padre;	//serve per l'else if
	if(x->padre == NULL)
	{
		*T = y;		//T.root = y secondo il libro
	}
	else if(x == t->left)
	{
		t->left = y;
	}
	else
	{
		t->right = y;
	}
	y->left = x;
	x->padre = y;

}

void right_rotate2(albero_lett **T, albero_lett *x)
{
	albero_lett *y, *t;
	y = x->left;
	x->left = y->right;

	if(y->right != NULL)
	{
		t = y->right;
		t->padre = x;
	}
	y->padre = x->padre;
	t = x->padre; //per l'else if
	if(x->padre == NULL)
	{
		*T = y;
	}
	else if(x == t->right)
	{
		t->right = y;
	}
	else
	{
		t->left = y;
	}
	y->right = x;
	x->padre = y;
}

void fixup1(albero **T, albero *z)
{
	albero *y, *t, *w;
	t = z->padre;
	if(t == NULL)
	{
		(**T).colore = BLACK;
		return;
	}
	w = t->padre;
	while(t != NULL && t->colore == RED)	//ho controllato, prima controlla che non sia nullo, nel caso lo sia, blocca il cilco senza errori
	{
		if(t == w->left)
		{
			y = w->right;
			if(y != NULL && y->colore == RED)
			{
				t->colore = BLACK;
				y->colore = BLACK;
				w->colore = RED;
				z = w;
			}
			else
			{
				if(z == t->right)
				{
					z = t;
					left_rotate1(T, z);
				}
				t = z->padre;		//devo aggiungerlo, potrebbe cambiare
				w = t->padre;
				t->colore = BLACK;
				w->colore = RED;
				right_rotate1(T, w);
			}

		}
		else if(t == w->right)	//t == w->right
		{
			y = w->left;
			if(y != NULL && y->colore == RED)
			{
				t->colore = BLACK;
				y->colore = BLACK;
				w->colore = RED;
				z = w;
			}
			else
			{
				if(z == t->left)
				{
					z = t;
					right_rotate1(T, z);
					//printf("\nok");
				}
				//printf("\nnot ok");
				//printf("\nz = %d t = %d", z->stato, t->stato);
				t = z->padre;
				w = t->padre;
				t->colore = BLACK;
				w->colore = RED;
				//printf("\nw = %d colore di w: %d", w->stato, w->colore);
				left_rotate1(T, w);
			}
		}
		t = z->padre;	//per sicurezza
		if(t != NULL)
		{
			w = t->padre;	//per sicurezza
		}
	}
	(**T).colore = BLACK;
}


// Inserisce lo stato nell'albero
// parametri in ingresso:
//    albero **T = puntatore alla struttura albero
//    long valore = valore da inserire
// parametri di uscita:
//    ritorna la posizione del parametro nell'albero dopo l'inserimento
albero *inserisci_stato (albero **T, long valore)
{
	albero *y, *x, *z, *copia;
	z = (albero *)malloc(sizeof(albero));
	z->stato = valore;
	z->figlio = NULL;
	y = NULL;
	x = *T;
	while(x != NULL)
	{
		y = x;
		if(z->stato == x->stato)
		{
			free(z);
			return(x);		//lo stato � gi� presente, restituisco quello che esiste
		}
		else if(z->stato < x->stato)
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}
	z->padre = y;
	if(y == NULL)		//z � il primo elemento inserito
	{
		*T = z;
	}
	else if(z->stato < y->stato)
	{
		y->left = z;
	}
	else
	{
		y->right = z;
	}
	z->left = NULL;
	z->right = NULL;
	z->colore = RED;
	copia = z;
	fixup1(T, z);
	return(copia);	//perch� fixup sposta lo z
}

void fixup2(albero_lett **T, albero_lett *z)		//da sistemare il while
{
	albero_lett *y, *t, *w;
	t = z->padre;
	if(t == NULL)
	{
		(**T).colore = BLACK;
		return;
	}
	w = t->padre;
	while(t != NULL && t->colore == RED)	//ho controllato, prima controlla che non sia nullo, nel caso lo sia, blocca il cilco senza errori
	{
		if(t == w->left)
		{
			y = w->right;
			if(y != NULL && y->colore == RED)
			{
				t->colore = BLACK;
				y->colore = BLACK;
				w->colore = RED;
				z = w;
			}
			else
			{
				if(z == t->right)
				{
					z = t;
					left_rotate2(T, z);
				}
				t = z->padre;		//devo aggiungerlo, potrebbe cambiare
				w = t->padre;
				t->colore = BLACK;
				w->colore = RED;
				right_rotate2(T, w);
			}

		}
		else if(t == w->right)	//t == w->right
		{
			y = w->left;
			if(y != NULL && y->colore == RED)
			{
				t->colore = BLACK;
				y->colore = BLACK;
				w->colore = RED;
				z = w;
			}
			else
			{
				if(z == t->left)
				{
					z = t;
					right_rotate2(T, z);
				}
				t = z->padre;
				w = t->padre;
				t->colore = BLACK;
				w->colore = RED;
				left_rotate2(T, w);
			}
		}
		t = z->padre;	//per sicurezza
		if(t != NULL)
		{
			w = t->padre;	//per sicurezza
		}
	}
	(**T).colore = BLACK;
}



albero_lett *inserisci_lett(albero *stato, long carattere) 	//non serve doppio puntatore, perch� non modifico l'indirizzo
{
	albero_lett *y, *x, *z, *copia;
	albero_lett **T; //facciamo tutto uguale per sicurezza
	T = &(stato->figlio);
	z = (albero_lett *)malloc(sizeof(albero_lett));
	z->lettera = carattere;
	z->figlio = NULL;
	y = NULL;
	x = *T;
	while(x != NULL)
	{
		y = x;
		if(z->lettera == x->lettera)
		{
			free(z);
			return(x);		//lo stato � gi� presente, restituisco quello che esiste
		}
		else if(z->lettera < x->lettera)
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}
	z->padre = y;
	if(y == NULL)		//z � il primo elemento inserito
	{
		*T = z;
	}
	else if(z->lettera < y->lettera)
	{
		y->left = z;
	}
	else
	{
		y->right = z;
	}
	z->left = NULL;
	z->right = NULL;
	z->colore = RED;
	copia = z;
	fixup2(T, z);	//preferisco tenere fixup2 con il doppio puntatore come fixup1, quindi serve l'indirizzo di T
	return(copia);	//perch� fixup sposta lo z
}


vettore_regola *crea_regola(char _scrittura, char _mossa, long _stato_arrivo)
{
	vettore_regola *t;
	t = (vettore_regola *)malloc(sizeof(vettore_regola));
	t->mossa = _mossa;
	t->scrittura = _scrittura;
	t->stato_arrivo = _stato_arrivo;
	t->ultimo = 1;	//inizializzo a 1 per convenzione
	return(t);
}

void inserisci_vett(albero_lett *T, vettore_regola *regola)
{
	vettore_regola *temp;
	temp = T->figlio;
	if(temp == NULL)
	{
		T->figlio = regola;
		regola->ultimo = 1;
	}
	else
	{
		if((regola->mossa == temp->mossa) && (regola->scrittura == temp->scrittura) && (regola->stato_arrivo == temp->stato_arrivo))	//risolto bug in cui scriveva due volte se solo uno salvato
		{
            free(regola);
			return;
		}

		while(temp->succ != NULL)
		{
			if((regola->mossa == temp->mossa) && (regola->scrittura == temp->scrittura) && (regola->stato_arrivo == temp->stato_arrivo))	//la regola � gi� presente in memoria
			{
                free(regola);
				return;
			}
			temp = temp->succ;

		}
		temp->ultimo = 0; 			//quello che era ultimo ora non lo � pi�
		temp->succ = regola;
		regola->ultimo = 1;
	}
}


void add_albero(albero **T, long stato, long lettura, char scrittura, char direzione, long stato_uscita)
{
	albero *a;
	albero_lett *b;
	vettore_regola *regola;
	a = inserisci_stato(T, stato);
	b = inserisci_lett(a, lettura);
	regola = crea_regola(scrittura, direzione, stato_uscita);
	inserisci_vett(b, regola);
	//printf("\nstato: %d\n lettura %d\n scrittura: %c\n direzione: %c\nstato uscita: %d\n", a->stato, b->lettera, v->scrittura, v->mossa, v->stato_arrivo);
}

void leggi_tr(albero **T)
{
	char strg[MAX_STRINGA]; //variabile per stringa
	long stato = 0, stato_uscita = 0;
	char lettura = 'a', scrittura = 'a', direzione = 'S';
	*T = NULL;		//nel caso ci si dimenticasse di inizializzarlo
	fflush(stdin);
	fgets(strg, MAX_STRINGA, stdin);
	fflush(stdin);						//leggo "tr". Utilizzo gets perch� scanf d� problemi con lo spazio
	if(strcmp("tr\n",strg) == 0)
	{
		//printf("ok\n");
	}
	fgets(strg, MAX_STRINGA, stdin);
	fflush(stdin);
	while (strcmp("acc\n", strg) != 0)
	{

		sscanf(strg, "%ld %c %c %c %ld", &stato, &lettura, &scrittura, &direzione, &stato_uscita); 	//OCCHIO alle &!!!
		add_albero(T, stato, ((int)lettura), scrittura, direzione, stato_uscita);
		fgets(strg, MAX_STRINGA, stdin);
		fflush(stdin);
	}

}

//CREA_STRINGA
//alloca e crea una nuova stringa, con _ a inizio e fine stringa tante volte quanto BLOCCO
//parametri di entrata:
//      puntatore a stringa di input
//      indirizzo del puntatore della nuova stringa (senza spazio allocato)
//parametri di uscita:
//      stringa in output
//      lunghezza della stringa, con carattere terminatore incluso

unsigned long crea_stringa(char *input, char **output, unsigned long *lunghezza)
{
    unsigned long num, i;
    char *temp;

    i = 0;
    while( input[i] != '\n' && input[i] != '\0')    //elimino eventuale \n finale
    {
        i++;
    }
    if( input[i] == '\n')
    {
        input[i] = '\0';
    }
    num = strlen(input);
    num = num + 1;  //per il terminatore di stringa
    *lunghezza = num + 2*BLOCCO;
    *output = malloc((*lunghezza)*sizeof(char));
    temp = *output;
    i = 0;
    while(i < BLOCCO)
    {
        temp[i] = '_';
        i++;
    }

    strcpy(&temp[BLOCCO], input);
    i = BLOCCO + num - 1; //vado sul terminatore appena dopo la stringa copiata
    while(i < (*lunghezza -1) )
    {
        temp[i] = '_';
        i++;
    }
    temp[*lunghezza-1] = '\0';
    return(BLOCCO);

}


void copia_stringa(char *input, char **output, unsigned long lunghezza)
{
    if(input == NULL)
	{
		output = NULL;
		return;
	}
	*output = malloc(lunghezza*sizeof(char));
	strcpy(*output, input);

}


void distruggi(char *stringa)
{
    free(stringa);
}


void procedi(vettore_regola *regola, char **stringa, long *stato, unsigned long *pos, unsigned long *lunghezza)
{
    char *nuovo, *temp;
    unsigned long i;
    temp = *stringa;
    temp[*pos] = regola->scrittura;
    *stato = regola->stato_arrivo;
    switch(regola->mossa)
    {
        case 'S':
            break;
        case 'L':
            if(*pos == 0)
            {
                nuovo = malloc( (*lunghezza+BLOCCO)*sizeof(char) );
                i = 0;
                while(i < BLOCCO)
                {
                    nuovo[i] = '_';
                    i++;
                }

                strcpy(&nuovo[BLOCCO], *stringa);
                *pos = *pos + BLOCCO - 1;
                free(*stringa);
                *stringa = nuovo;
                *lunghezza = *lunghezza + BLOCCO;

            }
            else
            {
                *pos = *pos -1;
            }
            break;
        case 'R':
            if(*pos == (*lunghezza-2))   //prima del terminatore
            {

                nuovo = malloc( (*lunghezza+BLOCCO)*sizeof(char) );

                strcpy(nuovo, *stringa);
                i = *lunghezza - 1;
                while(i < *lunghezza + BLOCCO - 1)  //si ferma all'ultimo elemento della stringa e non scrive _ , perch� dopo ci scriver� \0
                {
                    nuovo[i] = '_';
                    i++;
                }
                nuovo[i] = '\0';
                *pos = *pos + 1;
                free(*stringa);
                *stringa = nuovo;
                *lunghezza = *lunghezza + BLOCCO;

            }
            else
            {
                *pos = *pos +1;
            }
            break;
    }
}


char leggi_stringa(char *stringa, unsigned long pos)
{
    return(stringa[pos]);
}


vettore_regola *cerca_regola(long stato, long lettura, albero *T, int *errore_tabella)
{
	albero *albero;
	albero_lett *albero_lett;
	albero = T;		//non devo modificare la posizione del puntatore fuori da questa funzione

	while(albero != NULL && stato != albero->stato)
	{
		
		if(stato > albero->stato)
		{
			albero = albero->right;
		}
		else if(stato < albero->stato)
		{
			albero = albero->left;
		}
	}

	if(albero == NULL)
	{
		*errore_tabella = 1;
		return(NULL);
	}

	albero_lett = albero->figlio;
	while(albero_lett != NULL && lettura != albero_lett->lettera)
	{
		
		if(lettura > albero_lett->lettera)
		{
			albero_lett = albero_lett->right;
		}
		else if(lettura < albero_lett->lettera)
		{
			albero_lett = albero_lett->left;
		}
	}
	if(albero_lett == NULL)
	{
		*errore_tabella = 1;
		return(NULL);
	}

	return(albero_lett->figlio);
}


void add_lista_accettazione(lista_stato_accettazione **T, long stato)
{
	lista_stato_accettazione *z, *temp, *vecchio;
	z = (lista_stato_accettazione *)malloc(sizeof(lista_stato_accettazione));
	z->valore = stato;
	z->succ = NULL;
	if(*T == NULL)
	{
		*T = z;
		return;
	}
	else
	{
		temp = *T;
		while(temp != NULL)
		{
			if(temp->valore == stato)
			{
				free(z);
				return;
			}
			vecchio = temp;
			temp = temp->succ;	//lo aggiungo, non � g� presente
		}
		vecchio->succ = z;
		return;

	}
}

int appartenenza(long stato, lista_stato_accettazione *accettazione)	//1 se appatiene a stati finali, 0 se no
{
	lista_stato_accettazione *temp = accettazione;
	while (temp != NULL)
	{
		if(temp->valore == stato)
		{
			return(1);
		}
		temp = temp->succ;
	}
	return(0);
}

void leggi_acc(lista_stato_accettazione **accettazione)
{
	char strg[MAX_STRINGA];
	long stato_finale;
	fflush(stdin);
	fgets(strg, MAX_STRINGA, stdin);
	while(strcmp("max\n", strg) != 0)
	{
		sscanf(strg, "%ld", &stato_finale);
		add_lista_accettazione(accettazione, stato_finale);		//DA SCRIVEREEEEEE
		fgets(strg, MAX_STRINGA, stdin);
		fflush(stdin);
	}
}

void leggi_max(long unsigned *max)
{
	char strg[MAX_STRINGA];
	fflush(stdin);
	fgets(strg, MAX_STRINGA, stdin);
	sscanf(strg, "%lu", max); 	//max non ha & perch� � gi� puntatore
	fflush(stdin);
	fgets(strg, MAX_STRINGA, stdin);
	if(strcmp("run\n", strg) != 0)
	{
		printf("\nerrore in max!");
	}
}

void stampa(long valore)
{
    if(valore == 2)
    {
        printf("U\n");
    }
    else
    {
    	printf("%ld\n", valore);
    }
	return;
}

typedef struct __Macchina {
	char *stringa;
	long stato;
	unsigned long pos;
	unsigned long lunghezza;
	struct __Macchina *succ;
	struct __Macchina *prec;
} Macchina;

Macchina *crea_macchina(char *stringa, long stato, long unsigned pos, long unsigned lunghezza)
{
    Macchina *temp;
    if(stringa == NULL)
    {
        printf("\nerrore");
    }
    temp = (Macchina*)malloc(sizeof(Macchina));
    temp->stato = stato;
    temp->stringa = stringa;
    temp->pos = pos;
    temp->lunghezza = lunghezza;
    temp->succ = NULL;
    temp->prec = NULL;
    return(temp);
}

void distruggi_macchina(Macchina *macchina, long *flag_nullo, Macchina **T) //ECCO IL BUG, T non veniva gestito correttamente
{
    Macchina *temp;

    if(macchina->prec == T && macchina->succ == NULL)    ////sono state eliminate tutte le macchine, stringa non � accettata da nessuna combinazione
    {
        *flag_nullo = 1;
        free(macchina->stringa);
        free(macchina);
        *T = NULL;
        return;
    }
    temp = macchina->prec;
    if(temp != T)  //indirizzo di T, quindi solo T qui dentro, altrimenti indichertemmo anche il primo elemento puntato da T
    {
        temp->succ = macchina->succ;
    }
    else
    {
        *T = macchina->succ;
    }
    temp = macchina->succ;
    if(temp != NULL)
    {
        temp->prec = macchina->prec;
    }
    free(macchina->stringa);
    free(macchina);
    return; //che � il SUCCESSIVO di macchina
}

void libera_vettore_macchine(Macchina *T)
{
    Macchina *temp;
    while(T != NULL)
    {
        temp = T;
        T = T->succ;
        free(temp->stringa);
        free(temp);
    }
}

long trova_loop_locali(long stato_vecchio, long lettura, char scrittura, char mossa, long stato_nuovo, char *stringa, long *flag_loop, long unsigned pos) //1 se lo trova, 0 se no
{
    char *temp;
    temp = &(stringa)[pos];
    if(stato_vecchio != stato_nuovo)
    {
        return(0);
    }

    else
    {
        if(lettura == (int)(scrittura) && mossa == 'S') //loop su se stesso
        {
            *flag_loop = 1;
            return(1);
        }

        if(lettura == '_' && mossa == 'R')  //loop a destra se � tutto vuoto a destra
        {
            while( *temp != '\0' && *temp == '_')
            {
                temp++;
            }
            if(*temp == '\0')
            {
                *flag_loop = 1;
                return(1);
            }
        }

        if(lettura == '_' && mossa == 'L')  //loop a sinistra se � tutto vuoto a sinistra
        {
            while(*temp == '_' && temp != stringa)
            {
                temp--;
            }
            if(temp == stringa && *temp == '_')
            {
                *flag_loop = 1;
                return(1);
            }
        }
    }

    return(0);
}

long nuova_esegui(char *stringa, albero *albero, lista_stato_accettazione *accettazione, long unsigned max, long unsigned pos, long unsigned lunghezza_T)
{


    char *nuova_stringa, *stringa_debug;
    long unsigned contatore = 0, nuova_pos, nuova_lunghezza;
    long errore_tabella = 0, flag_nullo = 0, flag_loop = 0;
    long lettura;
    long nuovo_stato ;
    vettore_regola *regola;
    Macchina *T, *temp, *nuova_macchina, *da_distruggere, *controllo;    //puntatore a tutte le macchine
    T = crea_macchina(stringa, 0, pos, lunghezza_T);
    T->prec = &T;
    while(contatore != max)
    {
        temp = T;

        while (temp != NULL)
        {
            lettura = leggi_stringa(temp->stringa, temp->pos);
            regola = cerca_regola(temp->stato, lettura, albero, &errore_tabella);

            if(errore_tabella == 1)
            {
                da_distruggere = temp;
                temp = temp->succ;
                distruggi_macchina(da_distruggere, &flag_nullo, &T);
                errore_tabella = 0; //lo reinizializzo
                if(flag_nullo == 1)
                {
                    //libera_vettore_macchine(T);   //non � da mettere, � gi� vuoto T!
                    if(flag_loop == 1)
                    {
                        return(2);
                    }
                    else
                    {
                        return(0);     //sono state eliminate tutte le macchine, stringa non � accettata da nessuna combinazione
                    }
                }
            }
            else
            {
                if(regola->ultimo == 1)
                {
                    if(trova_loop_locali(temp->stato, (int)lettura, regola->scrittura, regola->mossa, regola->stato_arrivo, temp->stringa, &flag_loop, temp->pos)  == 1)
                    {
                        da_distruggere = temp;
                        temp = temp->succ;
                        distruggi_macchina(da_distruggere, &flag_nullo, &T);

                        if(flag_nullo == 1)
                        {
                            //libera_vettore_macchine(T);   //non � da mettere, � gi� vuoto T!
                            if(flag_loop == 1)
                            {
                                return(2);
                            }
                            else
                            {
                                return(0);     //sono state eliminate tutte le macchine, stringa non � accettata da nessuna combinazione
                            }
                        }
                    }
                    else
                    {
                        procedi(regola, &(temp->stringa), &(temp->stato), &(temp->pos), &(temp->lunghezza) );
                        if(appartenenza(temp->stato, accettazione) == 1)
                        {
                            libera_vettore_macchine(T);
                            return(1);
                        }
                        temp = temp->succ;
                    }
                }
                else
                {
                    while(regola->ultimo != 1)
                    {
                        copia_stringa(temp->stringa, &nuova_stringa, temp->lunghezza);
                        nuovo_stato = temp->stato;
                        nuova_pos = temp->pos;
                        nuova_lunghezza = temp->lunghezza;

                        if(trova_loop_locali(temp->stato, lettura, regola->scrittura, regola->mossa, regola->stato_arrivo, nuova_stringa, &flag_loop, temp->pos) == 0 )
                        {
                            procedi(regola, &nuova_stringa, &nuovo_stato, &nuova_pos, &nuova_lunghezza);
                            if(appartenenza(nuovo_stato, accettazione) == 1)
                            {
                                libera_vettore_macchine(T);
                                return(1);
                            }

                            //controllo che non ci sia gi� una macchina con lo stesso stato, la stessa posizione nel nastro e lo stesso nastro, per evitare dei doppioni
                            controllo = temp->prec;

                            while(controllo != &T)
                            {
                                if(controllo->stato == nuovo_stato)
                                {
                                    if(controllo->pos == nuova_pos)
                                    {
                                        if(strcmp(controllo->stringa, nuova_stringa))
                                        {
                                            break;
                                        }
                                    }
                                }

                                controllo = controllo->prec;
                            }
                            ////////////////////////////////////////////////////////////////

                            if(controllo == &T) //non ci sono doppioni di macchine
                            {
                                nuova_macchina = crea_macchina(nuova_stringa, nuovo_stato, nuova_pos, nuova_lunghezza);
                                nuova_macchina->succ = T;   //aggiungo in testa, ha gi� fatto il suo passo
                                T->prec = nuova_macchina;
                                T = nuova_macchina;
                                nuova_macchina->prec = &T;
                            }

                        }
                        else
                        {
                            //non faccio nulla, � un loop
                        }
                        regola = regola->succ;
                    }

                    if(trova_loop_locali(temp->stato, (long)lettura, regola->scrittura, regola->mossa, regola->stato_arrivo, temp->stringa, &flag_loop, temp->pos) == 1)
                    {
                        da_distruggere = temp;
                        temp = temp->succ;
                        distruggi_macchina(da_distruggere, &flag_nullo, &T);
                        if(flag_nullo == 1)
                        {
                            if(flag_loop == 1)
                            {
                                return(2);
                            }
                            else
                            {
                                return(0);     //sono state eliminate tutte le macchine, stringa non � accettata da nessuna combinazione
                            }
                        }
                    }
                    else
                    {
                        procedi(regola, &(temp->stringa), &(temp->stato), &(temp->pos), &(temp->lunghezza) );
                        if(appartenenza(temp->stato, accettazione) == 1)
                        {
                            libera_vettore_macchine(T);
                            return(1);
                        }
                        temp = temp->succ;
                    }

                }


            }

        }
        contatore++;
    }

    libera_vettore_macchine(T);
    return(2); //ha superato il numero max di passi
}

int main()
{

	albero *T;
	lista_stato_accettazione *accettazione;
	char *nastro;
	char strg[MAX_NASTRO];
	T = NULL;
	accettazione = NULL;
	long risultato;
	long unsigned max, pos, lunghezza;


	leggi_tr(&T);
	leggi_acc(&accettazione);
	leggi_max(&max);
    fflush(stdin);
    while( fgets(strg, MAX_NASTRO, stdin) > 0)
    
    {
        
        pos = crea_stringa(strg, &nastro, &lunghezza);
        risultato = nuova_esegui(nastro, T, accettazione, max, pos, lunghezza);
        stampa(risultato);
        fflush(stdin);
    }


	return(0);
}



