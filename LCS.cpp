#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <random>
#include <iomanip>
using namespace std;

void print_B(std::string &s1, std::string &s2, std::vector<std::vector<char>> &&b)
{
    cout << "\nMatrice b:\n";

    for (int i = 1; i <= s1.size(); ++i)
    {
        for (int j = 1; j <= s2.size(); ++j)
        {
            cout << b[i][j] << " ";
        }
        cout << endl;
    }
}

void print_C(std::string &s1, std::string &s2, std::vector<std::vector<int>> &&c) {
    cout << "Matrice c:\n";

    for (int i = 0; i <= s1.size(); ++i)
    {
        for (int j = 0; j <= s2.size(); ++j)
        {
            cout << c[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nLunghezza della LCS: " << c[s1.size()][s2.size()] << endl;
}

// CALCOLO MATRICI C, B E LCS 
pair<vector<vector<int>>, vector<vector<char>>> lcs(string &s1, string &s2) {
    int m = s1.size();
    int n = s2.size();

    // Matrice c: lunghezze delle LCS
    vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));

    // Matrice b: direzioni
    vector<vector<char>> b(m + 1, vector<char>(n + 1, ' '));

    // Riempimento bottom-up
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {

            if (s1[i - 1] == s2[j - 1]) {
                c[i][j] = c[i - 1][j - 1] + 1;
                b[i][j] = 'D';  // Diagonale ↖
            }
            else if (c[i - 1][j] >= c[i][j - 1]) {
                c[i][j] = c[i - 1][j];
                b[i][j] = 'U';  // Up ↑
            }
            else {
                c[i][j] = c[i][j - 1];
                b[i][j] = 'L';  // Left ←
            }
        }
    }

    return {c, b};
}

// STAMPA LCS
void print_lcs(const vector<vector<char>>& b, const string& X, int i, int j, bool print = true) {
    if (i == 0 || j == 0)
        return;

    if (b[i][j] == 'D') {  // Diagonale ↖
        print_lcs(b, X, i - 1, j - 1, print);
        if (print) cout << X[i - 1];  // Stampa il carattere corrispondente
    }
    else if (b[i][j] == 'U') {  // Up ↑
        print_lcs(b, X, i - 1, j, print);
    }
    else {  // Left ←
        print_lcs(b, X, i, j - 1, print);
    }
}

string generaStringa(int n) {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<int> dist(0, 3);

    string s;
    s.reserve(n);

    for (int i = 0; i < n; ++i) {
        s += 'A' + dist(gen);
    }

    return s;
}

// Ritorna il picco di working set corrente del processo, in KB.
long long picco_memoria_kb() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(),
                          reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc),
                          sizeof(pmc));
    return pmc.PeakWorkingSetSize / 1024;
}

int main() {
    // Necessario per calcolare l'overhead di memoria fisso del programma non dipendente dalla dimensione delle stringhe di input.
    string s1_baseline, s2_baseline;
    auto [c_baseline, b_baseline] = lcs(s1_baseline, s2_baseline);
    long long baseline_kb = picco_memoria_kb();

    int N[7] = {10, 100, 1000, 5000, 10000, 20000, 30000}; // 100000, 1000000};

    for (int i = 0; i < 7; ++i) {
        int n = N[i];
        string s1 = generaStringa(n);
        string s2 = generaStringa(n);

        auto t0 = std::chrono::high_resolution_clock::now();
        auto [c, b] = lcs(s1, s2);
        auto t1 = std::chrono::high_resolution_clock::now();

        double durata = std::chrono::duration<double, std::milli>(t1 - t0).count();  // In millisecondi
        long long picco_kb = picco_memoria_kb();
        long long overhead_kb = picco_kb - baseline_kb;
        
        if (overhead_kb < 0) overhead_kb = 0;

        cout << "n: " << setw(5) << n
             << "\tTempo: " << setw(7) << durata  << " ms"
             //<< ", \tOverhead Fisso: " << baseline_kb << " KB"
             //<< "\tPicco memoria totale: " << picco_kb << " KB"
             << "\t\tMemoria netta programma: " << overhead_kb << " KB\n";

        auto t2 = std::chrono::high_resolution_clock::now();
        print_lcs(b, s1, s1.size(), s2.size(), false);  // Stampa la LCS senza output   
        auto t3 = std::chrono::high_resolution_clock::now();
        
        double durata_stampa = std::chrono::duration<double, std::milli>(t3 - t2).count();

        cout << "Tempo stampa LCS (" << setw(5) << n << "):\t"<< durata_stampa << " ms\n\n";

    }

    // Stampa c
    //print_C(s1, s2, c);

    // Stampa b
    //print_B(s1, s2, b);

    //cout << "\nLCS: ";
    //print_lcs(b, s1, s1.size(), s2.size());

    return 0;
}

