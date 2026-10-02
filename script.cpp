// libs
#include<bits/stdc++.h>
#include<fstream>
#include "helper_modules/visualizer.hpp"
using namespace std;

class toc{
    // class variables
    public:
    // class variables,some helper functions
    unordered_set<string>final_states , alphabet , all_states , non_final_states, start_state , min_final_states , min_start_state , min_all_states;
    unordered_map<string , unordered_map<string , string>>main_map , min_main_map;
    vector<vector<unordered_set<string>>>all_equivalences;
    
    // NFA to DFA specific variables
    unordered_map<string, unordered_map<string, unordered_set<string>>> nfa_main_map;
    unordered_map<string, unordered_map<string, string>> dfa_main_map;
    unordered_set<string> dfa_start_state, dfa_final_states, dfa_all_states;
    void print_set(unordered_set<string>&st){
        for(auto &s:st){
            cout << s << " ";
        }
        cout << endl;
    }
    // TAKING THE INPUT
    void take_input(){
        // initialising the files
        ifstream alphabet_file("inputs/alphabet.txt");
        ifstream final_states_file("inputs/final_states.txt");
        ifstream general_input_file("inputs/input.txt");
        ifstream start_state_file("inputs/start_state.txt");
        string word;
        // taking alphabets,final states,start state and putting it in the respective set data-structres
        while(alphabet_file >> word){
            alphabet.insert(word);
        }
        while(final_states_file >> word){
            final_states.insert(word);
        }
        while(start_state_file >> word){
            start_state.insert(word);
        }
        string w1, w2, w3;
        // input is of the form state1 - transition symbol(alphabet) - state2
        while(general_input_file >> w1 >> w2 >> w3){
            all_states.insert(w1);
            all_states.insert(w3);
            main_map[w1][w2] = w3;
            if(w3 != "D" && w1 != "D"){
                nfa_main_map[w1][w2].insert(w3);
            }
        }
        for(string s : all_states){
            if(final_states.find(s) == final_states.end()){
                non_final_states.insert(s);
            }
        }

    }
    // PRINTING THE INPUTS
    void print_main_map(){
        for(auto p : main_map){
            cout << p.first << endl;
            for(auto &q : p.second){
                cout << q.first << " " << q.second << endl;
            }
            cout << endl;
        }
    }
    void print_start_states(){
        print_set(start_state);
    }
    void print_final_states(){
        print_set(final_states);
    }
    void print_alphabet(){
        print_set(alphabet);
    }
    void print_non_final_states(){
        print_set(non_final_states);
    }
    // PRINTING THE OUTPUTS
    void print_all_equivalences(){
        for(auto &v1 : all_equivalences){
            cout << "[ ";
            for(auto &v2 : v1){
                cout << "{ ";
                for(auto &s : v2){
                    cout << s << " ";
                }
                cout << " }";
            }
            cout << " ]";
            cout << endl;
        }
    }
    // SOLVER FUNCTIONS
    void solve_func_min_dfa(vector<vector<unordered_set<string>>>&all_equivalences , unordered_map<string,int>&get_indexes){
        // base case
        int size = all_equivalences.size();
        // meaning if the last two equivalences have been the same,therefore terminate the process.
        if(size >= 2 && all_equivalences[size-1].size() == all_equivalences[size-2].size()){
            return;
        }
        // this new_map links all the similar states to a set,we do this using a string code
        // 'code' is just a combination of the 
        unordered_map<string , unordered_set<string>>new_map;
        for(auto &state : all_states){
            string code;
            // key fix to initialise code w. current info of index,otherwise led to regrouping
            code += (get_indexes[state] + '0');
            // for every letter,we check the linkage,and put in our respective code
            // instead of putting the names of the states,we simply use the index it is located at for the code
            // this is the neighbour checking logic of the code.
            for(auto &letter : alphabet){
                // main_map[state] gives all the linkages
                // when we do main_map[state][leter] , we get the corresponding state
                // now we simply add the index to the code
                string linkage = main_map[state][letter];
                int int_index = get_indexes[linkage];
                code += (int_index + '0');
            }
            new_map[code].insert(state);
            // many of them will share the same code.
            // this is the grouping process we want
        }
        unordered_map<string , int>indexes;
        vector<unordered_set<string>>curr_equivalence;
        int i = 0;
        for(auto &p : new_map){
            curr_equivalence.push_back(p.second);
            for(auto &ele : p.second){
                indexes[ele] = i;
            }
            i++;
        }
        all_equivalences.push_back(curr_equivalence);
        // recursive call to solve for the next level of equivalences
        // it is the the modification in indexes data structre which drives the recursion forward
        solve_func_min_dfa(all_equivalences , indexes);
    }
    void solve_min_dfa(){
        // in the start,all final states in one set,non final in another set
        // starting case is simple
        // 0-eq = { all non final states , all final states}
        // so we give them indices 0 and 1 respectively
        vector<unordered_set<string>>zero_eq;
        unordered_set<string>s_final , s_non_final;
        unordered_map<string , int>indexes;
        for(auto &state:non_final_states){indexes[state]=0;}
        for(auto &state:final_states){indexes[state]=1;}        
        zero_eq.push_back(non_final_states);
        zero_eq.push_back(final_states);
        all_equivalences.push_back(zero_eq);
        solve_func_min_dfa(all_equivalences , indexes);
        analyse_to_min_dfa();
    }
    void analyse_to_min_dfa(){
        unordered_map<string, string> old_to_new_map;
        // get the last equivalence record
        auto last_record = all_equivalences[all_equivalences.size()-1];
        // of type vector of unordered_set,with each unordered_set being ONE element in min dfa
        for(auto &st : last_record){
            bool is_start = false , is_final = false;
            string new_name = "{";
            for(auto &s : st){
                if(s == ""){continue;}
                new_name += s + "";
                if(start_state.find(s) != start_state.end()){
                    is_start = true;
                }
                if(final_states.find(s) != final_states.end()){
                    is_final = true;
                }                
            }
            new_name += "}";
            if(new_name == "{}"){continue;}
            if(is_final){
                min_final_states.insert(new_name);
            }
            if(is_start){
                min_start_state.insert(new_name);
            }
            min_all_states.insert(new_name);
            for(auto &s : st){
                old_to_new_map[s] = new_name;
            }
        }
            for(auto &st : last_record){
                    if(st.empty()) continue;
                    
                    // Pick any single state from the equivalence class as a representative
                    string representative_old_state = *st.begin();
                    string from_new_state = old_to_new_map[representative_old_state];

                    // Translate its old transitions into the new Min-DFA space
                    for(auto const& transition : main_map[representative_old_state]){
                        string symbol = transition.first;
                        string dest_old_state = transition.second;
                        string dest_new_state = old_to_new_map[dest_old_state];
                        min_main_map[from_new_state][symbol] = dest_new_state;
                    }
                }
        }
    string get_dfa_state_name(const set<string>& state_set) {
        if (state_set.empty()) return "D";
        string name = "{";
        for (const auto& s : state_set) {
            name += s;
        }
        name += "}";
        return name;
    }

    void solve_func_nfa_dfa(queue<set<string>>& q, set<set<string>>& visited) {
        set<string> initial_set(start_state.begin(), start_state.end());
        q.push(initial_set);
        visited.insert(initial_set);

        while (!q.empty()) {
            auto current_dfa_state = q.front();
            q.pop();

            for (const auto& letter : alphabet) {
                set<string> next_dfa_state;
                for (const auto& nfa_state : current_dfa_state) {
                    if (nfa_main_map[nfa_state].find(letter) != nfa_main_map[nfa_state].end()) {
                        for (const auto& target_state : nfa_main_map[nfa_state][letter]) {
                            next_dfa_state.insert(target_state);
                        }
                    }
                }
                if (visited.find(next_dfa_state) == visited.end()) {
                    visited.insert(next_dfa_state);
                    q.push(next_dfa_state);
                }
            }
        }

        for (const auto& current_dfa_state : visited) {
            string from_state_name = get_dfa_state_name(current_dfa_state);
            dfa_all_states.insert(from_state_name);

            for (const auto& nfa_state : current_dfa_state) {
                if (final_states.find(nfa_state) != final_states.end()) {
                    dfa_final_states.insert(from_state_name);
                    break;
                }
            }

            for (const auto& letter : alphabet) {
                set<string> next_dfa_state;
                for (const auto& nfa_state : current_dfa_state) {
                    if (nfa_main_map[nfa_state].find(letter) != nfa_main_map[nfa_state].end()) {
                        for (const auto& target_state : nfa_main_map[nfa_state][letter]) {
                            next_dfa_state.insert(target_state);
                        }
                    }
                }
                string to_state_name = get_dfa_state_name(next_dfa_state);
                dfa_main_map[from_state_name][letter] = to_state_name;
            }
        }

        dfa_start_state.insert(get_dfa_state_name(initial_set));
    }

    void solve_nfa_dfa() {
        queue<set<string>> q;
        set<set<string>> visited;
        solve_func_nfa_dfa(q, visited);
    }

    // MAKING THE GRAPHS
    void make_graph_original(){
        string name = "outputs/Original_DFA.jpg";
        draw_dfa(main_map , final_states , start_state , name);
    }
    void make_graph_minimized(){
        string name = "outputs/Minimized_DFA.jpg";
        draw_dfa(min_main_map , min_final_states , min_start_state , name);
    }
    void make_graph_nfa_to_dfa(){
        string name = "outputs/NFA_to_DFA.jpg";
        draw_dfa(dfa_main_map, dfa_final_states, dfa_start_state, name);
    }

};



int main(){
    auto sol =  toc();
    // TAKING INPUT
    sol.take_input();
    // PRINTING INPUTS TO CONSOLE
    sol.print_alphabet();
    sol.print_start_states();
    sol.print_final_states();
    sol.print_main_map();
    sol.print_non_final_states();
    // SOLVING
    sol.solve_nfa_dfa();
    sol.solve_min_dfa();
    sol.print_all_equivalences();
    // MAKING GRAPHS
    // sol.make_graph_original();
    // sol.make_graph_minimized();
    // sol.make_graph_nfa_to_dfa();
}