#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include "LCS.h"

using namespace std;

int main() {
    string s1 = "GAC", s2 = "AGCAT";

    auto [c, b] = lcs(s1, s2);

    print_B(s1, s2, move(b));

    cout << endl;
    print_C(s1, s2, move(c));

    cout << endl;
}