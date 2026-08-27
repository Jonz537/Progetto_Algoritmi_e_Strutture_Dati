#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <random>
#include <iomanip>
#include <cstdint> 
using namespace std;

uintptr_t stack_base = 0;
size_t stack_at_base_case = 0;

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
    if (i == 0 || j == 0) {
        char base_marker;
        uintptr_t current_stack = reinterpret_cast<uintptr_t>(&base_marker);
        if (stack_base > current_stack) {
            stack_at_base_case = stack_base - current_stack;
        }
        return;
    }

    if (b[i][j] == 'D') {  // Diagonale ↖ (NON in coda: preserva lo stack frame per stampare dopo)
        print_lcs(b, X, i - 1, j - 1, print);
        if (print) cout << X[i - 1];
    }
    else if (b[i][j] == 'U') {  // Up ↑ (IN CODA PURA: trasformabile in salto iterativo)
        return print_lcs(b, X, i - 1, j, print);
    }
    else {  // Left ← (IN CODA PURA: trasformabile in salto iterativo)
        return print_lcs(b, X, i, j - 1, print); 
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

// Ritorna il picco di working set corrente del processo (RAM Heap complessiva), in KB.
long long picco_memoria_kb() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(),
                          reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc),
                          sizeof(pmc));
    return pmc.PeakWorkingSetSize / 1024;
}

void esperimento(int n, string s1 = "", string s2 = "") {
    string s1_baseline, s2_baseline;
    auto [c_baseline, b_baseline] = lcs(s1_baseline, s2_baseline);
    long long baseline_kb = picco_memoria_kb();

    if (s1.empty() && s2.empty()) {
        s1 = generaStringa(n);
        s2 = generaStringa(n);
    }

    // 1. Benchmark Fase Calcolo Tabelle (Matrici c e b)
    auto t0 = std::chrono::high_resolution_clock::now();
    auto [c, b] = lcs(s1, s2);
    auto t1 = std::chrono::high_resolution_clock::now();

    double durata = std::chrono::duration<double, std::milli>(t1 - t0).count();
    long long picco_kb = picco_memoria_kb();
    long long overhead_kb = picco_kb - baseline_kb;
    
    if (overhead_kb < 0) overhead_kb = 0;

    cout << "n: " << setw(5) << n << "\n" 
         << "Tempo calcolo: " << setw(7) << durata  << " ms"
         << "\t\tRAM Heap tabelle: " << overhead_kb << " KB\n";


    char base_marker;
    stack_base = reinterpret_cast<uintptr_t>(&base_marker); 
    stack_at_base_case = 0;                                 

    auto t2 = std::chrono::high_resolution_clock::now();
    print_lcs(b, s1, s1.size(), s2.size(), false); // false = non inquina la console
    auto t3 = std::chrono::high_resolution_clock::now();
    
    double durata_stampa = std::chrono::duration<double, std::milli>(t3 - t2).count();

    cout << "Tempo ricostruzione (" << setw(5) << n << "): " << setw(7) << durata_stampa << " ms"
         << "\tProfondita' Stack ricorsione: " << setw(7) << stack_at_base_case << " Byte (~" 
         << fixed << setprecision(2) << stack_at_base_case / 1024.0 << " KB)\n\n";
}

int main() {

    int N[] = {10, 100, 1000, 5000, 10000};
    int num_test = sizeof(N) / sizeof(N[0]);
    
    // Test con stringhe casuali
    for (int i = 0; i < num_test; ++i) {
        int n = N[i];
        esperimento(n);
    }

    int k = 10000;
    
    // Test con LCS = 0 (stringhe diverse)
    esperimento(k, string(k, 'A'), string(k, 'B'));

    return 0;
}