#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <climits>
#include <algorithm>
#include <unordered_map>

class state{
public:
    state();
    state(long long int state_value);
    ~state(){};
    long long int get_stateValue(){ return __state_value; };
    std::tuple<unsigned int, unsigned int> get_card();
    int eval();
    state gen_childState(int i, int alpha, int beta);
    void print_status();

    static int num_cards;

private:
    /*********************************************************************************
    bits        | representation
    -------------------------------------------------------
    3-0(4)      | previous number
    5-4(2)      | previous suit
    22-6(17)    | Alice cards state, 1->not used, 0->used
    39-23(17)   | Bob cards state
    40-48(9)    | alpha
    49-57(9)    | beta
    58-62(5)    | (None)
    63(1)       | current person, 0->Alice, 1->Bob

    -------------------------------------------------------
    Note:   previous num = 0  -> ROOT
            previous num > 13 -> PASS
    *********************************************************************************/
    long long int __state_value;

    std::string __Alice_state();
    std::string __Bob_state();
    std::string __card_state();
};
int state::num_cards = 0;
extern std::vector< std::tuple<unsigned int, unsigned int> > Alice_cards;
extern std::vector< std::tuple<unsigned int, unsigned int> > Bob_cards;

inline int get_kbit(long long int target, unsigned int k){
    return (target >> k) & 0b1;
}
inline long long int reverse_kbit(long long int target, unsigned int k){
    return target ^ ((long long int)1 << k);
}
inline long long int exchange(long long int target, int num, int pos){
    long long int sub, mask = ((long long int)0b111111111 << pos);
    sub = (num < 0) ? 1 : 0;
    for (int i=0; i<8; ++i){
        sub = (sub << 1) + ((num & 0b10000000)>>7);
        num <<= 1;
    }
    return (target&~mask) | (sub << pos);
}

state::state(){
    long long int individual_state = 0;
    for (int i=0; i<num_cards; ++i){
        individual_state = (individual_state << 1) + 1;
    }
    __state_value = (individual_state << (17 + 6)) + (individual_state << 6);
    __state_value = (__state_value | ((long long int)0b100000000<<40));
    __state_value = (__state_value | ((long long int)0b011111111<<49));
}

state::state(long long int state_value){
    __state_value = state_value;
}

state state::gen_childState(int i, int alpha, int beta){
    long long int child_state_value;
    if (i >= num_cards){
        // PASS action
        child_state_value = reverse_kbit(__state_value, 63);
        child_state_value |= 0b111111;
        return state(child_state_value);   
    }
    // current player is Alice
    if (__state_value >= 0){
        child_state_value = reverse_kbit(__state_value, 63);
        child_state_value = reverse_kbit(child_state_value, i+6);
        child_state_value = (child_state_value & ~0b110000) | ((long long int)std::get<0>(Alice_cards[i])<<4);
        child_state_value = (child_state_value & ~0b1111) | ((long long int)std::get<1>(Alice_cards[i]));
        child_state_value = exchange(child_state_value, alpha, 40);
        child_state_value = exchange(child_state_value, beta, 49);
        return state(child_state_value);
    }
    // current player is Bob
    else {
        child_state_value = reverse_kbit(__state_value, 63);
        child_state_value = reverse_kbit(child_state_value, i+23);
        child_state_value = (child_state_value & ~0b110000) | ((long long int)std::get<0>(Bob_cards[i])<<4);
        child_state_value = (child_state_value & ~0b1111) | ((long long int)std::get<1>(Bob_cards[i]));
        child_state_value = exchange(child_state_value, alpha, 40);
        child_state_value = exchange(child_state_value, beta, 49);
        return state(child_state_value);
    }
}

void state::print_status(){
    std::cout << "===============================================" << '\n';
    std::cout << "Current player: " << ((__state_value<0) ? "Bob" : "Alice") << '\n';
    std::cout << "Alice card state: " << __Alice_state() << '\n';
    std::cout << "Bob card state  : " << __Bob_state() << '\n';
    std::cout << "Previous card = " << __card_state() << std::endl;
}

int state::eval(){
    int Alice_state = (__state_value&0b11111111111111111000000) >> 6;
    int Bob_state = (__state_value&0b1111111111111111100000000000000000000000) >> 23;
    int score = 0;
    if (Alice_state == 0){  // Alice win
        for (int i=0; i<num_cards; ++i){
            score += get_kbit(__state_value, i+23) * std::get<1>(Bob_cards[i]);
        }
    }
    else if (Bob_state == 0){  // Bob win
        for (int i=0; i<num_cards; ++i){
            score -= get_kbit(__state_value, i+6) * std::get<1>(Alice_cards[i]);
        }
    }
    return score;
}

std::tuple<unsigned int, unsigned int> state::get_card(){
    unsigned int card_num = __state_value&0b1111;
    unsigned int card_suit = (__state_value&0b110000)>>4;
    return std::make_tuple(card_suit, card_num);
}

/*******************************
    private member functions  
********************************/
std::string state::__Alice_state(){
    std::string c = "";
    int Alice_state = (__state_value&0b11111111111111111000000) << (32 - num_cards - 6);
    for (int i=0; i<num_cards; ++i){
        if (Alice_state < 0) c += '1';
        else c += '0';
        Alice_state <<= 1;
    }
    return c;
}

std::string state::__Bob_state(){
    std::string c = "";
    int Bob_state = (__state_value&0b1111111111111111100000000000000000000000)>>23;
    Bob_state <<= (32 - num_cards);
    for (int i=0; i<num_cards; ++i){
        if (Bob_state < 0) c += '1';
        else c += '0';
        Bob_state <<= 1;
    }
    return c;
}

std::string state::__card_state(){
    std::string c = "";
    unsigned int card_num = __state_value&0b1111;
    if (card_num < 1) return "ROOT";
    if (card_num > 13) return "PASS";
    unsigned int card_suit = (__state_value&0b110000)>>4;
    switch (card_suit){
        case 0b00: c += 'C'; break;
        case 0b01: c += 'D'; break;
        case 0b10: c += 'H'; break;
        case 0b11: c += 'S'; break;
    }
    switch (card_num){
        case 1:  c += 'A'; break;
        case 10: c += 'T'; break;
        case 11: c += 'J'; break;
        case 12: c += 'Q'; break;
        case 13: c += 'K'; break;
        default: c += std::to_string(card_num);
    }
    return c;
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
