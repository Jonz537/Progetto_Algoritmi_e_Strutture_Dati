#include <iostream>   
#include <string>    
#include <algorithm> 
#include <vector>
#include "lcs_versioni.h"
using namespace std;

// ============================================================
// VERSIONE SPACE-OPTIMIZED: O(n*m) Tempo, O(n) Spazio
// ============================================================
// Convenzione: scorriamo s1 sulle righe (i) e s2 sulle colonne (j), come avviene in lcs()
int lcs_versione2(string &s1, string &s2) {

    if (s1.length() < s2.length()) {
        swap(s1, s2);
    }

    int m = s1.length(), n = s2.length();

    // due array lunghi n+1, anziché una matrice m+1 x n+1 
    vector<int> prev(n + 1, 0);
    vector<int> curr(n + 1, 0);

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {

            if (s1[i - 1] == s2[j - 1]) {
                // Diagonale: dipende da prev[j-1] corrisponde al vecchio c[i-1][j-1]
                curr[j] = prev[j - 1] + 1;
            }
            else if (prev[j] >= curr[j - 1]) {
                // Up: c[i-1][j] diventa prev[j] (riga precedente)
                curr[j] = prev[j];
            }
            else {
                // Left: c[i][j-1] diventa curr[j-1] (stessa riga, una colonna a sinistra)
                curr[j] = curr[j - 1];
            }
        }

        // La riga 'curr' viene fatta scorrere, diventa la 'prev' della prossima iterazione
        // 'curr' verrà sovrascritta nella prossima iterazione del ciclo for esterno 
        prev = curr;
    }

    // Il risultato si trova nell'ultima casella di prev al termine dei due cicli for
    return prev[n];
}

// int main() {

//     string s1 = "AGGTApB";
//     string s2 = "bGXTXAYpB";
//     int res = lcs_versione2(s1, s2);
//     cout << res;
//     return 0;
// }