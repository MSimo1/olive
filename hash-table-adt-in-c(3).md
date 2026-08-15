# Hash Table ADT in C

Materiale basato sulle slide **“Algorithms 1 – Lab: The Hash Table ADT”** di Marco Guazzone, DiSIT – University of Piemonte Orientale.

## Summary

Una **Hash Table** associa una chiave (key) a un valore (value). Una funzione hash trasforma la chiave in un indice della tabella. Se due chiavi producono lo stesso indice, si verifica una collisione.

~~~text
"Alice" → "9999"
"Bob"   → "2222"
~~~

## Goals

- Comprendere struttura, operazioni e complessità dell’ADT Hash Table.
- Implementare una Hash Table in C con **separate chaining**.
- Conoscere **open addressing**, **linear probing**, tombstone, resizing e rehashing.
- Usare void pointer, puntatori a funzione e gestione corretta della memoria.
- Prepararsi agli esercizi `ID_Marq` su separate chaining, linear probing e operazioni extra.

## Scope

Il documento tratta definizioni, funzioni hash, load factor, collisioni, due strategie di risoluzione, API generica `ID_Marq`, demo C e flusso degli esercizi.

## 1. Definizione e contesto

Una Hash Table è un array di M celle, chiamate slot o bucket. La funzione hash ha forma:

~~~text
h : U → {0, ..., M - 1}
~~~

dove U è l’universo delle chiavi e M la capacità.

Applicazioni: ricerca, tabelle simboliche dei compilatori, cache, Distributed Hash Tables, set e dizionari. La Hash Table è molto efficace per la ricerca, ma non mantiene le chiavi ordinate; non è quindi ideale per minimo, massimo o intervalli ordinati.

## 2. Complessità

| Struttura | Ricerca media | Inserimento medio | Caso peggiore | Chiavi ordinate |
|---|---:|---:|---:|---|
| BST | O(log N) | O(log N) | O(N) | Sì |
| Hash Table | O(1) | O(1) | Dipende dalle collisioni | No |

Con separate chaining lo spazio è tipicamente Θ(N + M). Con open addressing è Θ(M). O(1) è una complessità media: molte collisioni possono peggiorare le operazioni.

## 3. Load factor

Il load factor misura il riempimento:

~~~text
α = N / M
~~~

- N: numero di coppie chiave-valore;
- M: numero di slot.

| Valore | Effetto |
|---|---|
| α alto | Più collisioni e operazioni più lente |
| α basso | Più memoria inutilizzata |

## 4. Operazioni dell’ADT

| Operazione | Significato |
|---|---|
| put(t, k, v) | Inserisce o aggiorna k e restituisce il valore precedente |
| insert(t, k, v) | Inserisce solo se k non è duplicata |
| get(t, k) | Restituisce il valore associato |
| contains(t, k) | Verifica se k esiste |
| delete(t, k) | Rimuove la coppia |
| clear(t) | Rimuove tutte le coppie |
| is_empty(t) | Verifica se la tabella è vuota |
| size(t) | Restituisce il numero di chiavi |
| capacity(t) | Restituisce il numero di slot |
| load_factor(t) | Restituisce N / M |

## 5. Funzione hash

Una buona funzione hash deve essere:

- consistente: chiavi uguali producono lo stesso valore;
- efficiente: il calcolo è rapido;
- uniforme: le chiavi sono distribuite sugli slot.

Per interi:

~~~text
h(k) = k mod M
~~~

Per stringhe:

~~~text
h = (h * a + codice_del_carattere) mod M
~~~

Le slide citano djb2, Java String hashCode e varianti ispirate a Kernighan e Ritchie.

## 6. Flusso generale

Il diagramma è una sintesi del flusso descritto nelle slide.

~~~mermaid
flowchart TD
    A["Chiave"] --> B["Funzione hash"]
    B --> C["Indice 0..M-1"]
    C --> D{"Collisione?"}
    D -->|No| E["Inserisci o cerca"]
    D -->|Sì| F["Risoluzione collisione"]
    F --> G["Separate chaining o linear probing"]
    G --> E
~~~

## 7. Separate chaining

Ogni slot contiene il puntatore alla testa di una lista concatenata. Le chiavi con lo stesso indice finiscono nella stessa lista.

~~~mermaid
flowchart TD
    A["Chiave k"] --> B["h(k)"]
    B --> C["Slot"]
    C --> D["Lista concatenata"]
    D --> E{"Chiave trovata?"}
    E -->|Sì| F["Restituisci o aggiorna"]
    E -->|No| G["Continua o restituisci assenza"]
~~~

Rappresentazione C:

~~~c
typedef struct Entry {
    void *key;
    void *value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry **slots;
    size_t capacity;
    size_t size;
} HashTable;
~~~

### Ricerca

1. Calcolare h(k).
2. Accedere allo slot h(k).
3. Scorrere la lista.
4. Confrontare le chiavi.
5. Restituire il valore o NULL.

La lunghezza media della lista è circa N / M.

### Cancellazione

Si cerca il nodo mantenendo anche il precedente, si aggiornano i collegamenti, si liberano nodo, chiave e valore con free(), quindi si decrementa size.

## 8. Open addressing e linear probing

Gli elementi restano direttamente nell’array. Nel linear probing la posizione da provare è:

~~~text
c(k, i) = (h(k) + i) mod M
~~~

Se h(k) = 2, si provano 2, 3, 4, 5, ecc.

Problemi:

- la tabella può diventare piena;
- un load factor alto aumenta il numero di probe;
- una cancellazione ingenua può interrompere ricerche valide.

### Tombstone

Uno slot cancellato viene marcato invece di essere considerato completamente vuoto:

~~~text
key = NULL
value = NULL
tombstone = true
~~~

La ricerca attraversa un tombstone; un inserimento può riutilizzarlo.

### Resizing e rehashing

Quando il load factor supera una soglia, la capacità cambia e le chiavi devono essere reinserite, perché cambia l’indice calcolato da h(k). Le soglie indicate sono tipicamente:

- crescita quando α >= 0.5;
- riduzione quando α <= 0.125.

## 9. Confronto

| Aspetto | Separate chaining | Linear probing |
|---|---|---|
| Collisioni | Liste concatenate | Altri slot dell’array |
| Più di M chiavi | Possibile | Non possibile |
| Cancellazione | Scollegamento del nodo | Tombstone |
| Memoria | Θ(N + M) | Θ(M) |
| Rischio | Liste lunghe | Clustering e tabella piena |

## 10. ADT generico in C

Nel file del corso l’ADT è generico: la tabella non conosce il tipo concreto di chiave o valore. Usa puntatori `void *` e riceve dall’esterno le funzioni necessarie per hash e confronto.

~~~c
typedef size_t (*ID_Marq_ht_hasher_t)(const void *key, size_t capacity);
typedef int (*ID_Marq_ht_comparator_t)(const void *key1, const void *key2);
~~~

`ID_Marq_ht_hasher_t` deve restituire un indice valido `[0, capacity)`. Il comparatore restituisce `0` quando due chiavi sono uguali. La tabella memorizza gli indirizzi ricevuti: non copia automaticamente i dati. Il parametro `destroy_data` decide se, durante `delete`, `clear` e `destroy`, devono essere liberate anche chiave e valore.

Le strutture dati usate nei due esercizi sono:

~~~c
typedef struct ID_Marq_ht_sepchain_node_s ID_Marq_ht_sepchain_node_t;
typedef struct ID_Marq_ht_sepchain_s *ID_Marq_ht_sepchain_t;
typedef struct ID_Marq_ht_linprob_s *ID_Marq_ht_linprob_t;
~~~

Il prefisso `ID_Marq` sostituisce il prefisso del toolkit del corso; il modello dell’ADT rimane invariato.

### Operazioni dell’ADT

| Operazione | Comportamento |
|---|---|
| `put` | Inserisce oppure sostituisce il valore della chiave |
| `insert` | Inserisce solo se la chiave non esiste (crea uno nuovo slot o no fa niente) |
| `get` | Restituisce il valore oppure `NULL` |
| `contains` | Verifica la presenza della chiave |
| `delete` | Rimuove la coppia chiave-valore |
| `clear` | Svuota la tabella mantenendola utilizzabile |
| `is_empty` | Verifica se `size == 0` |
| `size` | Restituisce il numero di coppie |
| `capacity` | Restituisce il numero di slot/bucket |
| `load_factor` | Calcola `size / capacity` |
| `destroy` | Libera struttura e dati posseduti |

## 11.1. Demo didattico in C — Separate Chaining

Qui ogni bucket contiene il puntatore al primo nodo della lista. Le chiavi e i valori sono `void *`; quindi lo stesso codice può lavorare con interi, stringhe, strutture o altri tipi. Il demo usa i nomi delle strutture dell’ADT `ID_Marq`.

~~~c
#include <stdio.h>
#include <stdlib.h>

#define ID_MARQ_SC_CAPACITY 7u

// Struttura dati
typedef size_t (*ID_Marq_ht_hasher_t)(const void *, size_t);
typedef int (*ID_Marq_ht_comparator_t)(const void *, const void *);

typedef struct ID_Marq_ht_sepchain_node_s {
    void *key;
    void *value;
    struct ID_Marq_ht_sepchain_node_s *next;
} ID_Marq_ht_sepchain_node_t;

typedef struct ID_Marq_ht_sepchain_s {
    ID_Marq_ht_sepchain_node_t **buckets;
    size_t capacity;
    size_t size;
    ID_Marq_ht_hasher_t hasher;
    ID_Marq_ht_comparator_t comparator;
} *ID_Marq_ht_sepchain_t;

static ID_Marq_ht_sepchain_t ID_Marq_ht_sepchain_create(
    size_t capacity, ID_Marq_ht_hasher_t hasher,
    ID_Marq_ht_comparator_t comparator)
{
    ID_Marq_ht_sepchain_t ht = malloc(sizeof(*ht));
    if (ht == NULL || capacity == 0 || hasher == NULL || comparator == NULL) {
        free(ht);
        return NULL;
    }
    ht->buckets = calloc(capacity, sizeof(*ht->buckets));
    if (ht->buckets == NULL) { free(ht); return NULL; }
    ht->capacity = capacity; ht->size = 0;
    ht->hasher = hasher; ht->comparator = comparator;
    return ht;
}

static ID_Marq_ht_sepchain_node_t *ID_Marq_ht_sepchain_find(
    ID_Marq_ht_sepchain_t ht, const void *key)
{
    size_t i = ht->hasher(key, ht->capacity);
    for (ID_Marq_ht_sepchain_node_t *p = ht->buckets[i]; p != NULL; p = p->next)
        if (ht->comparator(p->key, key) == 0) return p;
    return NULL;
}

static int ID_Marq_ht_sepchain_put(ID_Marq_ht_sepchain_t ht,
                                   void *key, void *value, int destroy_data)
{
    ID_Marq_ht_sepchain_node_t *p = ID_Marq_ht_sepchain_find(ht, key);
    if (p != NULL) {
        if (destroy_data) free(p->value);
        p->value = value;
        return 1;
    }
    size_t i = ht->hasher(key, ht->capacity);
    p = malloc(sizeof(*p));
    if (p == NULL) return 0;
    p->key = key; p->value = value;
    p->next = ht->buckets[i]; ht->buckets[i] = p; ++ht->size;
    return 1;
}

static int ID_Marq_ht_sepchain_insert(ID_Marq_ht_sepchain_t ht,
                                      void *key, void *value)
{
    if (ID_Marq_ht_sepchain_find(ht, key) != NULL) return 0;
    return ID_Marq_ht_sepchain_put(ht, key, value, 0);
}

static void *ID_Marq_ht_sepchain_get(ID_Marq_ht_sepchain_t ht, const void *key)
{
    ID_Marq_ht_sepchain_node_t *p = ID_Marq_ht_sepchain_find(ht, key);
    return p == NULL ? NULL : p->value;
}

static int ID_Marq_ht_sepchain_contains(ID_Marq_ht_sepchain_t ht,
                                        const void *key)
{ return ID_Marq_ht_sepchain_find(ht, key) != NULL; }

static int ID_Marq_ht_sepchain_delete(ID_Marq_ht_sepchain_t ht,
                                      const void *key, int destroy_data)
{
    size_t i = ht->hasher(key, ht->capacity);
    ID_Marq_ht_sepchain_node_t *p = ht->buckets[i], *prev = NULL;
    while (p != NULL && ht->comparator(p->key, key) != 0) {
        prev = p; p = p->next;
    }
    if (p == NULL) return 0;
    if (prev == NULL) ht->buckets[i] = p->next; else prev->next = p->next;
    if (destroy_data) { free(p->key); free(p->value); }
    free(p); --ht->size; return 1;
}

static void ID_Marq_ht_sepchain_clear(ID_Marq_ht_sepchain_t ht, int destroy_data)
{
    for (size_t i = 0; i < ht->capacity; ++i) {
        ID_Marq_ht_sepchain_node_t *p = ht->buckets[i];
        while (p != NULL) {
            ID_Marq_ht_sepchain_node_t *next = p->next;
            if (destroy_data) { free(p->key); free(p->value); }
            free(p); p = next;
        }
        ht->buckets[i] = NULL;
    }
    ht->size = 0;
}

static int ID_Marq_ht_sepchain_is_empty(ID_Marq_ht_sepchain_t ht)
{ return ht->size == 0; }
static size_t ID_Marq_ht_sepchain_size(ID_Marq_ht_sepchain_t ht)
{ return ht->size; }
static size_t ID_Marq_ht_sepchain_capacity(ID_Marq_ht_sepchain_t ht)
{ return ht->capacity; }
static double ID_Marq_ht_sepchain_load_factor(ID_Marq_ht_sepchain_t ht)
{ return (double)ht->size / (double)ht->capacity; }
static void ID_Marq_ht_sepchain_destroy(ID_Marq_ht_sepchain_t ht, int destroy_data)
{ if (ht != NULL) { ID_Marq_ht_sepchain_clear(ht, destroy_data); free(ht->buckets); free(ht); } }

static size_t int_hash(const void *key, size_t capacity)
{ return *(const int *)key % capacity; }
static int int_cmp(const void *a, const void *b)
{ return (*(const int *)a > *(const int *)b) - (*(const int *)a < *(const int *)b); }

int main(void)
{
    ID_Marq_ht_sepchain_t ht = ID_Marq_ht_sepchain_create(
        ID_MARQ_SC_CAPACITY, int_hash, int_cmp);
    int k1 = 1, k2 = 8, k3 = 15, v1 = 100, v2 = 200, v3 = 300;
    if (ht == NULL) return EXIT_FAILURE;
    ID_Marq_ht_sepchain_insert(ht, &k1, &v1);
    ID_Marq_ht_sepchain_insert(ht, &k2, &v2); /* collisione: 1 % 7 == 8 % 7 */
    ID_Marq_ht_sepchain_put(ht, &k1, &v3, 0); /* aggiorna il valore */
    printf("presente=%d, valore=%d, size=%zu, alpha=%.2f\n",
           ID_Marq_ht_sepchain_contains(ht, &k1),
           *(int *)ID_Marq_ht_sepchain_get(ht, &k1),
           ID_Marq_ht_sepchain_size(ht), ID_Marq_ht_sepchain_load_factor(ht));
    ID_Marq_ht_sepchain_delete(ht, &k2, 0);
    ID_Marq_ht_sepchain_clear(ht, 0);
    printf("vuota=%d, capacity=%zu\n", ID_Marq_ht_sepchain_is_empty(ht),
           ID_Marq_ht_sepchain_capacity(ht));
    ID_Marq_ht_sepchain_destroy(ht, 0);
    (void)k3;
    return EXIT_SUCCESS;
}
~~~

Nota di ownership: nel demo le variabili sono automatiche, quindi `destroy_data` vale `0`. Se chiave e valore sono stati creati con `malloc`, si passa `1` alle operazioni che devono liberarli. `put` aggiorna una chiave esistente; `insert` rifiuta il duplicato.

La complessità media è `Θ(1)` se l’hash distribuisce bene le chiavi; nel caso peggiore, con una lista lunga, è `Θ(N)`. La memoria è `Θ(M + N)`.

## 11.2. Demo didattico in C — Open Addressing

Nell’open addressing non vengono creati nodi collegati: chiave e valore sono memorizzati direttamente nell’array della tabella. Qui si usa il `linear probing`:

~~~text
posizione(i) = (hash(key) + i) % CAPACITY
~~~

Ogni cella può trovarsi in uno dei tre stati seguenti:

| Stato | Significato nella ricerca |
|---|---|
| `EMPTY` | La ricerca può terminare: la chiave non è presente |
| `OCCUPIED` | Si confronta la chiave e si continua il probing se necessario |
| `DELETED` | È un tombstone: la ricerca deve continuare, ma l’inserimento può riusare la cella |

### Flowchart dell’Open Addressing

Il flusso seguente mostra la logica comune di ricerca e inserimento con
`linear probing`. Dopo una collisione si prova la cella successiva; una cella
`DELETED` non interrompe mai la ricerca.

~~~mermaid
flowchart TD
    A["Chiave k"] --> B["i = h(k)"]
    B --> C{"Stato della cella?"}
    C -->|EMPTY| D["Cella libera: assente o inseribile"]
    C -->|OCCUPIED| E{"Chiave uguale?"}
    E -->|Sì| F["Trovata: restituisci o aggiorna"]
    E -->|No| G["Prova i = (i + 1) mod M"]
    C -->|DELETED| G
    G --> C
    D --> H["Fine"]
    F --> H
~~~

Il diagramma è una sintesi del funzionamento del `linear probing`; il codice
deve inoltre fermarsi dopo aver esaminato al massimo `capacity` celle, per
evitare un ciclo infinito quando la tabella è piena.

La capacità è fissa per mantenere il demo semplice. In un’implementazione reale, quando il load factor diventa troppo alto, si aumenta la capacità e si esegue il rehashing di tutte le coppie.

~~~c
#include <stdio.h>
#include <stdlib.h>

#define ID_MARQ_OA_CAPACITY 11u

typedef size_t (*ID_Marq_ht_hasher_t)(const void *, size_t);
typedef int (*ID_Marq_ht_comparator_t)(const void *, const void *);
typedef enum { ID_MARQ_EMPTY, ID_MARQ_OCCUPIED, ID_MARQ_DELETED } ID_Marq_ht_state_t;

// Struttura dello Slot : Una singola cella della tabella : non è un nodo di una lista.
typedef struct {
    void *key;
    void *value;
    int  tombstone;
} ID_Marq_ht_linprob_slot_t;

// Struttura della Tabella di Hash : Array dinamico di capacity celle, indicizzato dal probing.
typedef struct ID_Marq_ht_linprob_s {
    ID_Marq_ht_linprob_slot_t *slots;
    size_t capacity;
    size_t size;
    ID_Marq_ht_hasher_t key_hasher; // la Funzione di hashing
    ID_Marq_ht_comparator_t key_cmp; // Funzione di confronto
} *ID_Marq_ht_linprob_t;

/* `slot` = una cella; `slots` = array di tutte le celle. */

// Creazione della tabella di Hash
static ID_Marq_ht_linprob_t ID_Marq_ht_linprob_create(
    size_t capacity, ID_Marq_ht_hasher_t key_hasher, ID_Marq_ht_comparator_t comparator)
{
    ID_Marq_ht_linprob_t ht = malloc(sizeof(*ht));
    if (ht == NULL || capacity == 0 || key_hasher == NULL || comparator == NULL) {
        free(ht); return NULL;
    }

    ht->slots = calloc(capacity, sizeof(*ht->slots));
    if (ht->slots == NULL) { free(ht); return NULL; }
    ht->capacity = capacity; 
    ht->size = 0;
    ht->key_hasher = key_hasher; 
    ht->comparator = comparator;
    return ht;
}

/* Restituisce la posizione della chiave; -1 significa "non trovata". */
static int ID_Marq_ht_linprob_find(ID_Marq_ht_linprob_t t, const void *key)
{
    size_t start = t->key_hasher(key, t->capacity);
    for (size_t step = 0; step < t->capacity; ++step) {
        size_t i = (start + step) % t->capacity;
        if (t->slots[i].state == ID_MARQ_EMPTY) return -1;
        if (t->slots[i].state == ID_MARQ_OCCUPIED &&
            t->comparator(t->slots[i].key, key) == 0) return (int)i;
    }
    return -1;
}

/* Cerca una posizione libera o un tombstone riutilizzabile. */
static int ID_Marq_ht_linprob_position_for_insert(ID_Marq_ht_linprob_t t,
                                                  const void *key)
{
    size_t start = t->hasher(key, t->capacity);
    int first_deleted = -1;
    for (size_t step = 0; step < t->capacity; ++step) {
        size_t i = (start + step) % t->capacity;
        if (t->slots[i].state == ID_MARQ_OCCUPIED) {
            if (t->comparator(t->slots[i].key, key) == 0) return (int)i;
        } else if (t->slots[i].state == ID_MARQ_DELETED) {
            if (first_deleted == -1) first_deleted = (int)i;
        } else {
            return first_deleted != -1 ? first_deleted : (int)i;
        }
    }
    return first_deleted;
}

static int ID_Marq_ht_linprob_put(ID_Marq_ht_linprob_t t,
                                  void *key, void *value, int destroy_data)
{
    int i = ID_Marq_ht_linprob_position_for_insert(t, key);
    if (i < 0) return 0;                 /* tabella piena */
    ID_Marq_ht_linprob_slot_t *slot = &t->slots[i];
    if (slot->state == ID_MARQ_OCCUPIED) {
        if (destroy_data) free(slot->value);
        slot->value = value;
        return 1;
    }
    slot->key = key; slot->value = value;
    slot->state = ID_MARQ_OCCUPIED; ++t->size;
    return 1;
}

static int ID_Marq_ht_linprob_insert(ID_Marq_ht_linprob_t t, void *key, void *value)
{
    if (ID_Marq_ht_linprob_find(t, key) >= 0) return 0;
    return ID_Marq_ht_linprob_put(t, key, value, 0);
}

static void *ID_Marq_ht_linprob_get(ID_Marq_ht_linprob_t t, const void *key)
{
    int i = ID_Marq_ht_linprob_find(t, key);
    return i < 0 ? NULL : t->slots[i].value;
}
static int ID_Marq_ht_linprob_contains(ID_Marq_ht_linprob_t t, const void *key)
{ return ID_Marq_ht_linprob_find(t, key) >= 0; }

static int ID_Marq_ht_linprob_delete(ID_Marq_ht_linprob_t t,
                                     const void *key, int destroy_data)
{
    int i = ID_Marq_ht_linprob_find(t, key);
    if (i < 0) return 0;
    if (destroy_data) { free(t->slots[i].key); free(t->slots[i].value); }
    t->slots[i].key = NULL; t->slots[i].value = NULL;
    t->slots[i].state = ID_MARQ_DELETED; --t->size;
    return 1;
}

static void ID_Marq_ht_linprob_clear(ID_Marq_ht_linprob_t t, int destroy_data)
{
    for (size_t i = 0; i < t->capacity; ++i) {
        if (t->slots[i].state == ID_MARQ_OCCUPIED && destroy_data) {
            free(t->slots[i].key); free(t->slots[i].value);
        }
        t->slots[i].key = NULL; t->slots[i].value = NULL;
        t->slots[i].state = ID_MARQ_EMPTY;
    }
    t->size = 0;
}
static int ID_Marq_ht_linprob_is_empty(ID_Marq_ht_linprob_t t) { return t->size == 0; }
static size_t ID_Marq_ht_linprob_size(ID_Marq_ht_linprob_t t) { return t->size; }
static size_t ID_Marq_ht_linprob_capacity(ID_Marq_ht_linprob_t t) { return t->capacity; }
static double ID_Marq_ht_linprob_load_factor(ID_Marq_ht_linprob_t t)
{ return (double)t->size / (double)t->capacity; }
static void ID_Marq_ht_linprob_destroy(ID_Marq_ht_linprob_t t, int destroy_data)
{ if (t != NULL) { ID_Marq_ht_linprob_clear(t, destroy_data); free(t->slots); free(t); } }

static size_t int_hash_oa(const void *key, size_t capacity)
{ return *(const int *)key % capacity; }
static int int_cmp_oa(const void *a, const void *b)
{ return (*(const int *)a > *(const int *)b) - (*(const int *)a < *(const int *)b); }

int main(void)
{
    ID_Marq_ht_linprob_t t = ID_Marq_ht_linprob_create(
        ID_MARQ_OA_CAPACITY, int_hash_oa, int_cmp_oa);
    int k1 = 1, k2 = 12, v1 = 100, v2 = 200, v3 = 300;
    if (t == NULL) return EXIT_FAILURE;
    ID_Marq_ht_linprob_insert(t, &k1, &v1);
    ID_Marq_ht_linprob_insert(t, &k2, &v2); /* collisione e linear probing */
    ID_Marq_ht_linprob_put(t, &k1, &v3, 0);
    printf("presente=%d, valore=%d, size=%zu, alpha=%.2f\n",
           ID_Marq_ht_linprob_contains(t, &k1),
           *(int *)ID_Marq_ht_linprob_get(t, &k1),
           ID_Marq_ht_linprob_size(t), ID_Marq_ht_linprob_load_factor(t));
    ID_Marq_ht_linprob_delete(t, &k1, 0); /* tombstone: non EMPTY */
    printf("dopo delete=%d, capacity=%zu\n",
           ID_Marq_ht_linprob_is_empty(t), ID_Marq_ht_linprob_capacity(t));
    ID_Marq_ht_linprob_clear(t, 0);
    ID_Marq_ht_linprob_destroy(t, 0);
    (void)v3;
    return EXIT_SUCCESS;
}
~~~

Nel blocco `ID_Marq_ht_linprob_find`, una cella `ID_MARQ_DELETED` non interrompe la ricerca. Questo è il punto essenziale del tombstone. Il caso medio è `Θ(1)` con load factor contenuto; il caso peggiore è `Θ(M)`. La memoria è `Θ(M)`.

Per compilare i due demo, estrarre ciascun blocco in un file `.c` separato e usare:

~~~bash
gcc -std=c11 -Wall -Wextra -pedantic demo_separate_chaining.c -o demo_sc
gcc -std=c11 -Wall -Wextra -pedantic demo_open_addressing.c -o demo_oa
~~~

## 12. Uso dell’ADT ID_Marq

Esempio generico: il valore restituito da `get` è `void *` e viene convertito al tipo corretto solo nel punto in cui lo studente sa quale tipo ha memorizzato.

~~~c
ID_Marq_ht_sepchain_t ht = ID_Marq_ht_sepchain_create(
    ID_MARQ_SC_CAPACITY, int_hash, int_cmp);
int key = 4, value = 42;
ID_Marq_ht_sepchain_insert(ht, &key, &value);
if (ID_Marq_ht_sepchain_contains(ht, &key))
    printf("%d\n", *(int *)ID_Marq_ht_sepchain_get(ht, &key));
ID_Marq_ht_sepchain_destroy(ht, 0);
~~~

Se i dati sono dinamici, il distruttore va chiamato con `1`, dopo aver verificato che la tabella ne possieda effettivamente la memoria.

## 13. Esercizi del laboratorio

### Exercise #1 — Separate chaining

Obiettivo: implementare `ID_Marq_ht_sepchain_t` in `src/hashtable.c`.

File indicati:

- include/ID_Marq/hashtable.h;
- src/hashtable_private.h;
- src/hashtable.c;
- test/mk/hashtable.mk;
- test/test_hashtable_sepchain.c;
- test/test_hashtable_sepchain_more.c.

~~~bash
make clean bin
make clean test
./test/test_hashtable_sepchain
valgrind --tool=memcheck --leak-check=full ./test/test_hashtable_sepchain
~~~

I test verificano creazione, distruzione, put/get/delete, insert/get/delete, clear, tabella vuota, size, funzioni hash e casi NULL.

### Exercise #2 — Linear probing

Prerequisito: completare Exercise #1.

~~~bash
make clean bin
make clean test
./test/test_hashtable_linprob
valgrind --tool=memcheck --leak-check=full ./test/test_hashtable_linprob
~~~

Sono verificati anche resizing e gestione dei tombstone.

### Exercise #3 — Operazioni extra

- keys(t): restituisce tutte le chiavi;
- traverse(t, v): applica una funzione a ogni coppia chiave-valore.

~~~bash
./test/test_hashtable_sepchain_more
valgrind --tool=memcheck --leak-check=full ./test/test_hashtable_sepchain_more
./test/test_hashtable_linprob_more
valgrind --tool=memcheck --leak-check=full ./test/test_hashtable_linprob_more
~~~

## Stakeholders

| Stakeholder | Interesse |
|---|---|
| Studente | Comprendere e implementare l’ADT |
| Docente | Verificare correttezza, complessità e memoria |
| Toolkit `ID_Marq` | Fornire interfacce, tipi e test |
| Compilatore/test runner | Validare il codice |
| Valgrind | Rilevare memory leak ed errori |

## Risks

- Funzione hash non uniforme: molte collisioni.
- Load factor alto: performance peggiori.
- Puntatori errati: lista corrotta.
- Cancellazione senza tombstone: chiavi valide non più raggiungibili.
- Mancanza di free(): memory leak.
- Hasher o comparatore incompatibile con void pointer: comportamento indefinito.
- Il demo didattico non implementa automaticamente resizing, rehashing, `keys` e `traverse`.

## Decisions

- Il demo usa separate chaining perché rende chiara la gestione delle collisioni.
- Il demo concreto usa indirizzi di interi automatici; l’ADT rimane completamente generico.
- La versione `ID_Marq` usa void pointer, hasher, comparator e destroy_data.
- Il linear probing è trattato separatamente perché richiede tombstone e resizing/rehashing.

## Next steps

1. Recuperare il toolkit `ID_Marq` e verificare l’header della hashtable.
2. Implementare prima `ID_Marq_ht_sepchain_t`.
3. Eseguire test base e Valgrind.
4. Implementare `ID_Marq_ht_linprob_t`.
5. Aggiungere resizing, rehashing e tombstone.
6. Implementare keys e traverse.
7. Verificare ownership e memory leak.

## Open questions

- Qual è il contenuto esatto dell’header `ID_Marq`?
- Quali funzioni sono marcate TO STUDENTS?
- Chi possiede e libera la memoria delle chiavi e dei valori?
- Quali tipi saranno usati nell’esercizio o nell’esame?

## Missing context

- Il PDF non contiene il codice completo di src/hashtable.c.
- Non sono disponibili i file effettivi del toolkit `ID_Marq`.
- Il demo standalone non può essere compilato contro l’header ufficiale senza il progetto del corso.
- Le firme definitive devono essere confermate nell’header ufficiale.

## Mini-riepilogo per l’orale

Una Hash Table associa chiavi e valori usando una funzione hash che trasforma la chiave in un indice. Quando due chiavi hanno lo stesso indice si verifica una collisione. Con la separate chaining le collisioni vengono memorizzate in liste concatenate. Con il linear probing si cercano altri slot secondo (h(k) + i) mod M. Il load factor è α = N / M: se aumenta, aumentano collisioni e tempi di ricerca. Nel linear probing la cancellazione richiede un tombstone, mentre il ridimensionamento richiede il rehashing. In C, un ADT generico usa void pointer, funzione hash, comparatore e politica esplicita di deallocazione.

## Sources

- Materiale principale: upload/08-hashtable.pdf, **Algorithms 1 – Lab: The Hash Table ADT**, Marco Guazzone, DiSIT – University of Piemonte Orientale.
- Demo C adattato: [hash_table_demo.c](hash_table_demo.c).
- Riferimento grafico citato nelle slide: [Hash table – Wikimedia Commons](https://commons.wikimedia.org/wiki/File:Hash_table_3_1_1_0_1_0_0_SP.svg).
