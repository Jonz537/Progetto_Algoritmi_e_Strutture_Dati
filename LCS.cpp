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
#include "lcs_versioni.h"
using namespace std;

uintptr_t stack_base = 0;
size_t stack_at_base_case = 0;

long long memoria_corrente_kb() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(),
                          reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc),
                          sizeof(pmc));
    return pmc.WorkingSetSize / 1024; // WorkingSetSize invece di PeakWorkingSetSize
}

void libera_memoria_processo() {
    EmptyWorkingSet(GetCurrentProcess());
}

void print_B(std::string &s1, std::string &s2, std::vector<std::vector<char>> &&b) {
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

    vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));
    vector<vector<char>> b(m + 1, vector<char>(n + 1, ' '));

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

    if (b[i][j] == 'D') {
        print_lcs(b, X, i - 1, j - 1, print);
        if (print) cout << X[i - 1];
    }
    else if (b[i][j] == 'U') {
        return print_lcs(b, X, i - 1, j, print);
    }
    else {
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

void esperimento(int n, int m, string s1 = "", string s2 = "") {
    if (s1.empty() && s2.empty()) {
        s1 = generaStringa(n);
        s2 = generaStringa(m);
    }

    // 1. Reset della memoria del working set prima del test
    libera_memoria_processo();
    long long mem_before_kb = memoria_corrente_kb();

    // 2. Allocazione ed esecuzione LCS
    auto t0 = std::chrono::high_resolution_clock::now();
    auto [c, b] = lcs(s1, s2);
    auto t1 = std::chrono::high_resolution_clock::now();

    // 3. Campionamento memoria con le matrici allocate in RAM
    long long mem_after_kb = memoria_corrente_kb();
    long long ram_effettiva_kb = mem_after_kb - mem_before_kb;
    if (ram_effettiva_kb < 0) ram_effettiva_kb = 0;

    double durata = std::chrono::duration<double, std::milli>(t1 - t0).count();

    cout << "n: " << setw(5) << n << "  m: " << setw(5) << m << "\n" 
         << "Tempo calcolo: " << setw(7) << durata  << " ms"
         << "\t\t\tRAM Heap tabelle: " << setw(8) << ram_effettiva_kb << " KB\n";

    // 4. Misurazione Stack
    char base_marker;
    stack_base = reinterpret_cast<uintptr_t>(&base_marker); 
    stack_at_base_case = 0; 

    auto t2 = std::chrono::high_resolution_clock::now();
    print_lcs(b, s1, s1.size(), s2.size(), false);
    auto t3 = std::chrono::high_resolution_clock::now();
    
    double durata_stampa = std::chrono::duration<double, std::milli>(t3 - t2).count();

    cout << "Tempo ricostruzione: " << setw(7) << durata_stampa << " ms"
         << "\tProfondita' Stack: " << setw(7) << stack_at_base_case << " Byte (~" 
         << fixed << setprecision(2) << stack_at_base_case / 1024.0 << " KB)\n\n";
}

void esperimento(int n, string s1 = "", string s2 = "") {
    esperimento(n, n, s1, s2);
}

void esperimento_2(int n, int m, string s1 = "", string s2 = "") {
    if (s1.empty() && s2.empty()) {
        s1 = generaStringa(n);
        s2 = generaStringa(m);
    }

    // 1. Reset della memoria del working set prima del test
    libera_memoria_processo();
    long long mem_before_kb = memoria_corrente_kb();

    // 2. Allocazione ed esecuzione LCS
    auto t0 = std::chrono::high_resolution_clock::now();
    auto c = lcs_versione2(s1, s2);
    auto t1 = std::chrono::high_resolution_clock::now();

    // 3. Campionamento memoria con le matrici allocate in RAM
    long long mem_after_kb = memoria_corrente_kb();
    long long ram_effettiva_kb = mem_after_kb - mem_before_kb;
    if (ram_effettiva_kb < 0) ram_effettiva_kb = 0;

    double durata = std::chrono::duration<double, std::milli>(t1 - t0).count();

    cout << "n: " << setw(5) << n << "  m: " << setw(5) << m << "\n" 
         << "Tempo calcolo: " << setw(7) << durata  << " ms"
         << "\t\t\tRAM Heap tabelle: " << setw(8) << ram_effettiva_kb << " KB\n\n";
}

void esperimento_2(int n, string s1 = "", string s2 = "") {
    esperimento_2(n, n, s1, s2);
}

void esperimento_3(int n, int m, string s1 = "", string s2 = "") {
    if (s1.empty() && s2.empty()) {
        s1 = generaStringa(n);
        s2 = generaStringa(m);
    }

    // 1. Reset della memoria del working set prima del test
    libera_memoria_processo();
    long long mem_before_kb = memoria_corrente_kb();

    // 2. Allocazione ed esecuzione LCS
    auto t0 = std::chrono::high_resolution_clock::now();
    auto c = lcs_versione3(s1, s2);
    auto t1 = std::chrono::high_resolution_clock::now();

    // 3. Campionamento memoria con le matrici allocate in RAM
    long long mem_after_kb = memoria_corrente_kb();
    long long ram_effettiva_kb = mem_after_kb - mem_before_kb;
    if (ram_effettiva_kb < 0) ram_effettiva_kb = 0;

    double durata = std::chrono::duration<double, std::milli>(t1 - t0).count();

    cout << "n: " << setw(5) << n << "  m: " << setw(5) << m << "\n" 
         << "Tempo calcolo: " << setw(7) << durata  << " ms"
         << "\t\t\tRAM Heap tabelle: " << setw(8) << ram_effettiva_kb << " KB\n\n";
}

void esperimento_3(int n, string s1 = "", string s2 = "") {
    esperimento_3(n, n, s1, s2);
}



int main() {
    int N[] = {10, 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
    int num_test = sizeof(N) / sizeof(N[0]);
    
    cout << "\nTEST CON STRINGHE CASUALI n = m\n";
    for (int i = 0; i < num_test; ++i) {
        esperimento(N[i]);
    }
    
    cout << "\nTEST CON STRINGHE CASUALI n VARIABILE E m COSTANTE (m = 100)\n";
    for (int i = 0; i < num_test; ++i) {
        esperimento(N[i], 100);
    }

    int k = 5000;
    cout << "\nESPERIMENTO LCS = 0\n";
    esperimento(k, string(k, 'A'), string(k, 'B'));

    cout << "\nESPERIMENTO STRINGHE UGUALI\n";
    esperimento(k, string(k, 'A'), string(k, 'A'));

    cout << "\n ESPERIMENTO ALFABETO SINGOLO CON n = 5000 E m = 200\n";
    esperimento(k, 200, string(k, 'A'), string(200, 'A'));

    int N_2[] = {10, 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 15000,
         20000, 25000, 30000, 35000, 40000, 45000, 50000};
    num_test = sizeof(N_2) / sizeof(N_2[0]);
    
    cout << "\n Confronto con versioni migliorate del codice\n";

    cout << "\nTEST CON STRINGHE CASUALI n = m\n";
    for (int i = 0; i < num_test; ++i) {
        cout << "\nVersione 1\n";
        esperimento(N_2[i]);
        cout << "\nVersione 2\n";
        esperimento_2(N_2[i]);
        cout << "\nVersione 3\n";
        esperimento_3(N_2[i]);
    }
    
    cout << "\nTEST CON STRINGHE CASUALI n VARIABILE E m COSTANTE (m = 100)\n";
    for (int i = 0; i < num_test; ++i) {
        cout << "\nVersione 1\n";
        esperimento(N_2[i], 100);
        cout << "\nVersione 2\n";
        esperimento_2(N_2[i], 100);
        cout << "\nVersione 3\n";
        esperimento_3(N_2[i], 100);
    }


    return 0;
}