#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include "LCS.h"
#include "lcs_versioni.h"

using namespace std;

int main() {
    system ("cls"); // Pulisce la console
    
    string s1 = "GCGAT", s2 = "ACTATA";

    auto [c, b] = lcs(s1, s2);

    cout << endl;
    print_C(s1, s2, move(c));
    print_B(s1, s2, move(b));

    cout << endl;

    cout << "LCS di \"" << s1 << "\" e \"" << s2 << "\" = ";
    print_lcs(b, s1, s1.size(), s2.size());

    cout << endl;

    int l2, l3, n = 1000;
    string s3, s4;
    s3 = generaStringa(n);
    s4 = generaStringa(n);    

    auto [c2, b2] = lcs(s3, s4);
    l2 = lcs_versione2(s3, s4);
    l3 = lcs_versione3(s3, s4);

    cout << endl << "Stringhe di lunghezza " << n << " generate casualmente.\n";
    cout << "Lunghezza della LCS trovata dalla Versione 1: "<< c2[n][n] << endl;
    cout << "Lunghezza della LCS trovata dalla Versione 2: "<< l2 << endl;
    cout << "Lunghezza della LCS trovata dalla Versione 3: "<< l3 << endl << endl;
}