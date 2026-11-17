# Laboratorio di Algoritmi e Strutture Dati

Progetto di laboratorio per il corso di **Algoritmi e Strutture Dati** (Università di Torino), a.a. 2025/2026.

Quattro esercizi in **C**: librerie generiche (basate su `void*` e puntatori a funzione), unit test per ogni libreria e un programma che le usa su dati reali.

## Esercizi

| # | Argomento | Programma principale |
|---|-----------|----------------------|
| 1 | Ordinamento ibrido QuickSort + SelectionSort | Ordina 20 milioni di record binari per un campo a scelta |
| 2 | Tavola hash con concatenamento | Trova la parola più frequente nell'Iliade, data una lunghezza minima |
| 3 | Heap ternario (coda con priorità) | Simula uno scheduler di task non preemptive con priorità |
| 4 | Grafi sparsi e visita in ampiezza | Visita il grafo delle distanze tra località italiane |

### Esercizio 1 – Ordinamento ibrido
`hybridsort` usa QuickSort sugli array con almeno `k` elementi e SelectionSort sotto la soglia. Include l'analisi sperimentale dei tempi al variare di `k` e del campo di ordinamento.

### Esercizio 2 – Tavola hash
Libreria generica con chiavi e valori di tipo arbitrario. Sviluppata con il supporto di un LLM, con documentazione del processo e verifica della complessità.

### Esercizio 3 – Heap ternario
Coda con priorità generica con operazioni a complessità ottima. Legge i task da un CSV e scrive l'ordine di esecuzione con tempi di inizio e fine.

### Esercizio 4 – Grafi
Grafo generico (diretto o non, etichettato o no), costruito sulla tavola hash dell'Esercizio 2 e ottimizzato per grafi sparsi. Include la visita in ampiezza.

## Struttura

```
ex1/ ex2/ ex3/ ex4/
  Makefile
  src/ ...
  bin/        # main_exN e test_exN, generati da make
```

## Compilazione e test

Da ogni cartella `exN`:

```bash
make all          # produce bin/main_exN e bin/test_exN
./bin/test_exN    # esegue gli unit test
```

## Utilizzo

```bash
# Esercizio 1
./bin/main_ex1 <input_file> <output_file> <field> <k>

# Esercizio 2
./bin/main_ex2 <file_testo> <lunghezza_minima>

# Esercizio 3
./bin/main_ex3 <input_file> <output_file>

# Esercizio 4
./bin/main_ex4 <file_distanze> <città_partenza> <output_file>
```

## Note

- I file dati (`records.bin`, `iliade.txt`, `tasks.csv`, `italian_dist_graph.csv`) e gli eseguibili non sono inclusi nel repository.
- Requisiti: compilatore C e `make`.
