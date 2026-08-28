#include <iostream>   
#include <string>    
#include <algorithm> 
#include <vector>
using namespace std;

// ============================================================
// VERSIONE SPACE-OPTIMIZED: O(n*m) Tempo, O(n) Spazio
// ============================================================
int lcs_versione3(string &s1, string &s2) {
  
    int m = s1.length(), n = s2.length();

    // Singolo vettore inizializzato a tutti zeri, memorizza i valori LCS per la riga corrente.
    // c[j] rappresenta la lunghezza della LCS di s1[0..i], s2[0..j]
    vector<int> c(n + 1, 0);

    for (int i = 1; i <= m; ++i) {


        // la variabile prev serve a memorizzare il valore "diagonale"  LCS[i-1][j-1] che viene sovrascritto durante l'aggiornamento di c[j]. 
        int prev = c[0];

        for (int j = 1; j <= n; ++j) {

            // Salva temporaneamente il valore corrente di c[j] prima che venga aggiornato
            int temp = c[j];

            // solito meccanismo: se uguali aggiunge uno al valore diagonale
            if (s1[i - 1] == s2[j - 1]) {
                c[j] = 1 + prev;
            }
            // Altrimenti, prende il massimo tra il valore a sinistra (c[j-1]) e quello sopra (c[j])
            else {
                c[j] = max(c[j - 1], c[j]);
            }

            // Aggiorna prev per la prossima iterazione
            prev = temp;
        }
    }

    // Alla fine dei due cicli il vettore c contiene l'ultima riga della matrice LCS 
    // e c[n] contiene la lunghezza della LCS di s1 e s2
    return c[n];
}

int main() {
    string s1 = "AGGTApBp";
    string s2 = "bGXTXAYpB";
    int res = lcs_versione3(s1, s2);
    cout << res;
    return 0;
}