#include <stdio.h>

// "OPEN ADDRESSING"

typedef struct upo_slot{
    void* key;
    void* value;
    int tombstone; // 0: slot occupato o vuoto, 1: slot vuoto ma eliminato
} upo_slot_t;

// funzioni generiche della tabella hash
typedef int (*key_hash_fun)(void* key, int i, size_t capacity);
typedef int (*key_compare_fun)(void* key1, void* key2);

 typedef struct upo_tablehash{
    // 1 Vettore di slot della tabella hash
    upo_slot_t* slots; 

    // 2 variabili della tabella hash
    size_t capacity; // Numero di slot disponibili nella tabella
    size_t size; // Numero di slot occupati nella tabella

    // 2 funzioni generiche della tabella hash
    key_hash_fun key_hash; // Funzione che calcola l'indice dello slot della chiave nella tabella
    key_compare_fun key_compare; // Funzione che confronta due chiavi e restituisce un valore -1,0,1

} upo_tablehash_t;

// Funzione GET : per ottenere il valore associato a una chiave nella tabella hash
void* upo_ht_linprod_get(void* key, upo_tablehash_t* ht);

// Funzione CONTAINS : per verificare se una chiave è presente nella tabella hash
int upo_ht_linprod_contains(void* key, upo_tablehash_t* ht);

// Funzione PUT : per inserire una coppia chiave-valore nella tabella hash
void* upo_ht_linprod_put(void* key, void* value, upo_tablehash_t* ht);

// Structure hash table con il metodo di "OPEN ADDRESSING" con linear probing
/*
STRUCTURE Hash table:

    ==> "1 vettore" nella struttura hash table:
    - Slots:(un VETTORE di STRUTTURE che contiene le coppie chiave-valore e un segnale di tombstone "Key - Value - Tombstone ")
        - Key: (la chiave della coppia)
        - Value: (il valore della coppia)
        - Tombstone: (un segnale che indica la coppia Key-Value in uno slot è stata eliminata)

    ==> "2 variabili" nella struttura hash table:
    - Capacity: (il numero di slot disponibili nella tabella)
    - Size: (il numero di slot occupati "con key-value" nella tabella)
    
    ==> "2 funzioni generici" della struttura hash table:
    - key_hash: (una funzione che calcola l'indice "numero dello slot" della chiave nella tabella)
    - key_compare: (una funzione che confronta due chiavi e restituisce un valore -1,0,1)


Ex: GET function (Value "trovato se key" / null "non trovato"):

get("chiave1", tabella) {

    for (int i = 0; i < tabella->capacity; i++) {

        // passo 1: calcolo l'indice dello slot della chiave "chiave1" nella tabella
        int index_key = tabella->key_hash("chiave1",i,tabella->capacity);

        // passo 2: controllo se lo slot d'indice index_key è stato eliminato o occupato dalla chiave cercata o vuoto
        // controllo 1:
        if (slots[index_key].tombstone == 1) {
            // slots[index_key].tombstone: Controlla se lo slot d'indice index_key è stato eliminato
            // Lo slot è stato eliminato, quindi la chiave non può essere trovata in questo slot

            continue; // Passa al prossimo slot (fare i++)
        }

        // controllo 2:
        if (key_compare("chiave1", slots[index_key].key) == 0) {
            // key_compare(key1, key2): Controlla se lo slot d'indice index_key è occupato dalla chiave cercata
            // La chiave è stata trovata nello slot index_key
            // slots[index_key].value contiene il valore associato alla chiave

            return slots[index_key].value; // Chiave trovata
        }

        // controllo 3:
        if (slots[index_key].key == NULL && slots[index_key].tombstone == 0) {
            // slots[index_key].key: Controlla se lo slot d'indice index_key è vuoto
            // slots[index_key].tombstone: Controlla se lo slot d'indice index_key è stato eliminato
            // Se lo slot è vuoto e non è un tombstone, la chiave non è presente nella tabella

            break; // Esci dal ciclo for, la chiave non è presente nella tabella
        }
    }  
    return NULL; // Chiave non trovata
}

*/


