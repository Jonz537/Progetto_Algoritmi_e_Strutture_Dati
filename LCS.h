#pragma once

#include <string>
#include <utility>
#include <vector>

long long memoria_corrente_kb();
void libera_memoria_processo();

void print_B(std::string& s1, std::string& s2,
             std::vector<std::vector<char>>&& b);
void print_C(std::string& s1, std::string& s2,
             std::vector<std::vector<int>>&& c);

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<char>>>
lcs(std::string& s1, std::string& s2);

void print_lcs(const std::vector<std::vector<char>>& b,
               const std::string& X, int i, int j, bool print = true);

std::string generaStringa(int n);

void esperimento(int n, int m, std::string s1 = "", std::string s2 = "");
void esperimento(int n, std::string s1 = "", std::string s2 = "");

void esperimento_2(int n, int m, std::string s1 = "", std::string s2 = "");
void esperimento_2(int n, std::string s1 = "", std::string s2 = "");

void esperimento_3(int n, int m, std::string s1 = "", std::string s2 = "");
void esperimento_3(int n, std::string s1 = "", std::string s2 = "");