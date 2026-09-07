#include "lcs_versioni.h"
#include <string>    

using namespace std;


int main() {
    string s1 = "GAC", s2 = "AGCAT";

    auto [c, b] = lcs(s1, s2);

    print_B(s1, s2, move(b));
    print_C(s1, s2, move(c));
}