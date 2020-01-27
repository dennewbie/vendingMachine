/*
    A.A. 2019-20
    Caruso Denny 
    0124002062
    
    Esercizio 2 - Distributore automatico
    Traccia:

    Si vuole simulare la gestione di un distributore automatico di prodotti. Si suppone di
    avere 10 prodotti identificati da un nome, da un codice identificativo e da un prezzo.

    Permettere all’utente di:
        - Di inserire dei soldi (10, 20, 50 centesimi, 1 euro) e scegliere un prodotto.
                > In base al costo verificare se è possibile prendere il prodotto.
                > Verificare se c’è bisogno di dare del resto.

        - Periodicamente controllare i prodotti sotto scorta (minore di 3 prodotti).

    Provvedere all’implementazione dell’algoritmo per la simulazione del distributore.
    Effettuare almeno un test per ognuna delle opzioni richieste dall’utente.
*/

/*
    Finalità:

    Il distributore automatico  qui realizzato è basato su una particolare struct.
    La decisione di utilizzare una struct come descritta qui di seguito è data dal fatto
    che c'era la necessità ovvia di memorizzare dati di diversa natura (tipo) insieme in
    maniera ordinata, in modo da gestire le informazioni ad essa legata in maniera automatizzata.

    Le finalità del programma sono quelle quindi di simulare il comportamento di un
    distributore automatico in tutti i suoi casi d'uso. Infatti si simula la visualizzazione
    della lista dei prodotto contenuti all'interno del distributore automatico, 
    l'inserimento di determinati tagli specifici di monete, la richiesta di un determinato
    prodotto inseguito all'inserimento di monete, l'ottenimento del prodotto stesso, 
    la richiesta di ricezione del resto e la visualizzazione del proprio credito 
    all'interno del distributore.

    È prevista anche la mancata ricezione del prodotto quando non sono state inserite 
    abbastanza monete per prelevare il prodotto in questione che di conseguenza ha un
    prodotto maggiore o quando non è più presente quel determinato prodotto 
    all'interno del distributore. 
    Dopo ogni richiesta e ottenimento di prodotto si visualizzerà un elenco dei prodotti
    sotto scorta cioè di cui sono presenti meno di 3 unità.
*/

#include<stdio.h> //Include le funzioni essenziali per l'input e per l'output
#include<stdlib.h> //Include la libreria con funzioni standard del C (es. per usare la system("pause") senza warnings)
#define nProd 10 //Definisco il numero di Prodotti come costante
#define FALSE 0  //Simulo il comportamento di una variabile booleana
#define TRUE 1

/*
    Struct Prodotto per rappresentare e schematizzare un modello di dato strutturato con i seguenti campi:
        - ID, codice numerico intero identificativo
        - Name, nome del prodotto
        - Price, prezzo del prodotto
        - Amount, quantità del prodotto presente all'interno del distributore

    Struct VendingMachine per rappresentare e schematizzare un distributore automatico con i seguenti campi:
        - Balance, l'ammontare del denaro inserito da parte dell'utente
        - Deposit, l'ammontare del denaro guadagnato dal distributore nel corso del tempo
        - products[], array di elementi di tipo Prodotto (struct precedente). Ovvero l'insieme dei prodotti 
          contenuti all'interno distributore.
        - warningProducts[], array di interi con lunghezza pari al numero di elementi contenuti all'interno
          del distributore e serve a memorizzare i prodotti che sono in stato di scorta (stan quasi per finire).
          Per fare questo si sfrutta l'array come una sorta di struttura sincronizzata con quello che è l'array
          di prodotti (ovvero il terzo campo della struct). Questo significa che la posizione i-esima dell'array
          warningProducts[] rappresenta il codice Identificativo del prodotto, mentre il suo contenuto 
          rappresenta (a seconda del suo valore), se questo è in stato di scorta oppure no. Cioè se alla
          posizione i-esima c'è un -1, vuol dire che quel determinato prodotto non è in stato di scorta, mentre 
          se al suo posto c'è un numero (che sarà sempre inferiore a 3), allora vuol dire che ci sono tante 
          scorte rimanenti del prodotto con codice ID pari alla posizione i-esima dove si trova questo determinato
          valore. 

          Es. 
              All'indice 4 di warningProducts[] c'è uno 0. Questo vuol dire che il prodotto con ID pari a 4, non
              presenta ulteriori scorte e dovrebbe essere ripristinato da un'operatore.
*/

/*
    - Bool è un tipo derivato sfruttato per simulare il comportamento di una variabile di tipo Booleano.

    - Choice è un tipo sfruttato per le variabili a scopo scelta durante l'interazione con l'utente.

    - Euro è un tipo derivato sfruttato per rappresentare le variabili che hanno un riferimento all'euro
    intesa come moneta. Viene usato in modo da migliorare la leggibilità e semantica del codice scritto.

    - ID è un tipo derivato sfruttato per indicare il tipo delle variabili ID (codice identificativo) ed è
    un unsigned short int perchè è senza segno e deve semplicemente rappresentare dei valori che vanno da 0
    a 9 nel nostro caso.
*/

typedef unsigned short int Bool;
typedef unsigned short int Choice; 
typedef float Euro;
typedef unsigned short int ID;

struct product {
    ID id;
    char *name;
    Euro price;
    unsigned short int amount;
};
typedef struct product Product;

struct vendingMachine {
    Euro balance; 
    Euro deposit; 
    Product products[nProd];
    short int warningProducts[nProd];
};
typedef struct vendingMachine VendingMachine;

//Gestione Prodotti
void setProductsList(VendingMachine *); 
void getProductsList(VendingMachine *);
void getProduct(VendingMachine *);
void addProduct(VendingMachine *);

//Gestione Saldo Utente e Resto
void clearBalance(VendingMachine *);
void setBalance(VendingMachine *);
void getBalance(VendingMachine *);
void getChange(VendingMachine *);

//Gestione Prodotti Sotto Scorta
void setWarnings(VendingMachine *);
void getWarnings(VendingMachine *);

//Avvio Distributore Automatico
void startVendingMachine(VendingMachine *);

//Sezione Admin
void vendingMachineStats(VendingMachine *);
void clearVendingMachine(VendingMachine *);
void getProductAdmin(VendingMachine *);
void setDeposit(VendingMachine *);
void getDeposit(VendingMachine *);
void takeDeposit(VendingMachine *);

//No Operation
void nop();

//Main: Creo una variabile di tipo vendingMachine. 
/*
        Creo un puntatore di tipo vendingMachine e lo faccio puntare all'indirizzo della variabile 
        creata precedentemente. In questo modo posso andare a lavorare direttamente sugli indirizzi,
        senza la necessità di dover far ritornare un dato di tipo struct ogni volta.
*/

void main() {
    VendingMachine venMachine;
    startVendingMachine(&venMachine);
}

//Procedure relative ai prodotti

/*
    setProductList è una funzione che molto banalmente prende in ingresso un puntatore a vendingMachine.
    Si inizializzano i vari valori dei prodotti e questo si fa per ogni prodotto contenuto all'interno
    della vendingMachine.
*/

void setProductsList(VendingMachine *vendingMachine) {
    Product products[nProd] = {{ 0, "Barretta al Cioccolato      \0", 0.50, 7  },
                               { 1, "Yogurt alla Fragola         \0", 1.00, 8  },
                               { 2, "Acqua Frizzante 50cl        \0", 0.70, 9  },
                               { 3, "Acqua Naturale 50cl         \0", 0.70, 10 },
                               { 4, "The alla Pesca 33cl         \0", 1.20, 0  },
                               { 5, "Patatine Classiche S. Carlo \0", 1.30, 2  },
                               { 6, "Pringles Gusto Paprika      \0", 2.20, 8  },
                               { 7, "Pepsi Cola 33cl             \0", 1.50, 9  },
                               { 8, "Fanta 33cl                  \0", 1.50, 1  },
                               { 9, "Tramezz. P. Cotto e Insalata\0", 3.00, 3  },
                             };

    for(int i=0; i<10; i++) {
        vendingMachine->products[i] = products[i];
    }
}

/*
    getProductsList non fa altro che stampare a video quelli che sono i prodotti contenuti
    all'interno del distributore automatico con tutti i dettagli di ciascuno di essi.
*/

void getProductsList(VendingMachine *vendingMachine) {
    for(int i=0; i<nProd; i++) {
        printf("\nID: %hu\t\tName: %s\t\tPrice: %0.2f\tQuantity: %hu", vendingMachine->products[i].id, vendingMachine->products[i].name, vendingMachine->products[i].price, vendingMachine->products[i].amount);
    }
}

/*
    getProduct è una delle procedure fondamentali e portanti del programma. 
    Si occupa, come si intende dal nome, di far prelevare/ottenere il prodotto
    all'utente. 

    Ovviamente si devono verificare almeno due condizioni:
        - la disponibilità del prodotto all'interno del distributore
        - la quantità di monete inserite all'interno del distributore deve essere 
        almeno pari al prezzo del prodotto che si sta cercando di prelevare.

    Per realizzare tutto ciò, si sfrutta un alto controllo sugli errori di input in
    modo da evitare eventuali crash improvvisi del programma stesso. Si prende quindi
    in input l'ID del prodotto che si vuole prelevare e dopo una prima attenta verifica 
    basata sul codice identificativo, si passa a verificare se le monete totali presenti 
    all'interno del distributore siano maggiori o uguali del prezzo del prodotto richiesto. 
    
    Se questo accade allora si verifica la disponibilità del prodotto all'interno del 
    distributore stesso. Se questo non è più presente (ovvero il suo amount è pari a 0)
    non si può prelevare il prodotto. In caso contrario invece si passa all'emissione 
    del prodotto, lo scalare delle monete presenti come Balance e sommate al deposit e 
    infine alla diminuzione dell'amount del prodotto nel distributore.
    Successivamente si fornisce un eventuale resto, si resettano i prodotti sotto scorta
    secondo le nuove disponibilità e infine si mandano in output quelli che sono i 
    prodotti attualmente sotto scorta in modo da segnalarlo all'utente.

    Ogni qual volta si richiede un prodotto, si manda anche in output quella che è la
    lista dei prodotti del distributore in modo tale da essere maggiormente di facile
    lettura la già difficoltosa esecuzione in termini di User Experience per l'utente finale.

    Qualora invece il prodotto non può essere richiesto a causa dell'assenza o non 
    sufficienza di monete inserite all'interno del distributore, allora si manda un 
    avviso all'utente e si specifica perchè e di quante monete ancora ha bisogno affinchè 
    possa chiedere quel determinato prodotto in questione.

    Alla procedura void gli viene passato come parametro sempre un puntatore alla 
    variabile di tipo struct vendingMachine.

*/

void getProduct(VendingMachine *vendingMachine) {
    Choice userChoice;
    Bool userError3 = FALSE;
    Euro productPrice;

    do {
        /*
            Controllo sull'input dell'utente in modo da prevenire eventuali crash o
            errori all'interno del buffer di input che condizionerebbero il resto 
            del programma.
        */

        userError3 ? printf("\n\t... Si prega di inserire un ID prodotto valido\n") : nop();
        getProductsList(vendingMachine);
        printf("\n\nInserisci il codice ID del prodotto che vorresti prendere: ");

        if (scanf("%hu", &userChoice) != 1) {
            fflush(stdin);
            userChoice = -1;
        }

        if (userChoice < 0 || userChoice > 9) {
            userError3 = TRUE;
        } else {
            userError3 = FALSE;
        }
    } while(userError3);
    
    /*
        Verifica che il valore delle monete inserite all'interno del distributore sia
        superiore o uguale al prezzo del prodotto che si sta cercando di richiedere.
    */
    if(vendingMachine->balance >= vendingMachine->products[userChoice].price) {
        if(vendingMachine->products[userChoice].amount == 0) {
            printf("\n\t... Ci dispiace. Purtroppo il prodotto da te richiesto non e' piu' disponibile.\n");
            printf("Prodotto Richiesto: %sQuantita' Presente nel Distrubutore: %d\n", vendingMachine->products[userChoice].name, vendingMachine->products[userChoice].amount);
            return;
        }
        productPrice = vendingMachine->products[userChoice].price;
        vendingMachine->balance -= productPrice;

        vendingMachine->deposit += productPrice;
        vendingMachine->products[userChoice].amount -= 1;

        /*
            Rispettivamente nell'ordine di esecuzione:
                - Assegnazione del prezzo con l'ID scelto dall'utente alla variabile productPrice
                - Sottrazione del prezzo al valore totale delle monete inserite all'interno del 
                  distributore
                - Aggiunta del prezzo pagato all'ammontare dei soldi depositato all'interno del
                  distributore (guadagno)
                - Sottrazione di un'unità di prodotto relativa a quel determinato prodotto di lì
                  a poco prelevato dall'utente
        */
        printf("\n\t... Il prodotto ti sta per essere consegnato...\n");
        printf("\n\t... Il tuo prodotto e' nel cestello. Non dimenticarlo...\n");
        
        getBalance(vendingMachine);
        setWarnings(vendingMachine); //Riorgranizzazione prodotti sotto scorta
        getWarnings(vendingMachine); //Visualizzazione e controllo periodico dei prodotti sotto scorta
    } else {
        Euro missingEuros = vendingMachine->products[userChoice].price;
        missingEuros -= vendingMachine->balance;

        printf("\n\t... Ci dispiace. Non hai inserito abbastanza monete per prendere questo prodotto.\n\tInserisci piu' monete e riprova. Prodotto Richiesto: %s Prezzo: %0.2f\n", vendingMachine->products[userChoice].name, vendingMachine->products[userChoice].price);
        printf("\n\tTi mancano in totale %0.2f euro per poter prelevare questo prodotto.\n", missingEuros);
        /*
            Segnalazione all'utente di quanto deve inserire ancora prima di poter riuscire a prelevare
            quel determinato di prodotto.
        */
    }
} 

//Procedure per il Balance

/*
    clearBalance consente di azzerare il valore totale delle monete inserite dall'utente.
    Procedura sfrutta quando per esempio si resetta il distributore, quando si dà il resto
    all'utente e quando si inizializza al primo avvio il distributore stesso.
*/

void clearBalance(VendingMachine *vendingMachine) {
    double initialBalance = 0.00f;
    vendingMachine->balance = initialBalance;
}

/*   
    setBalance è una delle altre procedure fondamentali di questo programma. 
    Consente di prendere in input le monete da parte dell'utente. Come al solito viene dedicato 
    ampio spazio al controllo e prevenzione dell'errore di input dell'utente. 
    
    Una volta fatto ciò si passa al sommare la quota inserita dall'utente al suo balance attuale 
    all'interno della macchinetta.
    
    Una volta inserita una moneta viene richiesto all'utente se ne vuole inserire ancora in modo
    da rendere la user experience più fluida e rapida, senza troppe difficoltà intrinseche.

    Come parametro anche questa procedura riceve un puntatore a struct di tipo vendingMachine.
*/

void setBalance(VendingMachine *vendingMachine) {
    Euro coin;
    Bool userError4 = FALSE, userError4_1 = FALSE;
    Choice userAnotherCoin = FALSE;

    do {
        //Doppio controllo errori sul valore della moneta inserita da parte dell'utente
        do {
            userError4 ? printf("\n\t... Si prega di inserire una moneta valida\n") : nop();
            printf("\nInserisci una moneta (0.10, 0.20, 0.50, 1.00 euro): ");
            if (scanf("%f", &coin) != 1) {
                fflush(stdin);
                coin = 0.00f;
            }

            if(coin != 0.10f && coin != 0.20f && coin != 0.50f && coin != 1.00f) {
                userError4 = TRUE;
            } else {
                userError4 = FALSE;
            }
        } while(userError4);

        //Aggiunta del valore finalmente verificato al saldo dell'utente
        vendingMachine->balance += coin;
        getBalance(vendingMachine);

        vendingMachine->balance > 190 ? printf("\nATTENZIONE: Il distributore potrebbe avere difficolta' a fornire resti molto elevati in termini di denaro.\n\n") : nop();

        do {
            /*
                Controllo sull'errore in input per quanto riguarda la scelta dell'utente di 
                inserire un'ulteriore moneta in input all'interno del distributore.
            */
            userError4_1 ? printf("\n\t... Si prega di inserire un valore di scelta consono con quelli forniti\n") : nop();

            printf("\n\nVuoi inserire un'altra moneta? (0 = No || 1 = Si)\nScegli: ");
            if (scanf("%hu", &userAnotherCoin) != 1) {
                fflush(stdin);
                userAnotherCoin = 2;
            }
            
            if(userAnotherCoin != 0 && userAnotherCoin != 1) {
                userError4_1 = TRUE;
            } else {
                userError4_1 = FALSE;
            }
        } while (userError4_1);
    } while(userAnotherCoin);
}

/*
    getBalance permette di mostrare in output l'attuale saldo delle monete totali inserite
    dall'utente all'interno della macchinetta distributrice.
*/

void getBalance(VendingMachine *vendingMachine) {
    printf("\nBalance: %0.2f", vendingMachine->balance);
}

/*
    getChange permette di far ottenere il resto all'utente qualora ce ne fosse bisogno.
    Infatti si va a controllare se sul saldo attuale e corrente dell'utente sono presenti
    o meno dei soldi per poi agire di conseguenza.
*/

void getChange(VendingMachine *vendingMachine) {
    if(vendingMachine->balance > 0.00) {
        printf("\n\t... Non dimenticare di prendere il resto. Ecco a te: %0.2f", vendingMachine->balance);
        clearBalance(vendingMachine);
    } else {
        printf("\n\t... Ci dispiace. Non c'e' alcun resto da dare.\n");
    }
}

//Procedure Warnings

/*
    Le procedure warnings sono utili a determinare e visualizzare i prodotti sotto scorta.
*/

/*
    setWarnings va a fare un controllo su ogni prodotto per vedere quali di questi ha un
    amount inferiore a 3 (come richiesto da traccia). Qualora accadesse allora posiziona 
    alla posizione i-esima che corrisponde all'ID del prodotto, la quantità di prodotti 
    rimanente di quel tipo. 

    Mentre qualora non dovesse succedere allora si pone la posizione i-esima a -1, che 
    vuol dire che quel valore non ha subito cambiamenti e variazioni, così come una cella
    di memoria inattiva.
*/

void setWarnings(VendingMachine *vendingMachine) {
    for(int i=0; i<nProd; i++) {
        //Verifico che la quantità del prodotto sia inferiore a 3
       vendingMachine->warningProducts[i] = vendingMachine->products[i].amount < 3 ? vendingMachine->products[i].amount : -1;

    }
}

/*
    getWarnings permette invece di visualizzare i prodotti che attualmente si trovano in
    stato di "sotto scorta" ovvero la cui quantità è inferiore a 3. Questo viene denotato
    dal fatto che alla posizione i-esima non c'è -1 ma c'è un numero che può essere tra 0
    e 2 ed indica la quantità del prodotto rimanente di quel determinato prodotto con
    codice ID pari all'indice i-esimo.
*/

void getWarnings(VendingMachine *vendingMachine) {
    printf("\n\nI seguenti prodotti della macchinetta distributrice sono quasi del tutto terminati: \n");

    for(int i=0; i<nProd; i++) {
        vendingMachine->warningProducts[i] != -1 ? printf("ID: %hu\t\tName: %s\t\tPrice: %0.2f\tQuantity: %hu\n", vendingMachine->products[i].id, vendingMachine->products[i].name, vendingMachine->products[i].price, vendingMachine->products[i].amount) : nop();
    }
}

//Procedure Admin: Codice Segreto di Accesso 32101

/*
    Le procedure amministratore consente di effettuare operazioni speciali con la macchinetta
    distributrice del tipo prelevare i contanti finora guadagnati, ricaricare i prodotti che
    si trovano sotto scorta o richiedere i prodotti senza dover pagarne il prezzo.

    Inoltre si ha anche la possibilità di ricevere le statistiche del distributore in modo da
    avere una panoramica sul suo funzionamento, e di resettare il distributore in caso di 
    malfunzionamenti generali.

    Fanno parte delle procedure Admin anche le procedure Deposit riportate successivamente e
    sottostanti a queste procedure Admin.
*/

/*
    vendingMachineStats permette di visualizzare a video le statistiche sull'andamento del 
    distributore automatico come la lista dei prodotti, la lista dei prodotti sotto scorta e 
    il guadagno complessivo ottenuto con la merce venduta.
*/

void vendingMachineStats(VendingMachine *vendingMachine) {
    printf("\n\n\t\t***Statistiche Macchinetta Distributrice***\n");

    getProductsList(vendingMachine);
    getWarnings(vendingMachine);
    getDeposit(vendingMachine);
}

/*
    clearVendingMachine permette di resettare completamente la macchinetta distributrice.
    Questo per fare in modo da inizializzarla nel migliore dei modi all'avvio o in seguito
    a eventuali errori di sistema. L'operazione può essere eseguita solo dall'amministratore.
*/

void clearVendingMachine(VendingMachine *vendingMachine) {
    clearBalance(vendingMachine);
    setProductsList(vendingMachine);
    setDeposit(vendingMachine);
    setWarnings(vendingMachine);
}

/*
    addProduct consente di aggiungere i prodotti all'interno del distributore. 
    In questo modo quando l'admin vede che questi sono andati sotto scorta allora può 
    accedere alla sua sezione dedicata e caricare i prodotti. 
    
    Anche in questo caso viene fatto un capillare controllo sull'errore in input, 
    per poi passare all'inserimento effettivo dei prodotti nel distributore tramite 
    l'incremento del campo amount del prodotto interessato.  
*/

void addProduct(VendingMachine *vendingMachine) {
    unsigned short int ID;
    unsigned short int newProd, prodAmount;
    Bool userError5 = FALSE, userError6 = FALSE;

    do {
        //Controllo Errori Input
        userError5 ? printf("\n\t... Si prega di inserire un codice ID valido.\n") : nop();

        printf("\nInserisci l'ID del prodotto di cui vuoi incrementare la quantita' nel distributore: ");
        if (scanf("%hu", &ID) != 1) {
            fflush(stdin);
            ID = -1;
        }
        
        if(ID < 0 || ID > 9) {
            userError5 = TRUE;
        } else {
            userError5 = FALSE;
        }
    } while(userError5);

    /*
        Si fa in modo che non si possano inserire più prodotti di quanti realmente accoglibili
        Si suppone che il limite di accoglibilità per ogni prodotto è di 100 unità.
    */

    prodAmount = vendingMachine->products[ID].amount;

    do {
        userError6 ? printf("\n\t... Si prega di inserire un input valido.\n") : nop();

        printf("\nInserisci il numero di nuovi prodotti '%s' vuoi inserire: ", vendingMachine->products[ID].name);
        if (scanf("%hu", &newProd) != 1) {
            fflush(stdin);
            newProd = 0;
        }

        if(newProd < 1 || newProd + prodAmount > 99) {
            printf("\n\t... Ci dispiace. Non puoi inserire questo numero di prodotti.\n");
            userError6 = TRUE;
        } else {
            userError6 = FALSE;
        }
    } while(userError6);

    vendingMachine->products[ID].amount += newProd;
}

/*
    getProductAdmin permette all'admin di prelevare un prodotto dal distributore senza
    dover pagare alcuna cifra. Ovviamente per accedere a questa e ad altre funzioni è
    necessario un codice segreto. 
    
    Il suo funzionamento è uguale alla procedura per prelevare un prodotto per l'utente
    solo che non va a fare la verifica sul prezzo, ma solo quella sulla disponibilità
    del prodotto richiesto dall'amministratore in questo caso.
*/

void getProductAdmin(VendingMachine *vendingMachine) {
    Choice userChoice;
    Bool userError7 = FALSE;

    do {
        userError7 ? printf("\n\t... Si prega di inserire un ID prodotto valido\n") : nop();

        getProductsList(vendingMachine);
        printf("\n\nInserisci il codice ID del prodotto che vorresti prendere: ");

        if (scanf("%hu", &userChoice) != 1) {
            fflush(stdin);
            userChoice = -1;
        }

        if (userChoice < 0 || userChoice > 9) {
            userError7 = TRUE;
        } else {
            userError7 = FALSE;
        }
    } while(userError7);
        if(vendingMachine->products[userChoice].amount <= 0) {
            printf("\n\t... Ci dispiace. Purtroppo il prodotto da te richiesto non e' piu' disponibile.\n");
            return;
        }

        vendingMachine->products[userChoice].amount -= 1;
        printf("\n\t... Il prodotto ti sta per essere consegnato...\n");
        printf("\n\t... Il tuo prodotto e' nel cestello. Non dimenticarlo...\n");
        
        setWarnings(vendingMachine);
        getWarnings(vendingMachine);
}

//Procedure Deposit

/*
    Fanno parte delle procedure Admin e permettono di gestire il guadagno totale realizzato
    con la vendita dei prodotti all'interno del distributore automatico.
*/

/*
    setDeposit permette di settare un deposito di base che potrebbe essere necessario per dare
    un eventuale resto spropositato all'utente.
*/

void setDeposit(VendingMachine *vendingMachine) {
    vendingMachine->deposit = 200.00f;
}

/*
    getDeposit consente di stampare il guadagno effettivo realizzato con il distributore
*/

void getDeposit(VendingMachine *vendingMachine) {
    printf("\n\nGuadagno (Euro): %0.2f", vendingMachine->deposit);
    printf("\nGuadagno Netto (Euro): %0.2f", vendingMachine->deposit - 200.0f);
}

/*
    takeDeposit permette all'admin di prendere l'ammontare di soldi presenti all'interno del distributore
*/

void takeDeposit(VendingMachine *vendingMachine) {
    printf("\n\nSoldi incassati...\n");
    getDeposit(vendingMachine);
    vendingMachine->deposit = 0.00f;

    printf("\n... Carico 200.00 euro nel distributore\n");
    setDeposit(vendingMachine);
}

//Procedura startVendingMachine

/*
    La procedura startVendingMachine consente di avviare e gestire la macchina nella sua interezza,
    durante tutto il ciclo di vita del distributore automatico. Si può affermare senza ombra di
    dubbio che questa è la procedura più importante dopo il main, da cui partono poi tutte le 
    chiamate alle altre procedure e di conseguenza il funzionamento del distributore stesso.
    
    Per iniziare vengono dichiarate alcune variabili di tipo Booleano e di tipo Choice che 
    serviranno successivamente per gesire le scelte dell'utente durante l'esecuzione del programma 
    stesso. Il controllo sull'input dell'utente per evitare errori anche qui è molto vasto e intenso.
    L'utente si ritroverà a dover scegliere l'opzione di un menù per proseguire che gli permetterà
    di eseguire azioni diverse a seconda del numero inserito. Qualora venisse inserito il codice 
    32101, allora si può accedere alla sezione e alle funzioni dedicate all'amministratore.

    Infine si chiede all'utente se vuole eseguire un'altra operazione all'interno del distributore 
    automatico in modo da rendere tale macchinetta quanto più aderente alla realtà ovvero ai 
    distributori automatici che si incontrano tutti i giorni. Il tutto viene gestito con delle 
    flag e degli opportuni cicli do-while che consentono di eseguire il corpo del ciclo almeno una volta.
*/

void startVendingMachine(VendingMachine *vendingMachine) {
    Choice userChoice, adminChoice;
    Bool userContinue = FALSE, userError1 = FALSE, userError2 = FALSE, adminExit = FALSE;
    char welcomeString[500] = "\n\n\t\t***Benvenuti nel Distributore Automatico***\nGuarda i prodotti, scegli quello o quelli di cui hai bisogno e inserisci le monete\nnecessarie per riceverlo. Il distributore accetta solo monete da 0.10, 0.20, 0.50 e\n1.00 euro. Naviga nel menu di scelta per avere piu' informazioni sui prodotti.";
    
    clearVendingMachine(vendingMachine);
    printf("%s", welcomeString);
    
    do {
        //Controllo Errori Input Utente
        do {
            userError1 ? printf("\n\t... Si prega di effettuare una scelta valida tra quelle presenti all'interno del Menu'") : nop();

            //Visualizzazione del Menù di Scelta
            printf("\n\nMenu':\n1) Lista Prodotti\n2) Inserisci Monete\n3) Stampa Saldo Monete\n4) Richiedi Prodotto\n5) Richiedi Resto\n6) Esci\n\nInserisci il numero dell'operazione che vuoi eseguire: ");
            
            if (scanf("%hu", &userChoice) != 1) {
                fflush(stdin);
                userChoice = 0;
            }
            
            if((userChoice < 1 || userChoice > 6) && userChoice != 32101){
                userError1 = TRUE;
            } else {
                userError1 = FALSE;
            }
        } while(userError1);

        switch(userChoice) {
            case 1: 
                //Stampa Prodotti
                getProductsList(vendingMachine);
                break;
            case 2:
                //Inserisci Monete (Modifica il Saldo Utente)
                setBalance(vendingMachine);
                break;
            case 3:
                //Stampa Saldo Utente
                getBalance(vendingMachine);
                break;
            case 4:
                //Richiedi un Prodotto 
                getProduct(vendingMachine);
                break;
            case 5:
                //Richiedi il resto
                getChange(vendingMachine);
            break;
                case 6:
                //Uscita Forzata e Saluti
                printf("\n\nGrazie :D\n\n");
                system("pause");
                return;
                break;
            //Entrata nella sezione Admin
            case 32101:
                printf("\n\n...Admin Section...\n");
                do {
                    //Controllo Errori Input
                    do {
                        printf("\n\nMenu' Admin:\n1) Aggiungi Prodotto\n2) Estrai Ricavo\n3) Reset Distributore\n4) Richiedi Prodotto Plus\n5) Esci\nScegli: ");

                        if (scanf("%hu", &adminChoice) != 1) {
                            fflush(stdin);
                            adminChoice = 0;
                        }
                    } while (adminChoice < 1 || adminChoice > 5);
                    
                    switch(adminChoice) {
                        case 1:
                            //Aggiungi Scorte Prodotto
                            addProduct(vendingMachine);
                            break;
                        case 2:
                            //Estrai il ricavo
                            takeDeposit(vendingMachine);
                            break;
                        case 3:
                            //Reset Distributore
                            printf("\n... Reset in corso... \n");
                            clearVendingMachine(vendingMachine);
                            break;
                        case 4:
                            //Ottieni Prodotto senza pagare
                            getProductAdmin(vendingMachine);
                            break;
                        case 5:
                            break;
                        default:
                            printf("\n\nError no. #3");
                            break;
                    }

                    do {
                        printf("\n\nVuoi eseguire un'altra funzione admin? (0 = No || 1 = Si)\nScegli: ");

                        if (scanf("%hu", &adminExit) != 1) {
                            fflush(stdin);
                            adminExit = 2;
                        }
                        
                    } while (adminExit != FALSE && adminExit != TRUE);
                } while (adminExit);
                break;
            default:
                printf("\n\nError no. #2.");
                break;
        }

        do {
            //Controllo Errore Input
            userError2 ? printf("\n\n\t...Si prega di inserire un valore di scelta consono con quelli forniti\n") : nop();

            //Verifica se l'utente vuole effettuare qualche altra operazione con il distributore oltre a quella già eseguita
            printf("\n\nVuoi interagire nuovamente con il distributore? (0 = No || 1 = Si)\nScegli: ");
            if (scanf("%hu", &userContinue) != 1) {
                fflush(stdin);
                userContinue = 2;
            }

            if(userContinue != FALSE && userContinue != TRUE) {
                userError2 = TRUE;
            } else {
                userError2 = FALSE;
            }
        } while(userError2);
    } while(userContinue);
    printf("\n\nGrazie e Arrivederci :D\n\n");
    system("pause");
}

/*
    nop corrisponde a No Operation. Viene sfruttata quando nella realizzazione di un if su una sola riga, 
    la clausola else non può restare vuota e lasciata a sè stante.
*/

void nop() {
    return;
}

/*    
_________________________________________________________________________________________________________________________________
|                                                                                                                               |
|                                                        All rights reserved                                                    |
|                                                        Made by: @dennewbie                                                    |
|                                        Contact: https://www.linkedin.com/in/denny-caruso/                                     |
|                                                                                                                               |
|_______________________________________________________________________________________________________________________________|
*/