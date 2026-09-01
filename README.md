# Progetto_Algoritmi_e_Strutture_Dati
Codice relativo al progetto di Algoritmi e Strutture Dati primo anno magistrale


Comando per la compilazione:

```g++ -O1 -fno-optimize-sibling-calls -fno-inline .\LCS.cpp .\lcs_versione2.cpp .\lcs_versione3.cpp -o no_tail.exe -lpsapi ; g++ -O1 -foptimize-sibling-calls -fno-inline .\LCS.cpp .\lcs_versione2.cpp .\lcs_versione3.cpp -o tail_opt.exe -lpsapi```

Comando per l'esecuzione:

```.\no_tail.exe; .\tail_opt.exe```

Comando per l'esecuzione con output su file:

```.\no_tail.exe > results_no_tail.txt; .\tail_opt.exe > results_tail.txt```

Utilizzando i comandi riportati si genereranno 2 versioni del codice:
- Con compilatore ottimizzante per le ricorsioni in coda
- Senza compilatore ottimizzante per le ricorsioni in coda
Questo permette di rilevare le differenza nella pila dei contesti

