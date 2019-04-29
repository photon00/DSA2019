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