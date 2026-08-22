#include <stdio.h>
#include <stdlib.h>
#include "upo_bst.h"

/*
    CARATTERISTICHE DI UN BST:
    - un root (nodo radice)
    - ogni nodo (nodo padre) ha al massimo due figli
    - ogni nodo ha un valore (key) 
    - il figlio sinistro ha un valore (key) minore del nodo padre
    - il figlio destro ha un valore (key) maggiore del nodo padre
    - ogni nodo ha un puntatore al figlio sinistro e un puntatore al figlio destro
    - ogni nodo ha un puntatore al padre (opzionale)

    NB: un BST deve sempre essere bilanciato, altrimenti diventa un albero degenerato (lista collegata) 
        e le operazioni di ricerca, inserimento e cancellazione diventano O(n) invece di O(log n)
        
        ==> il BST deve sempre rispettare le seguenti proprietà:
            - ogni nodo ha al massimo due figli
            - il figlio sinistro ha un valore (key) minore del nodo padre
            - il figlio destro ha un valore (key) maggiore del nodo padre

    == DELETE NODO IN UN BST ==

    3 casi di DELETE:
    -> delete del nodo foglia
        =>senza ricollegamento dei figli
    -> delete del nodo con un solo figlio
        =>ricollegamento del figlio unico al padre
    -> delete del nodo con due figli
        =>ricollegamento del successore al posto del nodo da eliminare

    BN: il "ricollegamento" è IMPORTANTE.
    BN: IL "SUCCESSORE" sarà sempre un NODO FOGLIA con key:
        - il MASSIMO del SOTTOALBERO SINISTRO del nodo da eliminare (successore sinistro) 
            ==> (Alternative #1: use the largest predecessor)
        - il MINIMO del SOTTOALBERO DESTRO del nodo da eliminare (successore destro)
            ==> (Alternative #2: use the smallest successor)
 */

 // FUNZIONE DELETE NODO IN UN BST

 // 1) funzione del .h (wrapper)
 void upo_bst_delete(upo_bst_t* bst, void* key) {
    // chiamata alla funzione ricorsiva di delete
    bst->root = upo_bst_delete_IMPL(bst->root, key, bst->key_cmp);
}

// 2) funzione ricorsiva di delete (IMPLEMENTAZIONE)
upo_bst_node_t* upo_bst_delete_IMPL(upo_bst_node_t* nodo_corrente, void* key, key_compare_func key_cmp) {
    // Implementazione della funzione di cancellazione
    if (nodo_corrente == NULL) {
        return nodo_corrente; // Nodo non trovato
    }

    // ricerca del nodo da eliminare
    int cmp = key_cmp(key, nodo_corrente->key);

    if (cmp < 0) {
        // DISCESA RICORSIVA SINISTRA: la chiave da eliminare è nel lato sinistro.(nodo_corrente->left)
        nodo_corrente->left = upo_bst_delete_IMPL(nodo_corrente->left, key, key_cmp);
    } else if (cmp > 0) {
        // DISCESA RICORSIVA DESTRA: la chiave da eliminare è nel lato destro.(nodo_corrente->right)
        nodo_corrente->right = upo_bst_delete_IMPL(nodo_corrente->right, key, key_cmp);
    } else {
        // NODO TROVATO: nodo_corrente è il nodo da eliminare
        // QUANTI FIGLI HA IL NODO DA ELIMINARE?

        if (nodo_corrente->left != NULL && nodo_corrente->right != NULL) {
            // ===> Caso 1: HA 2 FIGLI
            nodo_corrente = upo_bst_delete_2_figli_IMPL(nodo_corrente, key_cmp);
        }else{
            // ===> Caso 2: HA 1 FIGLIO O NESSUN FIGLIO
            nodo_corrente = upo_bst_delete_1_figlio_IMPL(nodo_corrente);
        }
    }
    return nodo_corrente;
}

// ===> Caso 2: HA 1 FIGLIO O NESSUN FIGLIO
upo_bst_node_t* upo_bst_delete_1_figlio_IMPL(upo_bst_node_t* nodo_corrente) {
    // Salvare il nodo da eliminare
    upo_bst_node_t* temp;
    temp = nodo_corrente;

    // dove c'è in nodo figlio se esiste, altrimenti NULL
    if (nodo_corrente->left != NULL) {
        nodo_corrente = nodo_corrente->left; // Ricollegamento del figlio sinistro al padre
    } else {
        nodo_corrente = nodo_corrente->right; // Ricollegamento del figlio destro al padre
    }

    // Deallocazione del nodo da eliminare
    free(temp);

    // Restituisce il nodo corrente (il figlio ricollegato al padre)
    return nodo_corrente;
}

// ===> Caso 1: HA 2 FIGLI
upo_bst_node_t* upo_bst_delete_2_figli_IMPL(upo_bst_node_t* nodo_corrente, key_compare_func key_cmp) {
    // RICOLLEGAMENTO CON IL PIU GRANDE DEI FIGLI SINISTRI (successore)

    // Trova il nodo più grande del sottoalbero sinistro (successore)
    upo_bst_node_t* successore_sinistro = upo_bst_Max_IMPL(nodo_corrente->left);
    // si modifica la key e il value del NODO CORRENTE con quelli del successore sinistro
    nodo_corrente->key = successore_sinistro->key;
    nodo_corrente->value = successore_sinistro->value;

    // Elimina il VERO nodo successore sinistro dal sottoalbero sinistro( è un nodo foglia) 
    nodo_corrente->left = upo_bst_delete_IMPL(nodo_corrente->left, successore_sinistro->key, key_cmp);
    return nodo_corrente;
}

// MASSIMO KEY in a BST
upo_bst_node_t* upo_bst_Max_IMPL(upo_bst_node_t* nodo) {
    if (nodo == NULL) {
        return NULL; // BST vuoto
    }
    
    if(nodo->right != NULL){
        return upo_bst_Max_IMPL(nodo->right); // Discesa ricorsiva a destra
    }
    // Ritorna il nodo con la chiave massima (nodo foglia più a destra)
    return nodo;
}
