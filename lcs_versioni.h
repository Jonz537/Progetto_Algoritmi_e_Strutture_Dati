#pragma once

#include <string>    
#include <vector>

using namespace std;

int lcs_versione2(string &s1, string &s2);
int lcs_versione3(string &s1, string &s2);
void print_B(string &s1, string &s2, vector<vector<char>> &&b);
void print_C(string &s1, string &s2, vector<vector<int>> &&c);
pair<vector<vector<int>>, vector<vector<char>>> lcs(string &s1, string &s2);
void print_lcs(const vector<vector<char>>& b, const string& X, int i, int j, bool print);



