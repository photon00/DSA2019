#include "state.hpp"

inline int get_kbit(long long int target, unsigned int k){
    return (target >> k) & 0b1;
}

int maximize(state s, int alpha, int beta);
int minimize(state s, int alpha, int beta);

std::vector< std::tuple<unsigned int, unsigned int> > Alice_cards;
std::vector< std::tuple<unsigned int, unsigned int> > Bob_cards;
std::unordered_map<long long int, int> umap;

inline std::tuple<unsigned int, unsigned int> string_to_tuple(std::string card){
    unsigned int s, n;
    switch (card[0]){
        case 'C': s = 0; break;
        case 'D': s = 1; break;
        case 'H': s = 2; break;
        case 'S': s = 3; break;
    }
    switch (card[1]){
        case 'A': n = 1; break;
        case '2': n = 2; break;
        case '3': n = 3; break;
        case '4': n = 4; break;
        case '5': n = 5; break;
        case '6': n = 6; break;
        case '7': n = 7; break;
        case '8': n = 8; break;
        case '9': n = 9; break;
        case 'T': n = 10; break;
        case 'J': n = 11; break;
        case 'Q': n = 12; break;
        case 'K': n = 13; break;
    }
    return std::make_tuple(s, n);
}

int maximize(state s, int alpha, int beta){
    int test = s.eval();
    if (test != 0) return test;
    auto card = s.get_card();
    auto state_value = s.get_stateValue();
    if (umap.find(state_value) != umap.end()) return umap.find(state_value)->second;
    int score = -INT_MAX;
    bool flag = true;
    bool pruning = false;
    // current player is Alice
    for (int i=0; i<state::num_cards; ++i){
        if (std::get<1>(card) == 0 || std::get<1>(card) > 13  ||
            std::get<0>(card) == std::get<0>(Alice_cards[i]) ||
            std::get<1>(card) == std::get<1>(Alice_cards[i])){
            if (get_kbit(state_value, i+6)){
                score = std::max(score, minimize(s.gen_childState(i, alpha, beta), alpha, beta));
                flag = false;
            }
            if (score >= beta) { pruning = true; break; }
            alpha = std::max(score, alpha);
        }
    }
    if (flag && !pruning && std::get<1>(card) > 0 && std::get<1>(card) < 14)
        score = std::max(score, minimize(s.gen_childState(state::num_cards, alpha, beta), alpha, beta));
    umap[state_value] = score;
    return score;
}

int minimize(state s, int alpha, int beta){
    int test = s.eval();
    if (test != 0) return test;
    auto card = s.get_card();
    auto state_value = s.get_stateValue();
    if (umap.find(state_value) != umap.end()) return umap.find(state_value)->second;
    int score = INT_MAX;
    bool flag = true;
    bool pruning = false;
    // current player is Bob
    for (int i=0; i<state::num_cards; ++i){
        if (std::get<1>(card) > 13 || std::get<0>(card) == std::get<0>(Bob_cards[i]) ||
            std::get<1>(card) == std::get<1>(Bob_cards[i])){
            if (get_kbit(state_value, i+23)){
                score = std::min(score, maximize(s.gen_childState(i, alpha, beta), alpha, beta));
                flag = false;
            }
            if (score <= alpha) { pruning = true; break; }
            beta = std::min(score, beta);
        }
    }
    if (flag && !pruning && std::get<1>(card) > 0 && std::get<1>(card) < 14)
        score = std::min(score, maximize(s.gen_childState(state::num_cards, alpha, beta), alpha, beta));

    umap[state_value] = score;
    return score;
}

int main(){
    using namespace std;
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> state::num_cards;
    string tmp;
    for (int i=0; i<state::num_cards; ++i){
        cin >> tmp;
        Alice_cards.push_back(string_to_tuple(tmp));
    }
    for (int i=0; i<state::num_cards; ++i){
        cin >> tmp;
        Bob_cards.push_back(string_to_tuple(tmp));
    }
    state root;
    int final_score = maximize(root, -INT_MAX, INT_MAX);
    if (final_score > 0)
        cout << "Alice\n" << final_score << '\n';
    else
        cout << "Bob\n" << -final_score << '\n';
    return 0;
}
