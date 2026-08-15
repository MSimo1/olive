#include "upo_ht_linprod.h"
#include <stdio.h>
#include <stdlib.h>


// NB: index_key != Key, index_key è l'indice dello slot della chiave nella tabella, mentre key è la chiave stessa.
void* upo_ht_linprod_get(void* key, upo_tablehash_t* ht) {

    for (int i = 0; i < ht->capacity; i++) {
        // Passo 1: Calcolo l'indice dello slot della chiave nella tabella
        int index_key = ht->key_hash(key, i, ht->capacity); //  (key+i) % ht->capacity

        // Passo 2: Ricerca
        // Controllo se lo slot d'indice index_key è "stato eliminato" o "occupato dalla chiave cercata" o "vuoto"
        // Controllo 1: Se lo slot è stato eliminato, continua al prossimo 
        if (ht->slots[index_key].tombstone == 1) {
            continue; // non fare nulla, passa al prossimo slot (i++)
        }

        // Controllo 2: Se lo slot è occupato dalla chiave cercata, restituisci il valore associato
        if (ht->key_compare(key, ht->slots[index_key].key) == 0) {
            return ht->slots[index_key].value; // Chiave trovata
        }

        // Controllo 3: Se lo slot è vuoto e non è un tombstone, la chiave non è presente nella tabella
        if (ht->slots[index_key].key == NULL && ht->slots[index_key].tombstone == 0) {
            break; // Esci dal ciclo for, la chiave non è presente nella tabella
        }
    }
    return NULL;
}

// Funzione PUT : per inserire una coppia chiave-valore nella tabella hash
void* upo_ht_linprod_put(void* key, void* value, upo_tablehash_t* ht) {
    // Inserimento della coppia chiave-valore nella tabella hash con linear probing

    return NULL; // Restituisce NULL se l'inserimento non è riuscito   
}

// Funzione LOAD FACTOR : per calcolare il fattore di carico della tabella hash
static double upo_ht_linprob_load_factor(upo_tablehash_t* ht){ 
    return (double)ht->size / (double)ht->capacity; 
}

/*
Funzione PUT : per inserire una coppia chiave-valore nella tabella hash

void* upo_ht_linprod_put(void* key, void* value, upo_tablehash_t* ht) {
    // PUT: Inserimento della coppia chiave-valore nella tabella hash con linear probing

    // Passo 1: Controllo se la tabella hash è piena (loard factor >= 0.5), se sì, ridimensiona la tabella hash
    if (upo_ht_linprob_load_factor(ht) >= 0.5) {
        // Ridimensionamento della tabella hash : Resezing e Rehashing della tabella hash M -> 2M
        // ...
    }
    
    // NB: da cui, c'è spazion in tabella per inserire la coppia chiave-valore.
    // inzializzare il vecchio valore della chiave a NULL, se la chiave non era presente nella tabella
    void* old_value = NULL;
    int index_key;

    for(int i=0; i < ht->capacity; i++){

        // Passo 2: Calcolo l'indice dello slot della chiave nella tabella
        index_key = ht->key_hash(key, i, ht->capacity);

        // Passo 3: Ricerca di uno slot disponibile (vuoto o tombstone) per inserire la coppia chiave-valore
        // Controllo 1: Se lo slot è occupato dalla chiave cercata, aggiorna il valore associato e restituisci il vecchio valore
        if (ht->key_compare(key, ht->slots[index_key].key) == 0) {
            //Passo 4: Aggiornamento del valore associato alla chiave
            old_value = ht->slots[index_key].value; // Salva il vecchio valore della chiave
            ht->slots[index_key].value = value; // Modifico il valore associato alla chiave
            // Passo 6: Restituzione del vecchio valore della chiave
            return old_value; // Restituisce il vecchio valore della chiave
        }

        // Passo 4: Inserimento della coppia chiave-valore nello slot trovato

        // Passo 5: Aggiornamento della dimensione della tabella hash
    }
    
    // Passo 6: Restituzione del vecchio valore della chiave o NULL se la chiave non era presente nella tabella

    return old_value; // Restituisce NULL se l'inserimento non è riuscito   
}

*/