#include "state.hpp"

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

