#include "upo_ht_linprod.h"
#include <stdio.h>
#include <stdlib.h>


//================================================================================
// Funzione : GET
//================================================================================

// Pagina 42 della slide
void* upo_ht_linprod_get(void* key, upo_tablehash_t* ht) {
    
    int index_key = ht->key_hash(key, 0, ht->capacity);

    // il ciclo While fa:
    // se lo slot ha una chiave diversa da NULL e che la chiave sia diversa di quella cercata
    // (ht->slots[index_key].key != NULL && ht->key_compare(key, ht->slots[index_key].key) != 0)
    // OPPURE se lo slot è stato eliminato (tombstone == 1)
    // allora continua a cercare il prossimo slot
    while ((ht->slots[index_key].key != NULL && ht->key_compare(key, ht->slots[index_key].key) != 0)
            || (ht->slots[index_key].tombstone == 1)) {
        index_key = ht->key_hash(key, index_key + 1, ht->capacity);
    }

    // Se lo slot ha una chiave diversa da NULL e che la chiave sia uguale a quella cercata
    if(ht->slots[index_key].key != NULL) {
        return ht->slots[index_key].value;
    }else{
        return NULL;
    }

}
  

//================================================================================
// Funzione : PUT
//================================================================================
// Pagina 45 della slide
void* upo_ht_linprod_put(upo_tablehash_t* ht, void* key, void* value) {
    // Inserimento della coppia chiave-valore nella tabella hash con linear probing
    void* value_old = NULL; // Inizializza il vecchio valore della chiave a NULL, se la chiave non era presente nella tabella

    if(upo_ht_linprob_load_factor(ht) >= 0.5) {
        // Ridimensionamento della tabella hash : Resezing e Rehashing della tabella hash M -> 2M
        upo_ht_linprod_resize(ht, ht->capacity * 2);
    }

    int index_key = ht->key_hash(key, 0, ht->capacity);
    int tombstone_trovato = 0; // Flag per indicare se è stato trovato un tombstone
    int index_tombstone = -1; // Indice del primo tombstone trovato

    while ((ht->slots[index_key].key != NULL && ht->key_compare(key, ht->slots[index_key].key) != 0)
            || (ht->slots[index_key].tombstone == 1)) {
        
        // Se troviamo un tombstone, memorizziamo il suo indice
        if (ht->slots[index_key].tombstone == 1 && !tombstone_trovato) {
            tombstone_trovato = 1;
            index_tombstone = index_key;
        }
        // Calcola il prossimo indice usando la funzione di hash
        index_key = ht->key_hash(key, index_key + 1, ht->capacity);
    }

    if(ht->slots[index_key].key == NULL) {
        
        // Se abbiamo trovato un tombstone, inseriamo la nuova coppia chiave-valore in quello slot
        if (tombstone_trovato) {
            index_key = index_tombstone;
        }
        ht->slots[index_key].key = key;
        ht->slots[index_key].value = value;
        ht->slots[index_key].tombstone = 0; // Lo slot ora è occupato
        ht->size++;
    } else {
        // La chiave esiste già, aggiorniamo il valore e restituiamo il vecchio valore
        value_old = ht->slots[index_key].value;
        ht->slots[index_key].value = value;
    }

    return value_old; // Restituisce il vecchio valore se l'inserimento è riuscito   
}

//================================================================================
// Funzione : LOAD FACTOR
//================================================================================
static double upo_ht_linprob_load_factor(upo_tablehash_t* ht){ 
    return (double)ht->size / (double)ht->capacity; 
}