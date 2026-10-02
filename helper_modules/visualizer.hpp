#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using namespace std;
#include<bits/stdc++.h>

// draws the dfa on to the screen
void draw_dfa(const unordered_map<string, unordered_map<string, string>>&dfa , const unordered_set<string>&final_states , const unordered_set<string>&intitial_states ,string &file_name);
#endif